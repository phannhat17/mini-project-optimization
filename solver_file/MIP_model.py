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
    # Ro represent for R in the presentation file/ pdf model file
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

        for m in range(k):

            x[(i,m)] = solver.IntVar(0, 1, 'x_[%i]_[%i]' %(i,m))

            # item i must not exceed area of car
            solver.Add(r[i] <= (1-x[(i,m)]) * M + W_truck[m])
            solver.Add(l[i] <= (1-x[(i,m)]) * M + W_truck[m])
            solver.Add(t[i] <= (1-x[(i,m)]) * M + H_truck[m])
            solver.Add(b[i] <= (1-x[(i,m)]) * M + H_truck[m])    

    for i in range(n):
        solver.Add(sum(x[(i,m)] for m in range(k)) == 1)


    # if 2 items is packed in the same car, they must be not overlaped
    for i in range(n - 1):
        for j in range(i + 1, n):
            for m in range(k):
                e = solver.IntVar(0, 1, f'e[{i}][{j}]')
                solver.Add(e >= x[i,m] + x[j,m] - 1)
                solver.Add(e <= x[i,m])
                solver.Add(e <= x[j,m])

                # Binary variables for each constraint
                c1 = solver.IntVar(0, 1, f'c1[{i}][{j}]')
                c2 = solver.IntVar(0, 1, f'c2[{i}][{j}]')
                c3 = solver.IntVar(0, 1, f'c3[{i}][{j}]')
                c4 = solver.IntVar(0, 1, f'c4[{i}][{j}]')
                
                # Constraints that the binary variables must satisfy
                solver.Add(r[i] <= l[j] + M * (1 - c1))
                solver.Add(r[j] <= l[i] + M * (1 - c2))
                solver.Add(t[i] <= b[j] + M * (1 - c3))
                solver.Add(t[j] <= b[i] + M * (1 - c4))

                solver.Add(c1 + c2 + c3 + c4 + (1-e)*M >= 1 )
                solver.Add(c1 + c2 + c3 + c4 <= e*M )

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
    print(status)
    if status == pywraplp.Solver.OPTIMAL or status == pywraplp.Solver.FEASIBLE:
        print('--------------Solution Found--------------')
        # for i in range(n):
        #     print(f'put item {i+1} with rotation {int(Ro[i].solution_value())}', end=' ') 
        #     for j in range(k):
        #         if x[i,j].solution_value() ==1:
        #             print(f'in bin {j+1}', end=' ')
        #     print(f'at {l[i].solution_value()} {b[i].solution_value()} -> {r[i].solution_value()} {t[i].solution_value()}')
        print(f'Number of bin used  :',int(sum(y[m].solution_value() for m in range(k))))
        print(f'Total cost          : {solver.Objective().Value()}')
        print('----------------Statistics----------------')
        if status == pywraplp.Solver.OPTIMAL:
            print('Status              : OPTIMAL')
        else:
            print('Status              : FEASIBLE')
        print(f'Time limit          : {time_limit}')
        print(f'Running time        : {solver.WallTime() / 1000}')
    



