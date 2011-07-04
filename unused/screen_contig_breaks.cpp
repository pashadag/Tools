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
string sbuf, wholeLine;
string filename, baseFilename;
#include "defs.h"

class interval {
public:
	string chr;
	int start;
	int end;
	bool operator< (const interval & i) const {
		if (chr == i.chr) return end < i.end;
		return chr < i.chr;
	}
	

	bool contains(const interval & c1) {
		if (c1.chr != chr) return false;
		if ((start <= c1.start) && (c1.end <= end)) return true;
		return false;
	}
	bool overlaps(const interval &c) {
		if (c.chr != chr) return false;
		if ((c.start <= start) && (c.end >= start)) return true;
		if ((c.start <= end)   && (c.end >= end)) return true;
		return false;
	}


};

bool mycomp (const interval & i1, const interval & i2) {
	if (i1.chr != i2.chr) return (i1.chr < i2.chr);
	return (i1.end <= i2.start);
	//assuming intervals are non-overlapping
}


vector<interval> contigBreaks;

int main(int argc, char * argv[]) {
	init_options(argc, argv);
	int tolerance, mean, sd;
	opt.get("mean", mean);	
	opt.get("sd", sd);	
	opt.get("toleranceAroundBreaks", tolerance);	
	//opt.get("toleranceAroundBreaks", tolerance);	
	tolerance = mean + tolerance * sd; 
	opt.get("breaksFile", filename);
	ifstream breaks_file(filename.c_str());

	while (getline(breaks_file, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> sbuf >> i.chr >> i.start >> i.end;
		//i.start -= tolerance;
		//i.end += tolerance;
		contigBreaks.push_back(i);
	}
	
	//don't need to sort, assume input file is sorted, but just in case
	sort(contigBreaks.begin(), contigBreaks.end());

	//opt.get("donor", filename);
	//ifstream file(filename.c_str());

	while (getline(cin, wholeLine)) {
		istringstream line(wholeLine);
		interval origKey, startKey, endKey;
		int id, clusType ;
		line >> origKey.chr >> origKey.start >> origKey.end >> clusType >> id;


		//searching for left point
		startKey = origKey;
		if (clusType != 3) {
			startKey.start += tolerance;
		} else {
			startKey.start -= tolerance;
		}
		startKey.end = startKey.start;
		vector<interval>::iterator hit = lower_bound(contigBreaks.begin(), contigBreaks.end(), startKey);
		if (hit != contigBreaks.end()) {
			if (hit->contains(startKey)) {
				//cout << "DISQUALIFY LEFT \t";
				//cout << "KEY " << origKey.chr << " " << add_commas(origKey.start) <<  " " << add_commas(origKey.end) << " " << clusType << " " << id << "\t\t\t";
				//cout << " HIT " << hit->chr << " " << add_commas(hit->start) <<  " " << add_commas(hit->end) << endl;
				continue;
			}
		} 
		
		//searching for right point
		endKey = origKey;
		if (clusType != 2) {
			endKey.end -= tolerance;
		} else {
			endKey.end += tolerance;
		}
		endKey.start = endKey.end;
		hit = lower_bound(contigBreaks.begin(), contigBreaks.end(), endKey);
		if (hit != contigBreaks.end()) {
			if (hit->contains(endKey)) {
				//cout << "DISQUALIFY RIGHT\t";
				//cout << "KEY " << origKey.chr << " " << add_commas(origKey.start) <<  " " << add_commas(origKey.end) << " " << clusType << " " << id << "\t\t\t";
				//cout << " HIT " << hit->chr << " " << add_commas(hit->start) <<  " " << add_commas(hit->end) << endl;
				continue;
			}
		} 


		//PART II : remove any cluster going across a gap
	/*	interval full;
		full = origKey;
		if (full.start > full.end) swap(full.start, full.end);
		hit = lower_bound(contigBreaks.begin(), contigBreaks.end(), full, mycomp);
		if (hit != contigBreaks.end()) {
			if (hit->overlaps(full)) {
				continue;
			}
		}

		*/
				

			
		

		cout << wholeLine << endl;
		
	}
	

	


}

