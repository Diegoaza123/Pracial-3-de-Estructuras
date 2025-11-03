#include "graph.hpp"
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

vector<int> dijkstraWithPrev(Graph& graph, int source, vector<int>& prev) {
    int V = graph.getVertices();
    vector<int> dist(V + 1, INF);
    vector<bool> visited(V + 1, false);
    prev.assign(V + 1, -1);
    
    // Min-heap: (distancia, vértice)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[source] = 0;
    pq.push({0, source});
    prev[source] = -1;
    
    while (!pq.empty()) {
        int u = pq.top().second;
        int current_dist = pq.top().first;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        for (const auto& neighbor : graph.getAdjList()[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            
            if (!visited[v] && current_dist != INF) {
                int new_dist = current_dist + weight;
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    prev[v] = u;
                    pq.push({new_dist, v});
                }
            }
        }
    }
    
    return dist;
}