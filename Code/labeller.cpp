#include "labeller.h"

int* label(int SCC_count, map<string, int> str_to_int, int function_SCC[], int SCC_transitions[]){
	//Very simplistic labelling algo used - distance from main
	int SCC_main= function_SCC[str_to_int["main"]];
	queue<int> bfs;
	int *labelling = (int *)malloc(SCC_count * sizeof(int));
	bool visited[SCC_count] = {false};

	visited[SCC_main] = true;
	labelling[SCC_main] = 0;
	bfs.push(SCC_main);

	while(!bfs.empty()){
		for(int i=0;i<SCC_count;i++) if (SCC_transitions[i*SCC_count + bfs.front()] && !visited[i]) {
			visited[i] = true;
			labelling[i] = labelling[bfs.front()]+1;
			bfs.push(i);
		}
		bfs.pop();
	}

	return labelling;
}