#include <bits/stdc++.h>
using namespace  std;

int main(int argc, char const *argv[])
{
	
	int kmerSize = atoi(argv[1]);
	int noOfSpecies = atoi(argv[2]);
	int noOfLines = atoi(argv[3]);
	// No. of rows of data used for calculating entorpy
	int noOfRowsEntropy = 5000, temp;
	if(noOfRowsEntropy > noOfLines){
		noOfRowsEntropy = noOfLines;
	}
	vector<int> rowNumber;
	srand (123);

	for(int i=0; i<noOfRowsEntropy; i++){
		temp = rand() % noOfLines + 1 ;
		if(find(rowNumber.begin(), rowNumber.end(), temp) == rowNumber.end()){
		    rowNumber.push_back(temp);
		}
		else{
		    i--;
		}
	}
	sort(rowNumber.begin(), rowNumber.end());

	ifstream file("kmer_exist_output.txt");
	string line, line_new;
	int val[noOfSpecies];
	double count = 0, zero = 0, one = 0, one_new, zero_new,check = 0;
	double entropy = 0, shannon = 0, entropyFixed =0, shannonFixed = 0;
	int rowNumberIndex = 0;
	bool isRandomLine = false;
	while(getline(file,line)){
		zero = one = one_new = zero_new = 0;
		isRandomLine = false;
		if(rowNumberIndex == noOfRowsEntropy) break;
		if(count == rowNumber[rowNumberIndex]){
			rowNumberIndex++;
			isRandomLine = true;
		}
		count++;
		line_new = line;
		stringstream linestream(line);
		if(count == 1 || !isRandomLine) {
			continue;
		}
		// getline(linestream,data,'\t');
		for (int i = 0; i < noOfSpecies; ++i)
		{
			linestream >> val[i];
		}
		// linestream >> val[0] >> val[1] >> val[2] >> val[3] >> val[4] >> val[5];
		for (int i = 0; i < noOfSpecies; ++i)
		{
			
			if(val[i]) one++;
			else zero++;
		}
		
		
		if(zero){
			entropyFixed = entropyFixed + (-(zero/noOfSpecies)*(log2(zero/noOfSpecies) ) );
		}
		if(one){
			entropyFixed = entropyFixed + (-(one/noOfSpecies)*(log2(one/noOfSpecies) ) );
		}
	}
	
	ofstream entropyFixed_file;
	entropyFixed_file.open("../entropyRandomOutput.txt",ios_base::app);
	// freopen("../entropyOutput.txt","a+",stdout);
	// ofstream entropy_file("entropy_output.txt", std::ofstream::out | std::ofstream::app);
	entropyFixed_file<<kmerSize<<"	"<<entropyFixed<<"\n";
	// printf("whay\n");
	entropyFixed_file.close();

	
	return 0;
}