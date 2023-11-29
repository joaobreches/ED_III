#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void criaTabela(char* nomeArquivoCSV, char* nomeArquivoBinario); //funcionalidade 1
void imprimeArquivo(char* nomeArquivoBinario); //funcionalidade 2
void recuperaDados(char *arquivoEntrada, int n); //funcionalidade 3
void recuperaRegistro(char *arquivoEntrada, int rrn); //funcionalidade 4
void criarIndiceArvoreB(char *arquivoDados, char *arquivoIndice); //funcionalidade 5
void selectArvoreB(char *arquivoDados, char *arquivoIndice, int n, char **campos, char **valores); //funcionalidade 6

#endif