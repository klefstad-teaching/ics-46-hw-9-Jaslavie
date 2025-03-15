#include "dijkstras.h"
#include <vector>

const int UNDEFINED = -1;

// return a vector of distances to all vertices with predecessor mapping for each node to the source
vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous){
    int numVertices = G.size();
    vector<int> distances(numVertices, INF);

    // init source node
    distances[source] = 0; // set distance of source to 0 (source->source is 0)
    previous[source] = UNDEFINED; // set previous node of source to undefined

    // create min heap priority queue of vertices based on ascending weights
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap; // pair<vertex, weight>
    minHeap.push({0, source});

    // loop through all vertices and update place in queue
    while(!minHeap.empty()) {
        int u = minHeap.top().second; // get vertex with minimum and first element from the pair (vertex)
        int dist = minHeap.top().first;
        minHeap.pop();

        // Skip if we've already found a better path
        if(dist > distances[u]) continue;
        
        // Process all edges from u
        for(const Edge& edge : G[u]){
            int v = edge.dst;
            int weight = edge.weight; 
            // distances[u] is the current shortest path known to src
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight; // update  new shortest path to v
                previous[v] = u;
                minHeap.push({distances[v], v});
            }
        }
    }
    // return distances to all vertices
    return distances;
}

// outputs sequence of vertices traverses from source to all other nodes
vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination){
    vector<int> path; // vertices to traverse in order for each vertex from src node
    // if dest not reachable
    if (distances[destination] == INF) return path;
    // trace back path from dest -> src (since we are given dest not src)
    for (int vertex = destination; vertex != UNDEFINED; vertex = previous[vertex])
        path.push_back(vertex);
    // reverse path to get correct order
    reverse(path.begin(), path.end());

    return path;
}

// prints path for each vertex
void print_path(const vector<int>& v, int total){
    // print all vertices in path
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i < v.size() - 1) cout << " "; // space between each vertex
    }
    cout << endl;
    cout << "Total cost is " << total << endl;
}