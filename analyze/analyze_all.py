import matplotlib.pyplot as plt

resuslt_path = ["results/results_CP1/results_CP1_300_1.csv", "results/results_MIP/results_MIP_300_1.csv", "results/results_HEU/results_HEU_1.csv"]

labels = ['CP', 'MIP', 'HEURISTIC']
success_tests = []
fail_tests = []
n_packs = []
cost = []

for path in resuslt_path:
    with open(path, "r") as f:
        _n_packs=[]
        _cost=[]
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
        n_packs.append(_n_packs)
        cost.append(_cost)
        success_tests.append(len(remove_fail))
        fail_tests.append(len(data)-len(remove_fail))

fig1, ax1 = plt.subplots(figsize=(8, 8))

ax1.bar(labels, success_tests, 0.5, label='Success tests')
ax1.bar(labels, fail_tests, 0.5, bottom=success_tests, label='Fail tests')

ax1.set_ylim([0, success_tests[2] + fail_tests[2] + 10])
ax1.set_ylabel('Tests')
ax1.legend()
plt.savefig('analyze/compare_number_success.png')



labels2 = ['Success', 'Fail']
    
fracs = []
for i in range(len(success_tests)):
    ss_frac = round(100*success_tests[i]/(success_tests[i]+fail_tests[i]),2)
    f_frac = round(100-ss_frac,2)
    fracs.append([
        ss_frac, f_frac
    ])

fig2, axs = plt.subplots(1, 2, figsize=(8, 8))

axs[0].pie(fracs[0], autopct='%1.1f%%', radius=1.2)
axs[0].set_title('CP')
axs[1].pie(fracs[1], autopct='%1.1f%%', radius=1.2)
axs[1].set_title('MIP')
fig2.legend(labels=labels2, loc='lower center', bbox_to_anchor=(0.5, 0.3))

plt.savefig('analyze/percentage.png')


fig3, ax3 = plt.subplots(figsize=(15, 9))
ax3.plot(n_packs[0], cost[0], "-", label = "CP")
ax3.plot(n_packs[2][:58], cost[2][:58], "-", label = "Heuristic")
axins = ax3.inset_axes([0.55, 0.05, 0.4, 0.4])
axins.plot(n_packs[0][:47], cost[0][:47], "-")
axins.plot(n_packs[2][:47], cost[2][:47], "-")

# sub region of the original image
x1, x2, y1, y2 = 3, 61, 0, 630
axins.set_xlim(x1, x2)
axins.set_ylim(y1, y2)
axins.set_xticklabels([])
axins.set_yticklabels([])
ax3.indicate_inset_zoom(axins)
ax3.set_ylabel('Total cost')
ax3.set_xlabel('Number of packs')
plt.legend()
plt.savefig('analyze/compare_exact_heuristic.png')
