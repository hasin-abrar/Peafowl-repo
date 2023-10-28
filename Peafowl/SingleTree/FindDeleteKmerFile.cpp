#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
	ifstream file("./KmerOutputs/entropyRandomOutput.txt");
	string line;
	double entropyVal, maxKmer;
    int32_t kmerSize;
    // max (31-9)/2 = 12 elements
    vector<pair<int32_t, double>> kmer_ent_vec;
	double maxEntropy = -1;
	while(getline(file,line)){
		stringstream linestream(line);
		linestream >> kmerSize >> entropyVal;
        kmer_ent_vec.push_back(make_pair(kmerSize, entropyVal));
	}
    
    string maxFn = "MaxEntropyFile.txt";
    int32_t consd_kmer, del_kmer, picked_kmer = 0;
    double maxEn;
    if(fopen(maxFn.c_str(), "r")){
        /**
         * Format
         * Considered_upto maxEntropy delete_kmer
        */
        ifstream maxfile(maxFn.c_str());
        
        getline(maxfile, line);
        stringstream ss(line);
        ss>>consd_kmer>>maxEn>>del_kmer;
        for(int64_t i=0; i<kmer_ent_vec.size(); i++){
            if(kmer_ent_vec[i].first > consd_kmer){
                if(kmer_ent_vec[i].second > maxEn){
                    maxEn = kmer_ent_vec[i].second;
                    picked_kmer = del_kmer;
                }
                else{
                    picked_kmer = kmer_ent_vec[i].first;
                }
                consd_kmer = kmer_ent_vec[i].first;
            }
        }
        maxfile.close();
        ofstream wrtF(maxFn.c_str());
        // cout<<consd_kmer<<" "<<maxEn<<" "<<picked_kmer<<endl;
        wrtF<<consd_kmer<<" "<<maxEn<<" "<<picked_kmer;
        wrtF.close();
    }
    else{
        ofstream wrtF(maxFn.c_str());
        consd_kmer = kmer_ent_vec[0].first;
        maxEn = kmer_ent_vec[0].second;
        picked_kmer = 0;
        // cout<<consd_kmer<<" "<<maxEn<<" "<<picked_kmer<<endl;
        wrtF<<consd_kmer<<" "<<maxEn<<" "<<picked_kmer;
        wrtF.close();
    }

	cout << picked_kmer  <<endl;
	return 0;
}