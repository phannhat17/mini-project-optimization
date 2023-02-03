import matplotlib.pyplot as plt

resuslt_path = ["results/results_CP1/results_CP1_300_1.csv", "results/results_CP1/results_CP1_300_2.csv", "results/results_CP2/results_CP2_300_1.csv", "results/results_CP2/results_CP2_300_2.csv", "results/results_MIP/results_MIP_300_1.csv", "results/results_MIP/results_MIP_300_2.csv"]

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

# compare CP1_1 vs CP1_2
fig1, ax1 = plt.subplots(figsize=(15,9))
ax1.plot(n_packs[0], cost[0], "-", label = "CP1_1")
ax1.plot(n_packs[1], cost[1], "-", label = "CP1_2")
axins = ax1.inset_axes([0.1, 0.55, 0.4, 0.4])
axins.plot(n_packs[0][:47], cost[0][:47], "-")
axins.plot(n_packs[1][:47], cost[1][:47], "-")

x1_1, x2_1, y1_1, y2_1 = 3, 61, 0, 630
axins.set_xlim(x1_1, x2_1)
axins.set_ylim(y1_1, y2_1)
axins.set_xticklabels([])
axins.set_yticklabels([])
ax1.indicate_inset_zoom(axins)

ax1.set_ylabel('Cost')
ax1.set_xlabel('Number of items')
plt.legend()
plt.savefig('analyze/CP1_1_vs_CP1_2.png')


# compare CP2_1 vs CP2_2
fig2, ax2 = plt.subplots(figsize=(15,9))
ax2.plot(n_packs[2], cost[2], "-", label = "CP2_1")
ax2.plot(n_packs[3], cost[3], "-", label = "CP2_2")
axins1 = ax2.inset_axes([0.1, 0.55, 0.4, 0.4])
axins1.plot(n_packs[2][:47], cost[2][:47], "-")
axins1.plot(n_packs[3][:47], cost[3][:47], "-")

x1_2, x2_2, y1_2, y2_2 = 3, 61, 0, 630
axins1.set_xlim(x1_2, x2_2)
axins1.set_ylim(y1_2, y2_2)
axins1.set_xticklabels([])
axins1.set_yticklabels([])
ax2.indicate_inset_zoom(axins1)

ax2.set_ylabel('Cost')
ax2.set_xlabel('Number of items')
plt.legend()
plt.savefig('analyze/CP2_1_vs_CP2_2.png')


# => CP1_2 better than CP1_1 and CP2_2 better than CP2_1

# compare CP1_2 vs CP2_2
fig3, ax3 = plt.subplots(figsize=(15,9))
ax3.plot(n_packs[1], cost[1], "-", label = "CP1_2")
ax3.plot(n_packs[3], cost[3], "-", label = "CP2_2")
axins2 = ax3.inset_axes([0.1, 0.55, 0.4, 0.4])
axins2.plot(n_packs[1][:47], cost[1][:47], "-")
axins2.plot(n_packs[3][:47], cost[3][:47], "-")

x1_3, x2_3, y1_3, y2_3 = 3, 61, 0, 630
axins2.set_xlim(x1_3, x2_3)
axins2.set_ylim(y1_3, y2_3)
axins2.set_xticklabels([])
axins2.set_yticklabels([])
ax3.indicate_inset_zoom(axins2)

ax3.set_ylabel('Cost')
ax3.set_xlabel('Number of items')
plt.legend()
plt.savefig('analyze/CP1_2_vs_CP2_2.png')

# compare MIP1_2 vs MIP2_2
fig4, ax4 = plt.subplots(figsize=(15,9))
ax4.plot(n_packs[4][:41], cost[4][:41], "-", label = "MIP1_2")
ax4.plot(n_packs[5][:41], cost[5][:41], "-", label = "MIP2_2")

ax3.set_ylabel('Cost')
ax3.set_xlabel('Number of items')
plt.legend()
plt.savefig('analyze/MIP1_2_vs_MIP2_2.png')