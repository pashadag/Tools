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
#include<cstdarg>
#include<algorithm>

using namespace std;
bool softBreak = false;

void usage(int argc, char * argv[]) {
	cerr << "Usage: int_sort -s bufferSize [-b (for soft break in case of error)].\n";
	exit(1);
}

class list_t {
	public:
		int first;
		string second;

		list_t * next;
		list_t * prev;

};

list_t * front = NULL;
list_t * back = NULL;
int curSize = 0;

int main(int argc, char * argv[]) {

	int winSize = -1;

	char ch;
	while ((ch = getopt(argc, argv, "s:b")) != -1) {
		switch (ch) {
			case 's':
				winSize = atoi(optarg);
				break;
			case 'b':
				softBreak = true;
				break;
		}
	}
	if (winSize == -1) {
		usage(argc, argv);
	}

	string temp;
	string sbuf;
	while (getline(cin, sbuf)) {

		list_t * cur= new list_t;
		cur->first = atoi(sbuf.c_str());
		cur->second = sbuf;
		cur->next = front;
		if (cur->next != NULL) cur->next->prev = cur;
		cur->prev = NULL;
		front = cur;
		if (cur->next == NULL) back = cur;
		curSize++;

		while ( (cur->next != NULL) && (cur->first < cur->next->first)) {
			//swap cur with cur->next
			assert (cur->next != NULL);
			list_t * x1 = cur->prev;
			list_t * x2 = cur;
			list_t * x3 = cur->next;
			list_t * x4 = cur->next->next;

			
			x3->prev = x1;
			x3->next = x2;
			x2->prev = x3;
			x2->next = x4;
			if (x1 == NULL) {
				front = x3;
			} else {
				x1->next = x3;
			}
			if (x4 == NULL) {
				back = x2;
			} else {
				x4->prev = x2;
			}

		}

		if (curSize == winSize + 1) {
			if (cur->next == NULL) { // cannot sort, window to small.
				cerr << "Sorting failed at line: " << sbuf << endl;
				if (!softBreak) exit(1);
			}
			cout << back->second << endl;
			list_t * newback = back->prev;
			newback->next = NULL;
			delete back;
			back = newback;
			curSize--;
		}
	}

	//dump the rest of the window
	for (list_t * cur = back; cur != NULL; ) {
		cout << cur->second << endl;
		list_t * oldcur = cur;
		cur = oldcur->prev;
		delete oldcur;
	}


}

