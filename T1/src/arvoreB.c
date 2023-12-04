#include "arvoreB.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

FILE* abreIndiceEscrita(char* nomeIndice){
  // Abre o aquivo de indice para escrita
  
  FILE* indice = abreBinarioLeitura(nomeIndice);
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);
  fclose(indice);

  indice = abreBinarioEscrita(nomeIndice);
  cabecalho.status = '0';   //define o status como '0'
  escreveCabecalhoArvoreB(indice, cabecalho);

  return indice;
}

void fechaIndiceEscrita(FILE* indice){
  // Fecha o arquivo indice aberto para escrita
  
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);

  cabecalho.status = '1'; // define o status como '1'
  escreveCabecalhoArvoreB(indice, cabecalho);

  fclose(indice);
}

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
  /*
  Funcao que escreve uma pagina em sua posicao correta no arquivo 
  de indice de arvore b de acordo com seu RRN
  */

  fseek(indiceEscrita, RRN * TAM_PAGINA + TAM_CABECALHO_ARVORE, SEEK_SET); // pula para o byte offset da pagina (RRN)

  // escreve cabecalho da pagina
  fwrite(&(pagina.nroChavesNo), sizeof(int), 1, indiceEscrita);
  fwrite(&(pagina.alturaNo), sizeof(int), 1, indiceEscrita);
  fwrite(&(pagina.RRNdoNo), sizeof(int), 1, indiceEscrita);

  // escreve cada uma das chaves que existem
  for(int i = 0; i < pagina.nroChavesNo; i++){
    fwrite(&(pagina.chave[i].ponteiroanterior), sizeof(int), 1, indiceEscrita);
    fwrite(pagina.chave[i].nome, sizeof(char), strlen(pagina.chave[i].nome), indiceEscrita);
    for(int j = 0; j < TAM_CHAVE - strlen(pagina.chave[i].nome); j++) {
      fwrite("$", sizeof(char), 1, indiceEscrita); // completa o tamanho do campo do nome da chave com lixo
    }
    fwrite(&(pagina.chave[i].ref), sizeof(int), 1, indiceEscrita);
  }

  // preenche as chaves vazias com lixo
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
  /*
  Le a pagina escrita em um arquivo de indice de arvore b de acordo com seu RRN
  */

  fseek(indiceLeitura, TAM_CABECALHO_ARVORE, SEEK_SET); // pula o byte offset do cabecalho
  fseek(indiceLeitura, RRN * TAM_PAGINA, SEEK_CUR); // pula para o byte offset da pagina que se quer ler

  // le o cabecalho da pagina da pagina
  Pagina pagina;
  fread(&pagina.nroChavesNo, sizeof(int), 1, indiceLeitura);
  fread(&pagina.alturaNo, sizeof(int), 1, indiceLeitura);
  fread(&pagina.RRNdoNo, sizeof(int), 1, indiceLeitura);

  // le as chaves existentes
  for(int i = 0; i < pagina.nroChavesNo; i++){
    fread(&pagina.chave[i].ponteiroanterior, sizeof(int), 1, indiceLeitura);
    pagina.chave[i].nome = (char*) malloc((TAM_CHAVE + 1) * sizeof(char));

    if(pagina.chave[i].nome == NULL){
      printf("Erro na alocacao de memoria.\n");
      exit(1);
    }

    fgets(pagina.chave[i].nome, TAM_CHAVE + 1, indiceLeitura);

    // finaliza a string do nome da chave, ignorando o lixo lido
    for(int j = 0; j < TAM_CHAVE; j++) {
      if(pagina.chave[i].nome[j] == '$') {
        pagina.chave[i].nome[j] = '\0';
        break;
      }
    }
    pagina.chave[i].nome[TAM_CHAVE] = '\0';

    fread(&pagina.chave[i].ref, sizeof(int), 1, indiceLeitura);

    // define os ponteiros seguintes das chaves (auxiliares)
    if(i > 0)
      pagina.chave[i - 1].ponteiroproximo = pagina.chave[i].ponteiroanterior;
  }

  fread(&pagina.chave[pagina.nroChavesNo - 1].ponteiroproximo, sizeof(int), 1, indiceLeitura);
  
  // define o ponteiro final da pagina
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
  Essa funcao imprime os dados de uma pagina fornecida.

  Essa eh uma funcao auxiliar usada para verificacao dos valores
  do cabecalho e nao foi implementada nas funcoes principais do projeto
  */
  
  printf("nroChaves: %d, altura: %d, RRN: %d, ponteirofinal: %d -> ", pagina.nroChavesNo, pagina.alturaNo, pagina.RRNdoNo, pagina.ponteirofinal);
  
  for(int i = 0; i < pagina.nroChavesNo; i++)
    printf("ponteiroanterior %d, chave %d: %s, RRN: %d | ", pagina.chave[i].ponteiroanterior, i, pagina.chave[i].nome, pagina.chave[i].ref);
  
  printf("\b\b\b\n");
}

