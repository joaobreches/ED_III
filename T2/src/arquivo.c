#include "arquivo.h"
#include "registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função que faz o caminho até o diretorio correto do arquivo
char* diretorioArquivo(char* nomeArquivo, char tipoArquivo) {
    /*
    Essa função encaminha o código na qual foi chamada direto para o diretorio correto do arquivo.
    É usada na maioria das funções que chamam diretamente algum arquivo.

    Por conta do runcodes, ela foi comentada e inutilizada no decorrer das funcoes do programa.
    */

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

// Imprime o arquivo binario
void imprimeBinario(char* binario){
//   binario = diretorioArquivo(binario, 'b');
  FILE* arquivoBinario = fopen(binario, "rb");

    if (arquivoBinario == NULL) { // confere se o arquivo existe
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    char c;
    while ((c = fgetc(arquivoBinario)) != EOF) {
        printf("char %c, ", c);
    }

    fclose(arquivoBinario); // Fechar o arquivo após o uso
    // free(binario); // Liberar a memória alocada dinamicamente

    printf("\n");
}

// Abre o arquivo para escrita e leitura
FILE* abreBinarioEscritaLeitura(char* nomeBinario){
    // nomeBinario = diretorioArquivo(nomeBinario, 'b');
    FILE* binario = fopen(nomeBinario, "wb+");

    if(binario == NULL){ // confere se o arquivo existe
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    // free(nomeBinario);
    return binario;
}

// Abre o arquivo binario para escrita
FILE* abreBinarioEscrita(char* nomeBinario){
    // nomeBinario = diretorioArquivo(nomeBinario, 'b');
    FILE* binario = fopen(nomeBinario, "wb");

    if(binario == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    // free(nomeBinario);
    return binario;
}

// abre o arquivo binario para leitura
FILE* abreBinarioLeitura(char* nomeBinario){
    // nomeBinario = diretorioArquivo(nomeBinario, 'b');
    FILE* binario = fopen(nomeBinario, "rb");

    if(binario == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    // free(nomeBinario);
    return binario;
}

// Abre o arquivo .CSV para escrita
FILE* abreCSVEscrita(char* nomeCSV){
    // nomeCSV = diretorioArquivo(nomeCSV, 'c');
    FILE* csv = fopen(nomeCSV, "r");

    if(csv == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    // free(nomeCSV);
    return csv;
}

void lidaGrafos(char* nomeArquivo){
    
    // Abrir o arquivo binário para leitura
    FILE *arquivo = abreBinarioLeitura(nomeArquivo);


    // Lê o número de registros no arquivo
    int numRegistros;
    fread(&numRegistros, sizeof(int), 1, arquivo);

    // Aloca espaço para os vértices
    Vertice *vertices = (Vertice*)malloc(numRegistros * sizeof(Vertice));

    // Lê os registros do arquivo
    for (int i = 0; i < numRegistros; i++) {
        fread(&vertices[i], sizeof(Vertice), 1, arquivo);
        // Aloca espaço para as arestas do vértice
        vertices[i].arestas = (Aresta*)malloc(vertices[i].numArestas * sizeof(Aresta));
        // Lê as arestas do arquivo
        fread(vertices[i].arestas, sizeof(Aresta), vertices[i].numArestas, arquivo);
    }

    // Fecha o arquivo
    fclose(arquivo);

}