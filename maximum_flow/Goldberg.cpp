#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Vertex {
	int height;
	int excess_flow;
};

struct Edge {
	int u, v; // edge from u -> v
	int flow;
	int capacity;
};

vector<Vertex> V;
vector<Edge> E;

int n; // number of vertices
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
		
		Edge e;
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				f >> e.capacity;
				if(e.capacity == 0) continue;
				e.flow = 0;
				e.u = i;
				e.v = j;
				E.push_back(e);
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


void preflow() {

	// initializing all vertices
	V.resize(n);
	for(int i = 0; i < n; i++) {
		V[i].height = 0;
		V[i].excess_flow = 0;
	}
	V[source].height = n;

	// edges from source
	for(int i = 0; i < E.size(); i++) {
		if(E[i].u == source) {
			// maximize flow
			E[i].flow = E[i].capacity;

			// initialize excess flow for adjacent vertices
			V[E[i].v].excess_flow += E[i].flow;

			// add an backward edge (in residual graph) with capacity 0
			Edge e;
			e.u = E[i].v;
			e.v = source;
			e.capacity = 0;
			e.flow = -E[i].flow;
			E.push_back(e);
		}
	}
}

// return index of overflowing Vertex different from source and sink
int overFlowVertex() {
	for (int i = 0; i < n; i++)
		if ( i != source && i != sink && V[i].excess_flow > 0)
			return i;
  
	// -1 if no overflowing Vertex
	return -1;
}

// Update reverse flow for flow added on i-th Edge
void updateReverseEdgeFlow(int i, int flow) {
	int u = E[i].v;
	int v = E[i].u;

	for(int j = 0; j < E.size(); j++) {
		if (E[j].v == v && E[j].u == u) {
			E[j].flow -= flow;
			return;
		}
	}
	
	// adding reverse Edge in residual graph
	Edge e;
	e.flow = 0;
	e.capacity = flow;
	e.u = u;
	e.v = v;
	E.push_back(e);
}


bool push(int u) {
	// find adjacent of u to which flow can be pushed
	for(int i = 0; i < E.size(); i++) {
		if(E[i].u == u) {

			if(E[i].flow == E[i].capacity) continue;

			if(V[u].height > V[E[i].v].height) {
				// Flow to be pushed is equal to minimum of
				// remaining flow on edge and excess flow
				int flow = min(E[i].capacity - E[i].flow, V[u].excess_flow);

				// reduce excess flow
				V[u].excess_flow -= flow;

				// increase excess flow for adjacent
				V[E[i].v].excess_flow += flow;

				// add residual flow with 0 capacity and negative flow
				E[i].flow += flow;
				updateReverseEdgeFlow(i, flow);

				return true;
			}
		}
	}
	return false;
}

// change height of vertex u
void relabel(int u) {
	int new_height = INT_MAX;

	// find adjacent with minimum height
	for(int i = 0; i < E.size(); i++) {
		if(E[i].u == u) {

			if (E[i].flow == E[i].capacity) continue;

			if(V[E[i].v].height < new_height) {
				new_height = V[E[i].v].height;

				// updating height of u
				V[u].height = new_height + 1;
			}
		}
	}
}

int goldberg() {
	// initialize heights and flows of all vertices
	preflow();

	// while there is a vertex that has excess flow
	int v = overFlowVertex();
	while (v != -1) {
		if (!push(v)) {
			relabel(v);
		}
		v = overFlowVertex();
	}
  
	// return sink excess flow, which is now equal to maximum possible flow
    return V[sink].excess_flow;
}


int main() {

	cout << "****************************************" << endl;
	cout << "|               Goldberg               |" << endl;
	cout << "****************************************" << endl << endl;

	loadInput();

	cout << "\nThe maximum possible flow is " << goldberg() << endl << endl;

	return 0;
}