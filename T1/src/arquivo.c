#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* diretorioArquivo(char* nomeArquivo, char tipoArquivo) {
    char* diretorio;
  
    if (tipoArquivo == 'b')
        diretorio = "arquivo/binario/";
    else
        diretorio = "arquivo/csv/";

    char* resultado = malloc(strlen(diretorio) + strlen(nomeArquivo) + 1);
    strcpy(resultado, diretorio);
    strcat(resultado, nomeArquivo);

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

FILE* abreBinarioEscritaLeitura(char* nomeBinario){
    nomeBinario = diretorioArquivo(nomeBinario, 'b');
    FILE* binario = fopen(nomeBinario, "wb+");

    if(binario == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    free(nomeBinario);
    return binario;
}

FILE* abreBinarioEscrita(char* nomeBinario){
    nomeBinario = diretorioArquivo(nomeBinario, 'b');
    FILE* binario = fopen(nomeBinario, "wb");

    if(binario == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    free(nomeBinario);
    return binario;
}

FILE* abreBinarioLeitura(char* nomeBinario){
    nomeBinario = diretorioArquivo(nomeBinario, 'b');
    FILE* binario = fopen(nomeBinario, "rb");

    if(binario == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    free(nomeBinario);
    return binario;
}

FILE* abreCSVEscrita(char* nomeCSV){
    nomeCSV = diretorioArquivo(nomeCSV, 'c');
    FILE* csv = fopen(nomeCSV, "r");

    if(csv == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    free(nomeCSV);
    return csv;
}
