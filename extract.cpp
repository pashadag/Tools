#include<cassert>
#include<cmath>
#include<string.h>
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

#include "dbtools/defs.h"


using namespace std;
char buf[500];
bool INTERACTIVE = true;
bool skipN = false;
int startpos;
int endpos;
string genome;

bool extract(int startpos, int endpos, string & s) {

	if ((startpos > endpos) || (startpos < 0) || (endpos >= genome.length())) {
		cout << "Bad Input\n"; 
		return false;
	}
	s = genome.substr(startpos, endpos - startpos + 1);
	return true;
}

void usage (int argc, char * argv[]) {
	cerr << "Usage: extract filename [start end] [skipN]." << endl;
	cerr << "If [start end] is not specified the program becomes interactive.\n";
	cerr << "Co-ordinates are 0-based.\n";
	cerr << "received instead ";
	for (int i = 0; i < argc; i++) cerr << argv[i] << "\t";
	cerr << endl;
	exit(1);
}

int main(int argc, char * argv[]) {
	if (argc < 2) {
		usage(argc, argv);
	} 
	if (argc == 3) {
		if (0 == strcmp(argv[2], "skipN")) {
			skipN =  true;
		} else {
			usage(argc, argv);
		}
	} else if (argc >= 4) {
		INTERACTIVE = false;
		startpos = atoi(argv[2]);
		endpos   = atoi(argv[3]);
		if (argc == 5) {
			if (0 == strcmp(argv[2], "skipN")) {
				skipN =  true;
			} else {
				usage(argc, argv);
			}
		}
	}

	string filename = argv[1];

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
	char c;
	while ((c = inFile.rdbuf()->sbumpc()) != EOF) {
		if (c != '\n') {
			if (!skipN || (c != 'N'))
				genome.push_back(c);

		}
	}
	inFile.close();
	cerr << "Input finished, with " << add_commas(genome.length()) << " character.\n";


	string s;
	if (INTERACTIVE == true) {
		string cmd, input;
		while (cmd != "q") { 
			cout << " Enter command (p, rc, q) : ";
			cin  >> cmd;
			if (cmd == "p" ) {
				cout << "Please enter startpos: " << flush;
				cin >> input;
				startpos = atoi(input.c_str());
				cout << "Please enter endpos: " << flush;
				cin >> input;
				if ((input.length() > 0) && (input.at(0) == '+')) {
					endpos = startpos - 1 + atoi(input.substr(1).c_str());
				} else
					endpos = atoi(input.c_str());
				if (!extract(startpos, endpos, s)) {
					cout << "Bad Input\n"; 
					continue;
				}
				cout << "> " << filename << " " << startpos << " " << endpos << endl << s << endl;
			} else if (cmd == "rc") {
				cout << "> " << filename << " rc " << endl << revcomp(s) << endl;

			} 
		}
	} else {
		cout << "> " << filename << "_" << startpos << "_" << endpos << endl;
		if (!extract(startpos, endpos, s)) {
			cout << "Bad Input\n"; 
		} else {
			cout << s << endl;
		}

	}



	return 0;
}

