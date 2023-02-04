# Multiple-type, two-dimensional finite bin packing problem
This is a mini project for topic 3 in Fundamental of Optimization course of SoICT - HUST
## Problem
There are `K` trucks `1,2,...,K` for transporting `N` packages of `1, 2, ..., N`. Trucks have a container size of `Wk * Lk`. Each package `I` has size `wi * li`. Packages that are placed in same container must not overlap. Assume that the number K can be large, leading to a large number of trucks that are not being used. The cost of using truck `k` is `ck`. Find a way to put these package in the trucks so that **the total cost is minimal**.  

The input data format and how we generated them can be found [here](./input_data/README.md) 

## Modeling the problem
- CP model: Details are written in [this file](CP_model.pdf)
- MIP model: Details are written in [this file](MIP_model.pdf)
- Heuristic: Details are written in [this file](Heuristic.pdf)

## Folder structure
```
.
├── analyze                 # contains some analysis information
│   └── ...
├── CP_model.pdf            # how we model the problem
├── MIP_model.pdf
├── Heuristic.pdf
├── assets
├── figure                  # contains generated figures
│   ├── generated_CP
│   │   └── ...
│   |── generated_HEU
│   |   └── ...
│   └── gen_figure.py       # figure generator
├── input_data              # contains generated data
│   └── ...
├── presentation
├── results                 # contains results from solver
│   └── ...
├── script                  # script file for collect result and gen figure
│   └── ...
└── solver_file             # contains solver files
    ├── CP_model_solver
    │   └── ...
    ├── Heuristic
    │   └── ...
    └── MIP_model.py
```

## Results
Here we are only collecting results based on **number of bins used and total cost** so while collecting we omitted the code that prints out how the packages are sorted into the bins **(this affects quite a bit the actual runtime of the solver)**.
- The results for each model are shown in the `results` folder.
- The overview of the results can be found [here](./results/results.pdf)
- CP and MIP model solvers only receive input data up to **600 packages**   
- You can use google colab to run this project like [this](https://colab.research.google.com/drive/1ouxqr2eeJTfJou74Oxw4Syih_zFGgm2p?usp=sharing)    

If you want to collect results by yourself, you can run the `collect_results` script by this command in the **root dir** of this repository:
```
./script/collect_results.sh {mode} {attempt}
```
Available solver modes: `CP1`, `CP2`, `MIP`, `HEU`

Example:
```
./script/collect_results.sh HEU 1
```  
The commmand above will collect the results created by [heuristic_main](/solver_file/Heuristic/) in the `1st attempt`     
  
![Example](./assets/example.gif)  
  
**Note:**   
**- Read the script for more details**  
**- Change the attempt number for each attempt or the results will rewrite each other**

## Analysis
- CP cannot handle data sets larger than 240. 
- MIP cannot handle data sets larger than 44.
- Heuristic can handle all test cases (the largest test size being 10,000).

Comparing results (total cost):
- MIP gives the **worst** results.
- CP1 and CP2 give **nearly equivalent** results, but with larger data sets, CP2 gives better results.
- Heuristic gives really good results, with tests of size <100 it is still a bit inferior to CP, but for all other tests it is significantly better than CP.

![Cost](./analyze/compare_cost_all.png)
![Cost](./analyze/zoomed_compare_cost_all.png)

Comparing run time:

- MIP reaches the time limit for all tests with size >= 15.
- CP reaches the time limit for all tests with size >= 22.
- Heuristic has a very short run time, mostly under 1 second even for test size 10,000 it takes only about 5 seconds to get the result.

![Example](./analyze/compare_run_time_all_first_25_test.png)

Therefore,

- MIP is not good in terms of results and run time,
- CP is better than MIP, with better results and faster run time (in some early tests),
- Heuristic gives the best results in both cost and run time.

## Figure
We have generated 13 figures (data from 7 to 24) for the results of CP solver **(because the running time is quite long for the large input data)** and all figures for Heuristic solver (No MIP because it take so long)    
  
You can generate by running this command:  
```
./script/gen_figure.sh {mode}
```
**Note: Currently, the script can only generate figure for the CP and Heuristic solver** 
  
![Example](./figure/generated_CP/0011/bin_10.png)  

