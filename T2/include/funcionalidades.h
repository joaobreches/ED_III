#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADE_H

#include "funcoesFornecidas.h"
#include "algoritmos.h"
#include <stdbool.h>

void recuperaDadosGrafo(char *nomeArquivo);
void listaNomes(char *nomeArquivo, int n);
void fortementeConexo(char* nomeArquivo);
void caminhoCurto(char* nomeArquivo, int n);

#endif