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

shopt -s extglob

# Following codes are used when tree is not estimated for all k-mer values
# calculate entropy to get best kmer length
mkdir "KmerOutputs"
./differentKmerEntropy.sh $entropy_lower $entropy_higher $specie_no $no_of_threads "$source_folder" $is_reverse_compliment
kmer_len="$?"

echo ''
echo -n "Tree will be constructed for k-mer size: "
echo $kmer_len

cp "KmerOutputs/KmerOutputs""_$kmer_len""/kmer_exist_output.txt" "./kmer_exist_output.txt"

./transpose.sh
# extract nsequnce length from kmer_exist_output.txt file (last line number with content - 1)
bin_len=$(wc -l < "kmer_exist_output.txt")

bin_len=`expr $bin_len - 1`

#add number of species and seq length in the first line of transpose output, "output.phy"
sed -i "1i \ $specie_no $bin_len" "output.phy"

# run raxml with BINGAMMA
raxmlHPC -m BINGAMMA -p 12345 -s output.phy -n T1
#raxmlHPC -m BINCAT -p 12345 -s output.phy -n T1
raxmlHPC -m BINGAMMA -p 12345 -n "$out_name" -f I -t RAxML_result.T1

# echo 'Executing Ktreedist...'

# perl Ktreedist.pl -r -n -s scaled_output_main.txt -rt "$tree" -ct "RAxML_rootedTree.""$out_name" >> comparison_result.txt

cp "KmerOutputs/entropyRandomOutput.txt" "./entropyRandomOutput.txt"

echo 'Deleting unncessary files...'

rm -r KmerOutputs
rm -r "$source_folder"
rm !("RAxML_rootedTree.""$out_name"|"RAxML_result.T1"|"entropyRandomOutput.txt")

mv "RAxML_result.T1" "Result_unrooted_tree.newick"
mv "RAxML_rootedTree.$out_name" "Result_rooted_tree.newick"

