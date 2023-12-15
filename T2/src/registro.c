#include "registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void escreveCabecalho(FILE *arquivo, Cabecalho cabecalho) {
  /*
  Escreve o cabecalho de  arquivo binario referente aos dados presentes no arquivo
  
  Essa funcao eh chamada na funcao "criaTabela" do cabecalho funcoesBasicas.h 
  */
  
  fseek(arquivo, 0, SEEK_SET); //inicia a escrita no começo do arquivo

  //escreve os dados do cabecalho
  fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
  fwrite(&cabecalho.proxRRN, sizeof(int), 1, arquivo);
  fwrite(&cabecalho.nroTecnologias, sizeof(int), 1, arquivo);
  fwrite(&cabecalho.nroParesTecnologias, sizeof(int), 1, arquivo);
}

void printCabecalho(FILE *arquivo) {
  /*
  Imprime o cabecalho de um arquivo binario
  
  Essa eh uma funcao auxiliar usada para verificacao dos valores do cabecalho e nao foi implementada nas funcoes principais do projeto
  */
  
  fseek(arquivo, 0, SEEK_SET); //inicia a leitura no começo do arquivo

  //armazena os dados do cabecalho em uma struct cabecalho e imprime seus valores
  Cabecalho cabecalho;
  fread(&cabecalho.status, sizeof(char), 1, arquivo);
  fread(&cabecalho.proxRRN, sizeof(int), 1, arquivo);
  fread(&cabecalho.nroTecnologias, sizeof(int), 1, arquivo);
  fread(&cabecalho.nroParesTecnologias, sizeof(int), 1, arquivo);
  printf("%c %d %d %d\n", cabecalho.status, cabecalho.proxRRN, cabecalho.nroTecnologias, cabecalho.nroParesTecnologias);
}

bool skipCabecalho(FILE *arquivoBinario){
  /*
  Essa funcao verifica se o arquivobinario esta consistente, se existem registros nele e pula para o byteoffset do primeiro registro

  Essa funcao eh chamada nas funcoes "imprimeArquivo" e "recuperaDados" do cabecalho funcoesBasicas.h

  retornos:
  0 - bem sucedido
  1 - arquivo inconsistente ou ausencia de registros
  */
  
  // le o cabecalho do arquivo binario e verifica se o arquivo esta consistente
  char status;
  fread(&status, sizeof(char), 1, arquivoBinario);
  if(status == '0'){
    printf("Falha no processamento do arquivo.\n");
    return 1;
  } 

  // verifica se ha registros
  int proxRRN;
  fread(&proxRRN, sizeof(int), 1, arquivoBinario);
  if (proxRRN < 1) {
    printf("Registro inexistente.\n");
    return 1;
  }

  // pula para o byteoffsset do primeiro registro
  fseek(arquivoBinario, 8, SEEK_CUR);
  
  return 0;
}

void imprimeRegistro(Registro registro) {
  /*
  Essa funcao imprime o registro no formato:
  
  nomTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso

  imprimindo os campos inteiros nulos (-1) como NULO

  Essa funcao eh chamada nas funcoes "imprimeArquivo", "recuperaDados" e "recuperaRegistro" do cabecalho funcoesBasicas.h
  */
  
  // imprime o registro, se o campo for nulo (-1) imprime NULO
  printf("%s, ", registro.TecnologiaOrigem.string);
  if(registro.grupo == -1)
    printf("NULO, ");
  else
    printf("%d, ", registro.grupo);
  if(registro.popularidade == -1)
    printf("NULO, ");
  else
    printf("%d, ", registro.popularidade);
  printf("%s, ", registro.TecnologiaDestino.string);
  if(registro.peso == -1)
    printf("NULO\n");
  else
    printf("%d\n", registro.peso);
}

