#include "../include/funcionalidades.h"
#include "../include/registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void criaTabela(char* nomeArquivoCSV, char* nomeArquivoBinario) {
  /*
  Essa funcao le um arquivo csv e escreve os registros com campo fixo e variavel em um arquivo binario. O arquivo inicialmente possui um cabecalho que indica o status do arquivo binario, o indice do proximo registro, o numero de tecnologias distintas presentes e o numero de pares de tecnologias.

  Essa funcao representa a funcionalidade 1 do exercicio introdutorio.
  */

 nomeArquivoCSV = diretorioArquivo(nomeArquivoCSV, 'c');

  // abre arquivo csv
  FILE* arquivoCSV = fopen(nomeArquivoCSV, "r");
  if (arquivoCSV == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  nomeArquivoBinario = diretorioArquivo(nomeArquivoBinario, 'b');

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
}

// incluir mensagens de erro - erro de nao registros ja incluido
void imprimeArquivo(char* nomeArquivoBinario) {
  /*
  Essa funcao le um arquivo binario e imprime seus registros.

  Essa funcao representa a funcionalidade 2 do exercicio introdutorio.
  */
  
  // abre arquivo binario 
  nomeArquivoBinario = diretorioArquivo(nomeArquivoBinario, 'b');

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
    trabalhaRegistros(arquivoBinario, &registro); 

    // imprime o registro
    imprimeRegistro(registro);

    // libera a memoria alocada
    free(registro.TecnologiaOrigem.string);
    free(registro.TecnologiaDestino.string);
  }

  // fecha o arquivo binario
  fclose(arquivoBinario);
}

void recuperaDados(char *arquivoEntrada, int n) {
  /*
  Essa funcao le um arquivo binario e imprime os registros que se adequam a condicao fornecida seus registros.

  Essa funcao representa a funcionalidade 3 do exercicio introdutorio.
  */

  // abre arquivo binario 
  arquivoEntrada = diretorioArquivo(arquivoEntrada, 'b');

  FILE *arquivo = fopen(arquivoEntrada, "rb");
  if (arquivo == NULL) {
    printf("Falha no processamento do arquivo. \n");
    return;
  }

  // cria registro auxiliar
  Registro registro;

  // loop para cada uma das n condicoes fornecidas como entrada
  for (int i = 0; i < n; i++) {
    char nomeCampo[TAM_REGISTRO_VARIAVEL];
    char valorCampo[TAM_REGISTRO_FIXO];

    // le o nome do campo e o valor a ser buscado
    scanf("%s %s", nomeCampo, valorCampo);

    // limpa o valorCampo se for uma string (remove as aspas)
    if(strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0 || strcmp(nomeCampo, "nomeTecnologiaDestino") == 0){
      int i;
      for(i = 0; valorCampo[i] != '\0'; i++){
        valorCampo[i] = valorCampo[i + 1];
      }
      valorCampo[i - 2] = '\0';
    }

    // indica se o registro atende a condicao fornecida ou nao
    int encontrado = 0;
    
    // reseta o arquivo para o inicio
    rewind(arquivo);

    // pula os bytes do cabecalho do arquivo
    int sucessoArquivo = skipCabecalho(arquivo);
    if (sucessoArquivo == 1){
      return;
    }

    // le os registros do arquivo e imprime-os
    while (1) {

      // verifica se o proximo registro existe
      if (fread(&registro.removido, sizeof(char), 1, arquivo) == 0) {
        break;
      }

      // le o registro
      trabalhaRegistros(arquivo, &registro); 

      // indica se o registro atende a condicao fornecida ou nao
      int registrocondiz = 0;
      
      // compara se o campo eh igual a condicao fornecida
      if (registro.removido == '0') {
          if (strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0 && strcmp(registro.TecnologiaOrigem.string, valorCampo) == 0) {
            encontrado = 1;
            registrocondiz = 1;
            } else if (strcmp(nomeCampo, "grupo") == 0 && registro.grupo == atoi(valorCampo)) {
            encontrado = 1;
            registrocondiz = 1;
          } else if (strcmp(nomeCampo, "popularidade") == 0 && registro.popularidade == atoi(valorCampo)) {
            encontrado = 1;
            registrocondiz = 1;
          } else if (strcmp(nomeCampo, "nomeTecnologiaDestino") == 0 && strcmp(registro.TecnologiaDestino.string, valorCampo) == 0) {
            encontrado = 1;
            registrocondiz = 1;
          } else if (strcmp(nomeCampo, "peso") == 0 && registro.peso == atoi(valorCampo)) {
            encontrado = 1;
            registrocondiz = 1;
          }
      }

      // imprime o registro se encontrado
      if (registrocondiz == 1) {
        imprimeRegistro(registro);
        // break;
      }

      // libera a memoria alocada
      free(registro.TecnologiaOrigem.string);
      free(registro.TecnologiaDestino.string);
    }

    // imprime se nao encontrado nenhum registro em todo o arquivo
    if (encontrado == 0) {
      printf("Registro inexistente.\n");
    }
  }

  // fecha o arquivo binario
  fclose(arquivo);
}

void recuperaRegistro(char *arquivoEntrada, int rrn) {
  /*
  Essa funcao le um arquivo binario e imprime o registro de acordo com o seu RRN.

  Essa funcao representa a funcionalidade 4 do exercicio introdutorio.
  */

  // abre arquivo binario 
  arquivoEntrada = diretorioArquivo(arquivoEntrada, 'b');

  FILE *arquivo = fopen(arquivoEntrada, "rb");
  if (arquivo == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // cria registro auxiliar
  Registro registro;

  // verifica o status do arquivo binario
  char status;
  fread(&status, sizeof(char), 1, arquivo);
  if(status == '0'){
    printf("Falha no processamento do arquivo.\n");
    return;
  }  
  
  // le o proxrrn presente no cabecalho do arquivo
  int proxrrn;
  fread(&proxrrn, sizeof(int), 1, arquivo);

  // verifica se ha um registro com o rrn desejado no arquivo
  if(proxrrn - 1 < rrn){
    printf("Registro inexistente.\n");
    fclose(arquivo);
    return;
  }

  // pula o ponteiro do arquivo para o byteoffset do registro com rrn desejado
  int byte_offset = TAM_CABECALHO - 5 + ((rrn) * TAM_REGISTRO);
  fseek(arquivo, byte_offset, SEEK_CUR);

  // verifica se o registro desejado foi removido
  fread(&registro.removido, sizeof(char), 1, arquivo);
  if (registro.removido == '1') {
    printf("Registro inexistente.\n");
    fclose(arquivo);
    return;
  }

  // le o registro desejado e imprime-o
  trabalhaRegistros(arquivo, &registro);
  imprimeRegistro(registro);

  // fecha o arquivo
  fclose(arquivo);
}