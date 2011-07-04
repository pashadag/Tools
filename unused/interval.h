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
        bool overlaps(const interval &c) { // but not cont
                if (c.chr != chr) return false;
                if ((c.start <= start) && (c.end >= start)) return true;
                if ((c.start <= end)   && (c.end >= end)) return true;
                return false;
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

ostream & operator << (ostream & out, const interval & i) {
	//out << i.chr << " " << i.start << " " << i.end;
	//if (i.label != "") out << " " << i.label;
	out << i.chr << "\t" << i.start << "\t" << i.end;
	if (i.label != "") out << "\t" << i.label;
	return out;
}


