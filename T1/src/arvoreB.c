#include "arvoreB.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

FILE* abreIndiceEscrita(char* nomeIndice){
    FILE* indice = abreBinarioLeitura(nomeIndice);
    CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);
    fclose(indice);

    indice = abreBinarioEscrita(nomeIndice);
    cabecalho.status = '0';
    escreveCabecalhoArvoreB(indice, cabecalho);

    return indice;
}

void fechaIndiceEscrita(FILE* indice){
    CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);

    cabecalho.status = '1';
    escreveCabecalhoArvoreB(indice, cabecalho);

    fclose(indice);
}

void escreveCabecalhoArvoreB(FILE *indiceEscrita, CabecalhoArvoreB cabecalho) {
  /*
  Escreve o cabecalho de arquivo binario referente aos dados presentes no arquivo
  
  Essa funcao eh chamada na funcao "criaTabela" do cabecalho funcoesBasicas.h 
  */
  
  fseek(indiceEscrita, 0, SEEK_SET); //inicia a escrita no começo do arquivo

  //escreve os dados do cabecalho
  fwrite(&cabecalho.status, sizeof(char), 1, indiceEscrita);
  fwrite(&cabecalho.noRaiz, sizeof(int), 1, indiceEscrita);
  fwrite(&cabecalho.RRNproxNo, sizeof(int), 1, indiceEscrita);
  
  char lixo[TAM_CABECALHO_ARVORE - 9];
  for (int i = 0; i <= TAM_CABECALHO_ARVORE-9; i++){
    lixo[i] = '$';
  }
  
  fwrite(lixo, sizeof(char), TAM_CABECALHO_ARVORE - 9, indiceEscrita);
}

CabecalhoArvoreB leCabecalhoArvoreB(FILE* indiceLeitura) {
  /*
  Imprime o cabecalho de um arquivo binario
  
  Essa eh uma funcao auxiliar usada para verificacao dos valores do cabecalho e nao foi implementada nas funcoes principais do projeto
  */
  
  fseek(indiceLeitura, 0, SEEK_SET); //inicia a leitura no começo do arquivo

  //armazena os dados do cabecalho em uma struct cabecalho e imprime seus valores
  CabecalhoArvoreB cabecalho;
  fread(&cabecalho.status, sizeof(char), 1, indiceLeitura);
  fread(&cabecalho.noRaiz, sizeof(int), 1, indiceLeitura);
  fread(&cabecalho.RRNproxNo, sizeof(int), 1, indiceLeitura);

  return cabecalho;
}

void printCabecalhoArvoreB(FILE* indiceLeitura){
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indiceLeitura);
  printf("%c %d %d\n", cabecalho.status, cabecalho.noRaiz, cabecalho.RRNproxNo);
}

bool skipCabecalhoArvore(FILE *indice){
  /*
  Essa funcao verifica se o arquivobinario esta consistente, se existem registros nele e pula para o byteoffset do primeiro registro

  Essa funcao eh chamada nas funcoes "imprimeArquivo" e "recuperaDados" do cabecalho funcoesBasicas.h

  retornos:
  1 - bem sucedido
  */
  
  fseek(indice, 0, SEEK_SET);

  // le o cabecalho do arquivo binario e verifica se o arquivo esta consistente
  char status;
  fread(&status, sizeof(char), 1, indice);
  if(status == '0'){
    printf("Arquivo inconsistente.\n");
    exit(1);
  } 

  // verifica se ha registros
  int proxRRN;
  fseek(indice, 1, SEEK_CUR); //skip noRaiz
  fread(&proxRRN, sizeof(int), 1, indice);
  // if (proxRRN < 1) {
  //   return 0;
  // }
  
  return 1;
}

void escrevePagina(Pagina pagina, int RRN, FILE* indiceEscrita){
  fseek(indiceEscrita, RRN * TAM_PAGINA + TAM_CABECALHO_ARVORE, SEEK_SET);

  fwrite(&(pagina.nroChavesNo), sizeof(int), 1, indiceEscrita);
  fwrite(&(pagina.alturaNo), sizeof(int), 1, indiceEscrita);
  fwrite(&(pagina.RRNdoNo), sizeof(int), 1, indiceEscrita);


  for(int i = 0; i < pagina.nroChavesNo; i++){
    fwrite(&(pagina.chave[i].ponteiroanterior), sizeof(int), 1, indiceEscrita);
    fwrite(pagina.chave[i].nome, sizeof(char), strlen(pagina.chave[i].nome), indiceEscrita);
    for(int j = 0; j < TAM_CHAVE - strlen(pagina.chave[i].nome); j++) {
      fwrite("$", sizeof(char), 1, indiceEscrita);
    }
    fwrite(&(pagina.chave[i].ref), sizeof(int), 1, indiceEscrita);
  }

  int menosUm = -1;
  for(int i = pagina.nroChavesNo; i < ORDEM_ARVORE_B - 1; i++){
    fwrite(&menosUm, sizeof(int), 1, indiceEscrita);
    for(int j = 0; j < TAM_CHAVE; j++){
      fwrite("$", sizeof(char), 1, indiceEscrita);
    }
    fwrite(&menosUm, sizeof(int), 1, indiceEscrita);
  }

  fwrite(&(pagina.ponteirofinal), sizeof(int), 1, indiceEscrita);
}

