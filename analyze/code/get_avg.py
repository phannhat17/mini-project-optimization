import pandas as pd

resuslt_CP1_path = ["results/results_CP1/results_CP1_300_1.csv", "results/results_CP1/results_CP1_300_2.csv", "results/results_CP1/results_CP1_300_3.csv"]

resuslt_CP2_path = ["results/results_CP2/results_CP2_300_1.csv", "results/results_CP2/results_CP2_300_2.csv", "results/results_CP2/results_CP2_300_3.csv"]

resuslt_MIP_path = ["results/results_MIP/results_MIP_300_1.csv", "results/results_MIP/results_MIP_300_2.csv", "results/results_MIP/results_MIP_300_3.csv"]

resuslt_HEU_path = ["results/results_HEU/results_HEU_1.csv", "results/results_HEU/results_HEU_2.csv", "results/results_HEU/results_HEU_3.csv"]


all_paths = [resuslt_CP1_path, resuslt_CP2_path, resuslt_MIP_path]

def get_data(list_path):
    cost = []
    run_time = []
    for file in list_path:
        with open(file, "r") as f:
            n_packs = []
            n_bins = []
            _cost=[]
            _run_time=[]
            data = f.readlines()
            data.pop(0)
            for line in data:
                values = line.strip().split(",")
                if 'NO SOLUTION FOUND' not in line:
                    _cost.append(float(values[3]))
                    _run_time.append(float(values[5]))
            
                    n_packs.append(values[0])
                    n_bins.append(values[1])
            cost.append(_cost)
            run_time.append(_run_time)

    return n_packs, n_bins, cost, run_time

def get_avg(data):
    avgs = []
    for i in range(len(data[1])):
        avg = (data[0][i] + data[1][i] + data[2][i]) / 3

        avgs.append(avg)

    return avgs

def write_csv(path_in, path_out):
    
    n_packs, n_bins, cost, run_time = get_data(path_in)

    avg_cost = get_avg(cost)
    avg_time = get_avg(run_time)

    df = pd.DataFrame({'n_packs': n_packs,
                   'n_bins': n_bins,
                   'cost': avg_cost,
                   'run_time': avg_time})

    df.to_csv(path_out, index=False)

write_csv(resuslt_CP1_path, 'results/results_CP1/results_CP1_300_avg.csv')
write_csv(resuslt_CP2_path, 'results/results_CP2/results_CP2_300_avg.csv')
write_csv(resuslt_MIP_path, 'results/results_MIP/results_MIP_300_avg.csv')
write_csv(resuslt_HEU_path, 'results/results_HEU/results_HEU_avg.csv')


