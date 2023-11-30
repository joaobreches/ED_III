#include "arvoreB.h"
#include "registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

  for(int i = 0; i < TAM_PAGINA_DISCO - sizeof(CabecalhoArvoreB); i++){
    fwrite("$", sizeof(char), 1, arquivo);
  }
}

CabecalhoArvoreB leCabecalhoArvoreB(FILE *arquivo) {
  /*
  Imprime o cabecalho de um arquivo binario
  
  Essa eh uma funcao auxiliar usada para verificacao dos valores do cabecalho e nao foi implementada nas funcoes principais do projeto
  */
  
  fseek(arquivo, 0, SEEK_SET); //inicia a leitura no começo do arquivo

  //armazena os dados do cabecalho em uma struct cabecalho e imprime seus valores
  CabecalhoArvoreB cabecalho;
  fread(&cabecalho.status, sizeof(char), 1, arquivo);
  fread(&cabecalho.noRaiz, sizeof(int), 1, arquivo);
  fread(&cabecalho.RRNproxNo, sizeof(int), 1, arquivo);
}

void printCabecalhoArvoreB(FILE *arquivo){
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(arquivo);
  printf("%c %d %d\n", cabecalho.status, cabecalho.noRaiz, cabecalho.RRNproxNo);
}

// void imprimePagina(Pagina registro) {
//   /*
//   Essa funcao imprime o registro no formato:
  
//   nomTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso

//   imprimindo os campos inteiros nulos (-1) como NULO

//   Essa funcao eh chamada nas funcoes "imprimeArquivo", "recuperaDados" e "recuperaRegistro" do cabecalho funcoesBasicas.h
//   */
  
//   // imprime o registro, se o campo for nulo (-1) imprime NULO
//   printf("%s, ", registro.nomeTecnologiaOrigem);
//   if(registro.grupo == -1)
//     printf("NULO, ");
//   else
//     printf("%d, ", registro.grupo);
//   if(registro.popularidade == -1)
//     printf("NULO, ");
//   else
//     printf("%d, ", registro.popularidade);
//   printf("%s, ", registro.nomeTecnologiaDestino);
//   if(registro.peso == -1)
//     printf("NULO\n");
//   else
//     printf("%d\n", registro.peso);
// }


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

void escrevePagina(Pagina pagina, FILE *arqIndice){
  fwrite(&pagina.nroChavesNo, sizeof(int), 1, arqIndice);
  fwrite(&pagina.alturaNo, sizeof(int), 1, arqIndice);
  fwrite(&pagina.RRNdoNo, sizeof(int), 1, arqIndice);

  int tamStrings = 0;
  for(int i = 0; i < pagina.nroChavesNo; i++){
    fwrite(&pagina.chave[i-1].ponteiroanterior, sizeof(int), 1, arqIndice);
    fwrite(&pagina.chave[i-1].nome, sizeof(char), sizeof(pagina.chave[i-1].nome), arqIndice);
    fwrite(&pagina.chave[i-1].ref, sizeof(int), 1, arqIndice);
    tamStrings += sizeof(pagina.chave[i-1].nome); 
  }

  fwrite(&pagina.ponteirofinal, sizeof(int), 1, arqIndice);
}

void criaPaginaNova(FILE *arquivoIndice, CabecalhoArvoreB *cabecalho, int alturaNo, int ponteirofinal, Chave chave){
  Pagina pagina;
  pagina.nroChavesNo = 1;
  pagina.alturaNo = alturaNo;
  pagina.RRNdoNo = cabecalho->RRNproxNo;
  pagina.ponteirofinal = ponteirofinal;
  pagina.chave[0] = chave;

  escrevePagina(pagina, arquivoIndice);

  cabecalho->RRNproxNo++;
}

// Função auxiliar para inserir uma chave na árvore-B
void insereNaArvoreB(Chave chave, char* nomeArquivoIndice) {
  // printf("its me hi\n");
  FILE* arquivoIndice = fopen(diretorioArquivo(nomeArquivoIndice, 'b'), "wb");
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(arquivoIndice);
  if(cabecalho.status == '0'){
    printf("Arquivo inconsistente.\n");
    exit(-1);
  }

  cabecalho.status = '1';
  escreveCabecalhoArvoreB(arquivoIndice, cabecalho);

  printf("O OCABECALHO RRN: %d\n", cabecalho.RRNproxNo);

  if (cabecalho.RRNproxNo == 0){
    printf("at tea time\n");
    chave.ponteiroanterior = -1;
    criaPaginaNova(arquivoIndice, &cabecalho, 0, -1, chave);
    printf("proxrrn cabecalho: %d\n", cabecalho.RRNproxNo);
    return;
  }

  int chavePromovida;
  int RRNdoNovoNo;
  int houveSplit = 0;
  // int houveSplit = insereNaArvoreBRecursivo(chave.ref, chave.ref, 0, &chavePromovida, &RRNdoNovoNo, arquivoIndice);

  if (houveSplit) {
      // Cria um novo nó raiz
      Pagina novaRaiz;
      novaRaiz.nroChavesNo = 1;
      novaRaiz.alturaNo = 2;

      // Armazena o RRN do próximo nó no campo RRNdoNo[1] da nova raiz
      // novaRaiz.RRNdoNo[0] = RRN;
      // novaRaiz.RRNdoNo[1] = RRNdoNovoNo;
      // novaRaiz.chave[0] = chavePromovida;

      // Atualiza o cabeçalho
      fseek(arquivoIndice, 0, SEEK_SET);
      CabecalhoArvoreB cabecalho;
      fread(&cabecalho, sizeof(CabecalhoArvoreB), 1, arquivoIndice);
      // cabecalho.noRaiz = proximoRRNNo(arquivoIndice);
      cabecalho.RRNproxNo++;
      fseek(arquivoIndice, 0, SEEK_SET);
      fwrite(&cabecalho, sizeof(CabecalhoArvoreB), 1, arquivoIndice);

      // Escreve a nova raiz no arquivo
      // fseek(arquivoIndice, novaRaiz.RRNdoNo[0] * sizeof(Pagina), SEEK_SET);
      fwrite(&novaRaiz, sizeof(Pagina), 1, arquivoIndice);
  }
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
int buscaArvoreB(FILE *arquivoIndice, int RRN, int chave) {
    Pagina no;
    fseek(arquivoIndice, RRN * sizeof(Pagina), SEEK_SET);
    fread(&no, sizeof(Pagina), 1, arquivoIndice);

    int i = 0;
    while (i < no.nroChavesNo && chave > no.chave[i]) {
        i++;
    }

    if (i < no.nroChavesNo && chave == no.chave[i]) {
        return no.RRNdoNo[i];
    }

    if (no.alturaNo == 1) {
        // Não encontrou a chave em um nó folha
        return -1;
    }

    Desce para o próximo nível da árvore-B
    return buscaArvoreB(arquivoIndice, no.RRNdoNo, chave);
}