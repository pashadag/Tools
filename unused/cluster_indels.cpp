#include<cassert>
#include<cmath>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<queue>
#include<cstdarg>
#include<algorithm>
#include "limits.h"
//#include<mysql++.h>
//#include <boost/foreach.hpp>
#include "logClass.h"
#include "optionsClass.h"

using namespace std;
ostringstream oMsg;
logClass llog;
optionsClass opt;
string sbuf;
string filename, baseFilename;
#include "defs.h"

int colDist, colChr, colLeft, colRight, colID, colTemplate, baseLenFactor, mean, sd, mdJoinTolerance, mdSignifTol, avg_md_cuttoff;

template<class T>
class winScan {  //read in input file using a sliding window so as not to keep everything in memory.
public:
	
	deque<T> buf;	
	deque<bool> visited;

	winScan () {}
	winScan (istream * _in) : in(_in) {
		load_line();
	}
	void init (istream * _in) {
		in = _in;
		load_line();
	}


	void slideWin () {
		if (!buf.empty()){
			buf.pop_front();
			visited.pop_front();
		}
		if (buf.empty()) {
			load_line();
		}
	}	

	
	void incr(int & index) {
		assert (index >= 0);
		assert (!buf.empty());
		if (index == (buf.size() - 1)) {
			load_line();
		}
		index++;
	}

	istream * in;

private:

	bool load_line () {
		string s;
		if (getline(*in, s)) {
			//cout << "read line " << s << endl << flush;
			buf.push_back(make_T(s, dummyT));
			visited.push_back(false);
			return true;
		} else {
			return false;
		}
	}

	T dummyT;
	
};


string & make_T(string & s, string & dummy) {
	return s;
}

vector<string> make_T (string & s, vector<string> & dummy) {
	//tokenize the string
	vector<string> row;

	//cout << " RECEIVED " << s << endl;
	//cout << "TOKENIZED ";
	istringstream istrm(s);
	string val;
	while (istrm >> val) {
	       row.push_back(val);
	       //cout << val << ":";
	}
	//cout << endl;
	return row;


}

winScan<vector<string> > in;

int make_int(const string & s) {
	return atoi(s.c_str());
}

int codify_loc(vector<string> * line) {
	int chr;
	if (line->at(colChr) == "chrX") {
		chr = 23;
	} else if (line->at(colChr) == "chrY") {
		chr = 24;
	} else {
		chr = make_int(line->at(colChr).substr(3));
	}
	int loc = make_int(line->at(colLeft));
	return 100*loc + chr;
}

void dump_line (vector<string> * line) {

	if (colID != -1) cout << line->at(colID);  
	cout << "\tchr = " << line->at(colChr) << "\tleft = " << line->at(colLeft) << "\tright = " << line->at(colRight);
	cout << "\tmd = " << line->at(colDist) << "\ttemplate_id = " << line->at(colTemplate);
}

