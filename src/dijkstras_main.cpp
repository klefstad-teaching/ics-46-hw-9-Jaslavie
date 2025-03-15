// run dijkstras on a file
#include <iostream>
#include "dijkstras.h"

int main(){
    Graph G;
    try {
        file_to_graph("../src/small.txt", G);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // init vector to store previous vertices
    vector<int> previous(G.size(), -1);
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    // extract and print shortest path for each vertex
    for (int dest = 0; dest < G.size(); ++dest) {
        vector<int> path = extract_shortest_path(distances, previous, dest);
        print_path(path, distances[dest]); // total cost based on final distance
    }

    return 0;
}

