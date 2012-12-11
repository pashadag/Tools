#ifndef OPTIONSCLASS_H
#define OPTIONSCLASS_H

using namespace std;

typedef int bint;
map<string, bool> optionsClassPrintOnlyOnceMemory;


template<class T>
string make_string(const T & s) {
        ostringstream o;
        o << s;
        return o.str();
}

string add_commas(int num) {
	string s, retval;
	s = make_string(num);
	for (int i = 0; i < s.length(); i++) {
		retval.push_back(s.at(i));
		int j = s.length() - 1 - i;
		if (((j % 3) == 0) && (j != 0)) {
			retval.push_back(',');
		}
	}
	return retval;
}

string revcomp(const string &s) {
	string t("");
	for (int i = s.length() - 1; i >= 0; i--) {
		if (s[i] == 'A') 
			t.push_back('T');
		else if (s[i] == 'C') 
			t.push_back('G');
		else if (s[i] == 'G') 
			t.push_back('C');
		else if (s[i] == 'T') 
			t.push_back('A');
		else if (s[i] == 'a') 
			t.push_back('t');
		else if (s[i] == 'c') 
			t.push_back('g');
		else if (s[i] == 'g') 
			t.push_back('c');
		else if (s[i] == 't') 
			t.push_back('a');
		else if (s[i] == 'N') 
			t.push_back('N');


	}
	return t;
}

char complement(const char & c) {
		if (c == 'A') 
			return 'T';
		else if (c == 'C') 
			return 'G';
		else if (c == 'G') 
			return 'C';
		else if (c == 'T') 
			return 'A';
		return c;
}
string complement(const string &s) {
	string t("");
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == 'A') 
			t.push_back('T');
		else if (s[i] == 'C') 
			t.push_back('G');
		else if (s[i] == 'G') 
			t.push_back('C');
		else if (s[i] == 'T') 
			t.push_back('A');
	}
	return t;
}

char mid_char (const string & str, const int & beg, const int & len) {  //len should be odd
	return str.at(beg + ((len - 1)/ 2));
}
//compare two equal length strings with either none, one, or both of them revcomped on the fly.
bool bidir_string_lt_compare(const string & str1, const int & beg1, const string & str2, const int & beg2, const int & len) {

	int i;
	char c1, c2;
	char mid1;
	char mid2;
	bool flip1, flip2;

	if ((&str1 == &str2) && (beg1 == beg2)) return false;

	mid1 = str1.at(beg1 + ((len - 1)/ 2));
	mid2 = str2.at(beg2 + ((len - 1)/ 2));
	flip1 = ((mid1 == 'T') || (mid1 == 'G'));
	flip2 = ((mid2 == 'T') || (mid2 == 'G'));

	for (i = 0; i < len; i++) {
		if (flip1) 
			c1 = complement(str1.at(beg1 + len - i - 1));
		else 
			c1 = str1.at(beg1 + i);

		if (flip2) 
			c2 = complement(str2.at(beg2 + len - i - 1));
		else 
			c2 = str2.at(beg2 + i);
		  
		if (c1 < c2) {
			return true;
		} else if (c1 > c2) {
			return false;
		}
	}
	return false;
}

bool bidir_string_eq_compare(const string & str1, const int & beg1, const string & str2, const int & beg2, const int & len) {
	return (!bidir_string_lt_compare(str1, beg1, str2, beg2, len) && !bidir_string_lt_compare(str2, beg2, str1, beg1, len));
}

// Generate a random integer in the range [min, max]
inline int randNumber (int min, int max) { return min + int((((double) rand()) / RAND_MAX ) * (max - min + 1) ); }

// Generate a random double in the range [min, max]
inline double randNumberDouble (double min, double max) { return min + (((double) rand()) / RAND_MAX ) * (max - min) ; }

class optionsClass {
public:
	optionsClass() : data() {}
	bool  load(const char * fileName) {
		ifstream file(fileName);
		if (!file) return false;
		string key;
		string value;
		while (file >> key >> value) {
			if (key[0] != '#') data.insert(make_pair(key,value));
		}
		file.close();
		return true;
	}

	bool read_command_line (char * argv[], int size) {
		string s, key, val;
		cerr << "Command Line Options:\n";
		for (int i = 0; i < size; i++) { //for each of the options
			s = argv[i];
			string::size_type loc = s.find('=',0);
			if (loc == string::npos) { //boolean option
				key = s;
				val = "1";
			} else {  // bint option
				key = s.substr(0, loc);
				val = s.substr(loc+1);
			}
			if (data.find(key) == data.end()) {
				data.insert(make_pair(key,val));  //new element
			} else { // element already exists
				data.find(key)->second = val;
			}

			cerr << "    Key = " << key << ". Val = " << val << endl << flush;
		}
		return true;

	}
	void find_and_replace(const string &find, const string &repl) {
		map<string,string>::iterator it;
		string::size_type loc; 

		for (it = data.begin(); it != data.end(); it++) {
			loc = it->second.find(find);
			if (loc != string::npos ) { //found hit
				it->second.erase(loc, find.length());
				it->second.insert(loc, repl);
			}
		}
	}			

	bool get(const string &key, string &value, bool fail=true, string def="") {
		map<string, string>::iterator iter = data.find(key);
		if (iter == data.end()) {
			if (fail) {
				cerr << "Cannot get " <<  key << " from config file.  Fatal error.";
				exit(1);
			} else {
				value = def;
			}
			return false;
		}
		value = iter->second;
		return true;
	}


	bool get(const string &key, bint &value, bool fail=true, bint def=0) {
		string s;
		bool found = get(key,s,fail);
		if (!found)
			value = def;
		else {
			value = atol(s.c_str());
		}
		return found;
	}
	
	bool get(const string &key) {
		bint flag;
		get(key,flag,false,0);
		if (flag == 0) return false; else return true;
	}



	friend ostream& operator << (ostream &out, const optionsClass &opt);

	string toString() const  {
		string s;
		for (map<string, string>::const_iterator iter= data.begin(); iter != data.end() ;iter++ ) {
			s += "Key = " + iter->first + ". Value = " + iter->second + "\n";
		}
		return s;	
	}
private:
	map<string, string> data;

};


ostream& operator << (ostream& out, const optionsClass &opt) {	return (out << opt.toString()); }

#endif
