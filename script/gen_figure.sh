#!/bin/bash

mode=$1    

input_data_folder=input_data/                               # input data folder

if [ -z "$mode" ]; then
    echo "Missing solver mode"
    exit 1
elif [[ ! "$mode" =~ "CP" && ! "$mode" =~ "HEU" ]]; then
    echo "Invalid solver mode"
    exit 1  
else

    output_folder=figure/generated_${mode}             

    if [ $mode == "HEU" ]; then 

        files=$(ls $input_data_folder/*.txt)           # List 51 first *.txt files in input folder

    else

        files=$(ls $input_data_folder/*.txt | head -n 13)           # List 16 first *.txt files in input folder

    fi

    mkdir -p $output_folder                                         # Create output folder if it doesn't exist
fi


for file in $files; do
    
    out_put_for_each_file=$output_folder/$(basename ${file%.*})

    mkdir -p $out_put_for_each_file

    echo "Generating figure for $(basename $file)"

    # Run the solver 
    if [ $mode == "CP" ]; then 
        python solver_file/CP_model_solver/CP_model_2.py $file $time_limit | sed -n '/Solution Found/,/Number of bin used/p' | sed '/Solution Found\|Number of bin used/d' > $output_folder/$(basename $file).out 
    else
        ./solver_file/Heuristic/heuristic_main $file | sed -n '/Number of bin given:/,/Number of bin used/p' | sed '/Number of bin given:\|Number of bin used/d' > $output_folder/$(basename $file).out 
    fi
    # Gen figure
    python figure/gen_figure.py $file $output_folder/$(basename $file).out $out_put_for_each_file

    rm $output_folder/$(basename $file).out  

done
