#ifndef AUX_H
#define AUX_H

#include "aux.h"
#include <stdio.h> 
#include <stdlib.h>

#define TAM_PAG_DISCO 205  //tamanho fixo da página de disco nesse trabalho
#define TAM_CHAVE 55    //tamanho das chaves da árvore B
#define ORDEM 4
#define LIXO_CABECALHO 196  //quantidade de lixo no cabecalho do arquivo de índice

typedef struct {
    char status;
    int noRaiz;
    int RRNproxNo;
    char lixo[LIXO_CABECALHO];
} CabecalhoooArvoreB;

typedef struct {
    char C[55];   // chave correspondente ao registro
    int Pr;  // RRN do registro no arquivo de dados
} Dado;

typedef struct {    //Página/Nó da Árvore B
    int nroChavesNo;   
    int AlturaNo;
    int RRNdoNo;
    int P[ORDEM];  //RRN dos nós filhos
    Dado dado[ORDEM - 1];
} No;

void leBinario();
void comparaBinarios();

#endif