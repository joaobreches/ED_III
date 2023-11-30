#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADE_H

#include"registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void criaTabela(char* nomeArquivoCSV, char* nomeArquivoBinario);
void imprimeArquivo(char* nomeArquivoBinario);
void recuperaDados(char *arquivoEntrada, int n);
void recuperaRegistro(char *arquivoEntrada, int rrn);

#endif