#include "grafo.h"
#include "registro.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Função para liberar memória alocada para os vértices e arestas
void liberarMemoria(Vertice *vertices, int numRegistros) {
    for (int i = 0; i < numRegistros; i++) {
        free(vertices[i].arestas);
    }
    free(vertices);
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
    for (int i = 0; i < numVertices; i++) {
        grafo[i].visitado = 0;
        grafo[i].arestas = NULL;
        grafo[i].numArestas = 0;
    }
    return grafo;
}

void liberaGrafo(Vertice *grafo, int numRegistros) {
    for (int i = 0; i < numRegistros; i++) {
        free(grafo[i].arestas);
    }
    free(grafo);
}

// Função para adicionar uma aresta ao grafo
void adicionarAresta(Vertice* grafo, int origem, int destino) {
    grafo[origem].arestas = (Aresta*)realloc(grafo[origem].arestas, (grafo[origem].numArestas + 1) * sizeof(Aresta));
    grafo[origem].arestas[grafo[origem].numArestas].destino = destino;
    grafo[origem].numArestas++;
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