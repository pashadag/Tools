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

string add_commas(string s) {
	string retval;
	for (int i = 0; i < s.length(); i++) {
		retval.push_back(s.at(i));
		int j = s.length() - 1 - i;
		if (((j % 3) == 0) && (j != 0)) {
			retval.push_back(',');
		}
	}
	return retval;
}




void usage(int argc, char * argv[]) {
	cerr << "Usage: " << argv[0] << " column" << endl;
	cerr << "Program descrption.\n";
	exit(1);
}


int main(int argc, char * argv[]) {
	if (argc != 2) usage(argc, argv);

	int col = atoi(argv[1]);

	while (getline(cin, sbuf)) {
		istringstream line(sbuf);
		string elem;
		int counter = 1;
		while (line >> elem) {
			if ( counter  == col) {
				string fixed = add_commas(elem);
				if (fixed != "") { 
					cout << fixed << "\t";
				} else {
					cout << elem << "\t";
				} 
			} else {
				cout << elem << "\t";
			}
			counter++;
		}
		cout << endl;
	}

	return 0;
}


