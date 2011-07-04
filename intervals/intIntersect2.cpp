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
#include<deque>
#include<cstdarg>
#include<algorithm>

using namespace std;
string sbuf;
#include "interval.h"


//This program takes as input:
//	1) stdin, a set of intervals.
//	2) command line target interval
//The output is the set of intervals from 1 that intersect the target interval

int main(int argc, char * argv[]) {
	assert (argc == 4);
	interval target;
	target.chr = argv[1];
	target.start = atoi(argv[2]);
	target.end = atoi(argv[3]);
	cerr << "Target interval is: " << target << endl;

	while (getline(cin, sbuf) ) {
		istringstream line(sbuf);
		interval curInt;
		line >> curInt.chr >> curInt.start >> curInt.end;
		if (curInt.overlaps(target)) {
			cout << sbuf << endl;
		}
	}
	return 0;
}


