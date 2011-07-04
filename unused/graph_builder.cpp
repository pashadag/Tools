#ifndef NO_CPLEX
#define NO_CPLEX
#endif 
#ifndef NO_CS2
#define NO_CS2
#endif

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
#include<mysql++.h>
//#include <boost/foreach.hpp>
#include "union.h"
#include "logClass.h"
#include "optionsClass.h"



using namespace std;
ostringstream oMsg;
logClass llog;
optionsClass opt;
string sbuf;
string filename, baseFilename;


#include "graph.h"
#include "iofunct.h"
#include "defs.h"
//#include"collapse.cpp"
#include "dbIterator.h"

typedef graphClass< overlapVertexInfo, overlapEdgeInfo> overlapGraphClass;

#ifndef NO_CS2
	// #include "../include/cs2.c"
#endif

mysqlpp::Connection conn[2];

int hash_read_name(string templateName, bool dir) {
	return 2 * atoi(templateName.c_str()) + dir;
}
/*int hash_read_name(string readName, map< string, int > * tids = NULL) { //like 123_F3
	string templateName = readName.substr(0, readName.length() - 3);
	map<string, int>::iterator it;
	bool dir(false);
	if (readName.at(readName.length() - 2) == 'F') 
		dir = FWD_DIR;
	else if (readName.at(readName.length() - 2) == 'R') 
		dir = REV_DIR;

	if (tids == NULL)
		return hash_read_name(templateName, dir);
	else {
		it = tids->find(templateName);
		assert (it != tids->end());
		return hash_read_name(make_string(it->second), dir);
	}
}
*/
int hash_read_name(string readName, vector< string > * tidsBack = NULL) { //like 123_F3
	string templateName = readName.substr(0, readName.length() - 3);
	vector<string>::iterator it;
	bool dir(false);
	if (readName.at(readName.length() - 2) == 'F') 
		dir = FWD_DIR;
	else if (readName.at(readName.length() - 2) == 'R') 
		dir = REV_DIR;

	if (tidsBack == NULL)
		return hash_read_name(templateName, dir);
	else {
		it = lower_bound(tidsBack->begin(), tidsBack->end(), templateName);
		assert (*it == templateName);
		return hash_read_name(make_string(it - tidsBack->begin()), dir);
	}
}

string unhash_read_name(int id, vector<string> * tidsBack = NULL) {
	string dir;
	if ((id % 2) == 0) dir = affix[0]; else dir = affix[1];
	string tid;
	if (tidsBack == NULL)
		tid = make_string(int(id / 2));
	else 
		tid = tidsBack->at(id/2);
	return tid + dir + "3";
}




bool get_row(ifstream & inFile, vector<string> & row, bool & dir) {  
	string s;
	row.clear();
	getline(inFile, s, '\t');
	if (inFile.eof()) return false;
	if (s == "F") dir = FWD_DIR; else if (s == "R") dir = REV_DIR; else { cerr << "Unexpected string: " << s << endl; assert(0); }
	getline(inFile, s, '\t'); getline(inFile, s, '\t'); //skip next two entries
	for (int i = 0; i < NUM_ENTRIES - 1; i++) {
		getline(inFile, s, '\t'); 
		row.push_back(s);
	}
	getline(inFile, s); // the last entry will be terminated by a newline, not a tab
	row.push_back(s);
	return true;
}

void load_fasta_file(const string & key, string & genome, bool skipN = false) {
	char c;
	opt.get(key, filename);
	ifstream inFile;
	char buf[257];
	inFile.open(filename.c_str());
	if (!inFile) llog.error ("Cannot open input file: " + filename + ".");
	llog << "Reading in fasta sequence file...\n" << flush;
	inFile.get(buf, 256);	
	llog  << "First line of file is " << buf << ".\n" << flush;
	bool maskReference = opt.get("mask_reference");

	//read the file into genome
	while ((c = inFile.rdbuf()->sbumpc()) != EOF) {
		if (c != '\n')
			if (!skipN || (c != 'N'))
				genome.push_back((maskReference ? c : toupper(c) ));
				//genome.push_back(toupper(c));

	}
	inFile.close();
	return;
}

