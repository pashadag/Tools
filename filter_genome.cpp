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
//#include<mysql++.h>
//#include <boost/foreach.hpp>

using namespace std;
ostringstream oMsg;
string sbuf;
string filename, baseFilename;

void usage(int argc, char * argv[]) {
	cerr << "Usage: " << argv[0] << endl;
	cerr << "Removes everything except [acgtACGT] from the genome (fasta given as stdin), and capitilizes everything.\n";
	exit(1);
}


int main(int argc, char * argv[]) {

	if (argc != 1)  usage(argc, argv);
	char buf[257];

	//read the header
	cin.get(buf, 256);
	cout << buf << endl;

	char c;
	long counter = 0;
	while ((c = cin.rdbuf()->sbumpc()) != EOF) {
		if (c != '\n') {
			c = toupper(c);
			if(c == 'A' || c =='T' || c =='C' || c == 'G') {
				cout << c;
				if ((++counter % 50) == 0) cout << endl;
			}
		}
	}

	return 0;

}

