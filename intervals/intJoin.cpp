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

using namespace std;
ostringstream oMsg;
string sbuf;
string filename, baseFilename;


//This program takes standard in as the input a sorted interval file 
//The result are the set of intervals where overlapping intervals have been merged
//
//An optional parameter gap that closes gaps of size gap

void tokenize(string & s, string & chr, int & left, int & right) {
        //tokenize the string
        istringstream istrm(s);
	string val;
	istrm >> chr >> left >> right;
}



int main(int argc, char * argv[]) {

	string curChr, prevChr;
	int prevLeft, prevRight, curLeft, curRight, gap;
	string curLine, prevLine;

	if ((argc != 1+1) && (argc != 1+0)) {
		cerr << "Usage: intJoin [<gap>]";
		exit(1);
	}
	if (argc == 2) gap = atoi(argv[1]); else gap = 0;
	//cerr << "gap = " << gap << endl;
	
	if (!getline(cin, curLine)) {cerr << "empty file" << endl; exit(1);}
	prevLine = curLine;
	tokenize(prevLine, prevChr, prevLeft, prevRight);	

	while (getline(cin, curLine)) {
		tokenize(curLine, curChr, curLeft, curRight);	
		//cout << "READ IN " << curChr << " : " << curLeft << " : " << curRight << endl;	
		if ((curChr != prevChr) || (prevRight  + gap < curLeft)) { //no merging needed
			cout << prevChr << "\t" << prevLeft << "\t" << prevRight << endl;
			prevLine = curLine; prevChr = curChr; prevLeft = curLeft, prevRight = curRight;
		} else { // need to merge
			prevRight = max(prevRight, curRight);
		}

	}
	cout << prevChr << "\t" << prevLeft << "\t" << prevRight << endl;

	return 0;
}


