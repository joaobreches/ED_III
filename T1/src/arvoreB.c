#include "arvoreB.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Abre o aquivo de indice para escrita
FILE* abreIndiceEscrita(char* nomeIndice){
    FILE* indice = abreBinarioLeitura(nomeIndice);
    CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);
    fclose(indice);

    indice = abreBinarioEscrita(nomeIndice);
    cabecalho.status = '0';   //define o status como '0'
    escreveCabecalhoArvoreB(indice, cabecalho);

    return indice;
}

// Fecha o arquivo indice aberto para escrita
void fechaIndiceEscrita(FILE* indice){
    CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);

    cabecalho.status = '1'; // define o status como '1'
    escreveCabecalhoArvoreB(indice, cabecalho);

    fclose(indice);
}

// Escreve o cabecalho da arvore
void escreveCabecalhoArvoreB(FILE *indiceEscrita, CabecalhoArvoreB cabecalho) {
  /*
  Escreve o cabecalho de arquivo de indice de arvore b referente aos dados
  presentes no arquivo de indice
  */
  
  fseek(indiceEscrita, 0, SEEK_SET); //inicia a escrita no começo do arquivo

  //escreve os dados do cabecalho
  fwrite(&cabecalho.status, sizeof(char), 1, indiceEscrita);
  fwrite(&cabecalho.noRaiz, sizeof(int), 1, indiceEscrita);
  fwrite(&cabecalho.RRNproxNo, sizeof(int), 1, indiceEscrita);
  
  //completa os bytes para o tamanho do cabecalho com lixo '$'
  char lixo[TAM_CABECALHO_ARVORE - 9];
  for (int i = 0; i <= TAM_CABECALHO_ARVORE-9; i++){
    lixo[i] = '$';
  }
  
  fwrite(lixo, sizeof(char), TAM_CABECALHO_ARVORE - 9, indiceEscrita);
}

CabecalhoArvoreB leCabecalhoArvoreB(FILE* indiceLeitura) {
  /*
  Le o cabecalho de um arquivo de indice de arvore b
  */
  
  fseek(indiceLeitura, 0, SEEK_SET); //inicia a leitura no começo do arquivo

  //armazena os dados do cabecalho em uma struct cabecalho e retorna o cabecalho
  CabecalhoArvoreB cabecalho;
  fread(&cabecalho.status, sizeof(char), 1, indiceLeitura);
  fread(&cabecalho.noRaiz, sizeof(int), 1, indiceLeitura);
  fread(&cabecalho.RRNproxNo, sizeof(int), 1, indiceLeitura);

  return cabecalho;
}

void printCabecalhoArvoreB(FILE* indiceLeitura){
  /*
  Imprime o cabecalho de um arquivo de indice de arvore b
  
  Essa eh uma funcao auxiliar usada para verificacao dos valores
  do cabecalho e nao foi implementada nas funcoes principais do projeto
  */

  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indiceLeitura);
  printf("%c %d %d\n", cabecalho.status, cabecalho.noRaiz, cabecalho.RRNproxNo);
}

void escrevePagina(Pagina pagina, int RRN, FILE* indiceEscrita){
  // printf("ESCREVENDO PAGINA: %d, %d, %d\n", pagina.nroChavesNo, pagina.alturaNo, pagina.RRNdoNo);

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

  fseek(indiceLeitura, TAM_CABECALHO_ARVORE - 1, SEEK_CUR);
  fseek(indiceLeitura, RRN * TAM_PAGINA, SEEK_CUR);

  Pagina pagina;
  fread(&pagina.nroChavesNo, sizeof(int), 1, indiceLeitura);
  fread(&pagina.alturaNo, sizeof(int), 1, indiceLeitura);
  fread(&pagina.RRNdoNo, sizeof(int), 1, indiceLeitura);

  for(int i = 0; i < pagina.nroChavesNo; i++){
    fread(&pagina.chave[i].ponteiroanterior, sizeof(int), 1, indiceLeitura);
    pagina.chave[i].nome = (char*) malloc((TAM_CHAVE + 1) * sizeof(char));

    if(pagina.chave[i].nome == NULL){
      printf("Erro na alocacao de memoria.\n");
      exit(1);
    }

    fgets(pagina.chave[i].nome, TAM_CHAVE + 1, indiceLeitura);

    for(int j = 0; j < TAM_CHAVE; j++) {
      if(pagina.chave[i].nome[j] == '$') {
        pagina.chave[i].nome[j] = '\0';
        break;
      }
    }
    pagina.chave[i].nome[TAM_CHAVE] = '\0';

    fread(&pagina.chave[i].ref, sizeof(int), 1, indiceLeitura);

    if(i > 0)
      pagina.chave[i - 1].ponteiroproximo = pagina.chave[i].ponteiroanterior;
  }

  fread(&pagina.chave[pagina.nroChavesNo - 1].ponteiroproximo, sizeof(int), 1, indiceLeitura);
  
  if(pagina.nroChavesNo < 3){
    fseek(indiceLeitura, (ORDEM_ARVORE_B - pagina.nroChavesNo - 1) * (TAM_CHAVE + 8) - 4, SEEK_CUR);
    fread(&pagina.ponteirofinal, sizeof(int), 1, indiceLeitura);
    pagina.chave[pagina.nroChavesNo].ponteiroproximo = pagina.ponteirofinal;
  } else{
    pagina.ponteirofinal = pagina.chave[2].ponteiroproximo;
  }
  
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
  printf("nroChaves: %d, altura: %d, RRN: %d, ponteirofinal: %d -> ", pagina.nroChavesNo, pagina.alturaNo, pagina.RRNdoNo, pagina.ponteirofinal);
  for(int i = 0; i < pagina.nroChavesNo; i++)
    printf("ponteiroanterior %d, chave %d: %s, RRN: %d | ", pagina.chave[i].ponteiroanterior, i, pagina.chave[i].nome, pagina.chave[i].ref);
  printf("\b\b\n");
}

