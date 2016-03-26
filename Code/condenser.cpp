#include "condenser.h"

int *node, *lowlink, *SCC;
bool* stacked;
stack<int> onStack;

int ind = 0, ind_SCC = 0;

void strongconnect(list<int>* cg_stripped, int v) {
	node[v] = lowlink[v] = ind++;
	onStack.push(v);
	stacked[v] = true;

	for (list<int>::iterator i = cg_stripped[v].begin(); i != cg_stripped[v].end(); i++) {
		if (node[*i] == -1) {
			strongconnect(cg_stripped, *i);
			lowlink[v] = min(lowlink[v], lowlink[*i]);
		}
		else if (stacked[*i]) {
			lowlink[v] = min(lowlink[v], node[*i]);
		}
	}

	if (lowlink[v] == node[v]) {
		int w;
		do {
			w = onStack.top();
			stacked[w] = false;
			onStack.pop();
			SCC[w] = ind_SCC;
		} while (w != v);
		ind_SCC++;
	}
}

int* condense(list<int> * cg_stripped, int count, int &SCC_count) {
	node = (int *)malloc(count * sizeof(int));
	lowlink = (int *)malloc(count * sizeof(int));
	SCC = (int *)malloc(count * sizeof(int));
	stacked = (bool *)malloc(count * sizeof(bool));
	for (int i = 0; i < count; i++) {
		node[i] = lowlink[i] = SCC[i] = -1;
		stacked[i] = false;
	}

	for (int i = 0; i < count; i++) if (node[i] == -1) strongconnect(cg_stripped, i);

	SCC_count = ind_SCC;
	return SCC;
}