void dump_cluster(vector<vector< string> > curCluster, int & index, int clusterType) {
	//single hit is not a cluster
	if (curCluster.size() == 1) return;

	int i;

	//get the ranges of the matepair, as well as avg_md and sd
	
	int valLeft, valRight;
	double avg_md = 0;

	int x1, x2, x3, x4;
	//x1 is the leftmost start point of the left side of the generalized matepair
	//x2 is the rightmost start point of the left side of the generalized matepair
	//x3 is the leftmost start point of the right side of the generalized matepair
	//x4 is the rightmost start point of the right side of the generalized matepair

	//starting values	
	x1 = make_int(curCluster[0].at(colLeft)); 
	x2 = 0;
	x3 = make_int(curCluster[0].at(colRight));
	x4 = 0;


	for (i = 0; i < curCluster.size(); i++) {
		valRight = make_int(curCluster[i].at(colRight));
		valLeft  = make_int(curCluster[i].at(colLeft));
		if (valRight < x3) x3 = valRight;
		if (valRight > x4) x4 = valRight;
		if (valLeft  > x2) x2 = valLeft;
		if (valLeft  < x1) x1 = valLeft;

		avg_md += make_int(curCluster[i].at(colDist)) / double(curCluster.size());
	}


	//throw out if avg_md is too small
	if (avg_md < (avg_md_cuttoff)) return;
	//if (avg_md < (mdSignifTol*sd + mean)) return;

	
	vector<bool> skip;
	skip.resize(curCluster.size(), false);

	//throw away identical mappings, even if its different templates.  this is probably an artifact.
	string lastMd = "0";
	string lastLeft = "0";
	for (i = 0; i < curCluster.size(); i++) {
		if ((curCluster[i].at(colLeft) != lastLeft)) {
			lastMd = "0";
		}
		if (curCluster[i].at(colDist) == lastMd) skip[i] = true; 
		lastMd = curCluster[i].at(colDist);
		lastLeft = curCluster[i].at(colLeft);
	}

	//throw away all but the leftmost mapping of any template.
	set<string> templates;
 	pair<set<string>::iterator,bool> ret;
	for (i = 0; i < curCluster.size(); i++) {
		if (!skip[i]) {
			ret = templates.insert(curCluster[i].at(colTemplate));
			//cout << "RET " << *ret.first << " " << ret.second << endl;
			if (ret.second == false) skip[i] = true;
		}
	}



	int total = 0;
	for (i = 0; i < curCluster.size(); i++) {
		if (skip[i]) continue;
	       	total++;
	}
	if (total == 1) return;


	/*	
	
	//multiple hits of the same template are not a cluster;
	//single hit is not a cluster (again)
	string first = "bla";
	for (i = 0; i < curCluster.size(); i++) {
		if (skip[i]) continue;
		if (first == "bla") first = curCluster[i].at(colTemplate);
	       	total++;
		if (curCluster[i].at(colTemplate) != first) break;
	}
	if (i==curCluster.size()) return;
	if (total == 1) return;
	*/
	//find standard deviation
	
	double sd = 0;
	for (i = 0; i < curCluster.size(); i++) {
		if (!skip[i])
			sd += pow((atoi(curCluster[i].at(colDist).c_str()) - avg_md),  2.0) / double(curCluster.size());
	}
	sd = sqrt(sd);
	


	cout << endl;
	cout << "HEADER " << index << "\t" << curCluster.at(0).at(colChr) << "\t leftBase = |(" << x1 << ", " << x2 << ")| = " << x2 - x1;
	cout<< "\t rightBase = |(" << x3 << ", " << x4 << ")| = " << x4 - x3 << "\tmd = " << avg_md << " +- " << sd << endl;
	for (int i = 0; i < curCluster.size(); i++) {
		if (skip[i]) continue;
		cout << "CLUSTER" << clusterType << " " << index<< "\t";
		dump_line(&curCluster[i]);
		cout << endl << flush;
	}

	cout << curCluster.at(0).at(colChr) << "\t";
	int from = -1;
	int to = 1;
	int l_from = x2 - x1; 
	int l_to = x4 - x3;

	if (clusterType == 0) {
		from = x2; to = x3; 
	} else if (clusterType == 1) {
		//from = x4; to = x1; swap(l_from, l_to);
		from = x1; to = x4; 
	} else if (clusterType == 2) {
		from = x2; to = x4;
	} else if (clusterType == 3) {
		from = x1; to = x3; 
	}

	cout << from << "\t" << to << "\t" << clusterType << "\t" << total << "\t" << l_from << "\t" << l_to << "\t" << avg_md << "\t"  << index << "\tEDGE\n";

	index++;
	return;
}

void cluster_indels23(int clusterType) {

	//deque<vector<string> >::iterator it1, it2;
	//deque<bool >::iterator vit2;
	int curIndex;
	vector<string> * outerLine;
	vector<string> * innerLine;
	vector<vector<string> >  curCluster;  
	int outerMd, innerMd;
	int curClusterIndex = 1;
	assert(!in.buf.empty());

	int colMain, colSec;
	if (clusterType == 2) {
		colMain = colLeft;
		colSec  = colRight;
	} else {
		colMain = colRight;
		colSec  = colLeft;
	}	

	//Assume input is sored by colMain
	//For every unclustered line
	//	start a potential cluster with this line, topLine
	//	nextline = curline++
	//	while nextline.colMain is within mean + baseLenFactor*sd from topline.colMain
	//		goto nextline if this line has already been clustered
	//		if the difference between the main coordinates is within mdJoinToler*sd from the dif in sec coord
	//		then add this line to cluster

	do {
		if (!in.visited.front()) { //visited really means clustered
			outerLine = &in.buf.at(0);
			curCluster.push_back(*outerLine);
			//cout << "outer " << flush << outerLine->at(0) << " " << outerLine->at(1) << " " << outerLine->at(2) << " " << outerLine->at(3)  << endl << flush;
			outerMd = make_int(outerLine->at(colDist));
			curIndex = 0;
			in.incr(curIndex);
			while (curIndex < in.buf.size()) {   
				innerLine = &in.buf.at(curIndex);
				if ((innerLine->at(colChr) != outerLine->at(colChr)) || ((make_int(innerLine->at(colMain)) - make_int(outerLine->at(colMain))) > mean + baseLenFactor * sd)) 
					break;
				if (!in.visited.at(curIndex)) {
					innerMd = make_int(innerLine->at(colDist));
					int mainDiff = make_int(innerLine->at(colMain)) - make_int(outerLine->at(colMain));
					int secDiff = make_int(outerLine->at(colSec)) - make_int(innerLine->at(colSec));
					
					if (abs(mainDiff - secDiff) < mdJoinTolerance*sd) {
						in.visited.at(curIndex)  = true;
						curCluster.push_back(*innerLine);
					}
				}
				in.incr(curIndex); 
			}
			dump_cluster(curCluster, curClusterIndex, clusterType);
			curCluster.clear();
		}
		in.slideWin();

	} while (!in.buf.empty()); 



}

