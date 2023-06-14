# Peafowl
----------------------------------------------------------------------------------------------

Peafowl is an alignment free, k-mer based tool for phylogenetic tree construction.

Peafowl runs on 64-bit Intel-compatible processors supporting Linux. This implementation has been tested on ubuntu 16.04 LTS, Intel Core-i5, 8 GB RAM. Greater memory is preferable for large genome sequences.

Requirements
----------------------------------------------------------------------------------------------
Peafowl requires Jellyfish 2.2.4 and Raxml 8.2.4 tools. They can be installed on ubuntu 16.04 LTS using the following terminal commands.

1. jellyfish/xenial, 2.2.4-2 <br>
  - sudo apt-get update <br>
  - sudo apt-get install jellyfish <br>

2. raxml/xenial, 8.2.4-1 <br>
  - sudo apt-get update
  - sudo apt-get install raxml


Input and Output Formats
----------------------------------------------------------------------------------------------
**Input**: Set of DNA sequences in fasta format (Baboon.fasta, Gibbon.fasta, etc) A fasta file should contain the sequence of one specie only. Data should be sequential long-reads/full genome. Current version does not support short-reads or interleaved data. (See example folder for sample files) <br>
**Ouput**: Rooted and unrooted phylogenetic trees in Newick format for the set of input species (Result_rooted_tree.newick, Result_unrooted_tree.newick)


Installation and Run
----------------------------------------------------------------------------------------------
1. Download 'Peafowl' from this repository.
2. Create a new folder and put all fasta files (ex: Baboon.fasta, Gorilla.fasta, etc.) inside. Rename the folder (Do not use space/dot in the name and do not use the reserved word 'Peafowl'). This folder name should be provided as an argument when running the script. 
3. Put the folder with fasta files inside 'Peafowl' directory that contains a script 'peafowl.sh'. This directory should now have three separate entities ('your input folder', 'Peafowl', 'peafowl.sh').
4. From the terminal, change into the directory that contains 'peafowl.sh'.
5. Run the script 'peafowl.sh' with the following arguments as needed.

Peafowl Argument Options
----------------------------------------------------------------------------------------------
Usage: ./peafowl.sh -i input_folder [ -n no_of_threads ] [ -r ] [ -a ]

-i input_folder <br>
'-i' stands for input. 'input_folder' is the folder that contains more than one fasta files. This argument is compulsory for the program to run. 'input_folder' should not contain any space/dot character and should not have the reserved name 'KPhylo'.

[ -n no_of_threads ] <br>
'-n' stands for number of threads. 'no_of_threads' is the number of threads to be used for k-mer counting. This argument is optional. If used, specified value should be an integer. Default value of 'no_of_threads' is 16.

[ -r ] <br>
'-r' stands for reverse-complement. Use '-r' if input fasta files contain neocleotides from both single and double strand of DNA. Ignore if fasta files contain neocleotides from single strand DNA only. Default assumes single strand DNA data. This argument is optional.

[ -a ] <br>
'-a' stands for all-k-mer. Use '-a' to see all resultant trees from k-mer size 9 to 31. Default shows resultant tree for k-mer length corresponding to maximum entropy. This argument is optional.


Result
----------------------------------------------------------------------------------------------
If the run is successful, you will see a result folder created in the same location as the input. If input folder was named 'example', the result folder created will be 'example_Result'. This result folder should contain two trees (rooted and unrooted) in Newick format. Output trees are constructed from k-mer length yielding the maximum entropy.


Example Run
----------------------------------------------------------------------------------------------
A folder named 'example' is already provided with the source files. It contains 7 sample fasta files. Copy this folder to the 'Peafowl' directory containing the script 'kphylo.sh'. Change to this directory and execute the following command.

./peafowl.sh -i example -t 16 -r


Caution
----------------------------------------------------------------------------------------------
1. Input folder name should not include any space character, use underscore instead.
2. Fasta files should have the extension '.fasta' and the names of species should not include any space or dot character. Ex: 'C.chimp.fasta'/'C Chimp.fasta' will give indefinite results. 'C_chimp.fasta' will work.
3. The value of number of threads should be an integer.
4. If sequence files are large in size, computation can take a while to complete. Use kernels with larger memory if possible.

Citation
----------------------------------------------------------------------------------------------
1. Alexandros Stamatakis. Raxml version 8: a tool for phylogenetic analysis and post-analysis of large phylogenies. Bioinformatics, 30(9):1312–1313, 2014.

2. Qingpeng Zhang, Jason Pell, Rosangela Canino-Koning, Adina Chuang Howe, and C Titus Brown. These are not the k-mers you are looking for: efficient online k-mer counting using a probabilistic data structure. PloS one, 9(7):e101271, 2014.

3. https://howtoinstall.co/en/ubuntu/xenial/jellyfish

4. https://howtoinstall.co/en/ubuntu/xenial/raxml
