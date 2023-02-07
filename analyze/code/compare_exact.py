import matplotlib.pyplot as plt

resuslt_path = ["results/results_CP1/results_CP1_300_1.csv","results/results_CP2/results_CP2_300_1.csv", "results/results_MIP/results_MIP_300_1.csv", "results/results_HEU/results_HEU_1.csv"]

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
            values = line.strip().split(",")
            _n_packs.append(int(values[0]))
            _run_time.append(float(values[6]))
            if "FEASIBLE" not in line:
                _cost.append(float(values[3]))
            else:
                _cost.append(None)
        n_packs.append(_n_packs)
        cost.append(_cost)
        run_time.append(_run_time)
        success_tests.append(len(remove_fail))
        fail_tests.append(len(data)-len(remove_fail))



# compare only exact
fig, ax = plt.subplots(figsize=(15, 9))
ax.plot(n_packs[0][:13], cost[0][:13], "-", label = "Exact")
ax.plot(n_packs[3][:13], cost[3][:13], "-", label = "Heuristic")

ax.set_ylabel('Total cost')
ax.set_xlabel('Number of items')
plt.title('Total cost (lower is better)')
plt.legend()
plt.savefig('analyze/compare_only_exact.png')
# plt.show()
print(run_time[0][:13])
print(n_packs[0][:13])

# compare only exact
fig, ax = plt.subplots(figsize=(15, 9))
ax.plot(n_packs[0][:13], run_time[0][:13], "-", label = "CP1")
ax.plot(n_packs[1][:13], run_time[1][:13], "-", label = "CP2")
ax.plot(n_packs[2][:13], run_time[2][:13], "-", label = "MIP")

ax.set_ylabel('Run time')
ax.set_xlabel('Number of items')
plt.title('Run time (lower is better)')
plt.legend()
plt.savefig('analyze/compare_time_only_exact.png')
# plt.show()