#include "arvoreB.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

FILE* abreIndiceEscrita(char* nomeIndice){
    FILE* indice = abreBinarioEscrita(nomeIndice);

    CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(nomeIndice);

    cabecalho.status = '0';
    escreveCabecalhoArvoreB(indice, cabecalho);

    return indice;
}

void fechaIndiceEscrita(FILE* indice, char* nomeIndice){
    CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(nomeIndice);

    cabecalho.status = '1';
    escreveCabecalhoArvoreB(indice, cabecalho);

    fclose(indice);
}

void escreveCabecalhoArvoreB(FILE *arquivo, CabecalhoArvoreB cabecalho) {
  /*
  Escreve o cabecalho de arquivo binario referente aos dados presentes no arquivo
  
  Essa funcao eh chamada na funcao "criaTabela" do cabecalho funcoesBasicas.h 
  */
  
  fseek(arquivo, 0, SEEK_SET); //inicia a escrita no começo do arquivo

  //escreve os dados do cabecalho
  fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
  fwrite(&cabecalho.noRaiz, sizeof(int), 1, arquivo);
  fwrite(&cabecalho.RRNproxNo, sizeof(int), 1, arquivo);
}

CabecalhoArvoreB leCabecalhoArvoreB(char* nomeArquivo) {
  /*
  Imprime o cabecalho de um arquivo binario
  
  Essa eh uma funcao auxiliar usada para verificacao dos valores do cabecalho e nao foi implementada nas funcoes principais do projeto
  */

  FILE* arquivo = abreBinarioLeitura(nomeArquivo);
  
  fseek(arquivo, 0, SEEK_SET); //inicia a leitura no começo do arquivo

  //armazena os dados do cabecalho em uma struct cabecalho e imprime seus valores
  CabecalhoArvoreB cabecalho;
  fread(&cabecalho.status, sizeof(char), 1, arquivo);
  fread(&cabecalho.noRaiz, sizeof(int), 1, arquivo);
  fread(&cabecalho.RRNproxNo, sizeof(int), 1, arquivo);

  fclose(arquivo);
  return cabecalho;
}

void printCabecalhoArvoreB(char *nomeArquivo){
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(nomeArquivo);
  printf("%c %d %d\n", cabecalho.status, cabecalho.noRaiz, cabecalho.RRNproxNo);
}

bool skipCabecalhoArvore(FILE *arquivoBinario){
  /*
  Essa funcao verifica se o arquivobinario esta consistente, se existem registros nele e pula para o byteoffset do primeiro registro

  Essa funcao eh chamada nas funcoes "imprimeArquivo" e "recuperaDados" do cabecalho funcoesBasicas.h

  retornos:
  1 - bem sucedido
  */
  
  // le o cabecalho do arquivo binario e verifica se o arquivo esta consistente
  char status;
  fread(&status, sizeof(char), 1, arquivoBinario);
  if(status == '0'){
    printf("Arquivo inconsistente.\n");
    exit(1);
  } 

  // verifica se ha registros
  int proxRRN;
  fseek(arquivoBinario, 1, SEEK_CUR); //skip noRaiz
  fread(&proxRRN, sizeof(int), 1, arquivoBinario);
  // if (proxRRN < 1) {
  //   return 0;
  // }
  
  return 1;
}

escrevePagina(Pagina pagina, int RRN, FILE* arquivoIndice){
  fseek(arquivoIndice, RRN * sizeof(Pagina), SEEK_SET);
  fwrite(&pagina, sizeof(Pagina), 1, arquivoIndice);
}


