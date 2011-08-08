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


// GENERAL GRAPH OPERATIONS

typedef set< pair< long, long> > spAdjList; //to, length
typedef vector< spAdjList > spAdjType;

class CdbKeyValPair {
	public:
		int key;
		int value;
		CdbKeyValPair() : key(-1), value(-1) { }
		CdbKeyValPair(int x, int y) : key(x), value(y) {}
		bool operator< (const CdbKeyValPair & p) const { return value < p.value; }
};

void spRaw(vector<int> & dist, spAdjType & graph, priority_queue< CdbKeyValPair> & heap, set<int> &found, int maxDist) {

	// all nodes whithin maxDist of start are stored in distances along with their distances
	// dist has to be initialized to -1 and sizez of num vertices of the graph

	while (!heap.empty()) {
		CdbKeyValPair top = heap.top();
		heap.pop();
		top.value = top.value * -1;
		if (dist[top.key] != -1) continue;
		found.insert(top.key);
		dist[top.key] = top.value;
		//cout << "marked " << top.key << " " << top.value << endl;

		for (spAdjList::iterator i = graph[top.key].begin(); i != graph[top.key].end(); i++) {
			int spAdjNext = i->first;
			int spAdjLen  = i->second;
			if ((dist[spAdjNext] == -1) && (top.value + spAdjLen <= maxDist)) {
				heap.push(CdbKeyValPair(spAdjNext, -1 *(top.value + spAdjLen)));
			}
		}
	}
	return;
}

void sp(int start, vector<int> & dist, spAdjType & graph, set<int> &found, int maxDist = -1 ) {
	if (maxDist == -1) maxDist = numeric_limits<int>::max();
	priority_queue<CdbKeyValPair> heap;
	heap.push(CdbKeyValPair(start, 0));
	dist.resize(graph.size(), -1);
	spRaw(dist, graph, heap, found, maxDist);
}


/*class DotEdge {
public:
	string from;
	string to;
	string payload;

};
*/

void extractSubgraph(string filename, int vertex, int depth) { 
	vector<string> payloads;
	ifstream dotFile;
	open_file(dotFile, filename);
	vector<string> line;

	//Recall:
	//    typedef set< pair< long, long> > spAdjList; //to, length
	//    typedef vector< spAdjList > spAdjType;

	//get number of nodes
	int largestNodeIndex = 0;
	while(get_row_whitespace(dotFile, line)) {
		if (line.size() > 2 && line[1] == "->") { //this is an edge
			largestNodeIndex = max(max(atoi(line[0].c_str()), atoi(line[2].c_str())), largestNodeIndex);
		}
	}
	spAdjType graph(largestNodeIndex + 1);

	//read in graph from dot file into spAdjType
	//NOTE: this only works with numeric identifiers for now
	string sbuf;
	dotFile.close();
	open_file(dotFile, filename);
	while(get_row_whitespace(dotFile, line, sbuf)) {
		if (line.size() > 2 && line[1] == "->") { //this is an edge
			graph.at(atol(line[0].c_str())).insert(make_pair(atol(line[2].c_str()), 1));
			graph.at(atol(line[2].c_str())).insert(make_pair(atol(line[0].c_str()), 1));
			payloads.push_back(sbuf);
		}
	}

	//find nodes at an undirected distance of "depth"
	vector<int> dist;
	set<int> found;
	sp(vertex, dist, graph, found, depth - 1);

	cout << "digraph DB {\n";
	for(int i = 0; i < payloads.size(); i++) {
		istringstream line(payloads[i]);
		string dummy;
		int from, to;
		line >> from >> dummy >> to;

		if (found.find(from) != found.end() || found.find(to) != found.end()) { 
			cout << payloads[i] << endl;
		}
	}
	cout << "}\n";
}

