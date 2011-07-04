#ifndef DBITERATORCLASS_H
#define DBITERATORCLASS_H

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

using namespace std;

const int ID_FIELD[2] = {1,2};
const int START_POS_FIELD[2] = {8, 11};
const int END_POS_FIELD[2]= {9, 12};
const int TEMPLATE_ID_FIELD = 13;
const int FWD_DIR = 0;
const int REV_DIR = 1;
const int NUM_ENTRIES = 28;
const string direction[2] = {"fwd", "rev"};
const string affix[2] = {"_F", "_R"};





class dbIteratorClass{
public:
	ifstream file[2];
	vector<string> row[2];
	bool done[2];
	bool curDir;  

	dbIteratorClass(string basefilename) {
		string filename[2] = {basefilename + ".fwd", basefilename + ".rev"};
		file[FWD_DIR].open(filename[0].c_str());
		file[REV_DIR].open(filename[1].c_str());
		if (!file[0] || !file[1]) llog.error("Couldn't open fwd or rev im files");
		for (int dir = 0; dir < 2; dir++) done[dir] = !read_row(dir); // assuming there is at least one line
	}

	bool read_row (int dir) {
		string s;
		if (file[dir].eof()) return false;
		row[dir].clear();
		for (int i = 0; i < NUM_ENTRIES; i++) {
			file[dir] >> s;
			row[dir].push_back(s);
		}
		getline(file[dir], s);

		return true;
	}

	
	bool get_row(vector<string> & curRow, bool & dir) {  //is this the correct passing mechanism ?
		int startPos[2] = {-1, -1};

		while (done[0] && done[1]) return false;


		//llog << "revStartPos = " << revStartPos << " fwdStartPos = " << fwdStartPos << " ----------- " << flush   ;
		for (int i = 0; i < 2; i++) {
			if (!done[i]) startPos[i] = atoi(row[i].at(START_POS_FIELD[i]).c_str());
		}
		
		if (!done[REV_DIR] && (done[FWD_DIR] || (startPos[REV_DIR] < startPos[FWD_DIR]))) { 
			curRow = row[REV_DIR];  //this better be a real copy and not just a reference!!!
			curDir = REV_DIR; 
			if (!read_row(REV_DIR)) done[REV_DIR] = true;
		} else { assert(!done[FWD_DIR]); 
			curRow = row[FWD_DIR];
			//llog << "fwdRow start = " << fwdStartPos << endl << flush;
			curDir = FWD_DIR; 
			if (!read_row(FWD_DIR)) done[FWD_DIR] = true;
		} 
		dir = curDir;
		return true;
	} 
	
};


#endif
/*
int hash_read_name(string templateName, bool dir) {
	templateName = templateName.substr(5, 4) + templateName.substr(10, 4);
	return 2 * atoi(templateName.c_str()) + dir;
}


class dbIteratorClass{
public:
	mysqlpp::Connection * conn;
	string table_name;
	int curChr;
	string curChrStr;
	mysqlpp::UseQueryResult res[2];
	mysqlpp::Row row[2];
	bool done[2];
	bool curDir;  
	
	dbIteratorClass(mysqlpp::Connection * _conn, const string & table__name) :  conn(_conn), table_name(table__name), curChr(1), curChrStr("1") { init_chr(); }

	void init_chr() {
		try {
			mysqlpp::Query query[2] = {conn[0].query(), conn[1].query()};
			for (int i = 0; i < 2; i++) {
				query[i] = conn[i].query();
				query[i] << "SELECT * FROM interesting_matepairs_" << table_name << " WHERE " << direction[i] << "_chr='chr" << curChrStr << "' ORDER BY " << direction[i] << "_start_pos";
				cerr << query[i] << endl;
				res[i] = query[i].use();
				row[i] = res[i].fetch_row();
				done[i] = !row[i];
			}
		} catch (mysqlpp::Exception e) {
			cerr << "General mysqlpp error in init_chr: " << e.what() << endl;
			exit(1);
		}

	}

	bool get_row(mysqlpp::Row & curRow, bool & dir, ChrInterval * curInt = NULL) {  //is this the correct passing mechanism ?
		int startPos[2] = {-1, -1};

		while (done[0] && done[1]) { //chromosome finished
			if (curChr == 23) { //done with all chromosomes!
				return false;
			} else {  // increment chromosome
				curChr++;
				if (curChr == 23) {
					curChrStr = "X";
				} else {
					curChrStr = make_string(curChr);
				}
				init_chr();
			}
		}


		//llog << "revStartPos = " << revStartPos << " fwdStartPos = " << fwdStartPos << " ----------- " << flush   ;
		for (int i = 0; i < 2; i++) {
			if (!done[i]) startPos[i] = row[i].at(START_POS_FIELD[i]);
		}
		
		if (!done[REV_DIR] && (done[FWD_DIR] || (startPos[REV_DIR] < startPos[FWD_DIR]))) { 
			curRow = row[REV_DIR];  //this better be a real copy and not just a reference!!!
			curDir = REV_DIR; 
			if (!(row[REV_DIR] = res[REV_DIR].fetch_row())) done[REV_DIR] = true;
		} else { assert(!done[FWD_DIR]); 
			curRow = row[FWD_DIR];
			//llog << "fwdRow start = " << fwdStartPos << endl << flush;
			curDir = FWD_DIR; 
			if (!(row[FWD_DIR] = res[FWD_DIR].fetch_row())) done[FWD_DIR] = true;
		} 
		dir = curDir;
		if (curInt != NULL) {
			curInt->start = curRow[START_POS_FIELD[curDir]];
			curInt->end   = curRow[  END_POS_FIELD[curDir]];
			curInt->readId = hash_read_name(curRow[TEMPLATE_ID_FIELD].c_str(), curDir);
			//curRow[TEMPLATE_ID_FIELD].to_string(curInt->dbId);
			//curInt->dbId += affix[curDir];

		}
		return true;
	} 
	
};


// int hash_read_name(int templateName, bool dir) {
//	return (templateName * 2) + dir; 
// }
//string unhash_read_name(int readName) {
//	string retval;
//	return make_string(readName / 2) + affix[readName % 2];
//}


int hash_read_name(string readName) {
	bool dir;
	if (readName.substr(readName.length() - 2) == "_R") {
		dir = REV_DIR;
	} else if (readName.substr(readName.length() - 2) == "_F") {
		dir = FWD_DIR;
	} else {
		logClass::error("Unexpected error in hash_read_name with readName " + readName );
	}
	return hash_read_name(atoi(readName.substr(0, readName.length() - 2).c_str()), dir);
}
*/
