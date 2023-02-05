import matplotlib.pyplot as plt

resuslt_path = ["results/results_CP1/results_CP1_300_2.csv", "results/results_CP2/results_CP2_300_1.csv", "results/results_MIP/results_MIP_300_1.csv", "results/results_HEU/results_HEU_1.csv"]

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
            _cost.append(float(values[3]))
            _run_time.append(float(values[6]))
        n_packs.append(_n_packs)
        cost.append(_cost)
        run_time.append(_run_time)
        success_tests.append(len(remove_fail))
        fail_tests.append(len(data)-len(remove_fail))


# compare all
fig, ax = plt.subplots(figsize=(15, 9))
ax.plot(n_packs[0], cost[0], "-", label = "CP1")
ax.plot(n_packs[1], cost[1], "-", label = "CP2")
ax.plot(n_packs[2], cost[2], "-", label = "MIP")
ax.plot(n_packs[3][:58], cost[3][:58], "-", label = "Heuristic")
axins = ax.inset_axes([0.55, 0.05, 0.4, 0.4])
axins.plot(n_packs[0][:47], cost[0][:47], "-")
axins.plot(n_packs[1][:47], cost[1][:47], "-")
axins.plot(n_packs[2][:47], cost[2][:47], "-")
axins.plot(n_packs[3][:47], cost[3][:47], "-")

x1, x2, y1, y2 = 3, 61, 0, 630
axins.set_xlim(x1, x2)
axins.set_ylim(y1, y2)
axins.set_xticklabels([])
axins.set_yticklabels([])
ax.indicate_inset_zoom(axins)

ax.set_ylabel('Total cost')
ax.set_xlabel('Test size')
plt.title('Total cost (lower is better)')
plt.legend()
plt.savefig('analyze/compare_cost_all.png')


# zoomed in n_packs < 100 compare exact vs heuristics
fig, ax = plt.subplots(figsize=(15, 9))
ax.plot(n_packs[0][:47], cost[0][:47], "-", label = "CP1")
ax.plot(n_packs[1][:47], cost[1][:47], "-", label = "CP2")
ax.plot(n_packs[2][:47], cost[2][:47], "-", label = "MIP")
ax.plot(n_packs[3][:47], cost[3][:47], "-", label = "Heuristic")

ax.set_ylabel('Total cost')
ax.set_xlabel('Test size')
ax.set_ylim(0,620)
plt.title('Total cost (lower is better)')
plt.legend()
plt.savefig('analyze/zoomed_compare_cost_all.png')



# 
# 
# 
# compare CP vs heuristics
fig, ax = plt.subplots(figsize=(15, 9))
ax.plot(n_packs[0], cost[0], "-", label = "CP1")
ax.plot(n_packs[1], cost[1], "-", label = "CP2")
ax.plot(n_packs[3][:58], cost[3][:58], "-", label = "Heuristic")
axins = ax.inset_axes([0.55, 0.05, 0.4, 0.4])
axins.plot(n_packs[0][:47], cost[0][:47], "-")
axins.plot(n_packs[1][:47], cost[1][:47], "-")
axins.plot(n_packs[3][:47], cost[3][:47], "-")

x1, x2, y1, y2 = 3, 61, 0, 630
axins.set_xlim(x1, x2)
axins.set_ylim(y1, y2)
axins.set_xticklabels([])
axins.set_yticklabels([])
ax.indicate_inset_zoom(axins)

ax.set_ylabel('Total cost')
ax.set_xlabel('Test size')
plt.title('Total cost (lower is better)')
plt.legend()
plt.savefig('analyze/compare_cost_CP_heuristic.png')


# zoomed in n_packs < 100 compare CP vs heuristics
fig, ax = plt.subplots(figsize=(15, 10))
ax.plot(n_packs[0][:47], cost[0][:47], "-", label = "CP1")
ax.plot(n_packs[1][:47], cost[1][:47], "-", label = "CP2")
ax.plot(n_packs[3][:47], cost[3][:47], "-", label = "Heuristic")

ax.set_ylabel('Total cost')
ax.set_xlabel('Test size')
plt.title('Total cost (lower is better)')
plt.legend()
plt.savefig('analyze/zoomed_compare_cost_CP_heuristic.png')



# 
# 
# 
# compare run time of fisrt 25 input
fig, ax = plt.subplots(figsize=(10,5))
ax.plot(n_packs[0][:25], run_time[0][:25], "-", label = "CP1")
ax.plot(n_packs[1][:25], run_time[1][:25], "-", label = "CP2")
ax.plot(n_packs[2][:25], run_time[2][:25], "-", label = "MIP")
ax.plot(n_packs[3][:25], run_time[3][:25], "-", label = "Heuristic")
ax.set_ylabel('Run time (s)')
ax.set_xlabel('Test size')
plt.title('Run time (lower is better)')
plt.legend()
plt.savefig('analyze/compare_run_time_all_first_25_test.png')



