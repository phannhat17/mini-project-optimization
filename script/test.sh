#!/bin/bash


mode=$1         # CP, MIP
attempt=$2      # attempt


output_folder=results/result_${mode}_model      # output folder for results file
output_csv=$output_folder/results_$attempt.csv  # csv file for results 


input_data_folder=input_data/                   # input data folder


files=$(ls $input_data_folder/*.txt)            # List all *.txt files in input folder


time_limit=300                                  # Time limit for the test


mkdir -p $output_folder                         # Create output folder if it doesn't exist


# Create the columns for the results file
echo "n_packs, n_bins, n_bins_used, cost, status, running_time, time_limit," > $output_csv

stats=("Number of bin used" "Total cost" "Status" "Running time")

for file in $files; do

    # Get the input number of packages and number of bins from input file
    head -n 1 $file | sed -E 's/ /,/g' | tr '\n' ',' >> $output_csv

    # Run the solver 
    if [ $mode == "CP" ]; then 
        python solver_file/CP_model_solver/CP_model_1.py $file > $output_folder/$(basename $file).out
    fi

    # Get stats from the solver, if not return blank 
    for item in "${stats[@]}"; do
        value=$(grep "$item" "$output_folder/$(basename $file).out" | awk '{print $NF}')
        echo "$value" | tr '\n' ',' >> $output_csv
    done

    # Write the time_limit to result file
    echo $time_limit | tr '\n' ',' >> $output_csv    

    echo >> $output_csv                         # Add new line in the results file

    rm $output_folder/$(basename $file).out     # Remove the temporary output file

done
