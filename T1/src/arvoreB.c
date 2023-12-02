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

void fechaIndiceEscrita(FILE* indiceLeitura, FILE* indiceEscrita){
    CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indiceLeitura);

    cabecalho.status = '1';
    escreveCabecalhoArvoreB(indiceEscrita, cabecalho);

    fclose(indiceEscrita);
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

  fwrite(&pagina.nroChavesNo, sizeof(int), 1, indiceEscrita);
  fwrite(&pagina.alturaNo, sizeof(int), 1, indiceEscrita);
  fwrite(&pagina.RRNdoNo, sizeof(int), 1, indiceEscrita);


  for(int i = 0; i < pagina.nroChavesNo; i++){
    fwrite(&pagina.chave[i].ponteiroanterior, sizeof(int), 1, indiceEscrita);
    fwrite(&pagina.chave[i].nome, sizeof(char), strlen(pagina.chave[i].nome), indiceEscrita);
    fwrite("$", sizeof(char), TAM_CHAVE - strlen(pagina.chave[i].nome), indiceEscrita);
    fwrite(&pagina.chave[i].ref, sizeof(int), 1, indiceEscrita);
  }

  // for(int i = pagina.nroChavesNo; i < ORDEM_ARVORE_B - 1; i++){
    // fwrite("$", sizeof(char), TAM_CHAVE + 8, indiceEscrita);
  // }
  fwrite(&pagina.ponteirofinal, sizeof(int), 1, indiceEscrita);
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

  for(int i = 0; i < ORDEM_ARVORE_B - 1; i++){
    fread(&pagina.chave[i].ponteiroanterior, sizeof(int), 1, indiceLeitura);
    
    // for(int j = 0; j < TAM_CHAVE; j++){
    //   if(pagina.chave[i].nome[j] == '$'){
    //     pagina.chave[i].nome[j] = '\0';
    //     break;
    //   }
    // }

    fread(&pagina.chave[i].ref, sizeof(int), 1, indiceLeitura);
  }

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
    printf("ponteiroanterior %d, chave %d: %s, RRN: %d;\n", pagina.chave[i].ponteiroanterior, i, pagina.chave[i].nome, pagina.chave[i].ref);
}

void criaPaginaNova(FILE* indiceLeitura, FILE* indiceEscrita, int alturaNo, int ponteirofinal, Chave chave){
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indiceLeitura);
  
  Pagina pagina;
  pagina.nroChavesNo = 1;
  pagina.alturaNo = alturaNo;
  pagina.RRNdoNo = cabecalho.RRNproxNo;
  pagina.ponteirofinal = ponteirofinal;
  pagina.chave[0] = chave;

  escrevePagina(pagina, pagina.RRNdoNo, indiceEscrita);

  cabecalho.RRNproxNo++;
  if(cabecalho.noRaiz == -1)
    cabecalho.noRaiz++;
  escreveCabecalhoArvoreB(indiceEscrita, cabecalho);
}

// Função auxiliar para inserir uma chave em um nó não cheio
void insereEmNoNaoCheio(Pagina *pagina, Chave chave, FILE *indiceEscrita) {
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
    escrevePagina(*pagina, pagina->RRNdoNo, indiceEscrita);
}

