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
#include<unistd.h>
//#include<mysql++.h>
//#include <boost/foreach.hpp>

using namespace std;
ostringstream oMsg;
string sbuf;
#include "defs.h"
#include "collapsed_db_from_genome.h"
#include "sa.h"

void coverage(suffixArray & sa, string filename) {
	string contig;
	vector<double> cov(sa.genSize, 0.0);
	vector<int> hits;
	while (getline(cin, contig)) {
		sa.find(contig, hits);
		for (int i = 0; i < hits.size(); i++) {
			cov.at(hits[i]) += 1 / double(hits.size());
		}
		sa.find(revcomp(contig), hits);
		for (int i = 0; i < hits.size(); i++) {
			cov.at(hits[i]) += 1 / double(hits.size());
		}
	}
	ofstream outf;
	open_file_binary(outf, filename);
	for (int i = 0; i < cov.size(); i++) {
		outf.write((char *) &cov[i], sizeof(double));
	}
	outf.close();
}

void mapp(suffixArray & sa, string inputMode, int col, bool fullMap) {
	string sbuf;

	if (col != -1) inputMode = "raw";

	while (getline(cin, sbuf)) {
		if (sbuf.length() == 0) {
			cout << endl;
			continue;
		}
		if (inputMode == "fasta") {
			if (sbuf[0] != '>') {
				cerr << "Not a valid fasta header at line: " << sbuf << endl;
				exit(1);
			}
			cout << sbuf << endl;
			getline(cin, sbuf);
		}

		int res  = -2;
		int res2 = -2;

		if (col == -1) {
			res = sa.find(sbuf);
			res2 = sa.find(revcomp(sbuf));
			res = max(res,res2);
			if (!fullMap && res != -1) {
				cout << sbuf << "\tmaps" << endl;
			} else {
				cout << sbuf << "\t" << res << endl;
			}

		} else {
			istringstream line(sbuf);
			string contig;
			int counter = 0;
			while (line >> contig) {
				counter++;
				if (counter == col) {
					res = sa.find(contig);
					res2 = sa.find(revcomp(contig));
				}
				if (counter > 1) cout << "\t";
				//cout << contig;
			}
			cout << sbuf;
			res = max(res,res2);
			if (!fullMap && res != -1) {
				cout << "\t" << "maps" << endl;
			} else {
				cout << "\t" << res << endl;
			}
		}

	}
	return ;
}

//this works even if the seq is spread over multiple lines
void fa2raw() {
	string sbuf;
	string seq;
	getline(cin,sbuf);
	while (getline(cin,sbuf)) {
		if (sbuf[0] == '>') { //header
			seq = "";
			cout << endl;
		} else {
			cout << sbuf;
		}
	}
	cout << endl;
	return;
}

void fq2fa() {
	string header, seq, line3, qv;
	while (getline(cin, header) ){
		getline(cin,seq);
		getline(cin,line3);
		getline(cin,qv);
		cout << ">" << header << endl << seq << endl;
	}
	return;
}

void fa2fq() {
	string header, seq, line3, qv;
	while (getline(cin, header) ){
		getline(cin,seq);
		string qvs(seq.length(), 'H');
		cout << "@" << header << endl << seq << endl << header << endl << qvs << endl;
	}
	return;
}
void raw2fa() {
	string read;
	int counter = 0;
	while (getline(cin, read) ){
		cout << ">" << counter++ << endl << read << endl;
	}
	return;
}

void fq2raw() {
	string header, seq, line3, qv;
	while (getline(cin, header) ){
		getline(cin,seq);
		getline(cin,line3);
		getline(cin,qv);
		cout << seq << endl;
	}
	return;
}

void rcFile(string inputMode, int col) {
	string sbuf;
	while (getline(cin, sbuf)) {
		if (inputMode == "fasta") {
			cout << sbuf << endl;
			getline(cin, sbuf);
		}
		if (col == -1) {
			sbuf = revcomp(sbuf);
			cout << sbuf<< endl;
		} else {
			istringstream line(sbuf);
			string contig;
			int counter = 0;
			while (line >> contig) {
				counter++;
				if (counter == col) contig = revcomp(contig);
				if (counter > 1) cout << "\t";
				cout << contig;
			}
			cout << endl;
		}
	}
}

