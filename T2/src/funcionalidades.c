#include "funcionalidades.h"
#include "funcoesFornecidas.h"
#include "registro.h"
#include "arvoreB.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void criaTabela(char* nomeArquivoCSV, char* nomeArquivoBinario) {
  /*
  Essa funcao le um arquivo csv e escreve os registros com campo fixo e variavel em um arquivo binario. 

  O arquivo inicialmente possui um cabecalho que indica o status do arquivo binario, o indice do proximo registro, 
  o numero de tecnologias distintas presentes e o numero de pares de tecnologias.

  Essa funcao representa a funcionalidade 1 do exercicio introdutorio.
  */

  // nomeArquivoCSV = diretorioArquivo(nomeArquivoCSV, 'c');

  // abre arquivo csv
  FILE* arquivoCSV = fopen(nomeArquivoCSV, "r");
  if (arquivoCSV == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // nomeArquivoBinario = diretorioArquivo(nomeArquivoBinario, 'b');

  // abre arquivo binario
  FILE *arquivoBinario = fopen(nomeArquivoBinario, "wb+");
  if (arquivoBinario == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // inicializa o cabecalho do arquivo binario
  Cabecalho cabecalho;
  cabecalho.status = '0';
  cabecalho.proxRRN = 0;
  cabecalho.nroTecnologias = 0;
  cabecalho.nroParesTecnologias = 0;
  escreveCabecalho(arquivoBinario, cabecalho);

  // aloca memoria para leitura de uma linha
  char *linha = malloc(sizeof(char) * TAM_REGISTRO);
  if (linha == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // inicializa registro auxiliar para armazenar os valores lidos no csv e colunas para ler o que cada coluna do csv representa
  Registro registroAtual;
  char *colunas[5];

  // le a primeira linha e separa os tokens por virgula, armazena em no vetor de string coluna
  fgets(linha, TAM_REGISTRO, arquivoCSV);
  char *col = strtok(linha, ",");

  for (int i = 0; i < 5; i++) {
    if (i == 4)
      col[strlen(col) - 2] = '\0';
    colunas[i] = strdup(col);
    col = strtok(NULL, ",");
  }

  // le o arquivo csv ate o fim e escreve no arquivo binario fazendo os devidos tratamentos
  while (1) {
    // le o arquivo csv ate o fim
    if (fgets(linha, TAM_REGISTRO, arquivoCSV) == NULL) {
      break;
    }
    else {
      fseek(arquivoCSV, (-1)*strlen(linha), SEEK_CUR);
    }

    // define o registro atual como nao removido
    registroAtual.removido = '0';

    // le os cinco campos do registro char a char para tratar campos nulos
    for (int i = 0; i < 5; i++) {

      char c;
      char *campo = malloc(sizeof(char) * 1);
      int j = 0;

      // le cada char ate encontrar o uma virgula, fim da linha ou fim do arquivo
      do {
        c = fgetc(arquivoCSV);
        campo[j++] = c;
        campo = realloc(campo, sizeof(char) * (j + 1));
      } while (c != ',' && c != '\n' && c != EOF);

      // campo = realloc(campo, sizeof(char) * (j - 1));
      // finaliza a string do campo com '\0'
      if(campo[j - 1] == ',' || campo[j - 1] == EOF) {
        // campo = realloc(campo, sizeof(char) * (j - 2));
        campo[j - 1] = '\0';
      }
      else {
        // campo = realloc(campo, sizeof(char) * (j - 3));
        campo[j - 2] = '\0';
      }

      // armazena o campo no registro atual de acordo com a ordem das colunas
      if (strcmp(colunas[i], "nomeTecnologiaOrigem") == 0) {
        registroAtual.TecnologiaOrigem.tamanho = strlen(campo); // campo nulo = 0
        registroAtual.TecnologiaOrigem.string = strdup(campo);
      } else if (strcmp(colunas[i], "nomeTecnologiaDestino") == 0) {
        registroAtual.TecnologiaDestino.tamanho = strlen(campo); // campo nulo = 0
        registroAtual.TecnologiaDestino.string = strdup(campo);
      } else if (strcmp(colunas[i], "grupo") == 0) {
        if (strcmp(campo, "") == 0)
          registroAtual.grupo = -1; // campo nulo
        else
          registroAtual.grupo = atoi(campo);
      } else if (strcmp(colunas[i], "popularidade") == 0) {
        if (strcmp(campo, "") == 0)
          registroAtual.popularidade = -1; // campo nulo
        else
          registroAtual.popularidade = atoi(campo);
      } else if (strcmp(colunas[i], "peso") == 0) {
        if (strcmp(campo, "") == 0 || strcmp(campo, " ") == 0)
          registroAtual.peso = -1; // campo nulo
        else
          registroAtual.peso = atoi(campo);
      }
      // libera a memoria alocada
      free(campo);
      campo = NULL;
    }

    // atualiza a contagem de tecnologias e pares de tecnologia
    contaTecnologias(arquivoBinario, registroAtual, &cabecalho);

    // escreve campos no arquivo binario
    fwrite(&registroAtual.removido, sizeof(char), 1, arquivoBinario);
    fwrite(&registroAtual.grupo, sizeof(int), 1, arquivoBinario);
    fwrite(&registroAtual.popularidade, sizeof(int), 1, arquivoBinario);
    fwrite(&registroAtual.peso, sizeof(int), 1, arquivoBinario);
    fwrite(&registroAtual.TecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBinario);
    fwrite(registroAtual.TecnologiaOrigem.string, sizeof(char), registroAtual.TecnologiaOrigem.tamanho, arquivoBinario);
    fwrite(&registroAtual.TecnologiaDestino.tamanho, sizeof(int), 1, arquivoBinario);
    fwrite(registroAtual.TecnologiaDestino.string, sizeof(char), registroAtual.TecnologiaDestino.tamanho, arquivoBinario);

    // conta a quantidade de memoria ocupada pelo registro
    int tam_bytes = sizeof(registroAtual.grupo) + sizeof(registroAtual.popularidade) + sizeof(registroAtual.peso) + sizeof(registroAtual.TecnologiaOrigem.tamanho) + sizeof(registroAtual.TecnologiaDestino.tamanho) + sizeof(char) + strlen(registroAtual.TecnologiaDestino.string) + strlen(registroAtual.TecnologiaOrigem.string);

    // completa o tamanho fixo do registro com lixo
    for (int i = tam_bytes; i < TAM_REGISTRO; i++) {
      char lixo = '$';
      fwrite(&lixo, sizeof(char), 1, arquivoBinario);
    }

    // libera a memoria alocada
    free(registroAtual.TecnologiaDestino.string);
    free(registroAtual.TecnologiaOrigem.string);
    
    // atualiza o indice do proximo registro 
    cabecalho.proxRRN++;
  }

  // libera a memoria alocada
  free(linha);

  // atualiza o cabecalho e fecha os arquivos abertos
  cabecalho.status = '1';
  escreveCabecalho(arquivoBinario, cabecalho);
  fclose(arquivoBinario);
  fclose(arquivoCSV);

  // libera a memoria alocada
  for(int i = 0; i < 5; i++) {
    free(colunas[i]);
  }
  // free(nomeArquivoCSV);
  // free(nomeArquivoBinario);
}

void imprimeArquivo(char* nomeArquivoBinario) {
  /*
  Essa funcao le um arquivo binario e imprime seus registros.

  Essa funcao representa a funcionalidade 2 do exercicio introdutorio.
  */
  
  // abre arquivo binario 
  // nomeArquivoBinario = diretorioArquivo(nomeArquivoBinario, 'b');

  FILE *arquivoBinario = fopen(nomeArquivoBinario, "rb");
  if (arquivoBinario == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // cria registro auxiliar
  Registro registro;

  int sucessoArquivo = skipCabecalho(arquivoBinario);
  if (sucessoArquivo == 1){
    return;
  }

  // le os registros do arquivo e imprime-os
  while (1) {

    // verifica se o proximo registro existe
    if (fread(&registro.removido, sizeof(char), 1, arquivoBinario) == 0) {
      break;
    }

    // le o proximo registro
    leRegistro(arquivoBinario, &registro); 

    // imprime o registro
    imprimeRegistro(registro);

    // libera a memoria alocada
    free(registro.TecnologiaOrigem.string);
    free(registro.TecnologiaDestino.string);
  }

  // fecha o arquivo binario
  fclose(arquivoBinario);
  // free(nomeArquivoBinario);
}

// Função para executar a funcionalidade 8
void recuperaDados8(const char *nomeArquivo) {
    
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

    // Ordena os nomes das tecnologias para a saída ordenada
    char **nomesOrdenados = (char**)malloc(numRegistros * sizeof(char*));
    for (int i = 0; i < numRegistros; i++) {
        nomesOrdenados[i] = vertices[i].nomeTecnologia;
    }
    qsort(nomesOrdenados, numRegistros, sizeof(char*), compararStrings);

    // Imprime os dados ordenados
    for (int i = 0; i < numRegistros; i++) {
        int indice = 0;
        // Encontra o índice correspondente ao nome ordenado
        while (strcmp(vertices[indice].nomeTecnologia, nomesOrdenados[i]) != 0) {
            indice++;
        }

        printf("%s, %d, %d, %d, %d, ", vertices[indice].nomeTecnologia,
               vertices[indice].grupo, vertices[indice].grauEntrada,
               vertices[indice].grauSaida, vertices[indice].grau);

        // Ordena as arestas pelo nomeTecDestino para a saída ordenada
        qsort(vertices[indice].arestas, vertices[indice].numArestas, sizeof(Aresta), compararArestas);

        // Imprime as arestas
        for (int j = 0; j < vertices[indice].numArestas; j++) {
            printf("%s, %d", vertices[indice].arestas[j].nomeTecDestino, vertices[indice].arestas[j].peso);
            if (j < vertices[indice].numArestas - 1) {
                printf(", ");
            }
        }

        printf("\n");
    }

    // Libera a memória alocada
    for (int i = 0; i < numRegistros; i++) {
        free(vertices[i].arestas);
    }
    free(vertices);
    free(nomesOrdenados);
}

// Função para executar a funcionalidade 10
void listaNomes(const char *nomeArquivo, int n) {
    
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

  Registro registro;

    // Realiza a funcionalidade n vezes
    for (int k = 0; k < n; k++) {
        // Lê o nome da tecnologia passado como parâmetro
        char tecnologia[50];
        fgets(tecnologia, sizeof(tecnologia), stdin);
        tecnologia[strcspn(tecnologia, "\n")] = '\0'; // Remove o caractere de nova linha

        // Encontra o índice correspondente ao nome passado como parâmetro
        int indice = 0;
        while (strcmp(vertices[indice].nomeTecnologia, tecnologia) != 0) {
            indice++;
            if (indice >= numRegistros) {
                printf("Registro inexistente.\n\n");
                continue;
            }
        }

        // Imprime a tecnologia passada como parâmetro
        printf("\"%s\": ", tecnologia);

        // Ordena as arestas pelo nomeTecOrigem para a saída ordenada
        qsort(vertices[indice].arestas, vertices[indice].numArestas, sizeof(Aresta), compararArestas);

        // Imprime as tecnologias que originaram a tecnologia passada como parâmetro
        int primeiraTecnologia = 1;
        for (int j = 0; j < vertices[indice].numArestas; j++) {
            if (primeiraTecnologia) {
                printf("%s", vertices[indice].arestas[j].nomeTecOrigem);
                primeiraTecnologia = 0;
            } else {
                printf(", %s", vertices[indice].arestas[j].nomeTecOrigem);
            }
        }

        printf("\n\n");
    }

    // Libera a memória alocada
    for (int i = 0; i < numRegistros; i++) {
        free(vertices[i].arestas);
    }
    free(vertices);
}