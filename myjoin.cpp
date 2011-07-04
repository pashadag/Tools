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

string get_col(string & s, int col) {
	//tokenize the string

	istringstream istrm(s);
	string val;
	for (int i = 0; i < col; i++) istrm >> val;
	return val;
}




int main(int argc, char * argv[]) {
	assert (argc == 4);
	cerr << " left file = " << argv[1] << " right file = " << argv[2];
	ifstream left(argv[1]);
	ifstream right(argv[2]);
	int column = atoi(argv[3]);
	cerr << " column = "  << column << endl;
	//left file must not contain duplicate entries, and must be a subset of the other one.

	string curLeft, curRight, keyLeft;
	if (!getline(right, curRight)) {cerr << "empty right file" << endl; exit(1);}

	//state machine:  see paul's notebook for picture 
	int curState = 0;
	while (1) {
		if (curState == 0) {
			if (!getline(left, curLeft)) {
				cerr << "ERROR!!! cannot end in state 0\n";
				cerr << " curLeft = " << curLeft << " curRight = " << curRight << endl;
				assert(0);
				//cout << curLeft << endl;
				//curState = 3;
			} else if (get_col(curLeft, column) == curRight) {
				curState = 1;
			} else {
				cout << curLeft << endl;
				curState = 0;
			}
			//cout << "Got left line " << get_col(curLeft, column) << " at state 0 "<< endl << flush;
		} else if (curState == 1) {
			if (!getline(left, curLeft)) {
				curState = 3;
			} else if (get_col(curLeft, column) == curRight) {
				curState = 1;
			} else {
				curState = 2;
			}
			//cout << "Got left line " << get_col(curLeft, column) << " at state skip "<< endl << flush;
		} else if (curState == 2) {
			if (!getline(right, curRight)) {
				cout << curLeft << endl;
				curState = 3;
			} else if (get_col(curLeft, column) == curRight) {
				curState = 1;
			} else {
				cout << curLeft << endl;
				curState = 0;
			}
			//cout << "Got right line " << get_col(curRight, column) << " at state post skip "<< endl << flush;
		} else if (curState == 3) {
			//cerr << "Entering dump state with curLeft = " << curLeft << " and curRight = " << curRight << endl;
			while (getline(left, curLeft)) cout << curLeft << endl;
			return 0;
		} else {
			cerr << "Unexpected state\n";
			exit(1);
		}
	}



	return 0;
}


