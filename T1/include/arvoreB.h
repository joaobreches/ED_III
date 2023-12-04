#ifndef ARVOREB_H
#define ARVOREB_H

#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ORDEM_ARVORE_B 4
#define TAM_PAGINA 205
#define TAM_CHAVE 55
#define TAM_CABECALHO_ARVORE 205

// Estrutura para representar o registro de cabeçalho no arquivo de índice
typedef struct _CabecalhoArvoreB {
    char status;
    int noRaiz;
    int RRNproxNo;
} CabecalhoArvoreB;

typedef struct _Chave {
    int ponteiroanterior;
    int ponteiroproximo;
    char* nome;
    int ref;
} Chave;

// Estrutura para representar um nó na árvore-B
typedef struct _Pagina {
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;
    Chave chave[ORDEM_ARVORE_B - 1];
    int ponteirofinal;
} Pagina;

FILE* abreIndiceEscrita(char* nomeIndice);
void fechaIndiceEscrita(FILE* indice);
void escreveCabecalhoArvoreB(FILE *indice, CabecalhoArvoreB cabecalho);
CabecalhoArvoreB leCabecalhoArvoreB(FILE* indice);
void printCabecalhoArvoreB(FILE* indice);
void escrevePagina(Pagina pagina, int RRN, FILE* indice);
Pagina lePagina(FILE* indice, int RRN);
void imprimePagina(Pagina pagina);
void criaPaginaNova(FILE* indice, int alturaNo, int ponteirofinal, Chave chave);
void insereEmNoNaoCheio(Pagina *pagina, Chave chave, FILE *indice);
void clearUltimaChave(Pagina *pagina);
void particionaNo(Pagina pagina, Chave chave, int RRNSuperior, FILE *indice);
void insereNaArvoreB(Chave chave, int ponteirofinal, int nivel,  FILE* indice);
void insereNaArvoreBRecursivo(Pagina pagina, int RRNSuperior, Chave chave, FILE *indice);
Pagina desceArvore(Chave chave, int RRNpagina, int nivel, int *RRNSuperior, FILE *indice);
int buscaArvoreB(FILE *indice, int RRNpagina, char* chave, Pagina *pagina);

#endif