void rcnormFile(string inputMode, int col) {
	string sbuf;
	while (getline(cin, sbuf)) {
		if (inputMode == "fasta") {
			cout << sbuf << endl;
			getline(cin, sbuf);
		}
		if (col == -1) {
			sbuf = rcnorm(sbuf);
			cout << sbuf<< endl;
		} else {
			istringstream line(sbuf);
			string contig;
			int counter = 0;
			while (line >> contig) {
				counter++;
				if (counter == col) contig = rcnorm(contig);
				if (counter > 1) cout << "\t";
				cout << contig;
			}
			cout << endl;
		}
	}
}

void maskSingleBlock(vector<vector<string> > &rows, vector<string> & strings, int col) {
	maskStrings(strings);
	for (int i = 0; i < rows.size(); i++) {
		for (int j = 0; j < rows[i].size(); j++) {
			if (j != 0) cout << "\t";
			if (j == col) 
				cout << strings[i];
			else
				cout << rows[i][j];
		}
		cout << endl;
	}
}

void maskLines(int col) {
	if (col == -1) col = 0; else col--;
	vector<vector<string> > rows;
	vector<string> row;
	vector<string> strings;
	while (get_row_whitespace(cin, row)) {
		if (row.size() == 0) { //spit out old block, start new one
			maskSingleBlock(rows, strings, col);
			rows.clear(); strings.clear();
			cout << endl;
		} else {
			rows.push_back(row);
			strings.push_back(row.at(col));
		}
	}
	maskSingleBlock(rows, strings, col);

}

void splitBlocks(int col) {
	if (col == 0) {
		cerr << "Column has to be 1-based.\n";
		exit(1);
	}
	if (col != -1) col--; //input is given as 1-based
	string curStr, prevStr;
	vector<string> curRow;
	vector<string> prevRow;
	while (get_row_whitespace(cin, curRow, curStr)) {
		if (col != -1) {
			if (curRow.size() <= col  || prevRow.size() <= col || curRow[col] == prevRow[col]) { 
				cout << curStr << endl;
			} else {
				cout << endl << curStr << endl;
			}
		} else {
			if (prevStr == "" || curStr == "" || prevStr == curStr) {
				cout << curStr << endl;
			} else {
				cout << endl << curStr << endl;
			}
		}
		prevRow = curRow;
		prevStr = curStr;
	}
}

void calcnstat(int nval) { //n50 or whatever
	string sbuf;
	vector<long> nums;
	long totSum = 0;
	while (getline(cin, sbuf)) {
		long len = sbuf.length();
		nums.push_back(len);
		totSum += len;
	}
	sort(nums.begin(), nums.end());
	long cursum = 0;
	double midval = totSum * ((100 - nval) / (double) 100);
	for (int i = 0; i < nums.size(); i++) {
		cursum += nums[i];
		if (cursum > midval) {
			cout << "N50\t" << nums[i] << endl;
			return;
		}
	}
}


void usage(int argc, char * argv[]) {
	cerr << "Usage: " << argv[0] << " db -g genome -k kmersize -o outfile" << endl;
	cerr << "\tThis program builds the de Bruijn graph of a genome and outputs it.\n";
	cerr << endl;
	cerr << "Usage: " << argv[0] << " sa -g genome -o outfile {-k kmersize}" << endl;
	cerr << "\tThis program builds the (single-stranded) suffix array of genome.fa and outputs it to outfile.sa. If k is specified, then a second column is added with the corresponding k-mer.\n";
	cerr << endl;
	cerr << "Usage: " << argv[0] << " map -g genome { -l column -f}}" << endl;
	cerr << "\tThis program maps (in double-stranded fashion) a set of contigs (from stdin) to a genome which has a suffix array built (genome.sa). If specified, then the column is interpreted as a contig, otherwise the file is treated as a fasta file. The ouput appends the original file with a final column that has 'map' or -1 (with -f, a location of a hit instead of map).\n";
	cerr << endl;
	cerr << "Usage: " << argv[0] << " rcnorm|rc -i fasta|raw  { -l column}" << endl;
	cerr << "\tThis program takes a set of sequences from stdin, normalizes or revcomps them, and prints to the stdout.\n";
	cerr << "\tIf the column is specified, then the l'th column is the one that is normalized.\n";
	cerr << endl;
	cerr << "Usage: " << argv[0] << " mask {-l column}" << endl;
	cerr << "\tThis program takes from stdin a set of lines, and outputs them with masks over positions where the strings in column l are identical.\n";
	cerr << endl;
	cerr << "Usage: " << argv[0] << " fa2raw/fq2fa/fq2raw/fa2fq/raw2fa " << endl;
	cerr << "\tThis program converts fasta files from stdin to raw sequence files in standard out.\n";
	cerr << endl;
	cerr << "Usage: " << argv[0] << " cov -g genome -o output_file" << endl;
	cerr << "\tThis program takes raw reads in, maps them to genome.fa/sa, and outputs a binary double array to output_file.\n";
	cerr << endl;
	cerr << "Usage: " << argv[0] << " n50 {-n val}" << endl;
	cerr << "\tThis program takes raw contigs in, and calculates the n50 score.  If val is specified, then the n\"val\" is calculate (n90, for example).\n";
	cerr << endl;
	cerr << "Usage: " << argv[0] << " block {-l column}" << endl;
	cerr << "\tThis program takes raw space delim files in, and whenever the text in the column between consecutive lines differs, a newline is inserted.\n";
	cerr << "\tIf column is not specified, then the whole lines are compared\n";
	exit(1);
}


