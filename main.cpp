#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include "graph.hpp"

using namespace std;
using namespace std::chrono;

// Declaraciones de funciones en otros archivos
vector<int> bfsShortestPathWithPrev(Graph& graph, int source, vector<int>& prev);
vector<int> dijkstraWithPrev(Graph& graph, int source, vector<int>& prev);

vector<int> reconstructPath(int source, int target, const vector<int>& prev) {
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

void writeResults(const string& filename, const vector<int>& dist, 
                  const vector<int>& prev, int source, int tipo) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir " << filename << " para escritura" << endl;
        return;
    }
    
    int V = dist.size() - 1;
    
    for (int v = 1; v <= V; v++) {
        file << "Distancia de " << source << " a " << v << ": ";
        if (dist[v] == INF) {
            file << "INF" << endl;
        } else {
            file << dist[v] << endl;
        }
        
        // Reconstruir camino solo si es alcanzable
        if (dist[v] != INF) {
            vector<int> path = reconstructPath(source, v, prev);
            file << "Camino: ";
            if (!path.empty()) {
                for (size_t i = 0; i < path.size(); i++) {
                    file << path[i];
                    if (i < path.size() - 1) file << " -> ";
                }
            } else if (v == source) {
                file << source;
            } else {
                file << "No existe";
            }
            file << endl;
        } else {
            file << "Camino: No existe" << endl;
        }
        file << "---" << endl;
    }
    
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo_entrada>" << endl;
        cerr << "Ejemplo: " << argv[0] << " data/case_no_ponderado.txt" << endl;
        return 1;
    }
    
    string inputFile = argv[1];
    ifstream file(inputFile);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir " << inputFile << endl;
        return 1;
    }
    
    int n, m, s, tipo;
    file >> n >> m >> s >> tipo;
    
    // Crear directorio results si no existe
    system("mkdir -p results");
    
    Graph graph(n, true); // grafo dirigido
    
    for (int i = 0; i < m; i++) {
        int u, v, w;
        file >> u >> v >> w;
        graph.addEdge(u, v, w);
    }
    file.close();
    
    // Verificar pesos negativos en modo ponderado
    if (tipo == 1 && graph.hasNegativeWeights()) {
        string outputFile = "results/dist_" + inputFile.substr(inputFile.find_last_of("/\\") + 1);
        ofstream out(outputFile);
        out << "no soportado (pesos negativos)" << endl;
        out << "Explicación: Dijkstra no funciona con pesos negativos porque ";
        out << "asume que una vez que un nodo es marcado como visitado, ";
        out << "su distancia mínima ha sido encontrada. Con pesos negativos, ";
        out << "esta suposición no se cumple y puede llevar a resultados incorrectos." << endl;
        out.close();
        cout << "Pesos negativos detectados. Dijkstra no aplica." << endl;
        return 0;
    }
    
    vector<int> dist, prev;
    auto start = high_resolution_clock::now();
    
    if (tipo == 0) {
        // BFS para grafo no ponderado
        cout << "Ejecutando BFS..." << endl;
        dist = bfsShortestPathWithPrev(graph, s, prev);
    } else {
        // Dijkstra para grafo ponderado
        cout << "Ejecutando Dijkstra..." << endl;
        dist = dijkstraWithPrev(graph, s, prev);
    }
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    // Escribir resultados
    string outputFile = "results/dist_" + inputFile.substr(inputFile.find_last_of("/\\") + 1);
    writeResults(outputFile, dist, prev, s, tipo);
    
    // Guardar tiempo en CSV
    ofstream timeFile("results/tiempos.csv", ios::app);
    if (!timeFile) {
        timeFile.open("results/tiempos.csv");
        timeFile << "n,m,tipo,t_ms" << endl;
    }
    timeFile << n << "," << m << "," << tipo << "," << duration.count() / 1000.0 << endl;
    timeFile.close();
    
    cout << "Procesamiento completado. Resultados en: " << outputFile << endl;
    cout << "Tiempo de ejecución: " << duration.count() / 1000.0 << " ms" << endl;
    
    return 0;
}