#!/bin/bash

for (( i=$1;i<=$2;i+=2 ))
do
	outputPath="./KmerOutputs/KmerOutputs""_$i"
	mkdir "$outputPath"	
	./final_run.sh "$i" "$outputPath" "$3" "$4" "$5" "$6"
done
# '
g++ -Ofast findMaxEntropy.cpp -o findMaxEntropy -lpthread
kmer=$(./findMaxEntropy)

exit "$kmer"