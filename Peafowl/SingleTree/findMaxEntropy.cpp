#include <bits/stdc++.h>
using namespace std;

int main(int argc, char const *argv[])
{
	ifstream file("./KmerOutputs/entropyRandomOutput.txt");
	string line;
	double kmerSize, entropyVal, maxKmer;
	double maxEntropy = -1;
	while(getline(file,line)){
		stringstream linestream(line);
		linestream >> kmerSize >> entropyVal;
		if(entropyVal > maxEntropy){
			maxKmer = kmerSize;
			maxEntropy = entropyVal;
		}
	}
	cout << maxKmer <<endl;
	return 0;
}