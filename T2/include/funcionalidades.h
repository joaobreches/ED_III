#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADE_H

#include "funcoesFornecidas.h"
#include "grafo.h"
#include <stdbool.h>

void recuperaDadosGrafo(char *nomeArquivo, bool transposto);
void listaNomes(char *nomeArquivo, int n, char** strings);
void fortementeConexo(char* nomeArquivo);
void caminhoCurto(char* nomeArquivo, int n, char** strings);

#endif