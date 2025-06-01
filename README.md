**Trabalho 2 - Análise de Algoritmos**

Autores:
- FELIPE HENRIQUE ESPINOSA - 2133357
- RAFAEL BOMBONATTO FERREIRA - 2153287
- MATEUS Z.F PADILHA - 1984044

**Descrição:**
Este trabalho resolve as operacoes em três grafos diferentes (G1, G2 e G3), utilizando a linguagem C++ e apenas bibliotecas padrão, foram implementadas as funcionalidades solicitadas conforme o enunciado:

1. Leitura dos arquivos g1.txt, g2.txt e g3.txt, com construção das listas de adjacência.
2. Busca em largura (BFS) no grafo G1 a partir do vértice 'b', exibindo a ordem de visitação.
3. Busca em profundidade (DFS) no grafo G2 a partir do vértice 'a', exibindo a ordem de visitação.
4. Cálculo do caminho mínimo no grafo G3 entre os vértices 'x' e 't' utilizando o algoritmo de dijkstra mostrando o custo total e o caminho percorrido.

**Organização do codigo:**
- Funções separadas para cada tipo de grafo.
- Cada funcionalidade foi dividida de forma clara para facilitar a leitura.
- A ordenação lexicográfica foi aplicada conforme solicitado.
- Erros de leitura e entradas inválidas são tratados no código.

**Compilação:**
Use o seguinte comando:
g++ -std=c++11 -o grafos grafos.cpp

**Execução:**
./grafos
