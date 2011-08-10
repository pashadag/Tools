#include<cassert>
#include<cmath>
#include<string>
#include<iostream>
#include<iomanip>
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
//	2) the first parameter: a sorted interval file.
//The output annotates each interval from 1 by the highest percentage that it overlaps an interval in 2.



/* Takes an interval query and finds all intervals in database that overlap query.  
   These intervals are stored in matches.
   For the interval with the biggest overlap with query, the length of the overlap is stored as bestLen.
   The percentage of this best interval (relative to its length) that overlaps is stored as bestPerc.
   bestIndex is the index of the best overlap in matches.
   */

void getOverlaps(const interval & query, vector<interval> & database, int & bestLen, double &bestPerc, vector<interval> &matches, int &bestIndex) {

	bestLen = 0;
	bestIndex = -1;
	bestPerc = 0;
	interval j; j.chr = query.chr; j.start = query.end; j.end = query.end;
	vector<interval>::iterator last = lower_bound(database.begin(), database.end(), j, comp_le);

	for (vector<interval>::iterator cur = database.begin(); (cur != database.end()) && (cur != last); cur++) {
		int curOverlap = query.amountThatOverlaps(*cur);
		if (curOverlap > 0) {
			matches.push_back (*cur);
		}
		//cerr << "Considering " << *cur << " with " << curOverlap << endl;
		if (curOverlap > bestLen) {
			bestLen = curOverlap; 
			bestPerc = (double) cur->amountThatOverlaps(query) / (cur->end - cur->start + 1);
			bestIndex = matches.size() - 1;
		}
	}
}

/*int getBestOverlap(const interval & i, vector<interval> & theirs, double &otherDir, vector<interval> &allThem, int &forbidden) {

	int best = 0;
	interval j; j.chr = i.chr; j.start = i.end; j.end = i.end;
	vector<interval>::iterator last = lower_bound(theirs.begin(), theirs.end(), j, comp_le);

	for (vector<interval>::iterator cur = theirs.begin(); (cur != theirs.end()) && (cur != last); cur++) {
		int curOverlap = i.amountThatOverlaps(*cur);
		if (curOverlap > 0) {
			allThem.push_back (*cur);
		}
		//cerr << "Considering " << *cur << " with " << curOverlap << endl;
		if (curOverlap > best) {
			best = curOverlap; 
			otherDir = (double) cur->amountThatOverlaps(i) / (cur->end - cur->start + 1);
			forbidden = allThem.size() - 1;
		}
	}
	return best;
}
*/

void usage(char* argv[]) {
	cerr << "Usage: " << argv[0] << " validationCallFile <full4|full|full2|full3>" << endl;
	cerr << "Currently, full4 is the only maintained option.  It appends the following three columns to each query line: " << endl;
	cerr << "1. Percentage of query bases that are in the database." << endl;
	cerr << "2. Percentage of bases from overlapping db calls that are within the query." << endl;
	cerr << "3. Number of db calls that overlap the query." << endl;
}

int main(int argc, char * argv[]) {
	if ((argc != 1+1) && (argc != 1+2)) {
		usage(argv);
		exit(1);
	}
	bool full = false;
	bool full2 = false;
	bool full3 = false;
	bool full4 = false;
	bool full5 = false;
	if (argc == 3) {
		sbuf = argv[2];
		if (sbuf == "full") full = true;
		else if (sbuf == "full2") full2 = true;
		else if (sbuf == "full3") full3 = true;
		else if (sbuf == "full4") full4 = true;
		else if (sbuf == "full5") full5 = true;

		else {
			usage(argv);
			exit(1);
		}

	}

	vector<interval> db;
	ifstream db_file(argv[1]);
	while (getline(db_file, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> i.chr >> i.start >> i.end;
		db.push_back(i);
	}
	//cerr << "Read in " << db.size() << " of db intervals.\n";

	while (getline(cin, sbuf)){ 
		istringstream line(sbuf);
		double bestPerc;
		int bestIndex, bestLen;
		vector<interval> matches;
		interval curInt;

		line >> curInt.chr >> curInt.start >> curInt.end;
		getOverlaps(curInt, db, bestLen, bestPerc, matches, bestIndex);
		double intlen = (curInt.end - curInt.start + 1) ;
		double perOverlap = double(bestLen) / intlen;
		//cerr << "o i p = " << bestLen << " " << intlen << " " << perOverlap << endl;
		cout << fixed << setprecision (4);
		if (full4) {
			int queryCovered = 0;
			int dbCovered = 0;
			int matchesCovered = 0;
			for (int i = 0; i < matches.size(); i++) {
				queryCovered += curInt.amountThatOverlaps(matches[i]);
				dbCovered += matches[i].amountThatOverlaps(curInt);
				matchesCovered += matches[i].end - matches[i].start + 1;
			}
			double perQueryCovered = (double) queryCovered / (curInt.end - curInt.start + 1);
			double perMatchesCovered = (double) dbCovered / matchesCovered;
			cout << sbuf;
			if (matchesCovered == 0) {
				cout << "\t0\t0\t0";
			} else {
				cout << "\t" << perQueryCovered << "\t" << perMatchesCovered<< "\t" << matches.size();
			}
		} else if (full2) {
			cout << sbuf << "\t" << perOverlap << "\t" << bestPerc << "\t" << matches.size();
		}
		else if (full3) {
			cout << sbuf << "\t" << perOverlap;
			cout << "\t" << bestPerc << "\t" << matches.size();
			if (matches.size() != 0) {
				//cout << "\t" << matches[bestIndex].start;
				//cout << "\t" << matches[bestIndex].end;
				cout << "\t" << abs(matches[bestIndex].start - curInt.start);
				cout << "\t" << abs(matches[bestIndex].end - curInt.end);
			} else {
				cout << "\tNA\tNA";
			}

		}
		else if (full5) {
			cout << sbuf << "\t" << perOverlap;
			cout << "\t" << bestPerc << "\t" << matches.size();
			if (matches.size() != 0) {
				for (int i = 0; i < matches.size(); i++) {
					cout << "\t" << matches[i].start;
					cout << "\t" << matches[i].end;
				}
			} else {
				cout << "\tNA\tNA";
			}

		}

		else if (full) {
			cout << sbuf << "\t" << perOverlap;
			cout << "\t" << bestPerc << "\t" << matches.size() << "\t" << curInt.end - curInt.start + 1 ;
			for (int i = 0; i < matches.size(); i++) {
				if (i != bestIndex) {
					cout << "\t" << (double) curInt.amountThatOverlaps(matches[i]) / (curInt.end - curInt.start + 1);
					cout << "\t" << (double) matches[i].amountThatOverlaps(curInt) / (matches[i].end - matches[i].start + 1);
				}
			}
		}
		else {
			cout << sbuf << "\t" << perOverlap;
		}


		cout << endl;
	}

	return 0;
}


