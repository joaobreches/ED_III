#ifndef GRAFO_H
#define GRAFO_H

#include "registro.h"
#include "arquivo.h"
#include "pilha.h"
#include <limits.h>

typedef struct _vertice Vertice;
typedef struct _aresta Aresta;
typedef struct _grafo Grafo;

typedef struct _aresta {
    // char nomeTecDestino[50];
    // char nomeTecOrigem[50];
    int peso;
    Vertice* destino;
    Aresta* prox;
} Aresta;

typedef struct _vertice {
    char* nomeTecnologia;
    int grupo;
    int grauEntrada;
    int grauSaida;
    int grau;
    Aresta* ini;
    int numArestas;
    int visitado;
} Vertice;

typedef struct _grafo {
    Vertice** vertices;
    int numVertices;
} Grafo;

Grafo inicializarGrafo();
void liberaGrafo(Grafo grafo);
void imprimeGrafo(Grafo grafo);
void adicionaVertice(Grafo *grafo, char *nomeTecnologia, int grupo);
void adicionaAresta(Vertice** vertices, int origem, int destino, int peso);
Grafo criaGrafo(FILE *arquivo, bool transposto);
int compararArestas(const void *a, const void *b);
void dfs(Vertice* vertice, Pilha* pilha);
void dfsTransposto(Vertice* vertice, int* componente, int componenteAtual);
int bfs(Vertice* grafo, int numVertices, int origem, int destino);
int dijkstra(Vertice* grafo, int numVertices, int origem, int destino);
void quicksortRecursivoVertice(Vertice **v, int ini, int fim);
void quicksortVertice(Vertice **v, int n);

#endif