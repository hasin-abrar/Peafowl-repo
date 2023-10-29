#!/bin/bash
#initialize parameters
tree="$1"
# range of entropy
entropy_lower=9
entropy_higher=31
no_of_threads=$2

source_folder="$3"
is_reverse_compliment=$4
specie_no=$(ls "$source_folder" | wc -l) # number of species

#Copy necessary files to result folder (ex: "Primates_Results")
source_folder=$(echo "$source_folder" | sed 's:/*$::')
output="$source_folder"
output+="_Result"
out_name="Resultant_Tree"
mkdir "$output"

#TODO: Have to change the code not to copy contents from src fldr
cp -r "$source_folder" "$output"

# copying necessary files inside the sample folder
# cp "$tree" "$output"
cp "kmerMerge.cpp" "$output"
cp "transpose.sh" "$output"
cp "entropy.cpp" "$output"
cp "findMaxEntropy.cpp" "$output"
cp "differentKmerEntropy.sh" "$output"
cp "final_run.sh" "$output"
# file for computing distances
# cp "Ktreedist.pl" "$output"

cd "$output"

# shopt -s extglob

# Following codes are used when tree is not estimated for all k-mer values
# calculate entropy to get best kmer length
mkdir "KmerOutputs"
./differentKmerEntropy.sh $entropy_lower $entropy_higher $specie_no $no_of_threads "$source_folder" $is_reverse_compliment
kmer_len="$?"

echo ''
echo -n "Tree will be constructed for k-mer size: "
echo $kmer_len

mv "KmerOutputs/KmerOutputs""_$kmer_len""/kmer_exist_output.txt" "./kmer_exist_output.txt"

# bin_len=$(wc -l < "kmer_exist_output.txt")
# bin_len=`expr $bin_len - 1`
# echo "$specie_no $bin_len" >> output.phy

./transpose.sh
# extract nsequnce length from kmer_exist_output.txt file (last line number with content - 1)


#add number of species and seq length in the first line of transpose output, "output.phy"
# sed -i "1i \ $specie_no $bin_len" "output.phy"

# run raxml with BINGAMMA
# with avx support
# raxmlHPC-PTHREADS-AVX -m BINGAMMA -p 12345 -T $no_of_threads -s output.phy -n T1
# if no avx support
raxmlHPC-PTHREADS -m BINGAMMA -p 12345 -T $no_of_threads -s output.phy -n T1

#raxmlHPC -m BINCAT -p 12345 -s output.phy -n T1
# raxmlHPC-PTHREADS-AVX -m BINGAMMA -p 12345 -T $no_of_threads -n "$out_name" -f I -t RAxML_result.T1

# echo 'Executing Ktreedist...'

# perl Ktreedist.pl -r -n -s scaled_output_main.txt -rt "$tree" -ct "RAxML_rootedTree.""$out_name" >> comparison_result.txt

mv "KmerOutputs/entropyRandomOutput.txt" "./entropyRandomOutput.txt"

echo 'Deleting unncessary files...'

rm -r KmerOutputs
rm -r "$source_folder"
rm "differentKmerEntropy.sh" findMaxEntropy.cpp entropy.cpp final_run.sh
rm kmer_exist_output.txt output.phy findMaxEntropy kmerMerge.cpp transpose.sh
rm RAxML_info.T1 RAxML_parsimonyTree.T1 RAxML_bestTree.T1 RAxML_log.T1

mv "RAxML_result.T1" "Result_unrooted_tree_Kmer_"$kmer_len".newick"
# mv "RAxML_rootedTree.$out_name" "Result_rooted_tree.newick"
cd ..
rm differentKmerEntropy.sh final_run.sh kmerMerge.cpp entropy.cpp findMaxEntropy.cpp
rm main_script.sh transpose.sh
