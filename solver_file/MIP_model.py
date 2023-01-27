from ortools.linear_solver import pywraplp
import sys

def input_data(file_path):
    data = {}
    f = open(file_path,'r')
    [n,k] =[int(x) for x in f.readline().split()]
    data['size_item'] = []
    data['size_truck'] = []
    data['cost'] = []
    
    for i in range(1,n+1):
        line = f.readline().split()
        data['size_item'].append([int(line[0]),int(line[1])])
        # w[i] = data['size_item'][i][0]
        # h[i] = data['size_item'][i][1]

    for i in range(k):
        line = f.readline().split()
        data['size_truck'].append([int(line[0]),int(line[1])])
        data['cost'].append(int(line[2]))
    
    W_truck = [data['size_truck'][i][0] for i in range(k)]
    H_truck = [data['size_truck'][i][1] for i in range(k)]
    return n,k,data,W_truck,H_truck

if __name__ == '__main__':
    try:
        # Get input file path
        file_path = sys.argv[1]
    except IndexError:
        # Default input file if file path is not specified
        file_path = 'input_data/0015.txt'
    
    try:
        # Get input file path
        time_limit = int(sys.argv[2])
    except IndexError:
        # Default input file if file path is not specified
        time_limit = 300

    n,k,data,W_truck,H_truck = input_data(file_path)
    # W_truck is the list of width of cars
    # H_truck is the list of length of cars
    # n is the number of item
    # k is the number of car
    
    max_W = max(W_truck)
    max_H = max(H_truck) 

    # Create solver
    solver = pywraplp.Solver.CreateSolver('SCIP')
    
    # Create variables
    M = 1000000

    x = {} # x[(i,m)] = 1 iff item i is packed in car m else 0
    z = {} # z[i] = m + 1 iff item i is packed in car m else nothing
    Ro = {} # if Ro = 1 then rotation = 90 degree, else 0
    l = {} # left coordination of item
    r = {} # right coordination of item
    t = {} # top coordination of item
    b = {} # bottom coodination of item

    for i in range(n):
        Ro[i] = solver.IntVar(0, 1, 'Ro[%i] '%i)

        # coordinate
        l[i] = solver.IntVar(0, max_W,'l[%i]' % i)
        r[i] = solver.IntVar(0, max_W,'r[%i]' % i)
        t[i] = solver.IntVar(0, max_H,'t[%i]' % i)
        b[i] = solver.IntVar(0, max_H,'b[%i]' % i)        

        solver.Add(r[i] == (1-Ro[i]) * data['size_item'][i][0] + Ro[i] * data['size_item'][i][1] + l[i])
        solver.Add(t[i] == (1-Ro[i]) * data['size_item'][i][1] + Ro[i] * data['size_item'][i][0] + b[i])

        z[i] = solver.IntVar(1,k,f'z[{i}]')

        for m in range(k):

            x[(i,m)] = solver.IntVar(0, 1, 'x_[%i]_[%i]' %(i,m))

            # item i must not exceed area of car
            solver.Add(r[i] <= (1-x[(i,m)]) * M + W_truck[m])
            solver.Add(l[i] <= (1-x[(i,m)]) * M + W_truck[m])
            solver.Add(t[i] <= (1-x[(i,m)]) * M + H_truck[m])
            solver.Add(b[i] <= (1-x[(i,m)]) * M + H_truck[m])    

            # find the car that contain item i
            solver.Add(z[i] <= (1-x[(i,m)]) * M + m +1)
            solver.Add(z[i] >= x[(i,m)] * (m+1))
            # z[i] = m+1 iff x[(i,m)] = 1

    for i in range(n):
        solver.Add(sum(x[(i,m)] for m in range(k)) == 1)


    # if 2 items is packed in the same car, they must be not overlaped
    for i in range(n - 1):
        for j in range(i + 1, n):
            
            constraints = [
                (r[i], l[j]),
                (r[j], l[i]),
                (t[i], b[j]),
                (t[j], b[i]),
            ]

            # c[s] = 1 => e <= f
            # c[s] = 0 => e <= M + f => nothing
            c = list() 
            for s, (e,f) in enumerate(constraints):
                c.append(solver.IntVar(0, 1, f'c[{i}][{j}][{s}]'))
                solver.Add( e <= M * (1 - c[s]) + f)
            # if z[i] == z[j] then d = 1
            # else d == 0

            # got: sum >= 0
            # d = 1 => sum >= 1
            # d = 0 => sum = 0
            d = solver.IntVar(0, 1, f'b[{i}][{j}]')
            solver.Add(sum(c) <= d * M)
            solver.Add(sum(c) + (1 - d) * M >= 1)
            
            t0 = solver.IntVar(0, 1, f't0[{i}][{j}]')
            t1 = solver.IntVar(0, 1, f't1[{i}][{j}]')
            t2 = solver.IntVar(0, 1, f't2[{i}][{j}]')

            # t0 = 0 => z[i] == z[j] => d = 1 => sum >=1
            # t0 = 1 => z[i] != z[j] => d = 0 => sum = 0
            solver.Add(z[j] + t0 <= z[i] + M * t1)
            solver.Add(z[i] + t0 <= z[j] + M * t2)
            solver.Add(t1 + t2 == t0)

            solver.Add(d == 1 - t0)

    # find cars be used
    y = {} # y[m] = 1 iff car m be used
    for m in range(k):
        y[m] = solver.IntVar(0, 1, 'y[%i] ' %m)
        # if sum(x[i][m]) >= 1 then car m be used => y[m] = 1
        # else, y[m] = 0

        q = solver.IntVar(0,n,f'q[{m}]')
        solver.Add(q == sum(x[(i,m)] for i in range(n)))
        # car m be used iff there are at least 1 item be packed in car m, so sum(x[(i,m)] for i in range(n)) != 0 
        
        # q = 0 => y[m] = 0
        # q != 0 => y[m] = 1
        solver.Add(y[m] <= q * M)
        solver.Add(q <= y[m] * M)

    # objective
    cost = sum(y[m]*data['cost'][m] for m in range(k))
    solver.Minimize(cost)
    solver.set_time_limit(time_limit * 1000)

    status = solver.Solve()
    if status == pywraplp.Solver.OPTIMAL or status == pywraplp.Solver.FEASIBLE:
        print('--------------Solution Found--------------')
        # for i in range(n):
        #     print(f'put item {i+1} with rotation {Ro[i].solution_value()} in car {z[i].solution_value()} at {l[i].solution_value()} {b[i].solution_value()} -> {r[i].solution_value()} {t[i].solution_value()}')
        print(f'Number of bin used  :',len(set(z[i].solution_value() for i in range(n))))
        # print(f'bin be used         :',*set(z[i].solution_value() for i in range(n)))
        print(f'Total cost          : {solver.Objective().Value()}')
        print('----------------Statistics----------------')
        if status == pywraplp.Solver.OPTIMAL:
            print('Status              : OPTIMAL')
        else:
            print('Status              : FEASIBLE')
        print(f'Time limit          : {time_limit}')
        print(f'Running time        : {solver.WallTime() / 1000}')
    


