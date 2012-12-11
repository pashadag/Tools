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

// Hard masks fasta file input.  Converts any lower case dna characters to N.  Leaves everything else as is

void usage(int argc, char * argv[]) {
	cout << "Usage: " << argv[0] << endl;
	exit(1);
}

int main(int argc, char * argv[]) {
	if (argc != 1) usage(argc, argv);
	char c;

	string line;

	while (getline(cin, line)) {
		istringstream sLine(line);
		sLine.get(c);
		if (c == '>') {
			cout << line;
		} else {
			do {
				if ((c == 'a') || (c == 'c') || (c == 'g') || (c == 't')) 
					c = 'N';
				cout << c;
				sLine.get(c);
			} while (sLine.good());
		}
		cout << endl;
	}
}

