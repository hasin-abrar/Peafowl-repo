#!/bin/bash

tree=''
no_of_threads=16
source_folder='' # required
is_reverse_compliment='false'
is_top_kmer='true'

usage() {                                      # Function: Print a help message.
  echo "The arguments are expected as follows: -i input_folder [ -n no_of_threads ] [ -r ] [ -a ] " 
}
exit_abnormal() {                              # Function: Exit with error.
  usage
  exit 1
}

while getopts ":i:t:n:ra" options; do              
  case "${options}" in                         
    i)                                         
      source_folder="${OPTARG}"                  
      ;;
    t)                                         
      tree=${OPTARG}                           
      ;;
    n)                                         
      no_of_threads=${OPTARG}                  
      ;;
    r)                                         
      is_reverse_compliment='true'          
      ;;
    a)                                         
      is_top_kmer='false'          
      ;;
    :)                                         
      echo "Error: -${OPTARG} requires an argument."
      exit_abnormal                            
      ;;
    *)                                         
      exit_abnormal                            
      ;;
  esac
done

if [ "$source_folder" = "" ]; then             
  echo 'Error! No input folder provided!'
  exit_abnormal                          
fi

if [ ! -d "$source_folder" ]; then
    echo "Input folder not found!"
    exit_abnormal
fi

if [[ $is_top_kmer = 'true' ]]; then
	cp Peafowl/SingleTree/* .
else
	cp Peafowl/All_kmer/* .
fi

# echo "$tree"
# echo $no_of_threads
# echo $source_folder
# echo $is_reverse_compliment

./main_script.sh "$tree" $no_of_threads "$source_folder" "$is_reverse_compliment"

find . -maxdepth 1 -type f -not -name "peafowl.sh" -delete

echo 'Process completed successfully'