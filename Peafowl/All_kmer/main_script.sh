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

#put that in a new folder (ex: "Primates_Results") for further processing
# file="${source_folder}_Results_All_kmer"
file=$(echo "$source_folder" | sed 's:/*$::')
file="$file""_Results_All_kmer"
# echo "$file"
mkdir "$file"

cp -r "$source_folder" "$file"

# copying necessary files inside the sample folder
# cp "$tree" "$file"
cp "kmerMerge.cpp" "$file"
cp "transpose.sh" "$file"
cp "entropy.cpp" "$file"
cp "final_run.sh" "$file"
# file for computing distances
# cp "Ktreedist.pl" "$file"

cd "$file"

shopt -s extglob

for (( i=$entropy_lower;i<=$entropy_higher;i+=2 ))
do
	# creating a new folder to run jellyfish
	out_name="Tree_kmer_""$i"
	output="output_kmer_""$i"
	mkdir $output
	# copying necessary files inside the "output" folder
	cp "kmerMerge.cpp" "$output"
	cp "transpose.sh" "$output"
	cp "entropy.cpp" "$output"

	echo -n 'Now processing kmer size: '
	echo $i
	kmer_len=$i

	# this computes the k-mer existence
	./final_run.sh $kmer_len $output $specie_no $no_of_threads "$source_folder" $is_reverse_compliment

	cd "$output"

	./transpose.sh

	# extract nsequnce length from kmer_exist_output.txt file (last line number with content - 1)
	bin_len=$(wc -l < "kmer_exist_output.txt")

	bin_len=`expr $bin_len - 1`

	#add number of species and seq length in the first line of transpose output, "output.phy"
	sed -i "1i \ $specie_no $bin_len" "output.phy"

	echo "Estimating tree..."

	# run raxml with BINGAMMA
	raxmlHPC -m BINGAMMA -p 12345 -s output.phy -n T1

	raxmlHPC -m BINGAMMA -p 12345 -n "$out_name" -f I -t RAxML_result.T1


	cd ..


	# cp "$tree" "$output"
	# cp "Ktreedist.pl" "$output"

	cd "$output"

	# echo 'Executing Ktreedist...'


	# perl Ktreedist.pl -r -n -s scaled_output_main.txt -rt "$tree" -ct "RAxML_rootedTree.""$out_name" >> comparison_output_main.txt

	# perl Ktreedist.pl -p partition_output_main.txt -rt "$tree" -ct "RAxML_rootedTree.""$out_name"

	echo 'Removing unnecessary files...'

	rm !("RAxML_rootedTree.""$out_name"|"RAxML_result.T1")
	mv "RAxML_result.T1" "Result_unrooted_tree.newick"
	mv "RAxML_rootedTree.$out_name" "Result_rooted_tree.newick"

	cd ..

	echo -n 'Tree estimated for kmer size: '
	echo $i
	echo ''
	echo '---------------------------------'
	echo ''
done

#delete all files in current folder except entropy list and subdirectories
find . -maxdepth 1 -type f -not -name "entropyRandomOutput.txt" -delete
rm -r "$source_folder"