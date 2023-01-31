import matplotlib.pyplot as plt


resuslt_path = ["results/results_CP1/results_CP1_300_1.csv", "results/results_MIP/results_MIP_300_1.csv", "results/results_HEU/results_HEU_1.csv"]

for path in resuslt_path:
    with open(path, "r") as f:
        n_packs = []
        cost = []
        data = f.readlines()
        data.pop(0)
        temp = []
        for line in data:
            if 'NO SOLUTION FOUND' not in line:
                temp.append(line)
        for line in temp:
            values = line.strip().split(",")
            n_packs.append(int(values[0]))
            cost.append(float(values[3]))

        plt.figure(figsize=(9, 9))
        plt.plot(n_packs, cost, ".")
        plt.xlabel("number of packs")
        plt.ylabel("total cost")
        plt.title("n_packs vs cost")
        if "CP" in path:
            plt.savefig('analyze/n_packs_vs_cost_CP.png')
        elif "MIP" in path:
            plt.savefig('analyze/n_packs_vs_cost_MIP.png')
        else:
            plt.savefig('analyze/n_packs_vs_cost_HEU.png')

    with open(path, "r") as f:
        n_packs = []
        run_time = []
        data = f.readlines()
        data.pop(0)
        temp = []
        for line in data:
            if 'NO SOLUTION FOUND' not in line:
                temp.append(line)
        for line in temp:       
            values = line.strip().split(",")
            n_packs.append(int(values[0]))
            run_time.append(float(values[6]))

        plt.figure(figsize=(9, 9))
        plt.plot(n_packs, run_time, ".")
        plt.xlabel("number of packs")
        plt.ylabel("Running time")
        plt.title("n_packs vs run_time")
        if "CP" in path:
            plt.savefig('analyze/n_packs_vs_run_time_CP.png')
        elif "MIP" in path:
            plt.savefig('analyze/n_packs_vs_run_time_MIP.png')
        else:
            plt.savefig('analyze/n_packs_vs_run_time_HEU.png')
