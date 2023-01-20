#!/bin/bash

mode=$1         # CP1, CP2, MIP, HEU

attempt=$2      # attempt

time_limit=300                          # Time limit in second for the test

input_data_folder=input_data/           # input data folder

files=$(ls $input_data_folder/*.txt)    # List all *.txt files in input folder

if [ -z "$mode" ]; then
    echo "Missing solver mode"
    exit 1
else
    output_folder=results/result_${mode}_model      # output folder for results file

    output_csv=$output_folder/results_$attempt.csv  # csv file for results 

    mkdir -p $output_folder                         # Create output folder if it doesn't exist
fi

# Create the columns for the results file
echo "n_packs, n_bins, n_bins_used, cost, status, running_time, time_limit," > $output_csv

stats=("Number of bin used" "Total cost" "Status" "Running time")

for file in $files; do
    # Run the solver 
    if [ $mode == "CP1" ]; then 
        python solver_file/CP_model_solver/CP_model_1.py $file $time_limit > $output_folder/$(basename $file).out
    elif [ $mode == "CP2" ]; then 
        python solver_file/CP_model_solver/CP_model_2.py $file $time_limit > $output_folder/$(basename $file).out
    elif [ $mode == "MIP" ]; then 
        python solver_file/CP_model_solver/CP_model_2.py $file $time_limit > $output_folder/$(basename $file).out
    elif [ $mode == "HEU" ]; then 
        python solver_file/CP_model_solver/CP_model_2.py $file $time_limit > $output_folder/$(basename $file).out
    fi

    # Get the input number of packages and number of bins from input file
    head -n 1 $file | sed -E 's/ /,/g' | tr '\n' ',' >> $output_csv

    # Get stats from the solver, if not return blank 
    for item in "${stats[@]}"; do
        value=$(grep "$item" "$output_folder/$(basename $file).out" | awk '{print $NF}')
        if [[ -z "$value" && "$item" == "Status" ]]; then
            value="NO SOLUTION FOUND"
        fi
        echo "$value" | tr '\n' ',' >> $output_csv
    done

    echo $time_limit | tr '\n' ',' >> $output_csv       # Write the time_limit to result file

    echo >> $output_csv                                 # Add new line in the results file

    rm $output_folder/$(basename $file).out             # Remove the temporary output file
done
