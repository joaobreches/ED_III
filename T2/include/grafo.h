#ifndef GRAFO_H
#define GRAFO_H

#include "registro.h"
#include "arquivo.h"
#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct _vertice Vertice;
typedef struct _aresta Aresta;
typedef struct _grafo Grafo;

typedef struct _aresta {
    // char nomeTecDestino[50];
    // char nomeTecOrigem[50];
    int peso;
    Vertice* destino;
} Aresta;

typedef struct _vertice {
    char nomeTecnologia[50];
    int grupo;
    int grauEntrada;
    int grauSaida;
    int grau;
    Aresta* arestas;
    int numArestas;
    int visitado;
} Vertice;

typedef struct _grafo {
    Vertice* vertices;
    int numVertices;
} Grafo;

// void liberarMemoria(Vertice *grafo, int numRegistros);
int compararArestas(const void *a, const void *b);
Grafo inicializarGrafo();
void liberaGrafo(Grafo grafo);
void imprimeGrafo(Grafo grafo);
void adicionaVertice(Grafo *grafo, char *nomeTecnologia, int grupo);
void adicionaAresta(Vertice* vertices, int origem, int destino, int peso);
void dfs(Vertice* grafo, int vertice, Pilha* pilha);
void dfsTransposto(Vertice* grafo, int vertice, int* componente, int componenteAtual);
int bfs(Vertice* grafo, int numVertices, int origem, int destino);
int dijkstra(Vertice* grafo, int numVertices, int origem, int destino);

#endif