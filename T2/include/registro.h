#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define TAM_REGISTRO_VARIAVEL 55
#define TAM_CABECALHO 13

typedef struct _Cabecalho {
  char status; // 0 para arquivo inconsistente e 1 para arquivo consistente
  int proxRRN;
  int nroTecnologias;
  int nroParesTecnologias;
} Cabecalho;

typedef struct _StringVariavel {
  int tamanho;
  char *string;
} StringVariavel;

typedef struct _Registro {
  char removido;

  int grupo;
  int popularidade;
  int peso;

  StringVariavel TecnologiaOrigem;
  StringVariavel TecnologiaDestino;
} Registro;

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

// Pilha para o algoritmo de Kosaraju
typedef struct {
    int *array;
    int topo;
} Pilha;


void escreveCabecalho(FILE *arquivo, Cabecalho cabecalho);
void printCabecalho(FILE *arquivo);
bool skipCabecalho(FILE *arquivoBinario);
void imprimeRegistro(Registro registro);
int comparaTecnologias(FILE *arquivoBinario, StringVariavel novaTecOrigem, StringVariavel novaTecDestino);
void contaTecnologias(FILE* arquivoBinario, Registro registroAtual, Cabecalho* cabecalho);
Registro leRegistro(FILE *arquivo, Registro *registro);
int compararStrings(const void *a, const void *b);
int compararArestas(const void *a, const void *b);

#endif