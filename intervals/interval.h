class interval {  //the intervals are assumed to contain the endpoints.
	public:
		string chr;
		int start;
		int end;
		string label;

		bool operator< (const interval & i) const {
			if (chr == i.chr) return start < i.start;
			return chr < i.chr;
		}
		/*bool operator<= (const interval & i) const {
		  if (chr == i.chr) return start <= i.start;
		  return chr < i.chr;
		  }*/

		bool contains(const interval & c1) const {
			if (c1.chr != chr) return false;
			if ((start <= c1.start) && (c1.end <= end)) return true;
			return false;
		}

		bool contains(const string _chr, const int point) {
			if (_chr != chr) return false;
			if ((start <= point) && (point <= end)) return true;
			return false;
		}

		bool overlaps(const interval &c) { // note containment counts as overlaping
			if (c.chr != chr) return false;
			if ((c.start <= start) && (c.end >= start)) return true;
			if ((c.start <= end)   && (c.end >= end)) return true;
			if ((start <= c.start) && (c.end <= end)) return true;
			return false;
		}
		int amountThatOverlaps(const interval &c) const {
			if (c.chr != chr) return 0;
			if ((c.start <= start) && (start <= c.end)) return min(end - start + 1, c.end - start + 1);
			if ((c.start <= end)   && (end <= c.end)) return min(end - start + 1, end - c.start + 1);
			if ((start <= c.start) && (c.end <= end)) return c.end - c.start + 1;
			return 0;
		}

		bool fullyLeftOf(const interval &c) {
			if (chr != c.chr) return (chr < c.chr);
			if (end < c.start) return true;
			return false;
		}
		bool fullyRightOf(const interval &c) {
			if (chr != c.chr) return (chr > c.chr);
			if (c.end < start) return true;
			return false;
		}

};

bool comp_le (const interval &i1, const interval &i2) {
	if (i1.chr == i2.chr) return i1.start <= i2.start;
	return i1.chr < i2.chr;
}
/*bool comp_le_by_right (const interval &i1, const interval &i2) {
  if (i1.chr == i2.chr) return i1.end <= i2.end;
  return i1.chr < i2.chr;
  }
  bool fullyLeftOf(const interval &i1, const interval &i2) {
  if (i1.chr != i2.chr) return i1.chr < i2.chr;
  return i1.end < i2.start;
  }
 */

ostream & operator << (ostream & out, const interval & i) {
	//out << i.chr << " " << i.start << " " << i.end;
	//if (i.label != "") out << " " << i.label;
	out << i.chr << "\t" << i.start << "\t" << i.end;
	if (i.label != "") out << "\t" << i.label;
	return out;
}

template<class T>
void read_intervals(string filename, T & intervals) {
	string sbuf;
	ifstream file(filename.c_str());

	while (getline(file, sbuf)) {
		istringstream line(sbuf);
		interval i;
		line >> i.chr >> i.start >> i.end >> i.label; //should really be till end of line, not just the fourth
		intervals.push_back(i);
	}
}



