#include "pilha.h"

// Função para inicializar a pilha
Pilha* inicializarPilha(int tamanho) {
    Pilha *pilha = (Pilha*)malloc(sizeof(Pilha));
    pilha->array = (int*)malloc(tamanho * sizeof(int));
    pilha->topo = -1;
    return pilha;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(Pilha* pilha) {
    return pilha->topo == -1;
}

// Função para empilhar um elemento na pilha
void empilhar(Pilha* pilha, int item) {
    pilha->array[++pilha->topo] = item;
}

// Função para desempilhar um elemento da pilha
int desempilhar(Pilha* pilha) {
    if (!pilhaVazia(pilha)) {
        return pilha->array[pilha->topo--];
    }
    return -1; // Valor de sentinela para indicar pilha vazia
}
