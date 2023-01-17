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

if __name__ == "__main__":
    try:
        # Get input file path
        file_path = sys.argv[1]
    except IndexError:
        # Default input file if file path is not specified
        file_path = 'input_data/0020.txt'
    n_packs, n_bins, packs, bins = read_input(file_path)
    max_width = max(x[0] for x in bins)
    max_height = max(x[1] for x in bins)
    
    # Creates the model
    model = cp_model.CpModel()

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

    x = []
    y = []
    for i in range(n_packs):
        x.append(model.NewIntVar(0, max_width, f'x_{i}'))
        y.append(model.NewIntVar(0, max_height, f'y_{i}'))

    for i in range(n_packs):
        for j in range(n_bins):
            model.Add(x[i] <= bins[j][0]).OnlyEnforceIf(pack_in_bin[i, j])
            model.Add(y[i] <= bins[j][1]).OnlyEnforceIf(pack_in_bin[i, j])
            # add constraints to fix coordinates when pack i is placed in bin j
            model.Add(x[i] >= packs[i][0]).OnlyEnforceIf(pack_in_bin[i, j], rotate[i].Not())
            model.Add(y[i] >= packs[i][1]).OnlyEnforceIf(pack_in_bin[i, j], rotate[i].Not())
            model.Add(x[i] >= packs[i][1]).OnlyEnforceIf(pack_in_bin[i, j], rotate[i])
            model.Add(y[i] >= packs[i][0]).OnlyEnforceIf(pack_in_bin[i, j], rotate[i])            

    # If 2 pack in the same bin they cannot overlap
    for i in range(n_packs):
        for k in range(i+1, n_packs):
            for j in range(n_bins):
                # check if the two packs are not overlapping
                a1 = model.NewBoolVar('a1')        
                model.Add(x[i] <= x[k] - packs[k][0]).OnlyEnforceIf(a1)
                a2 = model.NewBoolVar('a2')        
                model.Add(y[i] <= y[k] - packs[k][1]).OnlyEnforceIf(a2)
                a3 = model.NewBoolVar('a3')        
                model.Add(x[k] <= x[i] - packs[i][0]).OnlyEnforceIf(a3)
                a4 = model.NewBoolVar('a4')        
                model.Add(y[k] <= y[i] - packs[i][1]).OnlyEnforceIf(a4)

                model.AddBoolOr(a1, a2, a3, a4).OnlyEnforceIf(pack_in_bin[i, j], pack_in_bin[k, j], rotate[i].Not(), rotate[k].Not())


                b1 = model.NewBoolVar('b1')                        
                model.Add(x[i] <= x[k] - packs[k][1]).OnlyEnforceIf(b1)
                b2 = model.NewBoolVar('b2')                        
                model.Add(y[i] <= y[k] - packs[k][0]).OnlyEnforceIf(b2)
                b3 = model.NewBoolVar('b3')                        
                model.Add(x[k] <= x[i] - packs[i][0]).OnlyEnforceIf(b3)
                b4 = model.NewBoolVar('b4')                        
                model.Add(y[k] <= y[i] - packs[i][1]).OnlyEnforceIf(b4)

                model.AddBoolOr(b1, b2, b3, b4).OnlyEnforceIf(pack_in_bin[i, j], pack_in_bin[k, j], rotate[i].Not(), rotate[k])

                c1 = model.NewBoolVar('c1')                        
                model.Add(x[i] <= x[k] - packs[k][0]).OnlyEnforceIf(c1)
                c2 = model.NewBoolVar('c2')                        
                model.Add(y[i] <= y[k] - packs[k][1]).OnlyEnforceIf(c2)
                c3 = model.NewBoolVar('c3')                        
                model.Add(x[k] <= x[i] - packs[i][1]).OnlyEnforceIf(c3)
                c4 = model.NewBoolVar('c4')                        
                model.Add(y[k] <= y[i] - packs[i][0]).OnlyEnforceIf(c4)

                model.AddBoolOr(c1, c2, c3, c4).OnlyEnforceIf(pack_in_bin[i, j], pack_in_bin[k, j], rotate[i], rotate[k].Not())


                d1 = model.NewBoolVar('d1')                        
                model.Add(x[i] <= x[k] - packs[k][1]).OnlyEnforceIf(d1)
                d2 = model.NewBoolVar('d2')                        
                model.Add(y[i] <= y[k] - packs[k][0]).OnlyEnforceIf(d2)
                d3 = model.NewBoolVar('d3')                        
                model.Add(x[k] <= x[i] - packs[i][1]).OnlyEnforceIf(d3)
                d4 = model.NewBoolVar('d4')                        
                model.Add(y[k] <= y[i] - packs[i][0]).OnlyEnforceIf(d4)

                model.AddBoolOr(d1, d2, d3, d4).OnlyEnforceIf(pack_in_bin[i, j], pack_in_bin[k, j], rotate[i], rotate[k])


    for j in range(n_bins):
        e = model.NewBoolVar('e')
        model.Add(sum(pack_in_bin[i, j] for i in range(n_packs)) == 0).OnlyEnforceIf(e)
        model.Add(bin_is_used[j] == 0).OnlyEnforceIf(e)
        model.Add(sum(pack_in_bin[i, j] for i in range(n_packs)) != 0).OnlyEnforceIf(e.Not())
        model.Add(bin_is_used[j] == 1).OnlyEnforceIf(e.Not())




    cost = sum(bin_is_used[j] * bins[j][2] for j in range(n_bins))
    model.Minimize(cost)


    solver = cp_model.CpSolver()
    status = solver.Solve(model)

    print(solver.StatusName(status))
    print(f'time : {solver.UserTime()}')
    print(f'Number of car {sum(solver.Value(bin_is_used[i]) for i in range(n_bins))}')
    print(f'cost : {solver.ObjectiveValue()}')
    print(f'branches : {solver.NumBranches()}')
