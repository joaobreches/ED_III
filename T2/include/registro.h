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
#define TAM_NOME 100

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

void escreveCabecalho(FILE *arquivo, Cabecalho cabecalho);
void printCabecalho(FILE *arquivo);
bool skipCabecalho(FILE *arquivoBinario);
void imprimeRegistro(Registro registro);
int comparaTecnologias(FILE *arquivoBinario, StringVariavel novaTecOrigem, StringVariavel novaTecDestino);
void contaTecnologias(FILE* arquivoBinario, Registro registroAtual, Cabecalho* cabecalho);
Registro leRegistro(FILE *arquivo, Registro *registro);
bool leRegistroNaoNulo(FILE *arquivo, Registro *registro);

#endif