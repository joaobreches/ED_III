#ifndef GRAFO_H
#define GRAFO_H

#include "registro.h"
#include "arquivo.h"
#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nomeTecDestino[50];
    char nomeTecOrigem[50];
    int peso;
    int destino;
} Aresta;

typedef struct {
    char nomeTecnologia[50];
    int grupo;
    int grauEntrada;
    int grauSaida;
    int grau;
    Aresta* arestas;
    int numArestas;
    int visitado;
} Vertice;

void liberarMemoria(Vertice *vertices, int numRegistros);
void liberaGrafo(Vertice *grafo, int numRegistros);
int compararArestas(const void *a, const void *b);
int compararNomes(const void *a, const void *b);
Vertice* inicializarGrafo(int numVertices);
void adicionarAresta(Vertice* grafo, int origem, int destino);
void dfs(Vertice* grafo, int vertice, Pilha* pilha);
void dfsTransposto(Vertice* grafo, int vertice, int* componente, int componenteAtual);
int bfs(Vertice* grafo, int numVertices, int origem, int destino);
int dijkstra(Vertice* grafo, int numVertices, int origem, int destino);

#endif