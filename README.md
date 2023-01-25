# Multiple-type, two-dimensional finite bin packing problem
This is a mini project for Fundamental of Optimization course of SoICT - HUST
## Problem
There are `K` trucks `1,2,...,K` for transporting `N` packages of `1, 2, ..., N`. Trucks have a container size of `Wk * Lk`. Each package `I` has size `wi * li`. Packages that are placed in same container must not overlap. Assume that the number K can be large, leading to a large number of trucks that are not being used. The cost of using truck `k` is `ck`. Find a way to put these package in the trucks so that **the total cost is minimal**.  

The input data format and how we generated them can be found [here](./input_data/README.md) 

## Modeling the problem
- CP model: Details are written in [this file](CP_model.pdf)
- MIP model:
- Heuristic: 

## Folder structure
```
.
├── CP_model.pdf            # how we model the problem
├── data_gen.py             # input data generator
├── input_data              # contains generated data
│   ├── 0010.txt
│   └── ...
├── license
├── README.md
├── results                 # contains results from solver
│       ...
├── script                  # script file for collect result and analyze
│   └── collect_results.sh
└── solver_file
    ├── CP_model_solver
    │       ...
    ├── Heuristic
    │       ...
    └── MIP_model.py
```

## The results
- The results for each model are shown in the `results` folder.
- If you want to collect results by yourself, you can run the `collect_results` script by this command:
```
./script/collect_results.sh {mode} {attempt}
```
Example:
```
./script/collect_results.sh MIP 1
```  
The commmand above will collect the results created by `MIP solver` in the `1st attempt`  
**Note:**   
**- Read the script for more details**  
**- Change the attempt number for each attempt or the results will rewrite each other**


