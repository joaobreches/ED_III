#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "grafo.h"

void quicksortRecursivoVertice(Vertice *v, int ini, int fim);
void quicksortVertice(Vertice *v, int n);
void quicksortRecursivoAresta(Aresta *a, int ini, int fim);
void quicksortAresta(Aresta *a, int n);
int compararStrings(const void *a, const void *b);

#endif