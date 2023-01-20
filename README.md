# Multiple-type, two-dimensional finite bin packing problem
This is a mini project for Fundamental of Optimization course of SoICT - HUST
## Problem
There are `K` trucks `1,2,...,K` for transporting `N` packages of `1, 2, ..., N`. Trucks have a container size of `Wk * Lk`. Each package `I` has size `wi * li`. Packages that are placed in same container must not overlap. Assume that the number K can be large, leading to a large number of trucks that are not being used. The cost of using truck `k` is `ck`. Find a way to put these package in the trucks so that **the total cost is minimal**.  

The input data format and how we generated them can be found [here](./input_data/README.md) 

## Modeling the problem
- CP model: Details are written in [this file](CP_model.pdf)
- MIP model:
- Heuristic: 

## The results
We collect the results by running `collect_results` on `google colab`. Specs of the wm:
```
Intel(R) Xeon(R) CPU @ 2.20GHz
Ram: ~12.68GB
```
- The results for each model are shown in the `results` folder.
- If you want to collect results by yourself, you can run the `collect_results` script by this command:
```
./script/collect_results.sh {mode} {attempt}
```
Example:
```
./script/collect_results.sh CP 1
```  
The commmand above will collect the results created by `CP_model solver` in the `1st attempt`  
**Note: if you dont change the attempt number, the results will rewrite each other**