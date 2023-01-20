#!/bin/bash

cd ../  # Change to root folder of the project 

mode=$1 # CP, MIP


output_folder=results/$mode_model   # output folder for results file
input_data_folder=input_data/       # input data folder


files=$(ls $input_data/*.txt)       # List all *.txt files in input folder
time_limit=300                      # Time limit for the test


mkdir -p $output_folder             # Create output folder if it doesn't exist


# Create the columns for the results file
echo "n_packs, n_bins, n_bins_used, cost, time_limit, running_time, status," > $output_folder/results.csv 


for file in $files; do

    # Get the input number of packages and number of bins from input file
    head -n 1 $input_data_folder/$file | sed -E 's/ /,/g' | tr '\n' ',' >> $output_folder/results.csv

    # Run the solver 
    if [ $mode == "CP" ]; then 
        python solver_file/CP_model_solver/CP_model_1.py $input_data_folder/$file > $output_folder/$(basename $file).out
    fi

    # Get the "Number of car used" value and write to the result file, if not found, write empty string
    grep -q 'Number of car used' $output_folder/$(basename $file).out
    if [ $? -eq 0 ]; then
        awk '/Number of car used/ {gsub(/[^0-9.]+/, ""); print $0}' $output_folder/$(basename $file).out | tr '\n' ',' >> $output_folder/results.csv
    else
        echo "" | tr '\n' ','>> $output_folder/results.csv
    fi

    # Get the "Total cost" value and write to the result file, if not found, write empty string
    grep -q 'Total cost' $output_folder/$(basename $file).out
    if [ $? -eq 0 ]; then
        awk '/Total cost/ {gsub(/[^0-9.]+/, ""); print $0}' $output_folder/$(basename $file).out | tr '\n' ',' >> $output_folder/results.csv
    else
        echo ""| tr '\n' ',' >> $output_folder/results.csv
    fi

    # Write the time_limit to result file
    echo $time_limit| tr '\n' ',' >> $output_folder/results.csv


    # Get the "Running time" value and write to the result file, if not found, write empty string
    grep -q 'Running time' $output_folder/$(basename $file).out
    if [ $? -eq 0 ]; then
        awk '/Running time/ {gsub(/[^0-9.]+/, ""); print $0}' $output_folder/$(basename $file).out | tr '\n' ',' >> $output_folder/results.csv
    else
        echo "" | tr '\n' ',' >> $output_folder/results.csv
    fi

    # Get the "Status" value and write to the result file, if not found, write "NO SOLUTION" 
    grep -q 'Status' $output_folder/$(basename $file).out
    if [ $? -eq 0 ]; then
        awk '/Status/ {gsub(/Status.*:/, ""); print $0}' $output_folder/$(basename $file).out | tr '\n' ',' >> $output_folder/results.csv
    else
        echo "NO SOLUTION" | tr '\n' ',' >> $output_folder/results.csv
    fi

    # New line in the results file
    echo >> $output_folder/results.csv

    # Remove the temporary output file
    rm $output_folder/$(basename $file).out

done
