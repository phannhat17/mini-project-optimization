import matplotlib.pyplot as plt

resuslt_path = ["results/results_CP1/results_CP1_300_1.csv", "results/results_CP1/results_CP1_300_2.csv", "results/results_CP1/results_CP1_300_3.csv","results/results_CP2/results_CP2_300_1.csv", "results/results_CP2/results_CP2_300_2.csv", "results/results_CP2/results_CP2_300_3.csv","results/results_MIP/results_MIP_300_1.csv", "results/results_MIP/results_MIP_300_2.csv", "results/results_MIP/results_MIP_300_3.csv"]

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

# compare CP1
fig1, ax1 = plt.subplots(figsize=(15,9))
ax1.plot(n_packs[0], cost[0], "-", label = "CP1_1")
ax1.plot(n_packs[1], cost[1], "-", label = "CP1_2")
ax1.plot(n_packs[2], cost[2], "-", label = "CP1_3")
axins = ax1.inset_axes([0.1, 0.55, 0.4, 0.4])
axins.plot(n_packs[0][:47], cost[0][:47], "-")
axins.plot(n_packs[1][:47], cost[1][:47], "-")
axins.plot(n_packs[2][:47], cost[2][:47], "-")

x1_1, x2_1, y1_1, y2_1 = 3, 61, 0, 630
axins.set_xlim(x1_1, x2_1)
axins.set_ylim(y1_1, y2_1)
axins.set_xticklabels([])
axins.set_yticklabels([])
ax1.indicate_inset_zoom(axins)

ax1.set_ylabel('Cost')
ax1.set_xlabel('Number of items')
plt.legend()
plt.savefig('analyze/CP1.png')


# compare CP2
fig2, ax2 = plt.subplots(figsize=(15,9))
ax2.plot(n_packs[3], cost[3], "-", label = "CP2_1")
ax2.plot(n_packs[4], cost[4], "-", label = "CP2_2")
ax2.plot(n_packs[5], cost[5], "-", label = "CP2_3")
axins1 = ax2.inset_axes([0.1, 0.55, 0.4, 0.4])
axins1.plot(n_packs[3][:47], cost[3][:47], "-")
axins1.plot(n_packs[4][:47], cost[4][:47], "-")
axins1.plot(n_packs[5][:47], cost[5][:47], "-")

x1_2, x2_2, y1_2, y2_2 = 3, 61, 0, 630
axins1.set_xlim(x1_2, x2_2)
axins1.set_ylim(y1_2, y2_2)
axins1.set_xticklabels([])
axins1.set_yticklabels([])
ax2.indicate_inset_zoom(axins1)

ax2.set_ylabel('Cost')
ax2.set_xlabel('Number of items')
plt.legend()
plt.savefig('analyze/CP2.png')



# compare MIP
fig4, ax4 = plt.subplots(figsize=(15,9))
ax4.plot(n_packs[6][:41], cost[6][:41], "-", label = "MIP_1")
ax4.plot(n_packs[7][:41], cost[7][:41], "-", label = "MIP_2")
ax4.plot(n_packs[8][:41], cost[8][:41], "-", label = "MIP_3")

ax4.set_ylabel('Cost')
ax4.set_xlabel('Number of items')
plt.legend()
plt.savefig('analyze/MIP.png')