// Função auxiliar para dividir um nó durante a inserção
void particionaNo(Pagina pagina, Chave chave, int RRNSuperior, FILE *indiceLeitura, FILE *indiceEscrita) {

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

  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indiceLeitura);

  // Atualiza o RRN do próximo nó no nó original
  novaPagina.RRNdoNo = cabecalho.RRNproxNo;

  // Atualiza o cabeçalho
  cabecalho.RRNproxNo++;
  escreveCabecalhoArvoreB(indiceEscrita, cabecalho);

  // Adiciona a chave no nó apropriado
  if (strcmp(chave.nome, novaPagina.chave[0].nome) < 0) {
      insereEmNoNaoCheio(&pagina, chave, indiceEscrita);
  } else {
      insereEmNoNaoCheio(&novaPagina, chave, indiceEscrita);
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
    insereEmNoNaoCheio(&pagina, chavePromovida, indiceEscrita);
    for (int i = 0; i < novaPagina.nroChavesNo - 1; i++) {
      novaPagina.chave[i] = novaPagina.chave[i + 1];
    }
      novaPagina.nroChavesNo--;
  }

  // Atualiza ponteiros
  chavePromovida.ponteiroanterior = RRNSuperior;
  chavePromovida.ponteiroproximo = novaPagina.RRNdoNo;

  // Insere chave promovida no nó pai
  insereNaArvoreB(chavePromovida, novaPagina.RRNdoNo, indiceLeitura, indiceEscrita);

  // Atualiza o nó original no arquivo
  escrevePagina(pagina, RRNSuperior, indiceEscrita);

  // Escreve o novo nó no arquivo
  escrevePagina(novaPagina, novaPagina.RRNdoNo, indiceEscrita);

}

// Função auxiliar para inserir uma chave na árvore-B
void insereNaArvoreB(Chave chave, int ponteirofinal, FILE* indiceLeitura, FILE* indiceEscrita) {  
  printf("its me hi ");
  int n;
  scanf("%d", &n);

  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indiceLeitura);

  if(cabecalho.status == '0'){
    printf("Arquivo inconsistente.\n");
    exit(-1);
  }
  if (cabecalho.noRaiz == -1){
    criaPaginaNova(indiceLeitura, indiceEscrita, 1, ponteirofinal, chave);
    imprimePagina(lePagina(indiceLeitura, 0));

    return;
  }
  else{
    int RRNSuperior = 0;

    Pagina pagina = desceArvore(chave, cabecalho.noRaiz, &RRNSuperior, indiceLeitura);
    insereNaArvoreBRecursivo(pagina, RRNSuperior, chave, indiceLeitura, indiceEscrita);
  }

}

// Função auxiliar para inserir uma chave na árvore-B recursivamente
void insereNaArvoreBRecursivo(Pagina pagina, int RRNSuperior, Chave chave, FILE *indiceLeitura, FILE *indiceEscrita) {
    if(pagina.nroChavesNo < ORDEM_ARVORE_B - 1){
      insereEmNoNaoCheio(&pagina, chave, indiceEscrita);
      return;
    } else {
      particionaNo(pagina, chave, RRNSuperior, indiceLeitura, indiceEscrita);
    }

}

Pagina desceArvore(Chave chave, int RRNpagina, int *RRNSuperior, FILE *indiceLeitura){
  Pagina pagina = lePagina(indiceLeitura, RRNpagina);
  
  if(pagina.alturaNo == 1){
    return pagina;
  }
  else{
    for(int i = 0; i < pagina.nroChavesNo; i++){
      if(strcmp(pagina.chave[i].nome, chave.nome) > 0){
        *RRNSuperior = RRNpagina;
        desceArvore(chave, pagina.chave[i].ponteiroanterior, RRNSuperior, indiceLeitura);
      }
      else if(i == pagina.nroChavesNo - 1){
        *RRNSuperior = RRNpagina;
        desceArvore(chave, pagina.ponteirofinal, RRNSuperior, indiceLeitura);
      }
    }
  }
}

// Função para buscar um registro na árvore-B
int buscaArvoreB(FILE *indiceLeitura, int RRNpagina, char* chave) {
  if(RRNpagina == -1) //nao achou a chave num no folha
    return -1;

  Pagina pagina= lePagina(indiceLeitura, RRNpagina);

  for(int i = 0; i < pagina.nroChavesNo; i++){
    if(strcmp(pagina.chave[i].nome, chave) == 0)
      return pagina.chave[i].ref;
    else if(strcmp(pagina.chave[i].nome, chave) > 0)
      buscaArvoreB(indiceLeitura, pagina.chave[i].ponteiroanterior, chave);
    else
      if(i == ORDEM_ARVORE_B - 2)
        buscaArvoreB(indiceLeitura, pagina.ponteirofinal, chave);
  }
}