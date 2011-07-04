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
#include "logClass.h"
#include "optionsClass.h"

using namespace std;
ostringstream oMsg;
logClass llog;
optionsClass opt;
string sbuf;
string filename, baseFilename;
#include "defs.h"


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

int main(int argc, char * argv[]) {
	assert (argc == 3);
	cerr << "file1 = " << argv[1] << " file2 = " << argv[2] << endl;
	ifstream file1(argv[1]);
	ifstream file2(argv[2]);

	string curLine, prevLine;
	if (!getline(file1, curLine)) {cerr << "empty file" << endl; exit(1);}
	prevLine = curLine;

	while (getline(cin, curLine)) {
		cout << prevLine << "\t" << curLine << endl;
		prevLine = curLine;
	}
	cout << prevLine << endl;

	return 0;
}


