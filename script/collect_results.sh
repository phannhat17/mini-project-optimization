#!/bin/bash

mode=$1         # CP1, CP2, MIP, HEU

attempt=$2      # attempt

time_limit=300                          # Time limit in second for the test

input_data_folder=input_data/           # input data folder

if [ -z "$mode" ]; then
    echo "Missing solver mode"
    exit 1
else
    output_folder=results/result_${mode}_model                      # output folder for results file
    if [ $mode == "HEU" ]; then 
        files=$(ls $input_data_folder/*.txt)    # List all *.txt files in input folder

        output_csv=$output_folder/results_${mode}_$attempt.csv      # csv file for results 
    else
        files=$(ls $input_data_folder/*.txt | head -n 58)    # List 57 first *.txt files in input folder

        output_csv=$output_folder/results_${mode}_${time_limit}_$attempt.csv 
    fi

    mkdir -p $output_folder                                         # Create output folder if it doesn't exist
fi

# Create the columns for the results file
echo "n_packs, n_bins, n_bins_used, cost, status, solver_running_time, real_running_time, time_limit_for_solver," > $output_csv

stats=("Number of bin used" "Total cost" "Status" "Running time" "Real running Time")

for file in $files; do
    # Run the solver 
    echo "Working with $(basename $file)"
    if [ $mode == "CP1" ]; then 
        /usr/bin/time -f "Real running Time: %e" -ao $output_folder/$(basename $file).out  python solver_file/CP_model_solver/CP_model_1.py $file $time_limit > $output_folder/$(basename $file).out 
    elif [ $mode == "CP2" ]; then 
        /usr/bin/time -f "Real running Time: %e" -ao $output_folder/$(basename $file).out  python solver_file/CP_model_solver/CP_model_2.py $file $time_limit > $output_folder/$(basename $file).out 
    elif [ $mode == "MIP" ]; then 
        /usr/bin/time -f "Real running Time: %e" -ao $output_folder/$(basename $file).out  python solver_file/MIP_model.py $file $time_limit > $output_folder/$(basename $file).out 
    elif [ $mode == "HEU" ]; then 
        /usr/bin/time -f "Real running Time: %e" -ao $output_folder/$(basename $file).out  ./solver_file/Heuristic/Guillotine/heuristic_guillotine $file > $output_folder/$(basename $file).out 
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
    if  [ $mode == "HEU" ]; then
        echo "None" | tr '\n' ','>> $output_csv
    else
        echo $time_limit | tr '\n' ',' >> $output_csv       # Write the time_limit to result file
    fi

    echo >> $output_csv                                 # Add new line in the results file

    rm $output_folder/$(basename $file).out             # Remove the temporary output file
done
