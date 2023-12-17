#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADE_H

#include "funcoesFornecidas.h"
#include "algoritmos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void criaGrafo(char *nomeArquivo);
void criaGrafoTransposto(char *nomeArquivo);
void listaNomes(char *nomeArquivo, int n);
void fortementeConexo(char* nomeArquivo);
void caminhoCurto(char* nomeArquivo, int n);

#endif