void repeat_mask_rows() {

	string genome;
	load_fasta_file("ref", genome);
	opt.get("table", filename);
	ifstream in(filename.c_str());
	opt.get("out", filename);
	ofstream out(filename.c_str());
	vector< string> row;

	while (get_row(in, row, '\t')) {
		int loc = atoi(row.at(15).c_str());
		int id = atoi(row.at(25).c_str());
		if (isupper(mid_char(genome, loc, 25))) {
			out << row[0];
			for (int i = 1; i < row.size(); i++) out << '\t' << row[i];
			out << '\t' << endl;
		}

	}
	in.close();
	out.close();
}
/*
void cluster_indels() {

	winScan<vector<string> > in(&cin);
	int i = 1;
	deque<vector<string> >::iterator it1, it2;
	int outerMd, innerMd; //mapped distance
	int curCluster = 0; 
	//visited really means clustered
	for (it1 = in.buf.begin(); it1 != in.buf.end(); in.incrIt(it1)) { 
		if (in.visited.at(it1 -  in.buf.begin())) continue;
		outerMd = it1->at(0);
		it2 = it1;
		in.incrIt(it2);
		while ((it2 != in.buf.end())) {
			if (in.visited.at(it2 - in.buf.begin())) continue;
			innerMd = it2->at(0);
			if (will cluster) {
				in.visited.at(it2 - in.buf.begin()) = true; 
				cout << curCluster << " " << it2->at(0) << endl;
			}
		}

		curCluster++;
	}


}
*/
void build_read_classes( vector<int> & data, vector< vector< int > > & otherWay ) {
	int prevStartPos, curStartPos, curReadId, prevReadId;
	vector<string> curRow, prevRow;
	int numReads;

	if (!opt.get("num_reads", numReads, false)) {
		llog.error("Please specify num_reads in ini, or fix this code to do it automatically.");
	}
	unionFindClass readClasses(numReads);
	opt.get("read_file", filename);
	//filename = baseFilename + ".im.ord";
	ifstream inFile(filename.c_str());
	llog << "Building read classes based on data in " << filename << "...\n" << flush;
	assert(inFile);
	
	prevStartPos = -1;
	get_row(inFile, curRow); //empty line
	while (get_row(inFile, curRow, ' ')) { 			
		//curReadId = hash_read_name(curRow.at(22), (curRow.at(24) == "F" ? FWD_DIR : REV_DIR));
		//curStartPos = atoi(curRow.at(15).c_str());
		//llog << curRow << endl;
		curReadId = hash_read_name(curRow.at(0));
		curStartPos = atoi(curRow.at(2).c_str());
		
		//llog << curReadId << " " << curStartPos << endl;
		readClasses.find_set(curReadId);  //creates a set for this read
		if ((prevStartPos == curStartPos) && (prevReadId != curReadId)) {
				readClasses.unionn(prevReadId, curReadId);
		}
		prevStartPos = curStartPos;
		prevReadId = curReadId;
	}  
	int numClasses = readClasses.num_classes();

	llog << "Found " << numClasses << " classes out of " << readClasses.num_elements() << "  elements.\n" << flush;
	llog << "Consolidating data structures...\n" << flush;
	readClasses.get_classes(data, otherWay);	

	ofstream out;
	opt.get("uf_file", filename);
	//filename = baseFilename + ".uf";
	llog << "Writing out union find mappings to " << filename << "...\n" << flush;
	out.open(filename.c_str());
	for (int i = 0; i < data.size(); i++) {
		if (data[i] != -1) {
			out << unhash_read_name(i) << '\t' << data[i] << endl;
		}
	}
	out.close();

/*	filename = baseFilename + ".internal.equiv";
	llog << "Writing out equivalence classes to " << filename << "...\n" << flush;
	out.open(filename.c_str());
	out << otherWay << endl << data;
	out.close();
*/
	
	
}


