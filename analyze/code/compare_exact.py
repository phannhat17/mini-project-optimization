import matplotlib.pyplot as plt

resuslt_path = ["results/results_CP1/results_CP1_300_1.csv", "results/results_MIP/results_MIP_300_1.csv", "results/results_HEU/results_HEU_1.csv"]

success_tests = []
fail_tests = []
n_packs = []
cost = []
run_time = []

# get data
for path in resuslt_path:
    with open(path, "r") as f:
        _n_packs=[]
        _cost=[]
        _run_time=[]
        data = f.readlines()
        data.pop(0)
        remove_fail = []
        for line in data:
            if 'NO SOLUTION FOUND' not in line:
                remove_fail.append(line)
        for line in remove_fail:
            if "FEASIBLE" not in line:
                values = line.strip().split(",")
                _n_packs.append(int(values[0]))
                _cost.append(float(values[3]))
                _run_time.append(float(values[6]))
            else:
                _n_packs.append(int(values[0]))
                _cost.append(None)
                _run_time.append(float(values[6]))
        n_packs.append(_n_packs)
        cost.append(_cost)
        run_time.append(_run_time)
        success_tests.append(len(remove_fail))
        fail_tests.append(len(data)-len(remove_fail))



# compare only exact
fig, ax = plt.subplots(figsize=(15, 9))
ax.plot(n_packs[0][:13], cost[0][:13], "-", label = "CP")
ax.plot(n_packs[1][:13], cost[1][:13], "-", label = "MIP")
ax.plot(n_packs[2][:13], cost[2][:13], "-", label = "Heuristic")

ax.set_ylabel('Total cost')
ax.set_xlabel('Test size')
plt.title('Total cost (lower is better)')
plt.legend()
plt.savefig('analyze/compare_only_exact.png')
# plt.show()