Pagina lePagina(FILE* indiceLeitura, int RRN){
  fseek(indiceLeitura, 0, SEEK_SET);

  char status;
  fread(&status, sizeof(char), 1, indiceLeitura);
  if(status == '0'){
    printf("Arquivo inconsistente.\n");
    exit(1);
  }
  fseek(indiceLeitura, TAM_CABECALHO_ARVORE - 1, SEEK_CUR);
  fseek(indiceLeitura, RRN * TAM_PAGINA, SEEK_CUR);

  Pagina pagina;
  fread(&pagina.nroChavesNo, sizeof(int), 1, indiceLeitura);
  fread(&pagina.alturaNo, sizeof(int), 1, indiceLeitura);
  fread(&pagina.RRNdoNo, sizeof(int), 1, indiceLeitura);

  for(int i = 0; i < pagina.nroChavesNo; i++){
    fread(&pagina.chave[i].ponteiroanterior, sizeof(int), 1, indiceLeitura);
    pagina.chave[i].nome = (char*) malloc(TAM_CHAVE * sizeof(char));
    if(pagina.chave[i].nome == NULL){
      printf("Erro na alocacao de memoria.\n");
      exit(1);
    }
    fgets(pagina.chave[i].nome, TAM_CHAVE + 1, indiceLeitura);

    for(int j = 0; j < TAM_CHAVE; j++){
      if(pagina.chave[i].nome[j] == '$'){
        pagina.chave[i].nome[j] = '\0';
        break;
      }
    }   

    fread(&pagina.chave[i].ref, sizeof(int), 1, indiceLeitura);
  }

  fseek(indiceLeitura, (ORDEM_ARVORE_B - pagina.nroChavesNo - 1) * (TAM_CHAVE + 8), SEEK_CUR);
  fread(&pagina.ponteirofinal, sizeof(int), 1, indiceLeitura);

  return pagina;
}

void imprimePagina(Pagina pagina){
  /*
  Essa funcao imprime o registro no formato:
  
  nomTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso

  imprimindo os campos inteiros nulos (-1) como NULO

  Essa funcao eh chamada nas funcoes "imprimeArquivo", "recuperaDados" e "recuperaRegistro" do cabecalho funcoesBasicas.h
  */
  
  // imprime o registro, se o campo for nulo (-1) imprime NULO
  printf("nroChaves: %d, altura: %d, RRN: %d, ponteirofinal: %d\n", pagina.nroChavesNo, pagina.alturaNo, pagina.RRNdoNo, pagina.ponteirofinal);
  for(int i = 0; i < pagina.nroChavesNo; i++)
    printf("ponteiroanterior %d, chave %d: %s, RRN: %d\n", pagina.chave[i].ponteiroanterior, i, pagina.chave[i].nome, pagina.chave[i].ref);
}

void criaPaginaNova(FILE* indice, int alturaNo, int ponteirofinal, Chave chave){
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);
  
  Pagina pagina;
  pagina.nroChavesNo = 1;
  pagina.alturaNo = alturaNo;
  pagina.RRNdoNo = cabecalho.RRNproxNo;
  pagina.ponteirofinal = ponteirofinal;
  pagina.chave[0] = chave;

  escrevePagina(pagina, pagina.RRNdoNo, indice);

  cabecalho.RRNproxNo++;
  if(cabecalho.noRaiz == -1)
    cabecalho.noRaiz++;
  escreveCabecalhoArvoreB(indice, cabecalho);
}

// Função auxiliar para inserir uma chave em um nó não cheio
void insereEmNoNaoCheio(Pagina *pagina, Chave chave, FILE *indice) {
    int i = pagina->nroChavesNo - 1;

    // Encontra a posição correta para a nova chave
    while (i >= 0 && strcmp(pagina->chave[i].nome, chave.nome) > 0) {
        pagina->chave[i + 1] = pagina->chave[i];
        i--;
    }

    // Insere a nova chave e atualiza os ponteiros dos filhos
    pagina->chave[i + 1] = chave;
    pagina->nroChavesNo++;

    // Atualiza o nó no arquivo
    escrevePagina(*pagina, pagina->RRNdoNo, indice);
}