void criaPaginaNova(FILE* indice, int alturaNo, int ponteirofinal, Chave chave){
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);
  // printCabecalhoArvoreB(indice);
  
  Pagina pagina;
  pagina.nroChavesNo = 1;
  pagina.alturaNo = alturaNo;
  pagina.RRNdoNo = cabecalho.RRNproxNo;
  pagina.ponteirofinal = ponteirofinal;
  pagina.chave[0] = chave;

  escrevePagina(pagina, pagina.RRNdoNo, indice);

  cabecalho.noRaiz = pagina.RRNdoNo;
  cabecalho.RRNproxNo++;
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

    i++;

    // Insere a nova chave e atualiza os ponteiros dos filhos
    pagina->chave[i] = chave;

    if(i == pagina->nroChavesNo)
      pagina->ponteirofinal = pagina->chave[pagina->nroChavesNo].ponteiroproximo;
    else
      pagina->chave[i + 1].ponteiroanterior = pagina->chave[i].ponteiroproximo;

    pagina->nroChavesNo++;

    // Atualiza o nó no arquivo
    escrevePagina(*pagina, pagina->RRNdoNo, indice);
}

void clearUltimaChave(Pagina *pagina){
  pagina->chave[pagina->nroChavesNo - 1].ponteiroanterior = -1;
  pagina->chave[pagina->nroChavesNo - 1].ponteiroproximo = -1;
  pagina->chave[pagina->nroChavesNo - 1].nome = NULL;
  pagina->chave[pagina->nroChavesNo - 1].ref = -1;
  pagina->nroChavesNo--;
}

