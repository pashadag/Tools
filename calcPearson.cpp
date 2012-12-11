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
#include "meanSD.h"

using namespace std;
ostringstream oMsg;
string sbuf;
string filename, baseFilename;

void usage(int argc, char * argv[]) {
	cout << "Usage: " << argv[0] << " <r,r2> " << endl;
	exit(1);
}

int main(int argc, char * argv[]) {
	if (argc != 2) usage(argc, argv);
	string output = argv[1];
	vector< double > xVals;
	vector< double > yVals;

	meanSD xCalc, yCalc;

	while (getline(cin, sbuf)) { //first pass to get the mean
		istringstream line(sbuf);
		double xVal, yVal;
		line >> xVal >> yVal;
		xVals.push_back(xVal);
		yVals.push_back(yVal);
		xCalc.addVal(xVal);
		yCalc.addVal(yVal);
	}

	//second pass to calc the pearson correlation coefficient
	double topSum = 0;
	double botSumX = 0;
	double botSumY = 0;

	for (int i = 0; i < xVals.size(); i++) {
		double xDiff, yDiff;
		xDiff = xVals[i] - xCalc.mean;
		yDiff = yVals[i] - yCalc.mean;
		topSum += xDiff * yDiff;
		botSumX += pow(xDiff, 2);
		botSumY += pow(yDiff, 2);
	}

	double r =  topSum / (pow(botSumX, 0.5) * pow(botSumY, 0.5));

	if (output == "r") cout << r << endl;
	else if (output == "r2") cout << pow(r,2) << endl;
	else cerr << "Unkown output format: " << output << endl;
	

}


