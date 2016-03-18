// Works on command produced by cflow -rlrd2 <rtl-file>
// Add support for same name different return val, params functions by changing fn_name
#include "parser.h"

map<string, list<pair<string, int> > > callee_graph;

string fn_name = "";
list<pair<string, int> > callees;

void add_callee_function(string line) {
	if (fn_name != "") callee_graph[fn_name] = callees;
	int pos = line.find("}") + 2;
	int end = line.find(")") - 1;
	fn_name = line.substr(pos, end - pos);
	callees.clear();
}

void add_caller_function(string line) {
	int pos = line.find("}") + 6;
	int end = line.find(")") - 1;
	string callee_name = line.substr(pos, end - pos);
	pos = line.find(":") + 1;
	end = line.find(">");
	int lineno = atoi(line.substr(pos, end - pos).c_str());
	callees.push_back(make_pair(callee_name, lineno));
}

map<string, list<pair<string, int> > > parse(char* argv[]) {
	string line;
	ifstream file (argv[1]);
	if (!file.is_open()) {
		cout << "Unable to open file!\n";
		exit(1);
	}

	while (!file.eof()) {
		getline(file, line);
		if (line[line.find("}") - 1] == '0') add_callee_function(line);
		else if (line[line.find("}") - 1] == '1') add_caller_function(line);
		else if (line == "") {
			callee_graph[fn_name] = callees;
			continue;
		}
		else {
			cout << "Unable to parse!\n";
			exit(1);
		}
	}
	file.close();
	// for(map<string, list<pair<string,int> > >::iterator i = callee_graph.begin();i!=callee_graph.end();i++){
	// 	cout<<i->first<<endl;
	// 	for(list<pair<string,int> >::iterator j = i->second.begin();j!=i->second.end();j++){
	// 		cout<<"\r"<<j->first<<" "<<j->second<<endl;
	// 	}
	// }
	return callee_graph;
}
