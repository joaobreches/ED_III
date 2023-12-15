#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADE_H

#include "registro.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void criaTabela(char* nomeArquivoCSV, char* nomeArquivoBinario);
void imprimeArquivo(char* nomeArquivoBinario);
void executarFuncionalidade8(const char *nomeArquivo);

#endif