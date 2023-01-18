#!/bin/bash


files=$(ls input_data | head -n 8)

echo "n_packs, n_bins, n_bins_used, cost, time_limit, running_time" > output/CP_model/results.csv
# Iterate through the first 5 files
for file in $files; do
    # Run CP_model_1.py with the current file as input
    python CP_model_solver/CP_model_1.py input_data/$file > output/CP_model/$(basename $file).out
    # Filter the specific lines from the output
    # echo -n $(basename $file) , >> output/CP_model/results.csv
    grep -E "Number of pack given|Number of car given|Number of car used|Total cost|Time limit|Running time" output/CP_model/$(basename $file).out | sed -E 's/[^0-9]+([0-9.]+).*/\1/g' | tr '\n' ',' >> output/CP_model/results.csv
    echo >> output/CP_model/results.csv
    #remove the output file
    rm output/CP_model/$(basename $file).out
done