int comparaTecnologias(FILE *arquivoBinario, StringVariavel novaTecOrigem, StringVariavel novaTecDestino) {
  /*
  Compara tecnologias e par de tecnologias com os demais dados de um arquivo binario. O inteiro retornado indica se ha tecnologias e/ou pares de tecnologias novos  
  
  Essa funcao eh chamada na funcao "contaTecnologias" do cabecalho funcoesBasicas.h 
  
  retornos:

  0: nada novo;
  1: apenas par novo;
  2: apenas destino e par novos;
  3: apenas origem e par novos;
  4: origem, destino e par novos;
  5: destino novo e origem nulo;
  6: origem nova e destino nulo.
  */

  // cria um registro auxiliar e define inteiros que indicam se as tecnologias atuais ja foram encontradas (0 como padrao e 1 para se encontrada)
  Registro registro;
  int tecRepetidaOrigem = 0;
  int tecRepetidaDestino = 0;
  printf("entrou!");
  // inicia a leitura do arquivo pulando o cabecalho
  if (fseek(arquivoBinario, TAM_CABECALHO, SEEK_SET) != 0) {
    printf("Falha no processamento do arquivo.\n");
    fclose(arquivoBinario);
    return -1;
  }

  // le o arquivo ate o final
  int RRN = 0;
  while (1) {
    if (fread(&registro.removido, sizeof(char), 1, arquivoBinario) == 0) {
      break;
    }

    // verifica se o registro foi removido e passa para a proxima interacao se verdadeiro
    if (registro.removido == '1') {
      RRN++;
      fseek(arquivoBinario, RRN * TAM_REGISTRO, SEEK_SET);
      continue;
    }

    // os primeiros campos do registro (grupo, popularidade e peso) e vai ate os campos das tecnologias
    fseek(arquivoBinario, 12, SEEK_CUR);

    // le a tecnologia de origem e armazena no registro auxiliar
    fread(&registro.TecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBinario);

    registro.TecnologiaOrigem.string = malloc((registro.TecnologiaOrigem.tamanho + 1) * sizeof(char));
    fread(registro.TecnologiaOrigem.string, sizeof(char), registro.TecnologiaOrigem.tamanho, arquivoBinario);
    registro.TecnologiaOrigem.string[registro.TecnologiaOrigem.tamanho] = '\0';

    // le a tecnologia de destino e armazena no registro auxiliar
    fread(&registro.TecnologiaDestino.tamanho, sizeof(int), 1, arquivoBinario);

    registro.TecnologiaDestino.string = malloc((registro.TecnologiaDestino.tamanho + 1) * sizeof(char));
    fread(registro.TecnologiaDestino.string, sizeof(char), registro.TecnologiaDestino.tamanho, arquivoBinario);
    registro.TecnologiaDestino.string[registro.TecnologiaDestino.tamanho] = '\0';

    // pula o lixo do registro com o ponteiro do arquivo 
    RRN++;
    fseek(arquivoBinario, TAM_REGISTRO * RRN, SEEK_SET);

    printf("%d, %d, %d", RRN, registro.TecnologiaDestino.tamanho, registro.TecnologiaOrigem.tamanho);
    // finaliza as strings das tecnologias com '\0'
    registro.TecnologiaOrigem.string[registro.TecnologiaOrigem.tamanho] = '\0';
    registro.TecnologiaDestino.string[registro.TecnologiaDestino.tamanho] = '\0';

    // compara as tecnologias lidas anteriormente com as tecnologias novas fornecidas como parametro da funcao
    if (strcmp(registro.TecnologiaOrigem.string, novaTecOrigem.string) == 0) {
      if (strcmp(registro.TecnologiaDestino.string, novaTecDestino.string) == 0) {
        return 0; // retorna se as tecnologias de origem e destino sao as mesmas e o par eh o mesmo tambem
      }
      tecRepetidaOrigem = 1; // tecnologia de origem eh a mesma
    }
    if (tecRepetidaDestino == 0 && strcmp(registro.TecnologiaDestino.string, novaTecDestino.string) == 0) {
      tecRepetidaDestino = 1; // tecnologia de destino eh a mesma
    }
    if (tecRepetidaDestino == 0 && strcmp(registro.TecnologiaOrigem.string, novaTecDestino.string) == 0) {
      tecRepetidaDestino = 1; // nova tecnologia de destino eh a mesma da tecnologia de origem lida
    }
    if (tecRepetidaOrigem == 0 && strcmp(registro.TecnologiaDestino.string, novaTecOrigem.string) == 0) {
      tecRepetidaOrigem = 1; // nova tecnologia de origem eh a mesma da tecnologia de destino lida
    }

    // libera a memoria alocada pelo registro auxiliar
    free(registro.TecnologiaOrigem.string);
    free(registro.TecnologiaDestino.string);
  }

  // tratamento de campos nulos
  if((novaTecOrigem.tamanho == 0 && novaTecDestino.tamanho == 0) || (novaTecOrigem.tamanho == 0 && tecRepetidaDestino == 1) || (novaTecDestino.tamanho == 0 && tecRepetidaOrigem == 1)){
    return 0; // retorna se os dois sao nulos ou um eh nulo e o outro eh repetido  
  }
  if(novaTecOrigem.tamanho == 0 && tecRepetidaDestino == 0){
    return 5; // retorna se a tecnologia de origem eh nula e a de destino eh nova
  }
  if(novaTecDestino.tamanho == 0 && tecRepetidaOrigem == 0){
    return 6; // retorna se a tecnologia de destino eh nula e a de origem eh nova
  }

  // trata os demais casos de retorno
  if (tecRepetidaOrigem == 1) {
    if (tecRepetidaDestino == 1) {
      return 1; // retorna se as tecnologias sao repetidas mas o par eh novo
    } else
      return 2; // retorna se a tecnologia de destino eh nova (consequentemente o par eh novo)
  }
  if (tecRepetidaDestino == 1) {
    return 3; // retorna se a tecnologia de origem eh nova (consequentemente o par eh novo)
  }
  return 4; // retorna se as tecnologias sao novas (consequentement o par eh novo)
}