void cluster_indels01(int clusterType) {

	int curIndex;
	vector<string> * outerLine;
	vector<string> * innerLine;
	vector<vector<string> >  curCluster;  
	int outerMd, innerMd;
	int curClusterIndex = 1;
	assert(!in.buf.empty());

	int colMain;
	if (clusterType == 0) {
		colMain = colLeft;
	} else {
		colMain = colRight;
	}	

	do {
		if (!in.visited.front()) { //visited really means clustered
			outerLine = &in.buf.at(0);
			curCluster.push_back(*outerLine);
			//cout << "outer " << flush << outerLine->at(0) << " " << outerLine->at(1) << " " << outerLine->at(2) << " " << outerLine->at(3)  << endl << flush;
			outerMd = make_int(outerLine->at(colDist));
			curIndex = 0;
			in.incr(curIndex);
			while (curIndex < in.buf.size()) {   
				innerLine = &in.buf.at(curIndex);
				if ((innerLine->at(colChr) != outerLine->at(colChr)) || ((make_int(innerLine->at(colMain)) - make_int(outerLine->at(colMain))) > mean + baseLenFactor * sd)) 
					break;
				if (!in.visited.at(curIndex)) {
					innerMd = make_int(innerLine->at(colDist));
					if (abs(innerMd - outerMd) < mdJoinTolerance*sd) {
						in.visited.at(curIndex)  = true;
						curCluster.push_back(*innerLine);
					}
				}
				in.incr(curIndex); 
			}
			dump_cluster(curCluster, curClusterIndex, clusterType);
			curCluster.clear();
		}
		in.slideWin();

	} while (!in.buf.empty()); 

}


int main(int argc, char * argv[]) {
	init_options(argc, argv);

	//winScan<vector<string> > in(&cin);
	in.init(&cin);

	opt.get("colDist", colDist);
	opt.get("colChr", colChr);
	opt.get("colLeft", colLeft);
	opt.get("colRight", colRight);
	opt.get("colID", colID, false, -1);
	opt.get("colTemplate", colTemplate);

	
	
	opt.get("sd", sd);
	opt.get("mean", mean);
	opt.get("baseLenFactor", baseLenFactor);
	opt.get("mdJoinTolerance", mdJoinTolerance);
	opt.get("mdSignifTol", mdSignifTol);
	avg_md_cuttoff = 2 * mean;
	string type;
	opt.get("type", type);

	if (atoi(type.c_str()) < 2) 
	       	cout << "PARAMS Things will be clustered with a leader if  their mds are within " << mdJoinTolerance << " * sd = " << mdJoinTolerance *sd << endl; 
	else 
	       	cout << "PARAMS Things will be clustered with a leader if  the differences between their endpoints are within " << mdJoinTolerance << " * sd = " << mdJoinTolerance *sd << endl; 

       cout <<  "PARAMS The cluster will be extended at most mean + " << baseLenFactor << " * sd = " << mean + baseLenFactor * sd << " from the leader.\n";
       cout <<  "PARAMS All clusters with avg_md < " << avg_md_cuttoff << " are dropped.\n";
	cout << "CHR\tFROM\tTO\tTYPE\t#MP\tL_FROM\tL_TO\tDIST\tID\tEDGE\n";






	if (type == "0") {
		cluster_indels01(0);
	} else if (type == "1") {
		cluster_indels01(1);
	} else if (type == "2") {
		cluster_indels23(2);
	} else if (type == "3") {
		cluster_indels23(3);
	}
}





