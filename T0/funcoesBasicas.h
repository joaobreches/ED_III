#ifndef FUNCOESBASICAS_H
#define FUNCOESBASICAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define TAM_REGISTRO_VARIAVEL 55
#define TAM_CABECALHO 13

typedef struct _Cabecalho Cabecalho;
typedef struct _StringVariavel StringVariavel;
typedef struct _Registro Registro;

void escreveCabecalho(FILE *arquivo, Cabecalho cabecalho);
void printCabecalho(FILE *arquivo);
int skipCabecalho(FILE *arquivoBinario);
void imprimeRegistro(Registro registro);
int comparaTecnologias(FILE *arquivoBinario, StringVariavel novaTecOrigem, StringVariavel novaTecDestino);
void contaTecnologias(FILE* arquivoBinario, Registro registroAtual, Cabecalho* cabecalho);
Registro trabalhaRegistros(FILE *arquivo, Registro *registro);
void criaTabela(char* nomeArquivoCSV, char* nomeArquivoBinario);
void imprimeArquivo(char* nomeArquivoBinario);
void recuperaDados(char *arquivoEntrada, int n);
void recuperaRegistro(const char *arquivoEntrada, int rrn);

#endif