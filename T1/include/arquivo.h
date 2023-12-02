#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* diretorioArquivo(char* nomeArquivo, char tipoArquivo);
void imprimeBinario(char* binario);
FILE* abreBinarioEscrita(char* nomeBinario);
FILE* abreBinarioLeitura(char* nomeBinario);
FILE* abreBinarioEscritaLeitura(char* nomeBinario);
FILE* abreCSVEscrita(char* nomeCSV);

#endif