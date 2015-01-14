#include<cassert>
#include<cmath>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
#include<list>
#include<queue>
#include<cstdarg>
#include<algorithm>
#include<unordered_map>

using namespace std;
#include "defs.h"


typedef map<string, int> KmerMap;

class kmerCounter {
	public:

		kmerCounter(string g, int k) {
			genome = g;
			kmerSize = k;
		}

		void build(char skip) {
			cerr << "Building kmer hash...\n";
			for (int i = 0; i < genome.size() - kmerSize + 1; i++) {
				string orkmer = genome.substr(i, kmerSize);
				if (orkmer.find(skip) != string::npos) continue;
				string kmer = rcnorm(orkmer);
				transform(kmer.begin(), kmer.end(), kmer.begin(),::toupper); //make upper case
				KmerMap::iterator it = kmers.find(kmer);
				if (it == kmers.end()) {
					kmers.insert(make_pair(kmer,1));
				} else {
					it->second++;
				}
			}
		}

		void save(string filename, bool duplicateforbidmode ) {
			cerr << "Saving kmer hash...\n";
			ofstream out;
			open_file(out, filename);
			string lastkmer = "";
			for (KmerMap::iterator it = kmers.begin(); it != kmers.end(); it++) {
				if (duplicateforbidmode) {
					out << "1\t" << it->first << endl;
				} else {
					out << it->second << "\t" << it->first << endl;
				}
			}
			out.close();
		}
private:
	string genome;
	KmerMap kmers;
	int kmerSize;

};

/*
class kmerCounter {
	public:
		kmerCounter(string g, int k) {
			genome = g;
			kmerSize = k;
		}

		void build(char skip) {
			cerr << "Building kmer array...\n";
			for (int i = 0; i < genome.size() - kmerSize + 1; i++) {
				string orkmer = genome.substr(i, kmerSize);
				if (orkmer.find(skip) != string::npos) continue;
				string kmer = rcnorm(orkmer);
				transform(kmer.begin(), kmer.end(), kmer.begin(),::toupper); //make upper case
				kmers.push_back(kmer);
			}
			cerr << "Sorting kmer array...\n";
			sort(kmers.begin(), kmers.end());
		}

		void save(string filename, bool duplicateforbidmode ) {
			cerr << "Saving kmer array...\n";
			ofstream out;
			open_file(out, filename);
			string lastkmer = "";
			int curCount = 0;
			for (int i = 0; i < kmers.size(); i++) {
				if (kmers[i] == lastkmer) {
					curCount++;
				} else {
					if (curCount > 0) { //avoid printing during the first iteration
						if (duplicateforbidmode) 
							curCount = 1;
						out << curCount << "\t" << lastkmer <<  endl;
					}
					lastkmer = kmers[i];
					curCount = 1;
				}
			}
			if (curCount > 0) { //avoid printing during the first iteration
				if (duplicateforbidmode) 
					curCount = 1;
				out << curCount << "\t" << lastkmer <<  endl;
			}
			out.close();
		}
	private:
		string genome;
		vector<string> kmers;
		int kmerSize;

};
*/
