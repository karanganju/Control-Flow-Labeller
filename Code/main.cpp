#include "parser.h"
#include "condenser.h"
#include "draw.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
	map<string, list<pair<string, int> > > callee_graph = parse(argv);

	//We create a string to int indexing and use it from now on to save space.
	//str_to_int is a mapping from function name to an index and int_to_str is the reverse mapping
	map<string, int> str_to_int;
	int count = 0;
	for (map<string, list<pair<string, int> > >::iterator i = callee_graph.begin(); i != callee_graph.end(); i++) str_to_int[i->first] = count++;
	string int_to_str[count];
	count = 0;
	for (map<string, list<pair<string, int> > >::iterator i = callee_graph.begin(); i != callee_graph.end(); i++) int_to_str[count++] = i->first;

	// Checks for parsing -- works for now
	// for (map<string, list<pair<string, int> > >::iterator i = callee_graph.begin(); i != callee_graph.end(); i++) {
	// 	cout<<i->first<<"X";
	// 	for (list<pair<string, int> >::iterator j = i->second.begin(); j != i->second.end(); j++) cout<<"X"<<j->first;
	// 	cout<<"X"<<endl;
	// }

	//For now we do not need the line number.
	//callee_graph_stripped represents a mapping of indexes connected to a list of other indexes
	list<int> callee_graph_stripped[count];
	for (map<string, list<pair<string, int> > >::iterator i = callee_graph.begin(); i != callee_graph.end(); i++) 
		for (list<pair<string, int> >::iterator j = i->second.begin(); j != i->second.end(); j++)
			callee_graph_stripped[str_to_int[i->first]].push_back(str_to_int[j->first]);

	//function_SCC gives a mapping between index and SCC number
	int SCC_count = 0;
	int* function_SCC = condense(callee_graph_stripped, count, SCC_count);
	int* SCC_transitions = (int *)malloc(SCC_count * SCC_count * sizeof(int));
	for (int i = 0; i < SCC_count; i++) for (int j = 0; j < SCC_count; j++) SCC_transitions[i * SCC_count + j] = 0;
	for (int i = 0; i < count; i++) for (list<int>::iterator j = callee_graph_stripped[i].begin(); j != callee_graph_stripped[i].end(); j++) SCC_transitions[function_SCC[i] * SCC_count + function_SCC[*j]] = 1;

	drawdot(count, SCC_count, int_to_str, function_SCC, SCC_transitions);

}