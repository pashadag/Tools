#include<cassert>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
#include<cstdarg>
#include<algorithm>
#include<ctime>
#include "optionsClass.h"
#include "interval.h"


using namespace std;


void load_fasta_file (string filename, string & genome, bool skipN) {
	ifstream inFile;
	char buf[256];
	char c;
	inFile.open(filename.c_str());
	if (!inFile) {
		cerr << "Cannot open input file: " << filename << ".";
		exit(1);
	}

	//cout << "Reading in fasta sequence file...\n" << flush;
	inFile.get(buf, 256);
	//cout  << "First line of file is " << buf << ".\n" << flush;
	bool maskReference = false; //opt.get("mask_reference");

	while ((c = inFile.rdbuf()->sbumpc()) != EOF) {
	if (c != '\n')
		if (!skipN || (c != 'N'))
			genome.push_back((maskReference ? c : toupper(c) ));
	}
	
	inFile.close();
	return;
}



class Event {
public:
	int type;
	int loc; //location of indel
	int size; //size of indel
	int insSource; //source of inserted sequence
	bool operator<(const Event & e) const { return loc < e.loc; }
        friend ostream& operator << (ostream &out, const Event &r);
};

ostream& operator << (ostream& out, const Event &r ) {
	out << "type=" << r.type << " loc=" << r.loc << " size=" << r.size << " insSource=" << r.insSource;
	return out;

}


int pickLoc (int size, vector<bool> & usedLoc) {
	bool used;
	int loc;
	int it = 0;
	do { 
		used = false; 
		loc = randNumber(0, usedLoc.size() - size);
		for (int pos = loc; pos < loc + size; pos++) used = used || usedLoc[pos];
		if (it++ > 1000) {
			cerr << "Saturated genome.  Aborting.\n";
			exit(1);
		}
	} while (used);
	for (int pos = loc; pos < loc + size; pos++) usedLoc[pos] = true; 
	return loc;
}


/*
 * usage - print a usage message
 * 		reference_genome: path to fasta-formatted reference genome
 * 		number: number of cnvs to simulate
 * 		type: 1 - just deletions , 2 - just random insertions, 4 - just tandem duplications 
 * 		type: think of these as bit values so to combine them just add the values.
 */
void usage(char ** argv) { 
	printf("usage: %s <reference_genome> <min_size> <max_size> <type> <number> <output_genome> <output_annotation>\n",argv[0]); 
}
#define EXPECTED_ARGS 7 	// expected number of input arguments


int main(int argc, char ** argv) {

	/* check input args */
	if (argc != EXPECTED_ARGS+1) { usage(argv); exit(1); }
	int min_size = atoi(argv[2]);
	int max_size = atoi(argv[3]);
	int type = atoi(argv[4]);
	int number_of_cnvs = atoi(argv[5]);
	ofstream outGen(argv[6]);
	ofstream outAnn(argv[7]);


	/* open input file */
	string orig_genome;
	load_fasta_file (argv[1], orig_genome, false);


	// parse type
	if (type < 1 || type > 7) { fprintf(stderr,"error: <type> must be between 1 and 7\n"); exit(1); }
	bool validEvents[3];
	validEvents[0]     = type % 2; //dels
	validEvents[1]     = (type >> 1) % 2; //randIns
	validEvents[2]     = (type >> 2) % 2; //tandDups
	//cout << "ValidEvents: " << validEvents[2] << validEvents[1] << validEvents[0] << endl;


	srand ( time(NULL) );
	vector<bool> usedLoc(orig_genome.size(), false);
	for (int i = 0; i < usedLoc.size(); i++) {
		if (orig_genome[i] == 'N') usedLoc[i] = true;
	}
	vector<Event> events;
	

	
	//create events
	for (int it=0; it < number_of_cnvs; it++) {
		//randomly choose event type from allowed types
		Event event;
		event.type = randNumber(0,2);
		while (!validEvents[event.type]) event.type = randNumber(0,2);

		if (event.type == 0) { //deletion
			event.size = randNumber(min_size, max_size);
			event.loc = pickLoc(event.size, usedLoc);
		} else if (event.type == 1) { //random insertion
			event.size = randNumber(min_size, max_size);
			event.insSource = pickLoc(event.size, usedLoc);
			event.loc = pickLoc(1, usedLoc);
		} else if (event.type == 2) { //tandem duplication
			event.size = randNumber(min_size, max_size);
			event.insSource = pickLoc(event.size + 1, usedLoc);
			event.loc = event.insSource + event.size;
			usedLoc.at(event.loc) = true;
		}

		events.push_back(event);
		//cout << event << " UsedLoc: ";
	       	//for (int i = 0; i < usedLoc.size(); i++) if (usedLoc[i]) cout << i;
		//cout << endl;
	}


	//execute events
	string new_genome = orig_genome;
	sort(events.begin(), events.end());
	for (int i = events.size() - 1; i >= 0; i--) {
		if ( events[i].type == 0) { //del
			new_genome = new_genome.erase(events[i].loc, events[i].size);
		} else if ( (events[i].type == 1) || (events[i].type == 2) ) {  //ins
			new_genome = new_genome.insert(events[i].loc, orig_genome, events[i].insSource, events[i].size);
		}
	}

	//output new genome 

	outGen << ">varsim with params min_size=" << min_size << " max_size=" << max_size << " type=" << type << " number=" << number_of_cnvs << endl << new_genome << endl;
	outGen.close();

	//output annotations file
	for (int i = 0; i < events.size(); i++) {
		if (events[i].type == 0) { //del
			outAnn << "chr1 " << events[i].loc << " " << events[i].loc + events[i].size - 1 << " -1\n";
		} else if ( (events[i].type == 1) || (events[i].type == 2) ) { //ins
			outAnn << "chr1 " << events[i].insSource << " " << events[i].insSource + events[i].size - 1 << " +1 ";
		       	outAnn << events[i].loc - 1 << " " << events[i].loc << "\n";
		}
	}
	outAnn.close();

	return 0;
}
