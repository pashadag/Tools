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
//	1) stdin, a set of intervals
//	2) the first parameter: a non-overlapping sorted interval file.
//The output is the set of intervals from 1 where endpoints are not contained in any forbidden interval.

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
		interval curInt, leftInt, rightInt;
		line >> curInt.chr >> curInt.start >> curInt.end;
		leftInt.chr = rightInt.chr = curInt.chr;
		leftInt.start = leftInt.end = curInt.start;
		rightInt.start = rightInt.end = curInt.end;
	
		//cout << "left: " << leftInt << "right: " << rightInt << flush << endl;
		
		res = lower_bound(badInts.begin(), badInts.end(), leftInt, comp_le);
		if (res != badInts.begin()) res--;
		//cout << "Left search result: " << *res << endl;

		if (!res->contains(leftInt)) {
			res = lower_bound(badInts.begin(), badInts.end(), rightInt, comp_le);
			if (res != badInts.begin()) res--;
			//cout << "Right search result: " << *res << endl;
			
			if (!res->contains(rightInt)) {
				cout << sbuf << endl;
			}
		}
	}

	return 0;
}


