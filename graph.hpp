#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <utility>
#include <limits>

#define INF std::numeric_limits<int>::max()

class Graph {
private:
    int V; // número de vértices
    std::vector<std::vector<std::pair<int, int>>> adj; // lista de adyacencia: (vecino, peso)
    bool directed;

public:
    Graph(int vertices, bool isDirected = false);
    void addEdge(int u, int v, int w = 1);
    std::vector<int> bfsShortestPath(int source);
    std::vector<int> dijkstra(int source);
    std::vector<int> reconstructPath(int source, int target, const std::vector<int>& prev);
    bool hasNegativeWeights();
    int getVertices() const { return V; }
    const std::vector<std::vector<std::pair<int, int>>>& getAdjList() const { return adj; }
};

#endif
