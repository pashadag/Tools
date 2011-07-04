#ifndef CPCOUNT_IOFUNCT_H
#define CPCOUNT_IOFUNCT_H

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
typedef int bint;

//dynamic buffer



template<class T>
class winScan {
public:
	
	deque<T> buf;	
	deque<bool> visited;

	winScan (istream * _in) : in(_in) {
		typename deque<T>::iterator dummy = buf.end();
		incrIt(dummy);
	}


	void slideWin () {
		buf.pop_front();
		visited.pop_front();
	}	

	

	void incrIt(typename deque<T>::iterator & it) {
		it++;
		if (it == buf.end()) { 
			string s;
			if (getline(*in, s)) {
				buf.push_back(make_T(s, dummyT));
				visited.push_back(false);
				it = buf.end(); it--;
			}
		}
	}


private:
	istream * in;
	T dummyT;
	
};


string & make_T(string & s, string & dummy) {
	return s;
}

vector<string> make_T (string & s, vector<string> & dummy) {
	//tokenize the string
	vector<string> row;

	//cout << " RECEIVED " << s << endl;
	//cout << "TOKENIZED ";
	istringstream istrm(s);
	string val;
	while (istrm >> val) {
	       row.push_back(val);
	       //cout << val << ":";
	}
	//cout << endl;
	return row;


}
#endif 