void contaTecnologias(FILE* arquivoBinario, Registro registroAtual, Cabecalho* cabecalho) {
  /*
  Atualiza o cabecalho contando as tecnologias do arquivo. Essa funcao recebe um registro e compara as tecnologias com as existentes num arquivo binario. Caso encontre tecnologias ou par de tecnologia novos atualiza-se o contador.
  
  Essa funcao eh chamada na "criaTabela" do cabecalho funcoesBasicas.h
  */


  int novaTec = comparaTecnologias(arquivoBinario, registroAtual.TecnologiaOrigem, registroAtual.TecnologiaDestino);

  switch (novaTec) {
    case 0:
      break;
    case 1:
      cabecalho->nroParesTecnologias++;
      break;
    case 2:
      cabecalho->nroParesTecnologias++;
      cabecalho->nroTecnologias++;
      break;
    case 3:
      cabecalho->nroParesTecnologias++;
      cabecalho->nroTecnologias++;
      break;
    case 4:
      cabecalho->nroParesTecnologias++;
      cabecalho->nroTecnologias += 2;
      break;
    case 5:
      cabecalho->nroTecnologias++;
      break;
    case 6:
      cabecalho->nroTecnologias++;
      break;
  }
}

Registro leRegistro(FILE *arquivo, Registro *registro){
  /* 
  Essa função le o registro e lida com as suas questões gerais, como verificar se o mesmo existe, lidar com nulos, lidar com lixo e le os campos do registro, armazenando a leitura no registro auxiliar

  Essa funcao eh chamada nas funcoes "imprimeArquivo", "recuperaDados" e "recuperaRegistro" do cabecalho funcoesBasicas.h
  */

  // verifica se o registro existe
  if (registro->removido == '1') {
    fseek(arquivo, TAM_REGISTRO - 1, SEEK_CUR);
    return *registro;
  }

  // le os campos do registro e aramazena no registro auxiliar
  fread(&registro->grupo, sizeof(int), 1, arquivo);
  fread(&registro->popularidade, sizeof(int), 1, arquivo);
  fread(&registro->peso, sizeof(int), 1, arquivo);
  fread(&registro->TecnologiaOrigem.tamanho, sizeof(int), 1, arquivo);

  if (registro->TecnologiaOrigem.tamanho != 0) {
    registro->TecnologiaOrigem.string = malloc((registro->TecnologiaOrigem.tamanho + 1) * sizeof(char));
    if(registro->TecnologiaOrigem.string == NULL){
      printf("Falha no processamento do arquivo\n");
      fclose(arquivo);
      return *registro;
    }

    fread(registro->TecnologiaOrigem.string, sizeof(char), registro->TecnologiaOrigem.tamanho, arquivo);
    registro->TecnologiaOrigem.string[registro->TecnologiaOrigem.tamanho] = '\0';
  } else {
    registro->TecnologiaOrigem.string = strdup("NULO"); // imprime NULO se o campo da string for nulo
  }

  fread(&registro->TecnologiaDestino.tamanho, sizeof(int), 1, arquivo);

  if (registro->TecnologiaDestino.tamanho != 0) {
    registro->TecnologiaDestino.string = malloc((registro->TecnologiaDestino.tamanho + 1) * sizeof(char));
    if(registro->TecnologiaDestino.string == NULL){
      printf("Falha no processamento do arquivo\n");
      fclose(arquivo);
      return *registro;
    }

    fread(registro->TecnologiaDestino.string, sizeof(char), registro->TecnologiaDestino.tamanho, arquivo);
    registro->TecnologiaDestino.string[registro->TecnologiaDestino.tamanho] = '\0';
  } else {
    registro->TecnologiaDestino.string = strdup("NULO"); // imprime NULO se o campo da string for nulo
  }

  // verifica o a quantidade de lixo restante nesse registro e pula o ponteiro do arquivo para o proximo registro
  int tam_bytes =
      sizeof(registro->grupo) + sizeof(registro->popularidade) +
      sizeof(registro->peso) + sizeof(registro->TecnologiaOrigem.tamanho) +
      sizeof(registro->TecnologiaDestino.tamanho) + sizeof(char) +
      registro->TecnologiaOrigem.tamanho + registro->TecnologiaDestino.tamanho;
  fseek(arquivo, TAM_REGISTRO - tam_bytes, SEEK_CUR);


  return *registro;
}

// Função para comparar strings (usada para ordenação)
int compararStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Função para comparar arestas (usada para ordenação)
int compararArestas(const void *a, const void *b) {
    return strcmp(((Aresta*)a)->nomeTecDestino, ((Aresta*)b)->nomeTecDestino);
}
