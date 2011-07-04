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
//	1) command line argument is an interval 
//	2) stdin, a set of intervals.
//The output is the set of intervals from 2 that are contained in something from 2
//The program is slow now because it does a linear scan, so time n_1 * n_2.


int main(int argc, char * argv[]) {
	assert (argc == 4);
	interval target;
	target.chr = argv[1];
	target.start = atoi(argv[2]);
	target.end = atoi(argv[3]);
	cerr << "Target interval is: " << target << endl;

	vector<interval> allInts;

	while (getline(cin, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> i.chr >> i.start >> i.end;
		i.label = sbuf;
		allInts.push_back(i);
	}

	cerr << "Read in " << allInts.size() << " intervals to be searched.\n";

	int curInt;
	for (curInt = 0; curInt < allInts.size(); curInt++) {
		//cout << sAllints.at(curInt) << " contained_in " << sTarget.at(curTarget) << endl;
		if (target.contains(allInts.at(curInt))) {
			cout << allInts.at(curInt).label << endl;
		}
	}

	return 0;
}


