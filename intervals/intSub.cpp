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
ostringstream oMsg;
string sbuf;
string filename, baseFilename;
#include "interval.h"

//This program takes as input:
//	1) stdin, a non-overlapping sorted interval file, with an optional fourth column containing labels for each interval.
//	2) file specificed by first parameter -- a non-overlapping sorted interval file.
//The output is the set of intervals from 1 that do not intersect with anything from 2.  
//The intervals that do overlap are chopped up so as to remove the intervals from 2.
//The labels are maintained during chopping



int main(int argc, char * argv[]) {
	assert (argc == 2);

	string curLine, prevLine;
	vector<interval> breaks;
	deque<interval> ints;	

	ifstream breaks_file(argv[1]);

	while (getline(breaks_file, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> i.chr >> i.start >> i.end;
		breaks.push_back(i);
	}
	while (getline(cin, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> i.chr >> i.start >> i.end >> i.label;
		ints.push_back(i);
	}

	//cerr << "num breaks " << breaks.size() << " num ints " << ints.size() << endl;

	vector<interval>::iterator curBreak = breaks.begin();
	
	while (!ints.empty()){
		interval curInt = ints.front();
		ints.pop_front();

		while ((curBreak != breaks.end()) && curInt.fullyRightOf(*curBreak) ) {
			curBreak++;
			if (curBreak != breaks.end()) {
				//cerr << "skipping to next break: " << curBreak->chr << " " << curBreak->start << " " << curBreak->end << " \n" << flush;
			}
		}
		if (curBreak == breaks.end()) {
			cout << curInt.chr << "\t" << curInt.start << "\t" << curInt.end << "\t" << curInt.label << endl << flush;
		} else if (curInt.fullyLeftOf(*curBreak)) {
			cout << curInt.chr << "\t" << curInt.start << "\t" << curInt.end << "\t" << curInt.label << endl << flush;
		} else if (curBreak->contains(curInt)) {
			//cerr << "skipping interval because its contained\n";
		} else if (curInt.contains(*curBreak)) {
			if (curInt.start < curBreak->start) {
				cout << curInt.chr << "\t" << curInt.start << "\t" << curBreak->start - 1 << "\t" << curInt.label << endl << flush;
			}
			interval i;
			i.chr = curInt.chr;
			i.start = curBreak->end + 1;
			i.end = curInt.end;
			i.label = curInt.label;
			if (i.start <= i.end) {
				//cerr << "Pushing " << i.chr << " " << i.start << " " << i.end << " to front of queue.\n";
				ints.push_front(i);
			}
		} else if ((curBreak->start <= curInt.start) && (curBreak->end >= curInt.start)) { //overlaps start pos
			interval i;
			i.chr = curInt.chr;
			i.start = curBreak->end + 1;
			i.end = curInt.end;
			i.label = curInt.label;
			if (i.start <= i.end) {
				//cerr << "Pushing " << i.chr << " " << i.start << " " << i.end << " to front of queue.\n";
				ints.push_front(i);
			}
		} else if ((curBreak->start <= curInt.end) && (curBreak->end >= curInt.end)) { //overlaps end pos
			if (curInt.start < curBreak->start) {
				cout << curInt.chr << "\t" << curInt.start << "\t" << curBreak->start - 1 << "\t" << curInt.label << endl << flush;
			}
		}
	}

	return 0;
}







