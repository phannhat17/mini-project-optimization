# If the pack is rotated, then change the width and height of the pack

from ortools.sat.python import cp_model
import sys, time

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

def main_solver(file_path, time_limit):
    n_packs, n_bins, packs, bins = read_input(file_path)
    max_width = max(x[0] for x in bins)
    max_height = max(x[1] for x in bins)
    
    # Creates the model
    model = cp_model.CpModel()
    time1 = time.time()


    # 
    # Variables
    # 
    # pack_in_bin[i, j] = 1 iff item i is packed in bin j.
    pack_in_bin = {}
    for i in range(n_packs):
        for j in range(n_bins):
            pack_in_bin[i, j] = model.NewBoolVar(f'pack_{i}_in_bin_{j}')

    # bin_is_used[j] = 1 iff bin j has been used.
    bin_is_used = [model.NewBoolVar(f'bin_{j}_is_used)') for j in range(n_bins)]

    # rotate[i] = 1 iff item i is rotated
    rotate = [model.NewBoolVar(f'package_{i}_rotated') for i in range(n_packs)]

    # 
    # Constraint
    # 
    # Each pack can only be placed in one bin
    for i in range(n_packs):
        model.Add(sum(pack_in_bin[i, j] for j in range(n_bins)) == 1)

    # 
    width = []
    height = []
    for i in range(n_packs):
        width.append(model.NewIntVar(0, max_width, f'width_{i}'))
        height.append(model.NewIntVar(0, max_height, f'height_{i}'))

        model.Add(width[i] == packs[i][0]).OnlyEnforceIf(rotate[i].Not())
        model.Add(width[i] == packs[i][1]).OnlyEnforceIf(rotate[i])
        model.Add(height[i] == packs[i][1]).OnlyEnforceIf(rotate[i].Not())
        model.Add(height[i] == packs[i][0]).OnlyEnforceIf(rotate[i])

    x = []
    y = []
    for i in range(n_packs):
        x.append(model.NewIntVar(0, max_width, f'x_{i}'))
        y.append(model.NewIntVar(0, max_height, f'y_{i}'))

    for i in range(n_packs):
        for j in range(n_bins):
            model.Add(x[i] <= bins[j][0]).OnlyEnforceIf(pack_in_bin[i, j])
            model.Add(y[i] <= bins[j][1]).OnlyEnforceIf(pack_in_bin[i, j])
            model.Add(x[i] >= width[i]).OnlyEnforceIf(pack_in_bin[i, j])
            model.Add(y[i] >= height[i]).OnlyEnforceIf(pack_in_bin[i, j])            

    # If 2 pack in the same bin they cannot overlap
    for i in range(n_packs):
        for k in range(i+1, n_packs):
                # check if the two packs are not overlapping
                a1 = model.NewBoolVar('a1')        
                model.Add(x[i] <= x[k] - width[k]).OnlyEnforceIf(a1)
                a2 = model.NewBoolVar('a2')        
                model.Add(y[i] <= y[k] - height[k]).OnlyEnforceIf(a2)
                a3 = model.NewBoolVar('a3')        
                model.Add(x[k] <= x[i] - width[i]).OnlyEnforceIf(a3)
                a4 = model.NewBoolVar('a4')        
                model.Add(y[k] <= y[i] - height[i]).OnlyEnforceIf(a4)

                for j in range(n_bins):
                    model.AddBoolOr(a1, a2, a3, a4).OnlyEnforceIf(pack_in_bin[i, j], pack_in_bin[k, j])
                    
    for j in range(n_bins):
        e = model.NewBoolVar('e')
        model.Add(sum(pack_in_bin[i, j] for i in range(n_packs)) == 0).OnlyEnforceIf(e)
        model.Add(bin_is_used[j] == 0).OnlyEnforceIf(e)
        model.Add(sum(pack_in_bin[i, j] for i in range(n_packs)) != 0).OnlyEnforceIf(e.Not())
        model.Add(bin_is_used[j] == 1).OnlyEnforceIf(e.Not())


    print(time.time()-time1)
    cost = sum(bin_is_used[j] * bins[j][2] for j in range(n_bins))
    model.Minimize(cost)
    solver = cp_model.CpSolver()
    solver.parameters.max_time_in_seconds = time_limit
    status = solver.Solve(model)

    if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
        print(solver.StatusName(status))
        print(f'time : {solver.UserTime()}')
        print(f'Number of car {sum(solver.Value(bin_is_used[i]) for i in range(n_bins))}')
        print(f'cost : {solver.ObjectiveValue()}')
        print(f'branches : {solver.NumBranches()}')
    else:
        print('NO SOLUTIONS')
if __name__ == "__main__":
    try:
        # Get input file path
        file_path = sys.argv[1]
    except IndexError:
        # Default input file if file path is not specified
        file_path = 'input_data/0350.txt'
    time_limit = 120
    main_solver(file_path, time_limit)
    
