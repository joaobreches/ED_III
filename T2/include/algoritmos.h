#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "registro.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void liberarMemoria(Vertice *vertices, int numRegistros);
void liberaGrafo(Vertice *grafo, int numRegistros);
int compararArestas(const void *a, const void *b);
int compararNomes(const void *a, const void *b);
Pilha* inicializarPilha(int tamanho);
int pilhaVazia(Pilha* pilha);
void empilhar(Pilha* pilha, int item);
int desempilhar(Pilha* pilha);
Vertice* inicializarGrafo(int numVertices);
void adicionarAresta(Vertice* grafo, int origem, int destino);
void dfs(Vertice* grafo, int vertice, Pilha* pilha);
void dfsTransposto(Vertice* grafo, int vertice, int* componente, int componenteAtual);
int bfs(Vertice* grafo, int numVertices, int origem, int destino);
int dijkstra(Vertice* grafo, int numVertices, int origem, int destino);

#endif