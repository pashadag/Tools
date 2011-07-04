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

using namespace std;
ostringstream oMsg;
string sbuf;
#include "defs.h"



void usage(int argc, char * argv[]) {
	cerr << "Usage: " << argv[0] << " <db> -g genome -k kmersize -o outfile" << endl;
	cerr << "Program descrption.\n";
	exit(1);
}


int main(int argc, char * argv[]) {

	
	/*
	char ch;
	while ((ch = getopt(argc, argv, "g:k:o:")) != -1) {
		switch (ch) {
			case 'g':
				genome = read_genome(optarg);
				break;
			case 'k':
				kmersize = atoi(optarg);
				break;
			case 'o':
				base = optarg;
				break;
		}
	}
	*/
	return 0;
}


