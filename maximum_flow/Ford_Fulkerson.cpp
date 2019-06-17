#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

int n;
vector<vector<int> > graph, rGraph;
int source, sink;


// LOAD INPUT FROM FILE
// first line - whole number n - number of vetices in graph
// then - (n x n) matrix - adjacency matrix of the graph 
void loadInput() {

	string filename;
	cout << "Please enter input file name: ";
	cin >> filename;

	ifstream f(filename);
	
	if (f.is_open()) {
		
		f >> n;

		if(n < 2) {
			cout << "Number of vertices has to be at least 2!" << endl;
			f.close();
			exit(0);
		}
		
		graph.resize(n);
		for(int i = 0; i < n; i++) {
			graph[i].resize(n);
			for(int j = 0; j < n; j++) {
				f >> graph[i][j];
			}
		}
		
		f.close();

	} else {
		cout << "Unable to open file." << endl;
		exit(0);
	}

	cout << "\nGraph has been loaded! Please enter source vertex and sink vertex." << endl;
	cout << "(vertices are indexed from zero)" << endl;

	cout << "SOURCE: ";
	while(!(cin >> source) || source < 0 || source >= n) {
		cout << "Invalid value - {0,...," << n-1 << "}" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "SOURCE: ";
	}

	cout << "SINK: ";
	while(!(cin >> sink) || sink < 0 || sink >= n || sink == source) {
		cout << "Invalid value - {0,...," << n-1 << "} and also sink has to be different from source!" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "SINK: ";
	}
}

// find augmenting path from source to sink and return it
vector<int> BFS() {

	vector<int> parent(n, -1);

	queue<int> q;
	q.push(source);

	while (!q.empty()) {
		int v = q.front();
		q.pop();
		
		for (int i = 0; i < n; i++) {
			if ( parent[i] == -1 && rGraph[v][i] > 0) {
				q.push(i);
				parent[i] = v;
			}
		}
	}

	return parent;
}

int fordFulkerson() {

	rGraph = graph; // residual graph

	int maxFlow = 0;

	vector<int> path = BFS();

	// while there is augmenting path from source to sink
	while(path[sink] != -1) {
		// find the maximum flow through the path found.
		int pathFlow = INT_MAX; 
		int v = sink;
		while(v != source) {
			pathFlow = min(pathFlow, rGraph[path[v]][v]);
			v = path[v];
		}

		// update capacities
		v = sink;
		while(v != source) {
			int u = path[v];
			rGraph[u][v] -= pathFlow;
			rGraph[v][u] += pathFlow;
			v = path[v];
		}

		// Add path flow to overall flow
		maxFlow += pathFlow;

		// find another augmenting path
		path = BFS();
	}

	return maxFlow;
}

int main() {

	cout << "****************************************" << endl;
	cout << "|            Ford-Fulkerson            |" << endl;
	cout << "****************************************" << endl << endl;

	loadInput();

	cout << "\nThe maximum possible flow is " << fordFulkerson() << endl;

	string s;
	cout << "\nDo you want to see the flow ? [y/n] ";
	cin >> s;
	if( s == "y" ) {
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				cout << graph[i][j] - rGraph[i][j];
				(j == n-1) ? cout << "\n" : cout << " ";
			}
		}
	}
	cout << endl;

	return 0;
}