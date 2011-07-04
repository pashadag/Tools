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
//	1) stdin, a set of links.
//	2) command line target interval, either one of them or two of them
//The output is the set of links from 1 that fall with some endpoint into the target (for the case of one target)
// or fall with one endpoint into one target and with the other into the other target (for two targets).

int main(int argc, char * argv[]) {
	assert ((argc==4) || (argc == 7));

	bool twoTargets=false;
	interval target, target2;
	target.chr = argv[1];
	target.start = atoi(argv[2]);
	target.end = atoi(argv[3]);
	cerr << "Target interval is: " << target << endl;
	if (argc == 7) {
		twoTargets = true;
		target2.chr = argv[4];
		target2.start = atoi(argv[5]);
		target2.end = atoi(argv[6]);
		cerr << "Second target interval is: " << target2 << endl;
	}


	while (getline(cin, sbuf) ) {
		istringstream line(sbuf);
		string chr;
		int point1, point2;
		line >> chr >> point1 >> point2;
		if (!twoTargets) {
			if (target.contains(chr, point1) || target.contains(chr,point2)) {
				cout << sbuf << endl;
			}
		} else {
			if ((target.contains(chr, point1) && target2.contains(chr, point2)) || 
				(target.contains(chr, point2) && target2.contains(chr, point1))){
				cout << sbuf << endl;
			}
		}
	}
	return 0;
}


