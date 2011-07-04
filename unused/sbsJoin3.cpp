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


//This program takes standard in as the input, and joins every consecutive pair of lines (with no space)
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

int main(int argc, char * argv[]) {

	int skip = 0;
	if (argc == 2) {
		skip = atoi(argv[1]);
	}

	string line1, line2;
	for (int i = 0; i < skip; i++) getline(cin, line1);
	

	while (getline(cin, line1)) {
		if (getline(cin, line2)) {
			cout << line1 << line2 << endl;
		} else {
			cout << line1;
		}
	}

	return 0;
}