void imprimePagina(Pagina pagina) {
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

// Função auxiliar para inserir uma chave em um nó não cheio
void insereEmNoNaoCheio(Pagina *pagina, Chave chave, FILE *arquivoIndice) {
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
    // escrevePagina(*pagina, pagina->RRNdoNo, arquivoIndice);
}

// Função auxiliar para dividir um nó durante a inserção
void particionaNo(Pagina pagina, Chave chave, int RRNSuperior, FILE *arquivoIndice, char* nomeArquivoIndice) {

  // Cria um novo nó
  Pagina novaPagina;
  novaPagina.nroChavesNo = (ORDEM_ARVORE_B - 1) / 2;
  novaPagina.alturaNo = pagina.alturaNo;

  // Copia metade das chaves e RRNs para o novo nó
  for (int j = 0; j < novaPagina.nroChavesNo; j++) {
    novaPagina.chave[j] = pagina.chave[j + (ORDEM_ARVORE_B + 1) / 2];
  }

  // Atualiza a quantidade de chaves no nó original
  pagina.alturaNo = (ORDEM_ARVORE_B + 1) / 2 - 1;

  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(nomeArquivoIndice);

  // Atualiza o RRN do próximo nó no nó original
  novaPagina.RRNdoNo = cabecalho.RRNproxNo;

  // Atualiza o cabeçalho
  cabecalho.RRNproxNo++;
  escreveCabecalhoArvoreB(arquivoIndice, cabecalho);

  // Adiciona a chave no nó apropriado
  if (strcmp(chave.nome, novaPagina.chave[0].nome) < 0) {
      insereEmNoNaoCheio(&pagina, chave, arquivoIndice);
  } else {
      insereEmNoNaoCheio(&novaPagina, chave, arquivoIndice);
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
    insereEmNoNaoCheio(&pagina, chavePromovida, arquivoIndice);
    for (int i = 0; i < novaPagina.nroChavesNo - 1; i++) {
      novaPagina.chave[i] = novaPagina.chave[i + 1];
    }
      novaPagina.nroChavesNo--;
  }

  // Atualiza ponteiros
  chavePromovida.ponteiroanterior = RRNSuperior;
  chavePromovida.ponteiroproximo = novaPagina.RRNdoNo;

  // Insere chave promovida no nó pai
  insereNaArvoreB(chavePromovida, novaPagina.RRNdoNo, nomeArquivoIndice);

  // Atualiza o nó original no arquivo
  escrevePagina(pagina, RRNSuperior, arquivoIndice);

  // Escreve o novo nó no arquivo
  escrevePagina(novaPagina, novaPagina.RRNdoNo, arquivoIndice);

}

Pagina lePagina(FILE* arquivoIndice, int RRN){
  Pagina pagina;

  if(!skipCabecalhoArvore(arquivoIndice)){
    printf("Falha no processamento do arquivo.\n");
    exit(1);
  }

  fseek(arquivoIndice, RRN * TAM_PAGINA, SEEK_CUR);

  fread(&pagina.nroChavesNo, sizeof(int), 1, arquivoIndice);
  fread(&pagina.alturaNo, sizeof(int), 1, arquivoIndice);
  fread(&pagina.RRNdoNo, sizeof(int), 1, arquivoIndice);

  for(int i = 0; i < ORDEM_ARVORE_B - 1; i++){
    fread(&pagina.chave[i].ponteiroanterior, sizeof(int), 1, arquivoIndice);
    fread(&pagina.chave[i].nome, sizeof(char), TAM_CHAVE, arquivoIndice);
    fread(&pagina.chave[i].ref, sizeof(int), 1, arquivoIndice);
  }

  fread(&pagina.ponteirofinal, sizeof(int), 1, arquivoIndice);

  return pagina;
}

void criaPaginaNova(char* nomeArquivoIndice, int alturaNo, int ponteirofinal, Chave chave){
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(nomeArquivoIndice);
  
  Pagina pagina;
  pagina.nroChavesNo = 1;
  pagina.alturaNo = alturaNo;
  pagina.RRNdoNo = cabecalho.RRNproxNo;
  pagina.ponteirofinal = ponteirofinal;
  pagina.chave[0] = chave;

  FILE* arquivoIndice = abreIndiceEscrita(nomeArquivoIndice);
  fseek(arquivoIndice, cabecalho.RRNproxNo * TAM_PAGINA + TAM_CABECALHO_ARVORE, SEEK_SET);

  fwrite(&pagina.nroChavesNo, sizeof(int), 1, arquivoIndice);
  fwrite(&pagina.alturaNo, sizeof(int), 1, arquivoIndice);
  fwrite(&pagina.RRNdoNo, sizeof(int), 1, arquivoIndice);

  for(int i = 0; i < pagina.nroChavesNo; i++){
    fwrite(&pagina.chave[i-1].ponteiroanterior, sizeof(int), 1, arquivoIndice);
    fwrite(&pagina.chave[i-1].nome, sizeof(char), sizeof(pagina.chave[i-1].nome), arquivoIndice);
    fwrite("$", sizeof(char), TAM_CHAVE - sizeof(pagina.chave[i-1].nome), arquivoIndice);
    fwrite(&pagina.chave[i-1].ref, sizeof(int), 1, arquivoIndice);
  }

  for(int i = pagina.nroChavesNo; i < ORDEM_ARVORE_B; i++){
    fwrite("$", sizeof(char), TAM_PAGINA + 8, arquivoIndice);
  }

  fwrite(&pagina.ponteirofinal, sizeof(int), 1, arquivoIndice);

  cabecalho.RRNproxNo++;
  if(cabecalho.noRaiz == -1)
    cabecalho.noRaiz++;
  escreveCabecalhoArvoreB(arquivoIndice, cabecalho);

  fechaIndiceEscrita(arquivoIndice, nomeArquivoIndice);
}

// Função auxiliar para inserir uma chave na árvore-B
void insereNaArvoreB(Chave chave, int ponteirofinal, char* nomeArquivoIndice) {  
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(nomeArquivoIndice);

  if(cabecalho.status == '0'){
    printf("Arquivo inconsistente.\n");
    exit(-1);
  }
  
  if (cabecalho.noRaiz == -1){
    criaPaginaNova(nomeArquivoIndice, 1, ponteirofinal, chave);
    imprimePagina(lePagina(abreBinarioLeitura(nomeArquivoIndice), 0));
    return;
  }
  else{
    int RRNSuperior = 0;

    Pagina pagina = desceArvore(chave, cabecalho.noRaiz, &RRNSuperior, nomeArquivoIndice);
    FILE* arquivoIndice = abreIndiceEscrita(nomeArquivoIndice);
    insereNaArvoreBRecursivo(pagina, RRNSuperior, chave, arquivoIndice, nomeArquivoIndice);
    fechaIndiceEscrita(arquivoIndice, nomeArquivoIndice);
  }


  // int houveSplit = insereNaArvoreBRecursivo(chave, arquivoIndice);
  // int chavePromovida;
  // int RRNdoNovoNo;

  // if (houveSplit) {
  //     // Cria um novo nó raiz
  //     Pagina novaRaiz;
  //     novaRaiz.nroChavesNo = 1;
  //     novaRaiz.alturaNo = 2;

  //     // Armazena o RRN do próximo nó no campo RRNdoNo[1] da nova raiz
  //     // novaRaiz.RRNdoNo[0] = RRN;
  //     // novaRaiz.RRNdoNo[1] = RRNdoNovoNo;
  //     // novaRaiz.chave[0] = chavePromovida;

  //     // Atualiza o cabeçalho
  //     fseek(arquivoIndice, 0, SEEK_SET);
  //     CabecalhoArvoreB cabecalho;
  //     fread(&cabecalho, sizeof(CabecalhoArvoreB), 1, arquivoIndice);
  //     // cabecalho.noRaiz = proximoRRNNo(arquivoIndice);
  //     cabecalho.RRNproxNo++;
  //     fseek(arquivoIndice, 0, SEEK_SET);
  //     fwrite(&cabecalho, sizeof(CabecalhoArvoreB), 1, arquivoIndice);

  //     // Escreve a nova raiz no arquivo
  //     // fseek(arquivoIndice, novaRaiz.RRNdoNo[0] * sizeof(Pagina), SEEK_SET);
  //     fwrite(&novaRaiz, sizeof(Pagina), 1, arquivoIndice);
  // }
}

Pagina desceArvore(Chave chave, int RRNpagina, int *RRNSuperior, char* nomeArquivoIndice){
  FILE* arquivoIndice = abreBinarioLeitura(nomeArquivoIndice);

  Pagina pagina = lePagina(arquivoIndice, RRNpagina);
  
  if(pagina.alturaNo == 1){
    return pagina;
  }
  else{
    for(int i = 0; i < pagina.nroChavesNo; i++){
      if(strcmp(pagina.chave[i].nome, chave.nome) > 0){
        *RRNSuperior = RRNpagina;
        desceArvore(chave, pagina.chave[i].ponteiroanterior, RRNSuperior, nomeArquivoIndice);
      }
      else if(i == pagina.nroChavesNo - 1){
        *RRNSuperior = RRNpagina;
        desceArvore(chave, pagina.ponteirofinal, RRNSuperior, nomeArquivoIndice);
      }
    }
  }

  fclose(arquivoIndice);
}

// Função auxiliar para inserir uma chave na árvore-B recursivamente
void insereNaArvoreBRecursivo(Pagina pagina, int RRNSuperior, Chave chave, FILE *arquivoIndice, char* nomeArquivoIndice) {
    if(pagina.nroChavesNo < ORDEM_ARVORE_B - 1){
      insereEmNoNaoCheio(&pagina, chave, arquivoIndice);
      return;
    } else {
      particionaNo(pagina, chave, RRNSuperior, arquivoIndice, nomeArquivoIndice);
    }

    // Caso base: se o nó é uma folha
    // if (pagina.alturaNo == 1) {
    //     if (pagina.nroChavesNo < ORDEM_ARVORE_B - 1) {
    //         // Se há espaço no nó, insere a chave
    //         insereEmNoNaoCheio(&pagina, chave, -1, arquivoIndice);
    //         return 0;
    //     } else {
    //         // Se o nó está cheio, divide-o
    //         particionaNo(RRN, chave, RRNSuperior, arquivoIndice);
    //         return 1;
    //     }
    // } else {
        // Caso recursivo: desce na árvore
        // int houveSplit = insereNaArvoreBRecursivo(chave, pagina.RRNdoNo[i + 1], nivel + 1, chavePromovida, RRNdoNovoNo, arquivoIndice);

        // Se houve split no nível inferior, insere a chave promovida
        // if (houveSplit) {
        //     if (pagina.nroChavesNo < ORDEM_ARVORE_B - 1) {
        //         // Se há espaço no nó, insere a chave promovida
        //         insereEmNoNaoCheio(&pagina, *chavePromovida, RRN, *RRNdoNovoNo, arquivoIndice);
        //         return 0;
        //     } else {
        //         // Se o nó está cheio, divide-o
        //         particionaNo(RRN, i + 1, *chavePromovida, *RRNdoNovoNo, arquivoIndice);
        //         return 1;
        //     }
        // }

    //     return 0;
    // }
}

// Função para buscar um registro na árvore-B
int buscaArvoreB(FILE *arquivoIndice, int RRNpagina, char* chave) {
  if(RRNpagina == -1) //nao achou a chave num no folha
    return -1;

  Pagina pagina= lePagina(arquivoIndice, RRNpagina);

  for(int i = 0; i < pagina.nroChavesNo; i++){
    if(strcmp(pagina.chave[i].nome, chave) == 0)
      return pagina.chave[i].ref;
    else if(strcmp(pagina.chave[i].nome, chave) > 0)
      buscaArvoreB(arquivoIndice, pagina.chave[i].ponteiroanterior, chave);
    else
      if(i == ORDEM_ARVORE_B - 2)
        buscaArvoreB(arquivoIndice, pagina.ponteirofinal, chave);
  }
}