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
#define TAM_CABECALHO_ARVORE 9

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
void fechaIndiceEscrita(FILE* indice, char* nomeIndice);
void escreveCabecalhoArvoreB(FILE *arquivo, CabecalhoArvoreB cabecalho);
CabecalhoArvoreB leCabecalhoArvoreB(FILE* arquivo);
void printCabecalhoArvoreB(FILE* arquivoIndice);
bool skipCabecalhoArvore(FILE *arquivoBinario);
void escrevePagina(Pagina pagina, int RRN, FILE* arquivoIndice);
void imprimePagina(Pagina pagina);
void insereEmNoNaoCheio(Pagina *pagina, Chave chave, FILE *arquivoIndice);
void particionaNo(Pagina pagina, Chave chave, int RRNSuperior, FILE *arquivoIndice, char* nomeArquivoIndice);
Pagina lePagina(FILE* arquivoIndice, int RRN);
void criaPaginaNova(char* nomeArquivoIndice, int alturaNo, int ponteirofinal, Chave chave);
void insereNaArvoreB(Chave chave, int ponteirofinal, char* nomeArquivoIndice);
Pagina desceArvore(Chave chave, int RRNpagina, int *RRNSuperior, char* nomeArquivoIndice);
void insereNaArvoreBRecursivo(Pagina pagina, int RRNSuperior, Chave chave, FILE *arquivoIndice, char* nomeArquivoIndice);
int buscaArvoreB(FILE *arquivoIndice, int RRNpagina, char* chave);

#endif