#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* diretorioArquivo(char* nomeArquivo, char tipoArquivo) {
    char* diretorio;
  
    if (tipoArquivo == 'b')
        diretorio = strdup("arquivo/binario/");
    else
        diretorio = strdup("arquivo/csv/");

    char* resultado = malloc(strlen(diretorio) + strlen(nomeArquivo) + 1);
    strcpy(resultado, diretorio);
    strcat(resultado, nomeArquivo);

    free(diretorio); // Liberar a memória alocada para a string original

    return resultado;
}

void imprimeBinario(char* binario){
  binario = diretorioArquivo(binario, 'b');
  FILE* arquivoBinario = fopen(binario, "rb");

    if (arquivoBinario == NULL) {
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    char c;
    while ((c = fgetc(arquivoBinario)) != EOF) {
        printf("char %c, ", c);
    }

    fclose(arquivoBinario); // Fechar o arquivo após o uso
    free(binario); // Liberar a memória alocada dinamicamente

    printf("\n");
}

FILE* abreBinarioEscrita(char* nomeBinario){
    FILE* binario = fopen(diretorioArquivo(nomeBinario, 'b'), "wb");

    if(binario == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    return binario;
}

FILE* abreBinarioLeitura(char* nomeBinario){
    FILE* binario = fopen(diretorioArquivo(nomeBinario, 'b'), "rb");

    if(binario == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    return binario;
}

FILE* abreCSVEscrita(char* nomeCSV){
    FILE* csv = fopen(diretorioArquivo(nomeCSV, 'c'), "r");

    if(csv == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    return csv;
}