// Função auxiliar para dividir um nó durante a inserção
void particionaNo(Pagina pagina, Chave chave, int RRNSuperior, FILE *indice) {

  // Cria um novo nó
  Pagina novaPagina;
  novaPagina.nroChavesNo = (ORDEM_ARVORE_B - 1) / 2;
  novaPagina.alturaNo = pagina.alturaNo;

  // Copia metade das chaves e RRNs para o novo nó
  for (int j = 0; j < novaPagina.nroChavesNo; j++) {
    novaPagina.chave[j] = pagina.chave[j + (ORDEM_ARVORE_B + 1) / 2];
  }

  // Atualiza a quantidade de chaves no nó original
  pagina.nroChavesNo = (ORDEM_ARVORE_B + 1) / 2 - 1;

  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);

  // Atualiza o RRN do próximo nó no nó original
  novaPagina.RRNdoNo = cabecalho.RRNproxNo;

  // Atualiza o cabeçalho
  cabecalho.RRNproxNo++;
  escreveCabecalhoArvoreB(indice, cabecalho);

  // Adiciona a chave no nó apropriado
  if (strcmp(chave.nome, novaPagina.chave[0].nome) < 0) {
      insereEmNoNaoCheio(&pagina, chave, indice);
  } else {
      insereEmNoNaoCheio(&novaPagina, chave, indice);
  }

  // Chave a ser promovida
  Chave chavePromovida;

  // Se o nó original tiver mais chaves que o novo nó
  if (pagina.nroChavesNo > novaPagina.nroChavesNo) {
    // Chave a ser promovida é a última do nó original
    chavePromovida = pagina.chave[pagina.nroChavesNo - 1];
    pagina.nroChavesNo--;
  } else {
    // Chave a ser promovida é a primeira do novo nó
    chavePromovida = novaPagina.chave[0];
    insereEmNoNaoCheio(&pagina, chavePromovida, indice);
    for (int i = 0; i < novaPagina.nroChavesNo - 1; i++) {
      novaPagina.chave[i] = novaPagina.chave[i + 1];
    }
      novaPagina.nroChavesNo--;
  }

  // Atualiza ponteiros
  chavePromovida.ponteiroanterior = RRNSuperior;
  chavePromovida.ponteiroproximo = novaPagina.RRNdoNo;

  // Insere chave promovida no nó pai
  insereNaArvoreB(chavePromovida, novaPagina.RRNdoNo, indice);

  // Atualiza o nó original no arquivo
  escrevePagina(pagina, RRNSuperior, indice);

  // Escreve o novo nó no arquivo
  escrevePagina(novaPagina, novaPagina.RRNdoNo, indice);

}

// Função auxiliar para inserir uma chave na árvore-B
void insereNaArvoreB(Chave chave, int ponteirofinal, FILE* indice) {  
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);

  // printf("its me hi ");
  // int n;
  // scanf("%d", &n);

  if(cabecalho.status == '0'){
    printf("Arquivo inconsistente.\n");
    exit(-1);
  }
  if (cabecalho.noRaiz == -1){
    criaPaginaNova(indice, 1, ponteirofinal, chave);
    return;
  }
  else{
    int RRNSuperior = 0;

    Pagina pagina = desceArvore(chave, cabecalho.noRaiz, &RRNSuperior, indice);
    insereNaArvoreBRecursivo(pagina, RRNSuperior, chave, indice);

    for(int i = 0; i < pagina.nroChavesNo; i++)
      free(pagina.chave[i].nome);
  }

}

// Função auxiliar para inserir uma chave na árvore-B recursivamente
void insereNaArvoreBRecursivo(Pagina pagina, int RRNSuperior, Chave chave, FILE *indice) {
    if(pagina.nroChavesNo < ORDEM_ARVORE_B - 1){
      insereEmNoNaoCheio(&pagina, chave, indice);
      return;
    } else {
      particionaNo(pagina, chave, RRNSuperior, indice);
    }

}

Pagina desceArvore(Chave chave, int RRNpagina, int *RRNSuperior, FILE *indice){
  Pagina pagina = lePagina(indice, RRNpagina);
  
  if(pagina.alturaNo == 1){
    return pagina;
  }
  else{
    for(int i = 0; i < pagina.nroChavesNo; i++){
      if(strcmp(pagina.chave[i].nome, chave.nome) > 0){
        *RRNSuperior = RRNpagina;
        desceArvore(chave, pagina.chave[i].ponteiroanterior, RRNSuperior, indice);
      }
      else if(i == pagina.nroChavesNo - 1){
        *RRNSuperior = RRNpagina;
        desceArvore(chave, pagina.ponteirofinal, RRNSuperior, indice);
      }
    }
  }
}

// Função para buscar um registro na árvore-B
int buscaArvoreB(FILE *indice, int RRNpagina, char* chave, Pagina pagina) {
  if(RRNpagina == -1) //nao achou a chave num no folha
    return -1;

  pagina = lePagina(indice, RRNpagina);

  for(int i = 0; i < pagina.nroChavesNo; i++){
    if(strcmp(pagina.chave[i].nome, chave) == 0)
      return pagina.chave[i].ref;
    else if(strcmp(pagina.chave[i].nome, chave) > 0)
      buscaArvoreB(indice, pagina.chave[i].ponteiroanterior, chave, pagina);
    else
      if(i == ORDEM_ARVORE_B - 2)
        buscaArvoreB(indice, pagina.ponteirofinal, chave, pagina);
  }
}