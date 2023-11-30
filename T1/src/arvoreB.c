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
  0 - bem sucedido
  1 - arquivo inconsistente ou ausencia de registros
  */
  
  // le o cabecalho do arquivo binario e verifica se o arquivo esta consistente
  char status;
  fread(&status, sizeof(char), 1, arquivoBinario);
  if(status == '0'){
    printf("Falha no processamento do arquivo.\n");
    return 0;
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
// void insereEmNoNaoCheio(Pagina *no, int chave, int RRNdoNo, int RRNdoNovoNo, FILE *arquivoIndice) {
//     int i = no->nroChavesNo - 1;

//     // Encontra a posição correta para a nova chave
//     // while (i >= 0 && chave < no->chave[i]) {
//     //     no->chave[i + 1] = no->chave[i];
//     //     no->RRNdoNo[i + 2] = no->RRNdoNo[i + 1];
//     //     i--;
//     // }

//     // Insere a nova chave e atualiza os ponteiros dos filhos
//     // no->chave[i + 1] = chave;
//     // no->RRNdoNo[i + 2] = RRNdoNovoNo;
//     no->nroChavesNo++;

//     // Atualiza o nó no arquivo
//     fseek(arquivoIndice, RRNdoNo * sizeof(Pagina), SEEK_SET);
//     fwrite(no, sizeof(Pagina), 1, arquivoIndice);
// }



// // Função auxiliar para dividir um nó durante a inserção
// void particionaNo(int RRN, int i, int chave, int RRNdoNovoNo, FILE *arquivoIndice) {
//     // Lê o nó a ser dividido
//     fseek(arquivoIndice, RRN * sizeof(Pagina), SEEK_SET);
//     Pagina no;
//     fread(&no, sizeof(Pagina), 1, arquivoIndice);

//     // Cria um novo nó
//     Pagina novoNo;
//     novoNo.nroChavesNo = (ORDEM_ARVORE_B - 1) / 2;
//     novoNo.alturaNo = 1;

//     // Copia metade das chaves e RRNs para o novo nó
//     for (int j = 0; j < (ORDEM_ARVORE_B - 1) / 2; j++) {
//         // novoNo.chave[j] = no.chave[j + (ORDEM_ARVORE_B + 1) / 2];
//         // novoNo.RRNdoNo[j] = no.RRNdoNo[j + (ORDEM_ARVORE_B + 1) / 2];
//     }
    
//     // Atualiza a quantidade de chaves no nó original
//     no.nroChavesNo = (ORDEM_ARVORE_B - 1) / 2;

//     // Atualiza o RRN do próximo nó no nó original
//     // no.RRNdoNo[ORDEM_ARVORE_B / 2] = RRNdoNovoNo;

//     // Atualiza o cabeçalho
//     fseek(arquivoIndice, 0, SEEK_SET);
//     CabecalhoArvoreB cabecalho;
//     fread(&cabecalho, sizeof(CabecalhoArvoreB), 1, arquivoIndice);
//     cabecalho.RRNproxNo++;
//     fseek(arquivoIndice, 0, SEEK_SET);
//     fwrite(&cabecalho, sizeof(CabecalhoArvoreB), 1, arquivoIndice);

//     // Atualiza o nó original no arquivo
//     fseek(arquivoIndice, RRN * sizeof(Pagina), SEEK_SET);
//     fwrite(&no, sizeof(Pagina), 1, arquivoIndice);

//     // Escreve o novo nó no arquivo
//     fseek(arquivoIndice, RRNdoNovoNo * sizeof(Pagina), SEEK_SET);
//     fwrite(&novoNo, sizeof(Pagina), 1, arquivoIndice);
// }

// int proximoRRNNo(FILE *arquivoIndice) {
//     fseek(arquivoIndice, 0, SEEK_SET);
//     CabecalhoArvoreB cabecalho;
//     fread(&cabecalho, sizeof(CabecalhoArvoreB), 1, arquivoIndice);
//     return cabecalho.RRNproxNo;
// }

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

void criaPaginaNova(char* nomeArquivoIndice, CabecalhoArvoreB cabecalho, int alturaNo, int ponteirofinal, Chave chave){
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

    // for(int j = sizeof(pagina.chave[i-1].nome); i <= TAM_CHAVE; i++)
      fwrite("$", sizeof(char), TAM_CHAVE - sizeof(pagina.chave[i-1].nome), arquivoIndice);
    fwrite(&pagina.chave[i-1].ref, sizeof(int), 1, arquivoIndice);
  }

  fwrite(&pagina.ponteirofinal, sizeof(int), 1, arquivoIndice);

  cabecalho.RRNproxNo++;
  if(cabecalho.noRaiz == -1)
    cabecalho.noRaiz++;
  escreveCabecalhoArvoreB(arquivoIndice, cabecalho);

  fechaIndiceEscrita(arquivoIndice, nomeArquivoIndice);
}