void criaRaiz(FILE* indice, int alturaNo, int ponteirofinal, Chave chave){
  /*
  Essa funcao cria uma raiz nova para a arvore b. Ela eh chamada quando a arvore
  está vazia ou a chave eh promovida para alem da raiz atual da arvore
  */
  
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);
  
  // cria pagina, define seus valores e escreve-a no arquivo de indice
  Pagina pagina;
  pagina.nroChavesNo = 1;
  pagina.alturaNo = alturaNo;
  pagina.RRNdoNo = cabecalho.RRNproxNo;
  pagina.ponteirofinal = ponteirofinal;
  pagina.chave[0] = chave;

  escrevePagina(pagina, pagina.RRNdoNo, indice);

  // atualiza o cabecalho do arquivo de indice
  cabecalho.noRaiz = pagina.RRNdoNo;
  cabecalho.RRNproxNo++;
  escreveCabecalhoArvoreB(indice, cabecalho);
}

void insereEmNoNaoCheio(Pagina *pagina, Chave chave, FILE *indice) {
  /*
  Essa funcao adiciona uma nova chave em um no, desde que ele 
  nao esteja com sua capacidade maxima
  */
  
  int i = pagina->nroChavesNo - 1;

  // Encontra a posição correta para a nova chave
  while (i >= 0 && strcmp(pagina->chave[i].nome, chave.nome) > 0) {
      pagina->chave[i + 1] = pagina->chave[i];
      i--;
  }

  i++;

  // Insere a nova chave e atualiza os ponteiros
  pagina->chave[i] = chave;

  if(i == pagina->nroChavesNo)
    pagina->ponteirofinal = pagina->chave[pagina->nroChavesNo].ponteiroproximo;
  else
    pagina->chave[i + 1].ponteiroanterior = pagina->chave[i].ponteiroproximo;

  // atualiza cabecalho da pagina
  pagina->nroChavesNo++;

  // atualiza a pagina no arquivo de indice
  escrevePagina(*pagina, pagina->RRNdoNo, indice);
}

void clearUltimaChave(Pagina *pagina){
  // Limpa os valores da chave quando o ultimo elemento foi removido da pagina

  pagina->chave[pagina->nroChavesNo - 1].ponteiroanterior = -1;
  pagina->chave[pagina->nroChavesNo - 1].ponteiroproximo = -1;
  pagina->chave[pagina->nroChavesNo - 1].nome = NULL;
  pagina->chave[pagina->nroChavesNo - 1].ref = -1;
  pagina->nroChavesNo--;
}

void particionaNo(Pagina pagina, Chave chave, int RRNSuperior, FILE *indice) {
  // Função auxiliar para dividir um nó durante a inserção

  // Cria um novo nó e define seus valores
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

  // le os valores atuais do cabecalho do arquivo de indice
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);

  // Atualiza o RRN do próximo nó no nó original
  novaPagina.RRNdoNo = cabecalho.RRNproxNo;

  // Atualiza o cabeçalho
  cabecalho.RRNproxNo++;
  escreveCabecalhoArvoreB(indice, cabecalho);

  // verifica em qual no do split foi feita a insercao da nova chave para identificar qual chave sera promovida
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

  // atualiza paginas do split
  escrevePagina(pagina, RRNSuperior, indice);
  escrevePagina(novaPagina, novaPagina.RRNdoNo, indice);

  // Atualiza ponteiros
  chavePromovida.ponteiroanterior = RRNSuperior;
  chavePromovida.ponteiroproximo = novaPagina.RRNdoNo;

  // Insere chave promovida no nó pai
  insereNaArvoreB(chavePromovida, chavePromovida.ponteiroproximo, pagina.alturaNo + 1, indice);
}

