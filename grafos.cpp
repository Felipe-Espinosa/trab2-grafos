//README com as informações do trabalho
// FELIPE HENRIQUE ESPINOSA - 2133357
// RAFAEL BOMBONATTO FERREIRA - 2153287
// MATEUS Z. F. PADILHA - 1984044 

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>  
#include <stdexcept> 
#include <set>        
#include <queue>      
#include <algorithm>  
#include <climits>    

using namespace std;

// Estrutura para armazenar arestas ponderadas no Grafo G3
struct ArestaPonderada {
    string destino;
    int peso;
};

// Função auxiliar para separar uma string com delimitador (padrão ';')
vector<string> separar(const string& linha, char delimitador = ';') {
    vector<string> partes;
    stringstream ss(linha);
    string item;
    while (getline(ss, item, delimitador)) {
        partes.push_back(item);
    }
    return partes;
}

// Leitura do grafo G1 não direcionado
bool carregarGrafoNaoDirecionado(const string& nomeArquivo, map<string, vector<string>>& grafo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return false;
    }
    string linha;
    int numeroLinha = 0;
    while (getline(arquivo, linha)) {
        numeroLinha++;
        vector<string> partes = separar(linha);
        if (partes.size() == 2) {
            if (partes[0].empty() || partes[1].empty()) continue;
            string u = partes[0];
            string v = partes[1];
            grafo[u].push_back(v);
            grafo[v].push_back(u);
        }
    }
    return true;
}

// Leitura do grafo G2 direcionado
bool carregarGrafoDirecionado(const string& nomeArquivo, map<string, vector<string>>& grafo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return false;
    }
    string linha;
    int numeroLinha = 0;
    while (getline(arquivo, linha)) {
        numeroLinha++;
        vector<string> partes = separar(linha);
        if (partes.size() == 2) {
            if (partes[0].empty() || partes[1].empty()) continue;
            string u = partes[0];
            string v = partes[1];
            grafo[u].push_back(v);
        }
    }
    return true;
}

// Leitura do grafo G3 não direcionado e ponderado
bool carregarGrafoPonderado(const string& nomeArquivo, map<string, vector<ArestaPonderada>>& grafo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return false;
    }
    string linha;
    int numeroLinha = 0;
    while (getline(arquivo, linha)) {
        numeroLinha++;
        vector<string> partes = separar(linha);
        if (partes.size() == 3) {
            string u = partes[0], v = partes[1];
            int peso = stoi(partes[2]);
            grafo[u].push_back({v, peso});
            grafo[v].push_back({u, peso});
        }
    }
    return true;
}

// Impressão de grafo não ponderado
void imprimirListaAdjacencia(const string& nomeGrafo, const map<string, vector<string>>& grafo) {
    cout << "\n" << nomeGrafo << " - Lista de Adjacencia:\n";
    for (const auto& par : grafo) {
        cout << par.first << " -> ";
        for (size_t i = 0; i < par.second.size(); ++i) {
            cout << par.second[i] << (i == par.second.size() - 1 ? "" : ", ");
        }
        cout << endl;
    }
}

// Impressão de grafo ponderado
void imprimirListaAdjacenciaPonderada(const string& nomeGrafo, const map<string, vector<ArestaPonderada>>& grafo) {
    cout << "\n" << nomeGrafo << " - Lista de Adjacencia Ponderada:\n";
    for (const auto& par : grafo) {
        cout << par.first << " -> ";
        for (size_t i = 0; i < par.second.size(); ++i) {
            cout << "(" << par.second[i].destino << ", " << par.second[i].peso << ")" << (i == par.second.size() - 1 ? "" : ", ");
        }
        cout << endl;
    }
}

