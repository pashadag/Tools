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
//	1) stdin, a non-overlapping set of intervals, sorted by chr (alpha) and by left pos (numerically). 
//	2) file specificed by first parameter -- a non-overlapping set of intervals, agains sorted.
//The output is the set of intervals from 1 that intersect with anything from 2

int main(int argc, char * argv[]) {
	assert (argc == 2);

	vector<interval> target;

	ifstream target_file(argv[1]);

	while (getline(target_file, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> i.chr >> i.start >> i.end;
		target.push_back(i);
	}
	cerr << "Read in " << target.size() << " target intervals.\n";
	vector<interval>::iterator curTarget = target.begin();

	while (getline(cin, sbuf) && (curTarget != target.end())) {
		istringstream line(sbuf);
		interval curInt;
		line >> curInt.chr >> curInt.start >> curInt.end;
		while ((curTarget != target.end()) && curInt.fullyRightOf(*curTarget)) {
			curTarget++;
		}
		if ((curTarget != target.end()) && curInt.overlaps(*curTarget)) {
			cout << sbuf << endl;
		}
	}
	return 0;
}


