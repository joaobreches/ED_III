#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADE_H

#include "funcoesFornecidas.h"
#include "grafo.h"
#include <stdbool.h>

void recuperaDadosGrafo(char *nomeArquivo, bool transposto);
void listaNomes(char *nomeArquivo, int n, char** strings);
<<<<<<< HEAD
void fortementeConexo(char* nomeArquivo, int n);
void caminhoCurto(char* nomeArquivo, int n);
=======
void fortementeConexo(char* nomeArquivo);
void caminhoCurto(char* nomeArquivo, int n, char** strings);
>>>>>>> 736dc635b5ec729d4d87709cd5594bf96878e60c

#endif