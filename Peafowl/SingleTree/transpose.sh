#!/bin/bash
cols=`head -n 1 kmer_exist_output.txt | wc -w`

bin_len=$(wc -l < "kmer_exist_output.txt")
bin_len=`expr $bin_len - 1`
echo "$cols $bin_len" > output.phy

for (( i=1; i <= $cols; i++))
do 
    cut -f $i kmer_exist_output.txt | tr $'\n' " ">> output.phy
    echo "" >>output.phy
done
