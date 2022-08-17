#!/bin/bash
kmer_len=$1
output=$2
species_no=$3
no_of_threads=$4
source_folder=$5
is_reverse_compliment=$6

echo 'Starting counting with jellyfish...'

cd "$source_folder" #change sample file name
ls

for f in *
do

	FILENAME=$f
	FILESIZE=$(stat -c%s "$FILENAME")
	n=${FILENAME%.*}
	x='.jf'
	y='.txt'

	p=$n$x
	q=$n$y

	if [ "$is_reverse_compliment" = "false" ]; then
		jellyfish count -m $kmer_len -s $FILESIZE -t $no_of_threads $FILENAME -o $p
	else
		jellyfish count -m $kmer_len -s $FILESIZE -t $no_of_threads -C $FILENAME -o $p
	fi



	jellyfish dump $p > $q
	jellyfish dump $p > $q

done

# echo 'Completed counting with jellyfish'

cp ../kmerMerge.cpp ../"$output"/
cp ../entropy.cpp ../"$output"/
find -name '*.txt' -exec cp -t ../"$output" "{}" +

g='/'

for f in *.txt
do
   filearg+=$f
   filearg+=$g
done


find . -name "*.txt" -type f -delete
find . -name "*.jf" -type f -delete

cd ..

cd "$output" #change output filename

echo 'Constructing binary matrix...'

sizeFile="FileSize.txt"
stat -c %s *.txt > $sizeFile

g++ -std=c++11 kmerMerge.cpp -o kmerMerge -lpthread
./kmerMerge $filearg $kmer_len $no_of_threads $sizeFile

# echo 'Completed binary matrix'

# actual length + 1
bin_len=$(wc -l < "kmer_exist_output.txt")
# echo $bin_len

echo 'Starting entropy calculation...'
g++ entropy.cpp -o entropy -lpthread
./entropy "$1" "$3" $bin_len
# echo 'Completed entropy calculation'

echo -n 'Completed calculation for k-mer size: '
echo $kmer_len

