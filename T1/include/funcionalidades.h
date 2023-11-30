#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADE_H

#include "registro.h"
#include "arvoreB.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void criaTabela(char* nomeArquivoCSV, char* nomeArquivoBinario); //funcionalidade 1
void imprimeArquivo(char* nomeArquivoBinario); //funcionalidade 2
bool filtroRegistro(FILE* arquivo, char nomeCampo[TAM_REGISTRO_VARIAVEL], char valorCampo[TAM_REGISTRO_FIXO]); //funcionalidade 3
void busca(int caso, char *arquivoEntrada, char* arquivoIndice, int n); //busca geral para funcionalidades 3 e 6
void recuperaRegistro(char *arquivoEntrada, int rrn); //funcionalidade 4
void criaIndiceArvoreB(char *arquivoDados, char *arquivoIndice); //funcionalidade 5
bool filtroArvore(char* arquivoDados, char* arquivoIndice, char* chave); //funcionalidade 6
void insereRegistro(char *arquivoDados, char *arquivoIndice, int n); //funcionalidade 7

#endif