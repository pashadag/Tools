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
//#include<mysql++.h>
//#include <boost/foreach.hpp>

using namespace std;
ostringstream oMsg;
string sbuf;
string filename, baseFilename;


//This program takes standard in as the input, and joins every consecutive pair of lines (with tab)
//
//So, the file
//a
//b
//c
//d
//e
//Will output:
//ab
//cd
//e
//
// and now, it doesn't have to be pairs of lines, but any number of lines that could be merged

void usage (int argc, char * argv[]) {
	cerr << "sbsJoin3 [numlines to merge [delimeter [numlines to skip at beg]]]" << endl;
	exit(1);
}

int main(int argc, char * argv[]) {

	int numlines = 2;
	int skip = 0;
	string delim= "\t";

	if (argc > 4) usage(argc, argv);
	if (argc >= 2) {
		numlines = atoi(argv[1]);
	}
	if (argc >= 3) {
		delim = argv[2];
	}
	if (argc >= 4) {
		skip = atoi(argv[3]);
	}

	string line1, line2;
	for (int i = 0; i < skip; i++) getline(cin, line1);
	

	bool done = false;
	while (!done) {
		vector<string> lines;
		for (int i = 0; i < numlines; i++) {
			if (getline(cin, line1)){
				lines.push_back(line1);
			} else {
				done = true;
				break;
			}
		}

		if (lines.size() != 0) {
			cout << lines.at(0);
			for (int i = 1; i < lines.size(); i++) {
				cout <<  delim << lines.at(i);
			}
			cout << endl;
		}
	}



	return 0;
}


