#ifndef ARVOREB_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM_ARVORE_B 4

typedef struct {
    char lixo[3];        
    char nomeTecnologiaOrigem[15]; // 15 bytes
    char nomeTecnologiaDestino[40]; // 40 bytes
    char lixoChave[1];  
    char removidoLogico;   
    int chave;
    int grupo;
    int popularidade;
    int peso;
} RegistroDados;

// Estrutura para representar um registro de cabeçalho no arquivo de índice
typedef struct {
    char status;
    int noRaiz;
    int RRNproxNo;
} RegistroCabecalho;

// Estrutura para representar um nó na árvore-B
typedef struct {
    int nroChavesNo;
    int alturaNo;
    int chave[ORDEM_ARVORE_B - 1];
    int RRNdoNo[ORDEM_ARVORE_B];
} RegistroDadosArvoreB;


void imprimeRegistroDados(RegistroDados registro);
void insereEmNoNaoCheio(RegistroDadosArvoreB *no, int chave, int RRNdoNo, int RRNdoNovoNo, FILE *arquivoIndice);
void particionaNo(int RRN, int i, int chave, int RRNdoNovoNo, FILE *arquivoIndice);
int proximoRRNNo(FILE *arquivoIndice);
int insereNaArvoreBRecursivo(int chave, int RRN, int nivel, int *chavePromovida, int *RRNdoNovoNo, FILE *arquivoIndice);
void criarIndiceArvoreB(const char *arquivoDados, const char *arquivoIndice);
int buscaArvoreB(FILE *arquivoIndice, int RRN, int chave);

#endif


