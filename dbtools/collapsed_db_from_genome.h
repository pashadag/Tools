#include<cassert>
#include<cmath>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<queue>
#include<cstdarg>
#include<algorithm>

using namespace std;
#include "defs.h"



class edge {
	public:
		int from;
		int to;
		int length;
		int seqStart; //genomic location corresponding to the start of the sequence along this edge.
		bool operator<(const edge & e) const {
			if (from != e.from)  return from < e.from;
			return to < e.to;
			//if (to != e.to)  return to < e.to; return length < e.length;
		}
		bool operator==(const edge & e) const {
			return (e.from == from && e.to == to);
			//return (e.from == from && e.to == to && e.length == length);
		}
};

ostream & operator<<(ostream & o, const edge & e) {
	o << e.from << "\t" << e.to << "\t" << e.length << "\t" << e.seqStart;
	return o;

}

struct compeqFunctor {
	bool operator()(int x, int y) const {
		return genome.substr(x,kmersize) == genome.substr(y,kmersize);
	}
	int kmersize;
	string genome;
};

struct compltFunctor {
	bool operator()(int x, int y) const {
		if (x==y) return false;
		for (int i = 0; i < genSize - max(x,y) + 1; i++) {
			if (genome[x+i] != genome[y+i]) return genome[x+i] < genome[y+i];
		}
		return x > y; //no, this is not a mistake
	}
	string genome;
	int genSize;
};

struct compeqEdgeSequenceFunctor {
	bool operator()(edge x, edge y) const {
		if (x.from == y.from && x.to == y.to && genome.substr(x.seqStart, x.length) == genome.substr(y.seqStart, y.length)) return true;
		return false;
	}
	string genome;
};


struct compltEdgeSequenceFunctor {
	bool operator()(edge x, edge y) const {
		if (x.from < y.from) return true; else if (x.from > y.from) return false;
		if (x.to < y.to) return true; else if (x.to > y.to) return false;
		return (genome.substr(x.seqStart, x.length) < genome.substr(y.seqStart, y.length));
	}
	string genome;
};
	
class collapsed_db_from_genome {
	public:
		string genome;
		int kmersize;
		int genSize;
		vector<bool> branching;
		//int numNodes;
		vector<edge> edges;
		vector<int> sa2node;

		collapsed_db_from_genome (const string & g, int k) {
			genSize = g.size();
			genome = g + g;
			kmersize = k;
		}

		void save(string filename) {  
			ofstream out;
			open_file(out, filename);
			for (int i = 0; i < edges.size(); i++) out << "E\t" << edges[i] << endl;
			for (int i = 0; i < branching.size(); i++) if (branching[i]) out << "B\t" << i << endl;
			for (int i = 0; i < sa2node.size(); i++) if (sa2node[i] != -1) out << "G\t" << i << "\t" << sa2node[i] << endl;
			out.close();
		}
		void load(string filename) { 
			edges.clear(); branching.clear(); sa2node.clear();
			ifstream in;
			open_file(in, filename);
			string sbuf;
			assert(genSize > 0);
			branching.resize(genSize, false);
			sa2node.resize(genSize, -1);
			while (getline(in, sbuf)){
				istringstream line(sbuf);
				char c;
				line >> c;
				if (c == 'E') {
					int from, to, length, seqStart;
					line >> from >> to >> length >> seqStart;
					edge e; e.from = from; e.to = to; e.length = length; e.seqStart = seqStart;
					edges.push_back(e);
				} else if (c == 'B') {
					int val;
					line >> val;
					branching.at(val) = true;
				} else if (c == 'G') {
					int index, val;
					line >> index >> val;
					sa2node.at(index) = val;
				} else {
					cerr << "Unkown input line: " << sbuf << endl;
					exit(1);
				}
			}
			in.close();

		}

		/* Write the de bruijn graph out as a graph in the DOT language.
		 */
		void writeDOT(string filename) { 
			ofstream out;
			open_file(out, filename);
			out << "digraph DB {" << endl;
			for (int i = 0; i < edges.size(); i++) {
				out << edges[i].from << " -> " << edges[i].to << " [ label = \"" << edges[i].length <<  "\" ]; " << endl;
				//out << edges[i].from << " -> " << edges[i].to << " [ label = \"" << edges[i].length << "_" << edges[i].seqStart <<  "\" ]; " << endl;
			}
			out << "}" << endl;
			out.close();

		}

