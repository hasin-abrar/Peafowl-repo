#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
using namespace std;

#include <pthread.h>

#define HASH_TABLE_LENGTH 80000003

pthread_mutex_t hashTable_mutex = PTHREAD_MUTEX_INITIALIZER;

int noOfSpecies;
vector<string> nameOfSpecies;
int kmerLength; //change k-mer length, previously: int kmerLength=25, now value assigned in line 161 from arg
int NUM_THREADS=2;


class Kmer{
public:
	vector<int> kmerCount;
	long long int intValueOfKmer;
	vector<int> speciesNo;
};

class ShortKmer{
public:
    int kmerCount;
	long long int intValueOfKmer;
    long long int intValueOfRCKmer;
	int speciesNo;
};


vector<ShortKmer> kmer_array;//this could be given to hash table

vector<ShortKmer> * kmer_arrays;

ifstream * readfiles;

// ofstream taxafile;
ofstream outfile;

long long int valMax=0x3FFFFFFFFFFFFFFF;
long long int valBar=0;
long long int val=0;
long long int valInc=0x0010000000000000;


class HashTable{
public:
	vector<Kmer> kmers[HASH_TABLE_LENGTH];//this need to be take care of
	long long int totalKmers_Bucket[HASH_TABLE_LENGTH];
	pthread_mutex_t hashTableBuckets_mutex[HASH_TABLE_LENGTH];


	HashTable();
	void insertKmer(long long int intValueOfKmer, long long int intValueOfRCKmer, int kmer_count, int speciesNo);
	void dumpKmers();
};

HashTable::HashTable(){


	for(int i=0;i<HASH_TABLE_LENGTH;i++)
	{
		pthread_mutex_init(&hashTableBuckets_mutex[i],NULL);
		totalKmers_Bucket[i]=0;
	}
}

void HashTable::insertKmer(long long int intValueOfKmer, long long int intValueOfRCKmer, int kmer_count, int speciesNo){			

	unsigned long int index = (unsigned long int)intValueOfKmer % HASH_TABLE_LENGTH;

	int found=0;

	// if (intValueOfKmer == 10)
	// {
	// 	cout<<"Species: "<<speciesNo<<endl;
	// }

	pthread_mutex_lock(&hashTableBuckets_mutex[index]);

	for(unsigned long long int i=0;i<kmers[index].size();i++)
	{
		if(intValueOfKmer == kmers[index][i].intValueOfKmer)
        {        
            kmers[index][i].kmerCount.push_back(kmer_count);
            kmers[index][i].speciesNo.push_back(speciesNo);

   //          if (intValueOfKmer == 10)
			// {
			// 	cout<<"Inside Species: "<<speciesNo<<endl;
			// 	for (int j = 0; j < kmers[index][i].kmerCount.size(); ++j)
			// 	{
			// 		cout<<"Count "<<kmers[index][i].kmerCount[j];
			// 		cout<<" Species: "<<kmers[index][i].speciesNo[j]<<endl;
			// 	}
			// }
			found=1;
			break;
		}
	}
    if(found == 0)
    {
        unsigned long int indexRC = (unsigned long int)intValueOfRCKmer % HASH_TABLE_LENGTH;
        for(unsigned long long int i=0;i<kmers[indexRC].size();i++)
        {
            if(intValueOfRCKmer == kmers[indexRC][i].intValueOfKmer) // does not handle mutex
            {	
            	//this code should not execute
                kmers[indexRC][i].kmerCount.push_back(kmer_count);
                kmers[indexRC][i].speciesNo.push_back(speciesNo);

                found=1;
                break;
            }
        }
    }

	if(found==0)
	{
        Kmer km;
        km.kmerCount.push_back(kmer_count);
        km.intValueOfKmer=intValueOfKmer;
        km.speciesNo.push_back(speciesNo);

		kmers[index].push_back(km);

		totalKmers_Bucket[index]++;


	}

	pthread_mutex_unlock(&hashTableBuckets_mutex[index]);
}


void HashTable::dumpKmers()
{
	for(unsigned long int index=0;index<HASH_TABLE_LENGTH;index++)
	{
        	int count=kmers[index].size();

		if(count>0)
			cout<<count<<endl;
	}


}

HashTable *ht;

long long int getInt(string s){
	long long int val=0;
	int i=0;
	//char ch;
	while(s[i]){
		val=val<<2;
		if(s[i]=='A'){
			val=val|0;
		}
		else if(s[i]=='C'){
			val=val|1;
		}
		else if(s[i]=='G'){
			val=val|2;
		}
		else{
			val=val|3;
		}
		i++;
	}
	return val;
}

