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
//#include<mysql++.h>
//#include <boost/foreach.hpp>

using namespace std;
ostringstream oMsg;
string sbuf;
#include "defs.h"
#include "graph.h"


void usage(int argc, char * argv[]) {
	cerr << "Usage: " << argv[0] << " subgraph -g graphFile -n vertex -d depth" << endl;
	cerr << "\tThis program builds loads the graphFile in DOT format, then outputs the graph (in DOT)\n";
	cerr << "\tinduced by all vertices within a 'depth' undirected distance from vertex.\n";
	cerr << "\tNote that the identifiers in the graph have to be numeric for now.\n";
	cerr << endl;
	exit(1);
}

int main(int argc, char * argv[]) {

	string graphDOTFile;
	int searchDepth = -1;
	int mainNode = -1;

	char ch;

	while ((ch = getopt(argc, argv, "g:n:d:")) != -1) {
		switch (ch) {
			case 'g':
				graphDOTFile = optarg;
				break;
			case 'n':
				mainNode = atoi(optarg);
				break;
			case 'd':
				searchDepth = atoi(optarg);
				break;
		}
	}
	if ( optind != argc - 1 ) { //should be one other parameter
		cerr << "Invalid parameters.\n";
		usage(argc, argv);
	}

	string task = argv[optind];
	if (task == "subgraph") {
		if (graphDOTFile == "" || searchDepth == -1 || mainNode == -1) {
			cerr << "Invalid parameters.\n";
			usage(argc, argv);
		}
		extractSubgraph(graphDOTFile, mainNode, searchDepth);
	} else {
		cerr << "Unknown task: " << task << endl;
		exit(1);
	}
	return 0;
}