void build_read_classes_CLARA( vector<int> & data, vector< vector< int > > & otherWay ) {
	int prevStartPos, curStartPos, curEndPos, curReadId, prevReadId;
	vector<string> curRow, prevRow;
	int numReads;
	ifstream in;
	//map<string, int> tids;
	vector<string> tidsBack;
	
	opt.get("template_ids", filename);
	llog << "Reading in template ids from file " << filename << endl << flush;
	in.open(filename.c_str());
	assert(in);
	vector<string> row;
	int count = 0;
	while (get_row(in,row)) {
		if ((++count % 10000000) == 0) llog << "Read in " << add_commas(count) << " template ids.\n" << flush;
		tidsBack.push_back(row.at(1));
	}

	numReads = 2*tidsBack.size();
	unionFindClass readClasses(numReads);
	opt.get("read_file", filename);
	//filename = baseFilename + ".im.ord";
	ifstream inFile(filename.c_str());
	llog << "Building read classes based on data in " << filename << "...\n" << flush;
	assert(inFile);
	
	prevStartPos = -1;
	
	char buf[256];
	char * tok;
	while (inFile.getline(buf, 255)) {
		//llog << buf << " : ";
		tok = strtok(buf, " ");
		curReadId   = hash_read_name(tok, &tidsBack);
		tok = strtok(NULL, " "); tok = strtok(NULL, " ");
		curStartPos = atoi(tok);
		//llog << curReadId << " : " << curStartPos << endl;
		//llog << curRow.at(0) << " " << curReadId << " " << unhash_read_name(curReadId, &tidsBack) << endl << flush;		

		readClasses.find_set(curReadId);  //creates a set for this read
		if ((prevStartPos == curStartPos) && (prevReadId != curReadId)) {			
			readClasses.unionn(prevReadId, curReadId);
		}
		prevStartPos = curStartPos;
		prevReadId = curReadId;
	}  
/*	while (get_row(inFile, curRow, ' ')) { 			
		curReadId   = hash_read_name(curRow.at(0), &tidsBack);
		curStartPos = atoi(curRow.at(2).c_str());
		
		//llog << curRow.at(0) << " " << curReadId << " " << unhash_read_name(curReadId, &tidsBack) << endl << flush;		

		readClasses.find_set(curReadId);  //creates a set for this read
		if ((prevStartPos == curStartPos) && (prevReadId != curReadId)) {			
			readClasses.unionn(prevReadId, curReadId);
		}
		prevStartPos = curStartPos;
		prevReadId = curReadId;
	}  

*/
	int numClasses = readClasses.num_classes();

	llog << "Found " << numClasses << " classes out of " << readClasses.num_elements() << "  elements.\n" << flush;
	llog << "Consolidating data structures...\n" << flush;
	readClasses.get_classes(data, otherWay);	

	ofstream out;
	opt.get("uf_file", filename);
	//filename = baseFilename + ".uf";
	llog << "Writing out union find mappings to " << filename << "...\n" << flush;
	out.open(filename.c_str());
	for (int i = 0; i < data.size(); i++) {
		if (data[i] != -1) {
			out << unhash_read_name(i, &tidsBack) << '\t' << data[i] << endl;
		}
	}
	out.close();
}

void load_read_classes(vector<int> & readClasses, vector< vector< int> > & otherWay) {
	string s;
	filename = baseFilename + ".internal.equiv";
	llog << "Loading equivalence classes to " << filename << "...\n" << flush;
	ifstream in(filename.c_str());
	in >> otherWay >> readClasses ;

	/*while (in >> s >> s) {
		readClasses.push_back(atoi(s.c_str()));
	}*/
	in.close();
}


