#ifndef ARVOREB_H
#define ARVORE_H

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

// Estrutura para representar um nó na árvore-B
typedef struct _Pagina {
    int nroChavesNo;
    int alturaNo;
    int RRN;
    // int chave[ORDEM_ARVORE_B - 1];
    // int RRNdoNo[ORDEM_ARVORE_B];
} Pagina;


void imprimePagina(Pagina registro);
void insereEmNoNaoCheio(Pagina *no, int chave, int RRNdoNo, int RRNdoNovoNo, FILE *arquivoIndice);
void particionaNo(int RRN, int i, int chave, int RRNdoNovoNo, FILE *arquivoIndice);
int proximoRRNNo(FILE *arquivoIndice);
void insereNaArvoreB(int chave, int RRN, FILE *arquivoIndice);
int insereNaArvoreBRecursivo(int chave, int RRN, int nivel, int *chavePromovida, int *RRNdoNovoNo, FILE *arquivoIndice);
int buscaArvoreB(FILE *arquivoIndice, int RRN, int chave);

#endif