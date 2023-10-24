#!/bin/bash
cols=`head -n 1 kmer_exist_output.txt | wc -w`
for (( i=1; i <= $cols; i++))
do 
    cut -f $i kmer_exist_output.txt | tr $'\n' " ">> output.phy
    echo "" >>output.phy
done