int main(int argc, char * argv[]) {


	string genome;
	string contigsFilename;
	string genomeBase;
	string base;
	string inputMode = "fasta";
	int nval = 50;
	int column = -1;
	int kmersize = -1;
	bool fullMap = false;

	char ch;

	while ((ch = getopt(argc, argv, "l:g:k:n:o:c:i:f")) != -1) {
		switch (ch) {
			case 'g':
				genomeBase = optarg;
				genome = read_genome(genomeBase + ".fa");
				break;
			case 'k':
				kmersize = atoi(optarg);
				break;
			case 'n':
				nval = atoi(optarg);
				break;
			case 'l':
				column = atoi(optarg);
				break;
			case 'f':
				fullMap = true;
				break;
			case 'o':
				base = optarg;
				break;
			case 'c':
				contigsFilename = optarg;
				break;
			case 'i':
				inputMode = optarg;
				if (inputMode != "raw" && inputMode != "fasta") {
					cerr << "Unknown input mode: " << optarg << endl;
					usage(argc, argv);
				}
				break;
		}
	}
	if ( optind != argc - 1 ) { //should be one other parameter
		cerr << "Invalid parameters.\n";
		usage(argc, argv);
	}

	string task = argv[optind];
	if (task == "db") {
		if (genome == "" || base == "" || kmersize == -1 ) { //should be one other parameter
			cerr << "Invalid parameters.\n";
			usage(argc, argv);
		}
		collapsed_db_from_genome db(genome, kmersize);
		db.build();
		db.save(base + ".db");
		db.writeDOTbrief(base + ".brief.dot");
		db.writeDOT(base + ".dot");
	} else if (task == "sa") {
		if (genome == "" || base == "" ) { //should be one other parameter
			cerr << "Invalid parameters.\n";
			usage(argc, argv);
		}
		suffixArray sa(genome);
		sa.build();
		if (kmersize == -1) kmersize=0;
		sa.save(base + ".sa", kmersize);
	} else if (task == "map") {
		if (genome == "" ) { //should be one other parameter
			cerr << "Invalid parameters.\n";
			usage(argc, argv);
		}
		suffixArray sa(genome);
		sa.load(genomeBase + ".sa");
		mapp(sa, inputMode, column, fullMap);
	} else if (task == "rcnorm") {
		rcnormFile(inputMode, column);
	} else if (task == "rc") {
		rcFile(inputMode, column);
	} else if (task == "mask") {
		maskLines(column);
	} else if (task == "block") {
		splitBlocks(column);
	} else if (task == "cov") {  //not tested
		if (genome == ""  || base == "") { //should be one other parameter
			cerr << "Invalid parameters.\n";
			usage(argc, argv);
		}
		suffixArray sa(genome);
		sa.load(genomeBase + ".sa");
		coverage(sa, base);
	} else if (task == "fa2raw") {
		fa2raw();
	} else if (task == "fq2fa") {
		fq2fa();
	} else if (task == "fa2fq") {
		fa2fq();
	} else if (task == "raw2fa") {
		raw2fa();
	} else if (task == "fq2raw") {
		fq2raw();
	} else if (task == "n50") {
		calcnstat(nval);
	} else {
		cerr << "Unknown task: " << task << endl;
		exit(1);
	}

	return 0;
}


