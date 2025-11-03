#include "graph.hpp"
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

vector<int> bfsShortestPathWithPrev(Graph& graph, int source, vector<int>& prev) {
    int V = graph.getVertices();
    vector<int> dist(V + 1, INF);
    vector<bool> visited(V + 1, false);
    prev.assign(V + 1, -1);
    
    queue<int> q;
    dist[source] = 0;
    visited[source] = true;
    q.push(source);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (const auto& neighbor : graph.getAdjList()[u]) {
            int v = neighbor.first;
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                prev[v] = u;
                q.push(v);
            }
        }
    }
    
    return dist;
}