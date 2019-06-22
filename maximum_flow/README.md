Algorithms for finding maximum flow in a graph.

# Ford-Fulkerson

### fordFulkerson()
* Makes an residual graph, which at the beginning is equal to the original graph. (residual graph of a flow network is a graph which indicates additional possible flow).
* Initialize maxFlow to zero. During computation it will be increased and at the end it is returned.
* Main Loop - while there is an augmenting path from source to sink, it finds the maximum possible flow on that path, updates the capacities in residual graph and adds founded flow to the maxFlow variable.

### BFS()
* It is searching for an augmenting path from source to sink (performing standard BFS).
* It saves the path to a vector, which is returned at the end.

# Goldberg

Every vertex in graph has height and excess flow, which is equal to its inflow minus its outflow.
Every edge is represented by 2 vertices, has its capacity and flow which flows through that edge.

### goldberg()
* initialize heights and flow for vertices and edges
* Main Loop - while there is a vertex that has excess flow, "water" is pushed. If it cannot be pushed, vertex height is increased (vertex is relabeled)
* At the end it returns excess flow of the sink, which is the maximum possible flow.

### preflow()
* set height and excess flow of all vertices to 0. Height of source vertex is set to the number of vertices in graph
* maximize the flow of every edge from source (set its flow to its capacity), update excess flow of adjacent vertices and add backward edge

### overFlowVertex()
* find index of overflowing Vertex different from source and sink (vertex which has positive excess flow)
* return its index or -1 if there is no such vertex

### push()


# Input Format
Input is loaded from file.
At the beginning, program ask you to type file name. It has to be located in the same directory as the program.
When file is loaded, user is asked to enter verteces represented source and sink. **Verteces are counted from zero**

### Input File
On the first line there has to be one number **n** - number of verteces in graph.
Then there is n lines. On each line there is n numbers separated by space which represents the graph as adjacency matrix.

*input.txt* is an example of correct input file
