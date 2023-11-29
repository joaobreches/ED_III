#ifndef ARVOREB_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM_ARVORE_B 4

// Estrutura para representar um registro de cabeçalho no arquivo de índice
typedef struct {
    char status;
    int noRaiz;
    int RRNproxNo;
} RegistroCabecalho;

// Estrutura para representar um nó na árvore-B
typedef struct {
    int removidoLogico;
    int RRN;
    int nroChavesNo;
    int alturaNo;
    int chave[ORDEM_ARVORE_B - 1];
    int RRNdoNo[ORDEM_ARVORE_B];
} RegistroDados;


// void imprimeRegistroDados(RegistroDados registro);
void insereEmNoNaoCheio(RegistroDados *no, int chave, int RRNdoNo, int RRNdoNovoNo, FILE *arquivoIndice);
void particionaNo(int RRN, int i, int chave, int RRNdoNovoNo, FILE *arquivoIndice);
int proximoRRNNo(FILE *arquivoIndice);
void insereNaArvoreB(int chave, int RRN, FILE *arquivoIndice);
int insereNaArvoreBRecursivo(int chave, int RRN, int nivel, int *chavePromovida, int *RRNdoNovoNo, FILE *arquivoIndice);
int buscaArvoreB(FILE *arquivoIndice, int RRN, int chave);

#endif