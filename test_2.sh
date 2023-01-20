#!/bin/bash

# files=$(ls input_data | head -n 5)
files=$(ls generated_data)
time_limit=300

echo "n_packs, n_bins, n_bins_used, cost, time_limit, running_time, status," > output/CP_model/results.csv

for file in $files; do

    head -n 1 input_data/$file | sed -E 's/ /,/g' | tr '\n' ',' >> output/CP_model/results.csv

    python CP_model_solver/CP_model_2.py input_data/$file > output/CP_model/$(basename $file).out


    grep -q 'Number of car used' output/CP_model/$(basename $file).out
    if [ $? -eq 0 ]; then
        awk '/Number of car used/ {gsub(/[^0-9.]+/, ""); print $0}' output/CP_model/$(basename $file).out | tr '\n' ',' >> output/CP_model/results.csv
    else
        echo "" | tr '\n' ','>> output/CP_model/results.csv
    fi

    grep -q 'Total cost' output/CP_model/$(basename $file).out
    if [ $? -eq 0 ]; then
        awk '/Total cost/ {gsub(/[^0-9.]+/, ""); print $0}' output/CP_model/$(basename $file).out | tr '\n' ',' >> output/CP_model/results.csv
    else
        echo ""| tr '\n' ',' >> output/CP_model/results.csv
    fi


    echo $time_limit| tr '\n' ',' >> output/CP_model/results.csv

    grep -q 'Running time' output/CP_model/$(basename $file).out
    if [ $? -eq 0 ]; then
        awk '/Running time/ {gsub(/[^0-9.]+/, ""); print $0}' output/CP_model/$(basename $file).out | tr '\n' ',' >> output/CP_model/results.csv
    else
        echo "" | tr '\n' ',' >> output/CP_model/results.csv
    fi

    grep -q 'Status' output/CP_model/$(basename $file).out
    if [ $? -eq 0 ]; then
        awk '/Status/ {gsub(/Status.*:/, ""); print $0}' output/CP_model/$(basename $file).out | tr '\n' ',' >> output/CP_model/results.csv
    else
        echo "NO SOLUTION" | tr '\n' ',' >> output/CP_model/results.csv
    fi


    echo >> output/CP_model/results.csv

    rm output/CP_model/$(basename $file).out

done
