#include "graph.hpp"
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

Graph::Graph(int vertices, bool isDirected) {
    V = vertices;
    directed = isDirected;
    adj.resize(V + 1); // vértices etiquetados 1..n
}

void Graph::addEdge(int u, int v, int w) {
    if (u < 1 || u > V || v < 1 || v > V) return;
    adj[u].push_back({v, w});
    if (!directed) {
        adj[v].push_back({u, w});
    }
}

vector<int> Graph::bfsShortestPath(int source) {
    vector<int> dist(V + 1, INF);
    vector<bool> visited(V + 1, false);
    
    queue<int> q;
    dist[source] = 0;
    visited[source] = true;
    q.push(source);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (const auto& neighbor : adj[u]) {
            int v = neighbor.first;
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1; // BFS: cada arista cuenta como 1
                q.push(v);
            }
        }
    }
    
    return dist;
}

vector<int> Graph::dijkstra(int source) {
    vector<int> dist(V + 1, INF);
    vector<bool> visited(V + 1, false);
    
    // Min-heap: (distancia, vértice)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[source] = 0;
    pq.push({0, source});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        int current_dist = pq.top().first;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        for (const auto& neighbor : adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            
            if (!visited[v] && current_dist != INF) {
                int new_dist = current_dist + weight;
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    pq.push({new_dist, v});
                }
            }
        }
    }
    
    return dist;
}

vector<int> Graph::reconstructPath(int source, int target, const vector<int>& prev) {
    vector<int> path;
    if (prev[target] == -1 && target != source) {
        return path; // no hay camino
    }
    
    for (int at = target; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    
    if (!path.empty() && path[0] == source) {
        return path;
    }
    return vector<int>(); // camino no válido
}

bool Graph::hasNegativeWeights() {
    for (int u = 1; u <= V; u++) {
        for (const auto& neighbor : adj[u]) {
            if (neighbor.second < 0) {
                return true;
            }
        }
    }
    return false;
}