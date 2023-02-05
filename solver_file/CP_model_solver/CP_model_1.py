# This code is based on the CP model in the root folder of the project:
# If the pack is rotated, change the current coordinate


from ortools.sat.python import cp_model
import sys
def read_input(file_path):
    '''
        Return number of packs (n_packs); number of bins (n_bins); 
        and two tuple of pack's width and height; bin's width, height and cost.

        file_path: path to file containing the input data
    '''
    with open(file_path) as f:
        data = f.readlines()
        n_packs, n_bins = map(int, data[0].split())
        packs, bins = [], []

        for i in range(1, n_packs+1):
            packs.append(tuple(map(int, data[i].split())))

        for i in range(n_packs+1, len(data)):
            bins.append(tuple(map(int, data[i].split())))

    return n_packs, n_bins, packs, bins

def main_solver(file_path, time_limit: int = 600):
    n_packs, n_bins, packs, bins = read_input(file_path)

    max_width = max(x[0] for x in bins)
    max_height = max(x[1] for x in bins)

    # Creates the model
    model = cp_model.CpModel()

    # 
    # Variables
    # 
    # X[i, j] = 1 iff item i is packed in bin j.
    X = {}
    for i in range(n_packs):
        for j in range(n_bins):
            X[i, j] = model.NewBoolVar(f'pack_{i}_in_bin_{j}')

    # Z[j] = 1 iff bin j has been used.
    Z = [model.NewBoolVar(f'bin_{j}_is_used)') for j in range(n_bins)]

    # R[i] = 1 iff item i is rotated
    R = [model.NewBoolVar(f'package_{i}_rotated') for i in range(n_packs)]

    # Coordinate
    r = [] # right coordinate
    l = [] # left coordinate
    t = [] # top coordinate
    b = [] # bottom coordinate
    for i in range(n_packs):
        r.append(model.NewIntVar(0, max_width, f'r_{i}'))
        l.append(model.NewIntVar(0, max_width, f'l_{i}'))
        t.append(model.NewIntVar(0, max_height, f't_{i}'))
        b.append(model.NewIntVar(0, max_height, f'b_{i}'))

        # if pack rotated -> change the current coordinate
        model.Add(r[i] ==  l[i] + packs[i][0]).OnlyEnforceIf(R[i].Not())
        model.Add(r[i] ==  l[i] + packs[i][1]).OnlyEnforceIf(R[i])
        model.Add(t[i] ==  b[i] + packs[i][1]).OnlyEnforceIf(R[i].Not())
        model.Add(t[i] ==  b[i] + packs[i][0]).OnlyEnforceIf(R[i])

    # 
    # Constraint
    # 
    # Each pack can only be placed in one bin
    for i in range(n_packs):
        model.Add(sum(X[i, j] for j in range(n_bins)) == 1)

    # if pack in bin, it cannot exceed the bin size
    for i in range(n_packs):
        for j in range(n_bins):
            model.Add(r[i] <= bins[j][0]).OnlyEnforceIf(X[i, j])
            model.Add(t[i] <= bins[j][1]).OnlyEnforceIf(X[i, j])

    # If 2 pack in the same bin they cannot overlap
    for i in range(n_packs):
        for k in range(i+1, n_packs):
            a1 = model.NewBoolVar('a1')        
            model.Add(r[i] <= l[k]).OnlyEnforceIf(a1)
            model.Add(r[i] > l[k]).OnlyEnforceIf(a1.Not())
            a2 = model.NewBoolVar('a2')        
            model.Add(t[i] <= b[k]).OnlyEnforceIf(a2)
            model.Add(t[i] > b[k]).OnlyEnforceIf(a2.Not())
            a3 = model.NewBoolVar('a3')        
            model.Add(r[k] <= l[i]).OnlyEnforceIf(a3)
            model.Add(r[k] > l[i]).OnlyEnforceIf(a3.Not())
            a4 = model.NewBoolVar('a4')        
            model.Add(t[k] <= b[i]).OnlyEnforceIf(a4)
            model.Add(t[k] > b[i]).OnlyEnforceIf(a4.Not())

            for j in range(n_bins):
                model.AddBoolOr(a1, a2, a3, a4).OnlyEnforceIf(X[i, j], X[k, j])

    # Find which bin has been used
    for j in range(n_bins):
        b1 = model.NewBoolVar('b')
        model.Add(sum(X[i, j] for i in range(n_packs)) == 0).OnlyEnforceIf(b1)
        model.Add(Z[j] == 0).OnlyEnforceIf(b1)
        model.Add(sum(X[i, j] for i in range(n_packs)) != 0).OnlyEnforceIf(b1.Not())
        model.Add(Z[j] == 1).OnlyEnforceIf(b1.Not())

    # Objective function
    cost = sum(Z[j] * bins[j][2] for j in range(n_bins))
    model.Minimize(cost)

    # Creates a solver and solves the model
    solver = cp_model.CpSolver()
    solver.parameters.max_time_in_seconds = time_limit
    status = solver.Solve(model)

    # Print the results
    print('----------------Given data----------------')
    print(f'Number of pack given: {n_packs}')
    print(f'Number of bin given : {n_bins}')
    if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
        print('--------------Solution Found--------------')

        # Uncomment if you want to see the way to put packages in bins
        # Not necessary in the statistics, so we comment it out
        for i in range(n_packs):
            if solver.Value(R[i]) == 1:
                print(f'Rotate pack {i+1} and put', end=' ')
            else:
                print(f'Put pack {i+1}', end=' ')
            for j in range(n_bins):
                if solver.Value(X[i, j]) == 1:
                    print(f'in bin {j+1}', end=' ')
            print(f'that the top right corner coordinate (x, y) is ({solver.Value(r[i])}, {solver.Value(t[i])})')

        print(f'Number of bin used  : {sum(solver.Value(Z[i]) for i in range(n_bins))}')
        print(f'Total cost          : {solver.ObjectiveValue()}')
        print('----------------Statistics----------------')
        print(f'Status              : {solver.StatusName(status)}')
        print(f'Time limit          : {time_limit}')
        print(f'Running time        : {solver.UserTime()}')
        print(f'Explored branches   : {solver.NumBranches()}')
    else:
        print('NO SOLUTIONS')

if __name__ == "__main__":
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

    main_solver(file_path, time_limit)
    
