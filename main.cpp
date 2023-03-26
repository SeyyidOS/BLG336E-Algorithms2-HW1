#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

// Kid struct
struct Kid {
    int x;
    int y;
    int st;
};

// Squared distance calculator
double squaredDistance(const Kid& k1, const Kid& k2){
    int dx = k1.x - k2.x;
    int dy = k1.y - k2.y;
    return dx*dx + dy*dy;
}


// Graph class
class Graph{
private:
    int num_vertices; // Number of kids for this case
    vector< vector<int> > adj_m; // Adjacency matrix
    int dfs_src; // Source node for the DFS case
    bool dfs_has_cycle; // DFS cycle flag
    queue<int> dfs_cycle_path; // DFS cycle path
public:
    Graph(int num_vertices); // Constructor
    void addVertices(int src, int dst); // Add function for graph
    void writeGraph(); // Print function
    void bfs(int src, int tg); // BFS
    void dfsUtil(int w, int current_depth, vector<bool>& in_progress, vector<bool>& all_done); // DFS Util
    void dfs(int src); // DFS
};

Graph::Graph(int num_vertices){
    this->num_vertices = num_vertices;
    this->adj_m.resize(num_vertices, vector<int>(num_vertices));
}

void Graph::addVertices(int src, int dst){
    this->adj_m[src][dst] = 1;
    this->adj_m[dst][src] = 1;
}

void Graph::writeGraph(){
    ofstream file("graph.txt");
    file << "Graph:" << endl; 
    for (int i = 0; i < this->num_vertices; i++){
        for (int j = 0; j < this->num_vertices; j++){
            file<<this->adj_m[i][j];
            if (j != this->num_vertices-1){
                file<<" ";
            }
        }
        file<<endl;
    }
}

void Graph::bfs(int src, int tg){
    queue<int> wait_list; // Wait list 
    vector<int> visited(num_vertices, 0); // Visited flag array
    vector<int> parent(num_vertices, -1); // Parent vector to store the path

    wait_list.push(src); // Push source node to wait list firstly
    visited[src] = 1; // Set visited flag

    // While there are elements in the queue
    while (!wait_list.empty()) {
        int cur = wait_list.front();
        wait_list.pop();

        // Stop BFS when reached target
        if (cur == tg) {
            break;
        }

        // For every child
        for (int i = 0; i < num_vertices; i++) {
            // Check connection and visitied
            if (adj_m[cur][i] == 1 && !visited[i]) {
                visited[i] = 1; // Set visited
                parent[i] = cur; // Set parent for path
                wait_list.push(i); // Push every child to wait list for bfs
            }
        }
    }

    // Write results to the txt file
    ofstream file("bfs.txt"); 
    file << "BFS:" << endl;

    // If there is not valid path write -1
    if (!visited[tg]) {
        file << "-1";
        return;
    }

    // Construct path
    vector<int> path;
    for (int v = tg; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    // Write the path to file
    for (int i = 0; i < path.size(); i++) {
        file << path[i];
        if (i != path.size() - 1) {
            file << "->";
        }
    }
    file << endl;
}

void Graph::dfsUtil(int w, int current_depth, vector<bool>& in_progress, vector<bool>& all_done) {
    in_progress[w] = true; // In progress flag
    current_depth += 1; // Depth
    this->dfs_cycle_path.push(w); // Push path to que
    // If reached the src after at least 2 depth end recursion
    if (this->adj_m[w][this->dfs_src] && current_depth>2){
        this->dfs_cycle_path.push(this->dfs_src); // Push path
        this->dfs_has_cycle = true; // End the recursion
        return;
    }

    // Go deeper with recursion
    for (int i = 0; i < this->num_vertices; i++){
      if (this->adj_m[w][i] && !in_progress[i] && !all_done[i] && !this->dfs_has_cycle){
        dfsUtil(i, current_depth, in_progress, all_done);
      }
    }

    // If there is not cycle pop the queue elements
    if (!this->dfs_has_cycle)
    {
      this->dfs_cycle_path.pop();
    }

    current_depth -= 1; // Decrement after every recursion step
    all_done[w] = true;  // Set all done 
    in_progress[w] = false; // Set in progress
}

void Graph::dfs(int src) {
    vector<bool> in_progress(this->num_vertices, false); // In progress vector
    vector<bool> all_done(this->num_vertices, false); // All done checker
    
    this->dfs_src = src; 
    this->dfs_has_cycle = false;
    int current_depth = 0;

    dfsUtil(src, current_depth, in_progress, all_done); // Call DFS util

    // Write results to the dfs.txt
    ofstream file("dfs.txt"); 
    file << "DFS:" << endl;
    if (!this->dfs_has_cycle) {
        file << "-1" << endl;
    }
    else{
        int len = this->dfs_cycle_path.size();
        for (int i = 0; i < len; i++){
            file<< this->dfs_cycle_path.front();
            this->dfs_cycle_path.pop();

            if (i == len-1)
                file << endl;  
            else
                file << "->";
        }
    }
}

int main(int argc, char* argv[]){
    /*
        INSERT ALL YOUR CODE IN HERE!
        Program Compile Command: g++ -std=c++11 -Wall -Werror main.cpp -o main
        Program Run Command: ./main <input.txt>
        Expected input: /graphs/case{$n}/input_{$n}.txt
        Expected output: graph.txt bfs.txt dfs.txt
        Please, try to write clean and readable code. Remember to comment!!
    */

    // Read the input file
    string fn = argv[1];
    ifstream file(fn);

    // Get kid number, source node and destination node
    int num_kids, source_node, targ_node;
    file >> num_kids >> source_node >> targ_node;

    // Create kids vector
    vector<Kid> kids(num_kids);

    // Fill the kids vector
    for (int i = 0; i<num_kids; i++){
        file >> kids[i].x >> kids[i].y >> kids[i].st;
    }

    // Create graph
    Graph graph(num_kids);

    // Fill the adjaceny matrix with respect to given logic in hw desc file
    double dist;
    for (int i = 0; i < num_kids; i++){
        for (int j = i+1; j < num_kids; j++){
            dist = squaredDistance(kids[i], kids[j]); 
            if (dist <= kids[i].st && dist <= kids[j].st){
                graph.addVertices(i, j);
            }
        }
    }
    
    // Write created graph to txt
    graph.writeGraph();

    // BFS solution
    graph.bfs(source_node, targ_node);

    // DFS solution
    graph.dfs(source_node);

    // Close the file
    file.close(); 

    exit(0);
}
