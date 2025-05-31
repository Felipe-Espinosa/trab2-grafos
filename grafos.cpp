// FELIPE HENRIQUE ESPINOSA - 2133357
// RAFAEL BOMBONATTO FERREIRA - 
// MATEUS Z. F. PADILHA -

#include <iostream> 
#include <fstream>  
#include <vector>   
#include <map>      
#include <string>   
#include <sstream>  // Para (para a função separar)
#include <stdexcept> // Para (exceções de stoi)

using namespace std;

// armazena arestas ponderadas para o Grafo G3
struct ArestaPonderada {
    string destino;
    int peso;
};

// Função para separar uma string em partes 
vector<string> separar(const string& linha, char delimitador = ';') {
    vector<string> partes;
    stringstream ss(linha);
    string item;
    while (getline(ss, item, delimitador)) {
        partes.push_back(item);
    }
    return partes;
}

// Função para carregar um grafo não direcionado (G1) de um arquivo
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

        // O formato é "vertice1;vertice2;" que resulta em 2 partes após o split.
        // Se a linha terminar com ';', a última parte pode ser vazia se o split for ingênuo,
        // mas com stringstream e getline, ele lidará bem (não adicionará uma string vazia extra se for "a;b;;").
        // Para "a;b;", resultará em {"a", "b"}.
        if (partes.size() == 2) {
            if (partes[0].empty() || partes[1].empty()) {
                cerr << "Aviso em " << nomeArquivo << " (linha " << numeroLinha << "): Vértice vazio encontrado na linha: '" << linha << "'. Aresta ignorada." << endl;
                continue;
            }
            string u = partes[0];
            string v = partes[1];
            grafo[u].push_back(v);
            grafo[v].push_back(u); // Grafo não direcionado
        } else if (!linha.empty()) { // Informa sobre linhas malformatadas que não estão vazias
            cerr << "Aviso em " << nomeArquivo << " (linha " << numeroLinha << "): Formato inválido da linha: '" << linha << "'. Esperadas 2 partes, encontradas " << partes.size() << ". Linha ignorada." << endl;
        }
    }
    arquivo.close();
    return true;
}

// Função para carregar um grafo direcionado (G2) de um arquivo
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
            if (partes[0].empty() || partes[1].empty()) {
                cerr << "Aviso em " << nomeArquivo << " (linha " << numeroLinha << "): Vértice vazio encontrado na linha: '" << linha << "'. Aresta ignorada." << endl;
                continue;
            }
            string u = partes[0];
            string v = partes[1];
            grafo[u].push_back(v); // Grafo direcionado: u -> v
        } else if (!linha.empty()) {
            cerr << "Aviso em " << nomeArquivo << " (linha " << numeroLinha << "): Formato inválido da linha: '" << linha << "'. Esperadas 2 partes, encontradas " << partes.size() << ". Linha ignorada." << endl;
        }
    }
    arquivo.close();
    return true;
}

// Função para carregar um grafo ponderado e não direcionado (G3) de um arquivo
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
        // O formato é "vertice1;vertice2;peso;" 
        // Ex: "x;z;12;" -> {"x", "z", "12"}
        if (partes.size() == 3) {
            if (partes[0].empty() || partes[1].empty() || partes[2].empty()) {
                 cerr << "Aviso em " << nomeArquivo << " (linha " << numeroLinha << "): Vértice ou peso vazio encontrado na linha: '" << linha << "'. Aresta ignorada." << endl;
                continue;
            }
            string u = partes[0];
            string v = partes[1];
            int peso;

            try {
                peso = stoi(partes[2]); // Converte a string do peso para inteiro
                if (peso < 0 && nomeArquivo == "g3.txt") { // O algoritmo de Dijkstra geralmente assume pesos não-negativos
                    cerr << "Aviso em " << nomeArquivo << " (linha " << numeroLinha << "): Peso negativo (" << peso << ") encontrado na linha: '" << linha << "'. Alguns algoritmos de caminho mínimo podem não funcionar corretamente." << endl;
                }
                grafo[u].push_back({v, peso});
                grafo[v].push_back({u, peso}); // Grafo não direcionado
            } catch (const std::invalid_argument& ia) {
                cerr << "Erro de conversao em " << nomeArquivo << " (linha " << numeroLinha << "): Peso inválido '" << partes[2] << "' na linha: '" << linha << "'. Aresta ignorada. Detalhe: " << ia.what() << endl;
            } catch (const std::out_of_range& oor) {
                cerr << "Erro de conversao em " << nomeArquivo << " (linha " << numeroLinha << "): Peso fora do range permitido '" << partes[2] << "' na linha: '" << linha << "'. Aresta ignorada. Detalhe: " << oor.what() << endl;
            }
        } else if (!linha.empty()) {
             cerr << "Aviso em " << nomeArquivo << " (linha " << numeroLinha << "): Formato inválido da linha: '" << linha << "'. Esperadas 3 partes, encontradas " << partes.size() << ". Linha ignorada." << endl;
        }
    }
    arquivo.close();
    return true;
}

// Função para imprimir a lista de adjacência de um grafo simples (G1, G2)
void imprimirListaAdjacencia(const string& nomeGrafo, const map<string, vector<string>>& grafo) {
    cout << "\n" << nomeGrafo << " - Lista de Adjacência:\n";
    for (const auto& par : grafo) {
        cout << par.first << " -> ";
        for (size_t i = 0; i < par.second.size(); ++i) {
            cout << par.second[i] << (i == par.second.size() - 1 ? "" : ", ");
        }
        cout << endl;
    }
}

// Função para imprimir a lista de adjacência de um grafo ponderado (G3)
void imprimirListaAdjacenciaPonderada(const string& nomeGrafo, const map<string, vector<ArestaPonderada>>& grafo) {
    cout << "\n" << nomeGrafo << " - Lista de Adjacência Ponderada:\n";
    for (const auto& par : grafo) {
        cout << par.first << " -> ";
        for (size_t i = 0; i < par.second.size(); ++i) {
            const auto& aresta = par.second[i];
            cout << "(" << aresta.destino << ", " << aresta.peso << ")" << (i == par.second.size() - 1 ? "" : ", ");
        }
        cout << endl;
    }
}

int main() {
    
    map<string, vector<string>> g1;
    map<string, vector<string>> g2;
    map<string, vector<ArestaPonderada>> g3;

    cout << "Processando arquivos de grafos..." << endl;

    // Carregar e criar listas de adjacência 
    if (!carregarGrafoNaoDirecionado("g1.txt", g1)) {
        return 1; // Termina se houver erro crítico na abertura do arquivo
    }
    if (!carregarGrafoDirecionado("g2.txt", g2)) {
        return 1;
    }
    if (!carregarGrafoPonderado("g3.txt", g3)) {
        return 1;
    }

    cout << "\nListas de adjacência criadas com sucesso." << endl;

    // Exibir as listas de adjacência para conferência 
    imprimirListaAdjacencia("G1 (Não Direcionado)", g1);
    imprimirListaAdjacencia("G2 (Direcionado)", g2);
    imprimirListaAdjacenciaPonderada("G3 (Ponderado e Não Direcionado)", g3);

    return 0;
}