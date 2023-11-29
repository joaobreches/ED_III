#ifndef ARVOREB_H
#define ARVOREB_H

#include "registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM_ARVORE_B 4
#define TAM_PAGINA_DISCO 205

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
    Chave chave[ORDEM_ARVORE_B];
    int ponteirofinal;
} Pagina;

void escreveCabecalhoArvoreB(FILE *arquivo, CabecalhoArvoreB cabecalho);
CabecalhoArvoreB leCabecalhoArvoreB(FILE *arquivo);
void printCabecalhoArvoreB(FILE *arquivo);
void imprimePagina(Pagina registro);
void insereEmNoNaoCheio(Pagina *no, int chave, int RRNdoNo, int RRNdoNovoNo, FILE *arquivoIndice);
void particionaNo(int RRN, int i, int chave, int RRNdoNovoNo, FILE *arquivoIndice);
int proximoRRNNo(FILE *arquivoIndice);
void insereNaArvoreB(Chave chave, char *nomeArquivoIndice);
int insereNaArvoreBRecursivo(int chave, int RRN, int nivel, int *chavePromovida, int *RRNdoNovoNo, FILE *arquivoIndice);
int buscaArvoreB(FILE *arquivoIndice, int RRN, int chave);

#endif