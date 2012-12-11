#include<cassert>
#include<cmath>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<cstdarg>
//#include "meanSD.h"
#include "dbtools/defs.h"

using namespace std;

//takes in a white-space delimited list of doubles, outputs the mean and stdev

int main(int argc, char * argv[]) {
	double cur_val;
	MeanSD calculator;
	while (cin >> cur_val) {
		//cout << "cur_val " << cur_val << endl;
		calculator.addVal(cur_val);
	}
	cout << calculator.getMean()  << "\t" << calculator.getSD() << endl;
}

