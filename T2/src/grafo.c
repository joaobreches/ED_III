#include "grafo.h"


// Função para liberar memória alocada para os vértices e arestas
void liberarMemoria(Vertice *grafo, int numRegistros) {
    for (int i = 0; i < numRegistros; i++) {
        free(grafo[i].arestas);
    }
    free(grafo);
}

// Função para comparar arestas (usada para ordenação)
int compararArestas(const void *a, const void *b) {
    return strcmp(((Aresta*)a)->nomeTecDestino, ((Aresta*)b)->nomeTecDestino);
}

// Função para comparar nomes de tecnologias para qsort
int compararNomes(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Função para inicializar um grafo
Vertice* inicializarGrafo(int numVertices) {
    Vertice* grafo = (Vertice*)malloc(numVertices * sizeof(Vertice));

    if (grafo == NULL) {
        perror("Falha na execução da funcionalidade");
        fclose(arquivo);
        exit(1);
    }

    for (int i = 0; i < numVertices; i++) {
        grafo[i].nomeTecnologia = NULL;
        grafo[i].grupo = 0;
        grafo[i].grauEntrada = 0;
        grafo[i].grauSaida = 0;
        grafo[i].grau = 0;
        grafo[i].arestas = NULL;
        grafo[i].numArestas = 0;
        grafo[i].visitado = 0;
    }
    return grafo;
}

void liberaGrafo(Vertice *grafo, int numRegistros) {
    for (int i = 0; i < numRegistros; i++) {
        free(grafo[i].arestas);
    }
    free(grafo);
}

void adicionaVertice(Grafo *grafo, char *nomeTecnologia, int grupo){
    grafo.vertice = (Vertice) realloc (grafo.vertice, numVertices+1);

    grafo.numVertices++;
    grafo.vertice[numVertices].nomeTecnologia = nomeTecnologia;
    grafo.vertice[numVertices].grupo = grupo;
    grafo[i].grauEntrada = 0;
    grafo[i].grauSaida = 0;
    grafo[i].grau = 0;
    grafo[i].numArestas = 0;
    grafo[i].visitado = 0;
}

// Função para adicionar uma aresta ao grafo
void adicionarAresta(Vertice* grafo, int origem, int destino) {
    grafo[origem].arestas = (Aresta*)realloc(grafo[origem].arestas, (grafo[origem].numArestas + 1) * sizeof(Aresta));
    grafo[origem].arestas[grafo[origem].numArestas].destino = destino;
    grafo[origem].numArestas++;

}

// Le os componentes da aresta
void leAresta(Aresta *aresta, FILE *arquivo){
    fread(aresta.nomeTecDestino, sizeof(char), 50, arquivo);
    fread(aresta.nomeTecOrigem, sizeof(char), 50, arquivo);
    fread(aresta.peso, sizeof(int), 1, arquivo);
    fread(aresta.destino, sizeof(int), 1, arquivo);
}

// Função para realizar uma DFS no grafo
void dfs(Vertice* grafo, int vertice, Pilha* pilha) {
    grafo[vertice].visitado = 1;

    for (int i = 0; i < grafo[vertice].numArestas; i++) {
        int destino = grafo[vertice].arestas[i].destino;
        if (!grafo[destino].visitado) {
            dfs(grafo, destino, pilha);
        }
    }

    empilhar(pilha, vertice);
}

// Função para realizar uma DFS no grafo reverso (transposto)
void dfsTransposto(Vertice* grafo, int vertice, int* componente, int componenteAtual) {
    grafo[vertice].visitado = 1;
    componente[vertice] = componenteAtual;

    for (int i = 0; i < grafo[vertice].numArestas; i++) {
        int destino = grafo[vertice].arestas[i].destino;
        if (!grafo[destino].visitado) {
            dfsTransposto(grafo, destino, componente, componenteAtual);
        }
    }
}

// Função para realizar uma busca em largura no grafo
int bfs(Vertice* grafo, int numVertices, int origem, int destino) {
    int* visitado = (int*)malloc(numVertices * sizeof(int));
    for (int i = 0; i < numVertices; i++) {
        visitado[i] = 0;
    }

    // Fila para a BFS
    int* fila = (int*)malloc(numVertices * sizeof(int));
    int frente = 0;
    int tras = 0;

    // Inicia a busca a partir do vértice de origem
    visitado[origem] = 1;
    fila[tras++] = origem;

    while (frente != tras) {
        int verticeAtual = fila[frente++];
        for (int i = 0; i < grafo[verticeAtual].numArestas; i++) {
            int vizinho = grafo[verticeAtual].arestas[i].destino;
            if (!visitado[vizinho]) {
                visitado[vizinho] = 1;
                fila[tras++] = vizinho;
            }
        }
    }

    int alcancavel = visitado[destino];

    free(visitado);
    free(fila);

    return alcancavel;
}

// Função para calcular o caminho mais curto usando o algoritmo de Dijkstra
int dijkstra(Vertice* grafo, int numVertices, int origem, int destino) {
    int* distancia = (int*)malloc(numVertices * sizeof(int));
    int* visitado = (int*)malloc(numVertices * sizeof(int));

    for (int i = 0; i < numVertices; i++) {
        distancia[i] = INT_MAX;
        visitado[i] = 0;
    }

    distancia[origem] = 0;

    for (int i = 0; i < numVertices - 1; i++) {
        int u = -1;
        for (int j = 0; j < numVertices; j++) {
            if (!visitado[j] && (u == -1 || distancia[j] < distancia[u])) {
                u = j;
            }
        }

        visitado[u] = 1;

        for (int j = 0; j < grafo[u].numArestas; j++) {
            int v = grafo[u].arestas[j].destino;
            int peso = grafo[u].arestas[j].peso;

            if (distancia[u] != INT_MAX && distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
            }
        }
    }

    int resultado = distancia[destino];

    free(distancia);
    free(visitado);

    return resultado;
}