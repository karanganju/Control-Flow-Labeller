#include "draw.h"
#include <fstream>
#include <iostream>
using namespace std;

void drawdot(int count, int SCC_count, string names[], int SCC[], int *edges) {
	int SCC_rep[SCC_count];
	ofstream myfile;
	myfile.open ("graph.dot");
	myfile << "digraph G {\n\tgraph [fontsize=10 fontname=\"Verdana\" compound=true];\n\tnode [shape=record fontsize=10 fontname=\"Verdana\"];\n\t";
	for (int i = 0; i < SCC_count; i++) {
		myfile << "subgraph cluster_" << i << "{\n\t\tlabel = \"SCC_" << i << "\";\n\t\t";
		for (int j = 0; j < count; j++) if (SCC[j] == i) {
			myfile << names[j] << "; ";
			SCC_rep[i] = j;
		}
		myfile << "\n\t}\n\t";
	}
	for (int i = 0; i < SCC_count; i++) for (int j = 0; j < SCC_count; j++) if (edges[i * SCC_count + j]) {
		myfile << names[SCC_rep[i]]<<" -> "<<names[SCC_rep[j]]<<" [ltail=cluster_" << i << " lhead=cluster_" << j << "];\n\t";
	}
	myfile << "}";
	myfile.close();
}

