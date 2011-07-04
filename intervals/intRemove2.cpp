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
//	1) stdin, a set of intervals, with possible extra columns that serve as labels.
//	2) the first parameter: a non-overlapping sorted interval file.
//The output is the set of intervals from 1 which do not intersect any forbidden interval. The labels are maintained.

int main(int argc, char * argv[]) {
	assert (argc == 2);

	vector<interval> badInts;

	ifstream target_file(argv[1]);

	while (getline(target_file, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> i.chr >> i.start >> i.end;
		badInts.push_back(i);
	}
	cerr << "Read in " << badInts.size() << " forbidden intervals.\n";
	vector<interval>::iterator res;

	while (getline(cin, sbuf)){ 
		istringstream line(sbuf);
		interval curInt;
		line >> curInt.chr >> curInt.start >> curInt.end;
	
		//cout << "left: " << leftInt << "right: " << rightInt << flush << endl;
		
		res = lower_bound(badInts.begin(), badInts.end(), curInt, comp_le);

		if (res != badInts.end() && !res->overlaps(curInt)) {
			if (res != badInts.begin()) res--;
			if (!res->overlaps(curInt)) {
				cout << sbuf << endl;
			} else {
			//	cerr << "removed because of " << *res << endl;
			}
		} else {
			//cerr << "removed because of " << *res << endl;
		}

	}

	return 0;
}