// Função auxiliar para dividir um nó durante a inserção
void particionaNo(Pagina pagina, Chave chave, int RRNSuperior, FILE *indice) {

  // Cria um novo nó
  Pagina novaPagina;
  novaPagina.nroChavesNo = (ORDEM_ARVORE_B - 1) / 2;
  novaPagina.alturaNo = pagina.alturaNo;
  novaPagina.ponteirofinal = pagina.ponteirofinal;

  // Copia metade das chaves e RRNs para o novo nó
  for (int j = 0; j < novaPagina.nroChavesNo; j++) {
    novaPagina.chave[j] = pagina.chave[j + (ORDEM_ARVORE_B + 1) / 2];
  }

  // Atualiza a quantidade de chaves no nó original
  pagina.nroChavesNo = ORDEM_ARVORE_B - 1 - novaPagina.nroChavesNo;

  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);

  // Atualiza o RRN do próximo nó no nó original
  novaPagina.RRNdoNo = cabecalho.RRNproxNo;

  // Atualiza o cabeçalho
  cabecalho.RRNproxNo++;
  escreveCabecalhoArvoreB(indice, cabecalho);

  bool inseriuEsquerdo = 0;

  // Adiciona a chave no nó apropriado
  if (strcmp(chave.nome, novaPagina.chave[0].nome) < 0) {
      insereEmNoNaoCheio(&pagina, chave, indice);
      inseriuEsquerdo = 1;
  } else {
      insereEmNoNaoCheio(&novaPagina, chave, indice);
  }

  // Chave a ser promovida
  Chave chavePromovida;

  // Se o nó original tiver mais chaves que o novo nó
  if (inseriuEsquerdo) {
    // Chave a ser promovida é a última do nó original
    chavePromovida = pagina.chave[pagina.nroChavesNo - 1];
    pagina.ponteirofinal = pagina.chave[pagina.nroChavesNo - 1].ponteiroanterior; 
    clearUltimaChave(&pagina);
    novaPagina.chave[0].ponteiroanterior = chavePromovida.ponteiroproximo;

  } else {
    // Chave a ser promovida é a primeira do novo nó
    chavePromovida = novaPagina.chave[0];
    for (int i = 0; i < novaPagina.nroChavesNo - 1; i++){
      novaPagina.chave[i] = novaPagina.chave[i + 1];
    }
    clearUltimaChave(&novaPagina);
    pagina.ponteirofinal = chavePromovida.ponteiroanterior;
    novaPagina.chave[0].ponteiroanterior = chavePromovida.ponteiroproximo;
  }

  escrevePagina(pagina, RRNSuperior, indice);
  escrevePagina(novaPagina, novaPagina.RRNdoNo, indice);

  // Atualiza ponteiros
  chavePromovida.ponteiroanterior = RRNSuperior;
  chavePromovida.ponteiroproximo = novaPagina.RRNdoNo;

  // Insere chave promovida no nó pai
  insereNaArvoreB(chavePromovida, chavePromovida.ponteiroproximo, pagina.alturaNo + 1, indice);
}

// Função auxiliar para inserir uma chave na árvore-B
void insereNaArvoreB(Chave chave, int ponteirofinal, int nivel, FILE* indice) {
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);
  cabecalho.status = '0';
  escreveCabecalhoArvoreB(indice, cabecalho);
    
  int alturaArvore = -1;
  if(cabecalho.noRaiz != -1){
    Pagina paginaRaiz = lePagina(indice, cabecalho.noRaiz);
    alturaArvore = paginaRaiz.alturaNo;
    for(int i = 0; i < paginaRaiz.nroChavesNo; i++)
      free(paginaRaiz.chave[i].nome);
  }

  if (alturaArvore < nivel){
    criaPaginaNova(indice, nivel, ponteirofinal, chave);
    return;
  }
  else{
    int RRNSuperior = cabecalho.noRaiz;

    Pagina pagina = desceArvore(chave, cabecalho.noRaiz, nivel, &RRNSuperior, indice);
    insereNaArvoreBRecursivo(pagina, RRNSuperior, chave, indice);

    for(int i = 0; i < pagina.nroChavesNo; i++)
      free(pagina.chave[i].nome);
  }

  cabecalho = leCabecalhoArvoreB(indice);
  cabecalho.status = '1';
  escreveCabecalhoArvoreB(indice, cabecalho);

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

Pagina desceArvore(Chave chave, int RRNpagina, int nivel, int *RRNSuperior, FILE *indice){
  Pagina pagina = lePagina(indice, RRNpagina);

  while(pagina.alturaNo != nivel){
    pagina = lePagina(indice, RRNpagina);

    for(int i = 0; i < pagina.nroChavesNo; i++){
      if(strcmp(pagina.chave[i].nome, chave.nome) > 0){
        *RRNSuperior = RRNpagina;
        RRNpagina = pagina.chave[i].ponteiroanterior;
        break;
      }
      else if(i == pagina.nroChavesNo - 1){
        *RRNSuperior = RRNpagina;
        RRNpagina = pagina.ponteirofinal;
        break;
      }
    }
  }

  return pagina;
}

// Função para buscar um registro na árvore-B
int buscaArvoreB(FILE *indice, int RRNpagina, char* chave, Pagina *pagina) {
  
  while (RRNpagina != -1) {

    *pagina = lePagina(indice, RRNpagina);
    int i;

    for (i = 0; i < pagina->nroChavesNo; i++) {
      if (strcmp(pagina->chave[i].nome, chave) == 0) {
        return pagina->chave[i].ref;
      } else if (strcmp(pagina->chave[i].nome, chave) > 0) {
        RRNpagina = pagina->chave[i].ponteiroanterior;
        break;
      } else if (i == pagina->nroChavesNo - 1) {
        RRNpagina = pagina->chave[i].ponteiroproximo;
        break;
      }
    }

    if (i == pagina->nroChavesNo) {
      RRNpagina = -1; // Não encontrou a chave no nó atual
    }
  }

  return -1; // Não encontrou a chave

}