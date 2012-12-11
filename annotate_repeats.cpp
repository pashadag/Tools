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
#include"dbtools/defs.h"



using namespace std;
char buf[500];

int main(int argc, char * argv[]) {
	if (argc < 2) {
		cerr << "Usage: annotate_repeats filename chromosome\n";
		exit(1);
	} 

	string filename = argv[1];
	string chr = argv[2];

	ifstream inFile;
	inFile.open(filename.c_str());
	if (!inFile) {
		cerr <<  "Cannot open input file: " <<  filename  << endl;
		return 0;
	}
	cerr << "Reading in fasta sequence file from " << filename << "...\n" << flush;
	inFile.get(buf, 256);
	cerr << "First line of file is " << buf << ".\n" << flush;
	

	//read the file into genome
	string genome;
	char c;
	while ((c = inFile.rdbuf()->sbumpc()) != EOF) {
		if (c != '\n') {
			genome.push_back(c);

		}
	}
	inFile.close();
	cerr << "Input finished, with " << add_commas(genome.length()) << " character.\n";


	int startpos;
	int endpos;
	bool inRepeat = false;
	for (int i = 0; i < genome.length(); i++) {
		char c = genome[i];
		if (islower(c) && !inRepeat) {
			startpos = i;
			inRepeat = true;
		} else if (isupper(c) && inRepeat) {
			endpos = i - 1;
			inRepeat = false;
			cout << chr << '\t' << startpos << '\t' << endpos << endl;
		}
	}




	return 0;
}

