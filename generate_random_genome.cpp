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

inline int randNumber (int min, int max) { return min + int((((double) rand()) / RAND_MAX ) * (max - min + 1) ); }

void usage(int argc, char * argv[]) {
	cout << "Usage: " << argv[0] << " <len> {<seed>}" << endl;
	exit(1);
}

int main(int argc, char * argv[]) {
	if ((argc != 2) && (argc != 3)) usage(argc, argv);
	int len = atoi(argv[1]);
	if (argc == 3) {
		srand(atoi(argv[2]));
	} else {
		srand(time(NULL));
	}

	cout << ">random genome\n";
	for (int i = 0; i < len; i++) {
		int r = randNumber(0,3);
		if (r == 0) cout << "A";
		else if (r == 1) cout << "C";
		else if (r == 2) cout << "G";
		else if (r == 3) cout << "T";
	}


}