string getKmer(long long int val){
	int temp=0;
	string kmer = "";

	for(int i=kmerLength-1;i>=0;i--){
		temp=val&3;
		val=val>>2;
		if(temp==0)
			kmer+='A';
		else if(temp==1)
			kmer+='C';
		else if(temp==2)
			kmer+='G';
		else if(temp==3)
			kmer+='T';
	}
	//kmer[kmerLength]='\0';
	reverse(kmer.begin(), kmer.end());
	return kmer;
}

string getRC(string kmer)
{
    string rc = "";
    for(int i=kmer.length()-1; i>=0; i--){
        if (kmer[i] == 'A'){
            rc += 'T';
        }
        else if(kmer[i] == 'C'){
            rc += 'G';
        }
        else if(kmer[i] == 'G'){
            rc += 'C';
        }
        else{
            rc += 'A';
        }
    }
    return rc;
}

struct ThreadArg
{
	int threadID;
};


//readfile argument function is needed
void * readCases(void *threadid)
{
	ThreadArg *ta=(ThreadArg *)threadid;
   	int threadNo=ta->threadID;

	string count,kmer_name;

	for(int i=threadNo;i<noOfSpecies;i+=NUM_THREADS)
	{	
		// kmer_arrays[] stores the last kmer with val > valBar
		// thus, we store this val to hashvalue, and then see which are the other kmers
		// that has now value <= current valBar
		// If the stored kmer has value > current valBar, then all the kmers that
		// are in front of it in the file will also be the same, and the file should not be 
		// processed this time. Storing this kmer will give wrong ht value, as other files
		// may not process this kmer this time.
		if(kmer_arrays[i].size()>0)
		{
			if(kmer_arrays[i][0].intValueOfKmer<=valBar)
			{
				ht->insertKmer(kmer_arrays[i][0].intValueOfKmer, kmer_arrays[i][0].intValueOfRCKmer, kmer_arrays[i][0].kmerCount, kmer_arrays[i][0].speciesNo);
				kmer_arrays[i].clear();
			}
			else {
				continue;
			}
		}
		// readfiles[] pointer will always point to the latest line. So,
		// at any iteration after the first one, the file is not read 
		// from the start.
		while(!readfiles[i].eof()) 
		{
	        readfiles[i]>>count;
			readfiles[i]>>kmer_name;
			count=count.substr(1);
			stringstream geek(count);
			int value;
			geek>>value;

		//cout<<i<<" "<<kmer_name<<" "<<count<<endl;

			ShortKmer mykmer;
			string rc = getRC(kmer_name);
			mykmer.intValueOfKmer=getInt(kmer_name);
			mykmer.intValueOfRCKmer =getInt(rc);
			mykmer.kmerCount=value;
			mykmer.speciesNo=i;

			if(mykmer.intValueOfKmer<=valBar)
			{
				// if (kmer_name == "AAAAAAAGG")
				// {
				// 	cout<<"Species: "<<i<<" :value: "<<mykmer.intValueOfKmer<<endl;
				// }
				ht->insertKmer(mykmer.intValueOfKmer, mykmer.intValueOfRCKmer, mykmer.kmerCount, mykmer.speciesNo);
			}
			else
			{
				kmer_arrays[i].push_back(mykmer);
				break;
			}
		}	
	}

	pthread_exit(NULL);

}

bool cmpLex(const pair<string, string> &a,const pair<string, string> &b)
{
       return a.second<b.second;
}

void createSortedKmerFile(string filename){
	ifstream kmerFile(filename.c_str());
	string kmerCount, kmer;
	vector<pair<string, string> > kmerList; // first = >count, second = kmer
	while(getline(kmerFile,kmerCount)){
		getline(kmerFile, kmer);
		kmerList.push_back(make_pair(kmerCount, kmer));
	}
	sort(kmerList.begin(), kmerList.end(), cmpLex);
	kmerFile.close();
	// remove(filename);
	ofstream outKmerFile;
	outKmerFile.open(filename.c_str(), ios::trunc); //trunc to make sure empty file
	for (int i = 0; i < kmerList.size(); ++i)
	{
		outKmerFile<<kmerList[i].first<<endl;
		
		if(i == kmerList.size() - 1){
			outKmerFile<<kmerList[i].second;
		}
		else{
			outKmerFile<<kmerList[i].second<<endl;
		}
	}
	outKmerFile.close();
}