		/* Write the de bruijn graph out as a graph in the DOT language.
		   Edges that have no parallel edges are not printed.
		 */
		void writeDOTbrief(string filename) {
			ofstream out;
			open_file(out, filename);
			out << "digraph DB {" << endl;
			int numMultEdges = 0;
			int numEdges = 1; //this counts parallel edges as one
			bool flag = true;
			for (int i = 1; i < edges.size(); i++) {
				if (edges[i] == edges[i-1]) {
					if (flag) {
						numMultEdges++;
						out << edges[i-1].from << " -> " << edges[i-1].to << " [ label = \"" << edges[i-1].length << "\" ]; " << endl;
					}
					out << edges[i].from << " -> " << edges[i].to << " [ label = \"" << edges[i].length << "\" ]; " << endl;
					flag = false;
				} else {
					numEdges++;
					flag = true;
				}
			}

			out << "}" << endl;
			out.close();
		}

		void build() {
			vector<int> sa;
			vector<int> saback; //gen2sa

			cout << "Building suffix array...\n";
			for (int i = 0; i < genSize; i++) {
				sa.push_back(i);
			}

			compltFunctor complt; 
			complt.genome = genome;
			complt.genSize = genSize;
			sort(sa.begin(), sa.end(), complt);

			compeqFunctor compeq;
			compeq.genome = genome;
			compeq.kmersize = kmersize;

			//Build saback, branching
			saback.resize(genSize);
			int uniqCounter=0;
			branching.resize(sa.size(), false);
			saback[sa[0]]=0;
			for (int i = 1; i< sa.size(); i++) {
				if (!compeq(sa[i], sa[i-1])) {
					uniqCounter++;
				} else {
					if (genome[sa[i]+ kmersize] != genome[sa[i-1] + kmersize] || 
							genome[(sa[i] - 1) % genSize] != genome[(sa[i-1] - 1) % genSize]) 

						branching[uniqCounter]=true; // branching vertex
				}
				saback[sa[i]]=uniqCounter;

			}

			cout << "Removing duplicates...";
			sa.erase(unique(sa.begin(), sa.end(), compeq), sa.end());
			cout << sa.size() << " elements remaining in sa\n";
			assert(uniqCounter == sa.size() - 1);

			sa2node.resize(sa.size(), -1);
			int nodeCounter = 0;
			for (int i = 0; i < sa.size(); i++) {
				if (branching[i]) {
					sa2node[i] = nodeCounter;
					nodeCounter++;
				}
			}

			//there is a bug here if the graph is just a single loop
			cout << "Walking the genome and adding edges.\n";
			for (int i = 0; i < genSize; i++) {
				if (!branching[saback[i]]) continue;
				edge e;
				int ff = fastfwd(saback,i+1);

				e.from = sa2node[saback[i]];
				e.to   = sa2node[saback[ff]];
				e.seqStart = i;
				e.length = (ff - i + genSize) % genSize;
				edges.push_back(e);
				//cout << "Adding " << e.from << " --> " << e.to << endl;
			}

			sort(edges.begin(), edges.end()); 
		}
		
		void removeIdenticalEdges() {
			compltEdgeSequenceFunctor complt;
			complt.genome = genome;
			compeqEdgeSequenceFunctor compeq;
			compeq.genome = genome;
			sort(edges.begin(), edges.end(), complt);
			edges.erase(unique(edges.begin(), edges.end(), compeq), edges.end());
			return;
		}

		void writeEdgeSeqs(string filename) {
			ofstream out;
			open_file(out, filename);
			for (int i = 0; i < edges.size(); i++) {
				out << genome.substr(edges[i].seqStart, edges[i].length) << endl;
			}
			out.close();
		}


	private:
		int fastfwd(const vector<int> & saback, int cur) {
			cur = cur % genSize;
			while (!branching[saback[cur]]) cur = (cur + 1) % genSize;
			return cur;
		}
};