// Função auxiliar para inserir uma chave na árvore-B
void insereNaArvoreB(Chave chave, char* nomeArquivoIndice) {
  // printf("its me hi\n");
  
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(nomeArquivoIndice);

  if(cabecalho.status == '0'){
    printf("Arquivo inconsistente.\n");
    exit(-1);
  }
  
  if (cabecalho.noRaiz == -1){
    criaPaginaNova(nomeArquivoIndice, cabecalho, 0, -1, chave);
    imprimePagina(lePagina(abreBinarioLeitura(nomeArquivoIndice), 0));
    return;
  }

  // int chavePromovida;
  // int RRNdoNovoNo;
  // int houveSplit = 0;
  // // int houveSplit = insereNaArvoreBRecursivo(chave.ref, chave.ref, 0, &chavePromovida, &RRNdoNovoNo, arquivoIndice);

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

// // Função auxiliar para inserir uma chave na árvore-B recursivamente
// int insereNaArvoreBRecursivo(int chave, int RRN, int nivel, int *chavePromovida, int *RRNdoNovoNo, FILE *arquivoIndice) {
//     fseek(arquivoIndice, RRN * sizeof(Pagina), SEEK_SET);
//     Pagina no;
//     fread(&no, sizeof(Pagina), 1, arquivoIndice);

//     int i = no.nroChavesNo - 1;

//     // Encontra a posição correta para a nova chave
//     // while (i >= 0 && chave < no.chave[i]) {
//     //     i--;
//     // }

//     // Caso base: se o nó é uma folha
//     if (no.alturaNo == 1) {
//         if (no.nroChavesNo < ORDEM_ARVORE_B - 1) {
//             // Se há espaço no nó, insere a chave
//             insereEmNoNaoCheio(&no, chave, RRN, -1, arquivoIndice);
//             return 0;
//         } else {
//             // Se o nó está cheio, divide-o
//             particionaNo(RRN, i + 1, chave, *RRNdoNovoNo, arquivoIndice);
//             return 1;
//         }
//     } else {
//         // Caso recursivo: desce na árvore
//         // int houveSplit = insereNaArvoreBRecursivo(chave, no.RRNdoNo[i + 1], nivel + 1, chavePromovida, RRNdoNovoNo, arquivoIndice);

//         // Se houve split no nível inferior, insere a chave promovida
//         // if (houveSplit) {
//         //     if (no.nroChavesNo < ORDEM_ARVORE_B - 1) {
//         //         // Se há espaço no nó, insere a chave promovida
//         //         insereEmNoNaoCheio(&no, *chavePromovida, RRN, *RRNdoNovoNo, arquivoIndice);
//         //         return 0;
//         //     } else {
//         //         // Se o nó está cheio, divide-o
//         //         particionaNo(RRN, i + 1, *chavePromovida, *RRNdoNovoNo, arquivoIndice);
//         //         return 1;
//         //     }
//         // }

//         return 0;
//     }
// }

// Função para buscar um registro na árvore-B
int buscaArvoreB(FILE *arquivoIndice, int RRNarvore, char* chave) {
  Pagina pagina;

  while(1){
    pagina = lePagina(arquivoIndice, RRNarvore);

    for(int i = 0; i < pagina.nroChavesNo; i++){
      if(strcmp(pagina.chave[i].nome, chave) == 0)
        return pagina.chave[i].ref;
      else if(strcmp(pagina.chave[i].nome, chave) > 0)
        RRNarvore = pagina.chave[i].ponteiroanterior;
      else
        if(i == ORDEM_ARVORE_B - 2)
          RRNarvore = pagina.ponteirofinal;
        else continue;

      if(RRNarvore == -1)
        return -1;
    }
  }
}