int main(int argc, char **argv){

	string filename_arr = argv[1];
	kmerLength = stoi(argv[2]); //kmer length passed as argument when running cpp from run.sh
	NUM_THREADS=stoi(argv[3]);
	string sizeFileName = argv[4];
	double maxMem = 8 * 1024 * 1024;

	string fileLine;
	long long int totalSize = 0;
	long long int singleFileSize;
    int base = 10;
    char *endC; 

	ifstream sizeFile(sizeFileName.c_str());	
	while(getline(sizeFile, fileLine)){
		singleFileSize = strtoll(fileLine.c_str(), &endC, base);
		totalSize += singleFileSize;
	}
	// How many partition should be made of 8 GB kmers at max
	int partitionCount = ceil(totalSize / maxMem);
//	cout<<totalSize<<" "<<maxMem<<endl;
	cout<<"Total partitions: "<<partitionCount<<endl;
	
	
	//vector<string> file_arr;
	char *file_arr[500];


	int k = 0;

	while(filename_arr.size()>1){

        size_t pos = filename_arr.find("/");
        string name = filename_arr.substr(0,pos);
        createSortedKmerFile(name);
		// cout<<name<<endl;
		//file_arr.push_back(name);
		file_arr[k]=new char[name.size()+1];
		for(int i=0;i<name.size();i++){
			file_arr[k][i]=name[i];
		}
		file_arr[k][name.size()]='\0';
		//file_arr[k]=(char*)name;
		filename_arr=filename_arr.substr(pos+1);
		k++;
		noOfSpecies++;
	}



	//noOfSpecies = file_arr.size();

	// cout<<noOfSpecies<<endl;

	// for(int i=0; i<noOfSpecies; i++){
	// 	cout<<file_arr[i]<<endl;
	// }

	kmer_arrays=new vector<ShortKmer>[noOfSpecies];
	readfiles= new ifstream[noOfSpecies];



	ht=new HashTable();


	for(int i=0; i<noOfSpecies; i++){/////////
		string p = (string)file_arr[i];
        size_t pos = p.find(".");
        string name = p.substr(0,pos);

		nameOfSpecies.push_back(name);

	}

	// for(int i=0; i<noOfSpecies; i++)
	// {
	// 	cout<<nameOfSpecies[i]<<endl;
    // }




	for(int i=0; i<noOfSpecies; i++){

		readfiles[i].open(file_arr[i]);

	}


	// taxafile.open("taxa_output.txt");
	outfile.open("kmer_exist_output.txt");
	string printline = nameOfSpecies[0];
	for(int i=1; i<noOfSpecies; i++){
		printline=printline+"\t"+nameOfSpecies[i];

	}
	outfile<<printline<<"\n";


    //now we will create multithread
    // we will have total 8 thread and they will read from the vector

    ThreadArg *thArgs[NUM_THREADS];
    pthread_t caseThreads[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int rc;
	void *status;




	valMax=0;

	for(int i=0;i<kmerLength;i++)
	{
		valMax=valMax<<2;
		valMax=valMax|3;

	}



	valInc=valMax/partitionCount;
	// valInc = valMax;


	while(valBar<valMax)
	{
		valBar+=valInc;
		if (valBar + valInc > valMax){
			valBar = valMax;
		}
        for(int i=0;i<NUM_THREADS;i++)
    	{
    		thArgs[i]=new ThreadArg;
    		thArgs[i]->threadID=i;
    		rc = pthread_create(&caseThreads[i], NULL, readCases, (void *)thArgs[i]);
    		if (rc)
    		{
         		exit(-1);
    		}
    	}

        for(int i=0;i<NUM_THREADS;i++)
	   {
    		rc = pthread_join(caseThreads[i], &status);
    		delete thArgs[i];
    		if (rc)
      		{
     			exit(-1);
    		}
	   }


	   cout<<"-------------------------------------------------"<<endl;

    //now we have to output the matrix in a file

	//	ht->dumpKmers();

		for(unsigned long int index=0;index<HASH_TABLE_LENGTH;index++)
		{
    		int count=ht->kmers[index].size();

			for(unsigned int i=0;i<ht->kmers[index].size();i++)
			{

    			// printline=getKmer(ht->kmers[index][i].intValueOfKmer);
    			// taxafile<<printline<<"\n";
    			printline="";
				
				
				vector<int> speciesPresent(noOfSpecies,0);
				for (int p = 0; p < ht->kmers[index][i].speciesNo.size(); ++p)
				{
					int indx = ht->kmers[index][i].speciesNo[p];
					speciesPresent[indx] = 1;
				}
				
				for(int j=0;j<noOfSpecies;j++)
				{
       				if(speciesPresent[j] == 1)
					{
        				printline=printline+"1\t";
        			}
        			else printline=printline+"0\t";
    			}
    			outfile<<printline<<"\n";
    			/*
				int k=0;
    			for(int j=0;j<noOfSpecies;j++)
				{
       				if(j==ht->kmers[index][i].speciesNo[k] && k<ht->kmers[index][i].speciesNo.size())
					{
        				printline=printline+"1\t";
        				k++;
        			}
        			else printline=printline+"0\t";
    			}
    			*/
        		
        	}

        }
		for(unsigned long int index=0;index<HASH_TABLE_LENGTH;index++)
		{
    		ht->kmers[index].clear();
		}

	}

	cout<<"Finished Reading"<<endl;

	for(int i=1; i<noOfSpecies; i++)
	{

		readfiles[i].close();//file closed
	}

	outfile.close();
	// taxafile.close();

	return 0;

}














