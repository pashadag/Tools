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
//#include "defs.h"


//This program takes  two files names as the input, and prints a side by side join
//So, if the two files are 
//a
//b
//c
//and the second file is
//1 
//2
//3
//Will output:
//a 1
//b 2
//c 3

void usage(char * argv[]) {
	cerr << "Usage: " << argv[0] << " file1 file2 <delimiter>." << endl;
	exit(1);
}

int main(int argc, char * argv[]) {
	if ((argc != 1+2) && (argc != 1+3)) usage(argv);
	ifstream file1(argv[1]);
	ifstream file2(argv[2]);
	string delim;
	if (argc == 4) delim = argv[3]; else delim = " ";

	string line1, line2;
	//if (!getline(file1, curLine)) {cerr << "empty file" << endl; exit(1);}
	while (getline(file1, line1)) {
		if (getline(file2, line2)) {
			cout << line1 << delim <<  line2 << endl;
		} else {
			cout << line1 << delim << endl;
		}
	}
	while (getline(file2, line2)) {
		cout << delim << line2 << endl;
	}

	return 0;
}

