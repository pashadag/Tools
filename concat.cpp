#include<cassert>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
#include<list>
#include<cstdarg>
#include<algorithm>

using namespace std;


int main(int argc, char * argv[]) {

	ifstream in;
	in.open(argv[1]);
	
	char buf[256];

	in.get(buf, 256);

	string word;

	while (in >> word) {
		for (int i = 0; i < word.length(); i++) {
			if ((word[i] == 'A')|| (word[i] == 'C') || (word[i] == 'T') || (word[i] == 'G')) {
				cout << word[i];
			} else if (word[i] == 'a') cout << "A";
			 else if (word[i] == 'c') cout << "C";
			 else if (word[i] == 't') cout << "T";
			 else if (word[i] == 'g') cout << "G";
	//		else cerr << word[i];
		}

	}

}