void insereNaArvoreB(Chave chave, int ponteirofinal, int nivel, FILE* indice) {

  // printf("inserindo %s\n", chave.nome);

  // Função auxiliar para inserir uma chave na árvore-B
  
  CabecalhoArvoreB cabecalho = leCabecalhoArvoreB(indice);
  cabecalho.status = '0'; // define o status como inconsistente
  escreveCabecalhoArvoreB(indice, cabecalho);
    
  // verifica a altura atual da arvore
  int alturaArvore = -1; // altura para caso a arvore nao exista
  if(cabecalho.noRaiz != -1){
    Pagina paginaRaiz = lePagina(indice, cabecalho.noRaiz);
    alturaArvore = paginaRaiz.alturaNo;
    for(int i = 0; i < paginaRaiz.nroChavesNo; i++)
      free(paginaRaiz.chave[i].nome);
  }

  if(alturaArvore < nivel){   // chave sendo inserida sera uma nova raiz
    criaRaiz(indice, nivel, ponteirofinal, chave);
    return;
  }
  else{   // insercao padrao da chave
    // printf("no else\n");
    int RRNSuperior = cabecalho.noRaiz;

    Pagina pagina = desceArvore(chave, cabecalho.noRaiz, nivel, &RRNSuperior, indice); // encontra a pagina onde adicionar a chave
    
    // printf("a pagina que ele vai entrar eh: ");
    // imprimePagina(pagina);

    tipoInsercaoNaArvoreB(pagina, RRNSuperior, chave, indice); // insere a chave na pagina encontrada

    // libera memoria alocada
    for(int i = 0; i < pagina.nroChavesNo; i++)
      free(pagina.chave[i].nome);
  }

  // atualiza cabecalho
  cabecalho = leCabecalhoArvoreB(indice);
  cabecalho.status = '1'; // define status como consistente
  escreveCabecalhoArvoreB(indice, cabecalho);

  // printf("terminou a insercao\n");
}

void tipoInsercaoNaArvoreB(Pagina pagina, int RRNSuperior, Chave chave, FILE *indice) {
  /*
  Função auxiliar que define o tipo de insercao na arvore b de 
  acordo com a quantidade de chaves na pagina definida
  */

  if(pagina.nroChavesNo < ORDEM_ARVORE_B - 1){
    insereEmNoNaoCheio(&pagina, chave, indice);
    return;
  } else {
    particionaNo(pagina, chave, RRNSuperior, indice);
  }
}

Pagina desceArvore(Chave chave, int RRNpagina, int nivel, int *RRNSuperior, FILE *indice){
  /*
  Funcao que encontra em qual pagina a chave deve ser inserida de acordo com a altura (nivel) definido
  O nivel inicialmente eh 1 (folha), mas vai aumentando a medida que a chave eh promovida
  */

  Pagina pagina = lePagina(indice, RRNpagina);

  // printf("INICIO descendo: nivel %d, altura %d\n", nivel, pagina.alturaNo);

  while(pagina.alturaNo != nivel && RRNpagina != -1){
    pagina = lePagina(indice, RRNpagina);
  
    // printf("WHILE descendo: nivel %d, altura %d\nA pagina lida foi: ", nivel, pagina.alturaNo);
    // imprimePagina(pagina);

    // verifica para qual pagina (de acordo com o ponteiro/RRN) deve seguir ate chegar na altura desejada
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

int buscaArvoreB(FILE *indice, int RRNpagina, char* chave, Pagina *pagina) {
  // Função para buscar um registro na árvore-B
  
  while (RRNpagina != -1) {

    *pagina = lePagina(indice, RRNpagina);
    int i;

    // verifica se a chave esta no no atual, se nao desce para o ponteiro (RRN) correspondente
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