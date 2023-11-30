#ifndef ARVOREB_H
#define ARVOREB_H

#include "registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void escreveCabecalhoArvoreB(FILE *arquivo, CabecalhoArvoreB cabecalho);
CabecalhoArvoreB leCabecalhoArvoreB(FILE *arquivo);
void printCabecalhoArvoreB(FILE *arquivo);
bool skipCabecalhoArvore(FILE *arquivoBinario);
void imprimePagina(Pagina registro);
void insereEmNoNaoCheio(Pagina *no, int chave, int RRNdoNo, int RRNdoNovoNo, FILE *arquivoIndice);
void particionaNo(int RRN, int i, int chave, int RRNdoNovoNo, FILE *arquivoIndice);
int proximoRRNNo(FILE *arquivoIndice);
void escrevePagina(Pagina pagina, char* nomeArquivoIndice);
Pagina lePagina(FILE* arqIndice, int RRN);
void criaPaginaNova(FILE *arquivoIndice, CabecalhoArvoreB cabecalho, int alturaNo, int ponteirofinal, Chave chave);
void insereNaArvoreB(Chave chave, char *nomeArquivoIndice);
int insereNaArvoreBRecursivo(int chave, int RRN, int nivel, int *chavePromovida, int *RRNdoNovoNo, FILE *arquivoIndice);
int buscaArvoreB(FILE *arquivoIndice, int RRN, char* chave);

#endif