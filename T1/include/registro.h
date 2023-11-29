#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void escreveCabecalho(FILE *arquivo, Cabecalho cabecalho);
void printCabecalho(FILE *arquivo);
int skipCabecalho(FILE *arquivoBinario);
void imprimeRegistro(Registro registro);
int comparaTecnologias(FILE *arquivoBinario, StringVariavel novaTecOrigem, StringVariavel novaTecDestino);
void contaTecnologias(FILE* arquivoBinario, Registro registroAtual, Cabecalho* cabecalho);
Registro trabalhaRegistros(FILE *arquivo, Registro *registro);
char* diretorioArquivo(char* nomeArquivo, char tipoArquivo);
void imprimeBinario(char* binario);

#endif