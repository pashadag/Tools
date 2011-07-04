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
#include "../../include/logClass.h"
#include "../../include/optionsClass.h"



using namespace std;
optionsClass opt;
char buf[500];

int main(int argc, char * argv[]) {
	if (argc < 2) {
		cerr << "Usage: extract filename.\n";
		exit(1);
	} 
	if (!opt.read_command_line(argv + 2, argc - 2)) logClass::error("Cannot read command line options.");

	string filename = argv[1];

	ifstream inFile;
	inFile.open(filename.c_str());
	if (!inFile) {
		cerr <<  "Cannot open input file: " <<  filename  << endl;
		return 0;
	}
	//cout << "Reading in fasta sequence file from " << filename << "...\n" << flush;
	inFile.get(buf, 256);
	//cout << "First line of file is " << buf << ".\n" << flush;
	

	//read the file into genome
	string genome;
	char c;
	bool skipN = opt.get("skipN");
	while ((c = inFile.rdbuf()->sbumpc()) != EOF) {
		if (c != '\n') {
			if (!skipN || (c != 'N'))
				genome.push_back(c);

		}
	}
	inFile.close();


	int startpos;
	int endpos;
	string cmd, s, input;
	//cout << "Input finished, with " << add_commas(genome.length()) << " character.\n";
	while (cmd != "q") { 
		//cout << " Enter command (p, rc, q) : ";
		cin  >> cmd;
		if (cmd == "p" ) {
			//cout << "Please enter startpos: " << flush;
			cin >> input;
			startpos = atoi(input.c_str());
			//cout << "Please enter endpos: " << flush;
			cin >> input;
			if ((input.length() > 0) && (input.at(0) == '+')) {
				endpos = startpos - 1 + atoi(input.substr(1).c_str());
			} else
				endpos = atoi(input.c_str());
			if ((startpos > endpos) || (startpos < 0) || (endpos >= genome.length())) {
				cerr << "Bad Input\n"; 
				continue;
			}
			s = genome.substr(startpos, endpos - startpos + 1);
			cout << "> " << filename << " " << startpos << " " << endpos << endl << s << endl;
		} else if (cmd == "rc") {
			cout << "> " << filename << " rc " << endl << revcomp(s) << endl;

		} 
	}




	return 0;
}

