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
//	2) file specificed by first parameter -- a set of intervals.
//The output is the set of intervals from 2 that are contained in something from 2
//The program is slow now because it does a linear scan, so time n_1 * n_2.


int main(int argc, char * argv[]) {
	assert (argc == 2);

	vector<interval> allints, target;
	vector<string> sAllints, sTarget;


	ifstream allints_file(argv[1]);

	while (getline(allints_file, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> i.chr >> i.start >> i.end;
		allints.push_back(i);
		sAllints.push_back(sbuf);
	}
	cerr << "Read in " << allints.size() << " intervals.\n";

	while (getline(cin, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> i.chr >> i.start >> i.end;
		target.push_back(i);
		sTarget.push_back(sbuf);
		
	}
	cerr << "Read in " << target.size() << " target.\n";

	int curInt, curTarget;
	for (curTarget = 0; curTarget < target.size(); curTarget++) {
		for (curInt = 0; curInt < allints.size(); curInt++) {
			//cout << sAllints.at(curInt) << " contained_in " << sTarget.at(curTarget) << endl;
			if (target.at(curTarget).contains(allints.at(curInt))) {
				cout << sAllints.at(curInt) << " contained_in " << sTarget.at(curTarget) << endl;
			}
		}
	}

	return 0;
}