// BFS em G1 a partir de "b", ordem lexicografica
void bfsLex(const map<string, vector<string>>& grafo, const string& inicio) {
    set<string> visitado;
    queue<string> fila;
    fila.push(inicio);
    visitado.insert(inicio);

    cout << "\nResultado da busca em largura no grafo G1 iniciando pelo vertice '" << inicio << "':\n";
    cout << "Ordem de visitacao: ";
    while (!fila.empty()) {
        string atual = fila.front(); fila.pop();
        cout << atual << " ";
        vector<string> vizinhos = grafo.at(atual);
        sort(vizinhos.begin(), vizinhos.end());
        for (const auto& viz : vizinhos) {
            if (!visitado.count(viz)) {
                fila.push(viz);
                visitado.insert(viz);
            }
        }
    }
    cout << endl;
}

// DFS em G2 a partir de "a", ordem lexicográfica
void dfsLex(const map<string, vector<string>>& grafo, const string& atual, set<string>& visitado) {
    cout << atual << " ";
    visitado.insert(atual);

    if (grafo.find(atual) == grafo.end()) return;

    vector<string> vizinhos = grafo.at(atual);
    sort(vizinhos.begin(), vizinhos.end());
    for (const auto& viz : vizinhos) {
        if (!visitado.count(viz)) {
            dfsLex(grafo, viz, visitado);
        }
    }
}

// dijkstra em G3 de x ate t
void dijkstra(const map<string, vector<ArestaPonderada>>& grafo, const string& origem, const string& destino) {
    map<string, int> dist;
    map<string, string> prev;
    for (const auto& par : grafo) dist[par.first] = INT_MAX;
    dist[origem] = 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> fila;
    fila.push({0, origem});

    while (!fila.empty()) {
        pair<int, string> atual = fila.top(); fila.pop();
        int d = atual.first;
        string u = atual.second;

        for (const auto& aresta : grafo.at(u)) {
            int novaDist = d + aresta.peso;
            if (novaDist < dist[aresta.destino]) {
                dist[aresta.destino] = novaDist;
                prev[aresta.destino] = u;
                fila.push({novaDist, aresta.destino});
            }
        }
    }

    cout << "\nCaminho de menor custo no grafo G3 entre os vertices '" << origem << "' e '" << destino << "':\n";
    cout << "Custo minimo: " << dist[destino] << endl;
    cout << "Caminho:\n";

    vector<string> caminho;
    for (string at = destino; !at.empty(); at = prev[at]) {
        caminho.push_back(at);
        if (at == origem) break;
    }
    reverse(caminho.begin(), caminho.end());
    for (size_t i = 0; i < caminho.size() - 1; ++i) {
        const string& u = caminho[i];
        const string& v = caminho[i + 1];
        for (const auto& aresta : grafo.at(u)) {
            if (aresta.destino == v) {
                cout << u << " -> " << v << " (" << aresta.peso << ")\n";
                break;
            }
        }
    }
}

int main() {
    map<string, vector<string>> g1;
    map<string, vector<string>> g2;
    map<string, vector<ArestaPonderada>> g3;

    cout << "Processando arquivos de grafos" << endl;

    if (!carregarGrafoNaoDirecionado("g1.txt", g1)) return 1;
    if (!carregarGrafoDirecionado("g2.txt", g2)) return 1;
    if (!carregarGrafoPonderado("g3.txt", g3)) return 1;

    cout << "\nListas de adjacencia criadas com sucesso" << endl;
    imprimirListaAdjacencia("G1 (Nao Direcionado)", g1);
    imprimirListaAdjacencia("G2 (Direcionado)", g2);
    imprimirListaAdjacenciaPonderada("G3 (Ponderado e Nao Direcionado)", g3);

    bfsLex(g1, "b");

    cout << "\nResultado da busca em profundidade no grafo G2 iniciando pelo vertice 'a':\n";
    cout << "Ordem de visitacao: ";
    set<string> visitadoDFS;
    dfsLex(g2, "a", visitadoDFS);
    cout << endl;

    dijkstra(g3, "x", "t");

    return 0;
}
