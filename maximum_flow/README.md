Algorithm for finding maximum flow in a graph.

# Ford-Fulkerson

### fordFulkerson()
* Makes an residual graph, which at the beginning is equal to the original graph. (residual graph of a flow network is a graph which indicates additional possible flow).
* Initialize maxFlow to zero. During computation it will be increased and at the end it is returned.
* Main Loop - while there is an augmenting path from source to sink, it finds the maximum possible flow on that path, updates the capacities in residual graph and adds founded flow to the maxFlow variable.

### BFS()
* It is searching for an augmenting path from source to sink (performing standard BFS).
* It saves the path to a vector, which is returned at the end.


# Input Format
Input is loaded from file.
At the beginning, program ask you to type file name. It has to be located in the same directory as the program.
When file is loaded, user is asked to enter verteces represented source and sink. **verteces are counted from zero**

### Input File
On the first line there has to be one number **n** - number of verteces in graph.
Then there is n lines. On each line there is n numbers separated by space which represents the graph as adjacency matrix.

input.txt is an example of correct input file
