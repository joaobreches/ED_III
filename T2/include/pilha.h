#ifndef PILHA_H
#define PILHA_H

#include <stdlib.h>

// Pilha para o algoritmo de Kosaraju
typedef struct {
    int *array;
    int topo;
} Pilha;

Pilha* inicializarPilha(int tamanho);
int pilhaVazia(Pilha* pilha);
void empilhar(Pilha* pilha, int item);
int desempilhar(Pilha* pilha);

#endif