bool my_gt_comp(const string & row1, const string & row2) {
	istringstream s1(row1);
	istringstream s2(row2);
	string s, chr1, chr2;
	int start1, start2;
	
	getline(s1, s, ' ');getline(s1, chr1, ' '); getline(s1, s, ' '); start1 = atoi(s.c_str());
	getline(s2, s, ' ');getline(s2, chr2, ' '); getline(s2, s, ' '); start2 = atoi(s.c_str());

	if (chr1 == chr2) {
		return start1 > start2;
	} else {
		return chr1 > chr2;
	}
}


void sort_file () { //using insertion_sort
	vector< string> A;
	opt.get("in", filename);
	ifstream in(filename.c_str());
	string s, row;
	while (getline(in,s)) {
		if ((A.size() % 1000000) == 0) cerr << "Read in " << A.size() << " rows.\n";
		A.push_back(s);
	}
	//while(get_row(in, row, ' ')) A.push_back(row);  cerr << "Done reading in file.\n";
	for (int i = 1; i < A.size(); i++) {
		if ((i % 1000000) == 0) cerr << "Done sorting first " << i << " locations.\n";
		row = A[i];
        	int j = i-1;
		while ((j >= 0) && (my_gt_comp(A[j], row))) {
			A[j + 1] = A[j];
			j = j-1;
		}
		A[j+1] = row;
		//cout << A[i-1] << endl;
	}
	//cout << A.back();

	cerr << "Outputing...\n";
	for (int i = 0; i < A.size(); i++) {
		cout << A[i] << endl;
	}
	cerr << "Done.\n";
	
}

int main(int argc, char * argv[]) {
	overlapGraphClass overlapGraph;
	vector<int> readClasses;
	vector<vector< int > > otherWay;
	opt.get("base_file_name", baseFilename, false);

	init_options(argc, argv);	

	if (opt.get("build_read_classes")) {
		build_read_classes(readClasses, otherWay);
	//} else if (opt.get("cluster_indels")) {
	//	cluster_indels();
	} else if (opt.get("build_read_classes_CLARA")) {
		build_read_classes_CLARA(readClasses, otherWay);
	} else if (opt.get("repeat_mask_rows")) {
		repeat_mask_rows();
	/*} else if (opt.get("build_graph")) {
		load_read_classes(readClasses, otherWay);
		//build_vertices(readClasses, overlapGraph);
		//build_ref_edges(readClasses, otherWay, overlapGraph);
		
		filename = baseFilename + ".ref_graph";
		llog << "Dumping overlap graph based on reference genome into " << filename << "...\n" << flush;
		ofstream out(filename.c_str());	
		assert(out);
		overlapGraph.dump_velvet(out);
		out.close();
	*/
	} else if (opt.get("sort_file")) {
		sort_file();
	} else if (opt.get("test")) {
		string x;
		int  y;
		while (cin >> x >> y) 
			cout << endl << unhash_read_name(hash_read_name(x,y)) << endl;



		
	}

	return 0;
}

/*
void build_ref_edges(vector<int> & readClasses, vector< vector< int> > & otherWay, overlapGraphClass & overlapGraph) {
	
	llog << "Building reference overlap graph...\n" << flush;	
	overlapGraph.vertices.resize(otherWay.size());

	filename = baseFilename + ".im.mul.ord";
	ifstream inFile(filename.c_str());

	bool curDir;
	bool prevOr(false), curOr(false);
	int prevVertexId, curVertexId, curReadId, prevReadId;
	vector<string> curRow;

	overlapEdgeInfo edgeInfo;
	prevVertexId = -1;
	while (get_row(inFile,curRow,curDir)) {
		curReadId = hash_read_name(curRow.at(TEMPLATE_ID_FIELD), curDir);
		curVertexId = readClasses.at(curReadId);
		assert (curVertexId != -1);
		if (prevVertexId != -1) {
			overlapGraph.create_edge(prevVertexId, curVertexId, prevOr, curOr, edgeInfo);
		}
		prevReadId = curReadId;
		prevVertexId = curVertexId;
	}
	inFile.close();	
}
*/
