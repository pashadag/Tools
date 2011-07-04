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


//This program takes standard in as the input
//For each line, it prints that line followed by a tab followed by the next line of the input
//For the last line, it just prints the line
//So, the file
//a
//b
//c
//Will output:
//a b
//b c
//c

int main(int argc, char * argv[]) {
	assert (argc == 1);
	//cerr << "file = " << argv[1] << endl;
	//ifstream file(argv[1]);

	string curLine, prevLine;
	if (!getline(cin, curLine)) {cerr << "empty file" << endl; exit(1);}
	prevLine = curLine;

	while (getline(cin, curLine)) {
		cout << prevLine << "\t" << curLine << endl;
		prevLine = curLine;
	}
	cout << prevLine << endl;

	return 0;
}


