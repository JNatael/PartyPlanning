// problem_c.cpp : Defines the entry point for the console application.
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

/*
This function reads a line of int values into a vector function and returns that
vector.
*/
vector<int> readlineofints(int count) {
    // Input values
    vector<int> linevalues(count);
    for (int j = 0; j < count; j++) {
        int val;
        cin >> val;
        linevalues[j] = val;
    }
    return linevalues; // Return line values as a vector
}

/*
This function reads a line of int values into a vector function and returns that
vector. May be slower than readlineofints function but allows for variable line length.
Also requires a <string> and <sstream> include.
*/
vector<int> readlineofintstoend() {
    // Input values
    vector<int> linevalues;
    int val;
    string line;
    getline(std::cin, line); //Get line from screen
    stringstream stream(line); //Put line into stringstream
    int n; while (stream >> val) { //Get values from stringstream and put them into val
        linevalues.push_back(val); //Put current val into vector
    }
    return linevalues; // Return line values as a vector
}

// Edge struct
struct edge {
    int target;
    long weight;

    //Default constructor
    edge() {}

    //Constructor
    edge(int target, long weight) :target(target), weight(weight) {}

    //Override comparator operator
    bool operator<(const edge& rhs) const {
        return weight < rhs.weight;
    }
};

class Reversed_Edge_Comparator {
public:
    bool operator()(const edge a, const edge b) {
        return (a.weight > b.weight);
    }
};

// Graph code, at least in original part, from
// http://www.geeksforgeeks.org/graph-implementation-using-stl-for-competitive-programming-set-2-weighted-graph/
/*
Adds an undirected edge of set weight to the graph
*/
void addUndirectedEdge(vector<vector<edge>> &adj_list, int u, int v, long weight) {
    edge edge1(v, weight);
    edge edge2(u, weight);
    adj_list[u].push_back(edge1); // Add the v edge and the weight to u's list
    adj_list[v].push_back(edge2); // Add the u edge and the weight to v's list
}

/*
Adds a directed edge of set weight to the graph
*/
void addDirectedEdge(vector<vector<edge>> &adj_list, int u, int v, long weight) {
    edge edge1(v, weight);
    adj_list[u].push_back(edge1); // Add the v edge and the weight to u's list
}

/*
Updates the weight of set edge
*/
void updateEdgeWeight(vector<vector<edge>> &adj_list, int u, int v, long new_weight) {
    for (edge &e : adj_list[u]) {
        if (e.target == v) {
            e.weight = new_weight;
        }
    }
}

/*
Gets the weight of indicated edge and returns it
*/
long const get_edge_weight(const vector<vector<edge>> &adj_list, int u, int v) {
    for (const edge &e : adj_list[u]) {
        if (e.target == v) { return e.weight; }
    }
    return -1;
}

int dijkstra_dist(const vector<vector<edge>> &graph, int origin, int destination) {
    vector<long> distance(graph.size(), 2147483647); //Initialize vertices list with lengths set to max
    vector<int> prev_node(graph.size(), -1); //Previous node initialized to -1

    priority_queue<edge, vector<edge>, Reversed_Edge_Comparator> to_visit;
    to_visit.push(edge(origin, 0)); //Push the origin with a 0 weight as the first item in the queue
    distance[origin] = 0;

    while (to_visit.size() > 0) { //While there are unvisited nodes left
        edge this_node = to_visit.top(); //Get next node
        to_visit.pop(); //Pop the node we just got

        //Iterate through edges of next node
        for (const edge &next_node : graph[this_node.target]) {
            if (distance[next_node.target] > (distance[this_node.target] + next_node.weight)) { //If the path to this edge's target is longer than the path to this node plus this edge's weight
                distance[next_node.target] = distance[this_node.target] + next_node.weight; //update the distance to the next node
                to_visit.push(edge(next_node.target, next_node.weight)); //Insert this edge into to_visit list with negative weight to reverse sort order
                prev_node[next_node.target] = this_node.target; //Update that node with the current one as the previous node
            }
        }
    }
    return distance[destination]; //Return the list
}

int main() {
    std::ios_base::sync_with_stdio(false);

    // get test case count
    int t;
    std::cin >> t;

    // loop over all the test cases
    for (int i = 1; i <= t; i++) {
        // Read in params
        vector<int> params = readlineofints(1);

        // Initialize adjacency list graph
        vector<vector<edge>> graph(params[0] + 1);

        // Iterate over nodes
        for (int j = 0; j < params[0]; j++) {
            // Input edge
            int time, tasks;
            std::cin >> time;
            std::cin >> tasks;
            if (tasks == 0) { //If tasks is 0 add edge to dummy extra node to add weight on
                addDirectedEdge(graph, j, params[0], -time); //Add the edge, with weight, to graph
                break;
            }
            vector<int> this_edge = readlineofints(tasks);
            for (int &edge : this_edge) { //Iterate over successors
                addDirectedEdge(graph, j, edge - 1, -time); //Add the edge, with weight, to graph
            }
        }

        int x = -dijkstra_dist(graph, 0, params[0]);

        //Print result, incrementing to account for indexing offset at input
        std::cout << "Case #" << i << ": " << x << std::endl;
    }
    return 0;
}