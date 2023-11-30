#include "funcoesFornecidas.h"
#include "funcionalidades.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULO "-1"
#define LIXO "$"

#define REMOVIDO '1'
#define NAO_REMOVIDO '0'

int main() {
  
  char *leitura1, *leitura2, *leitura3, *linha;
  int funcionalidade;

  // aloca memoria para as strings
  linha = (char *) malloc(300 * sizeof(char));
  leitura1 = (char *) malloc(100 * sizeof(char));
  leitura2 = (char *) malloc(100 * sizeof(char));
  leitura3 = (char *) malloc(100 * sizeof(char));

  if (linha == NULL || leitura1 == NULL || leitura2 == NULL) {
    printf("Erro na alocação de memória.\n");
    return 1;
  }

  // le uma linha de entrada do terminal
  fgets(linha, 200, stdin);
  // armazena os valores lidos nas suas respectivas variaveis
  sscanf(linha, "%d %s %s", &funcionalidade, leitura1, leitura2);

  // realoca memoria para as strings e libera o espaço desnecessario
  leitura1 = realloc(leitura1, sizeof(leitura1));
  leitura2 = realloc(leitura2, sizeof(leitura2));
  leitura3 = realloc(leitura3, sizeof(leitura3));
  free(linha);

  CabecalhoArvoreB cabecalho;

  // chama cada funcionalidade de acordo com a entrada no terminal
  switch (funcionalidade){
    case 1:
      criaTabela(leitura1, leitura2);
      leitura2 = (char*) leitura2;
      binarioNaTela(leitura2);
      break;
    case 2:
      imprimeArquivo(leitura1);
      break;
    case 3:
      busca(3, leitura1, "", atoi(leitura2));
      break;
    case 4:   
      recuperaRegistro(leitura1, atoi(leitura2));
      break;
    case 5:
      criaIndiceArvoreB(leitura1, leitura2);
      leitura2 = (char*) leitura2;
      binarioNaTela(leitura2);
      break;
    case 6:
      busca(6, leitura1, leitura2, atoi(leitura3));
      break;
    case 7:   
      // insereRegistros(leitura1, leitura2, atoi(leitura3));
      break;
    case 0:
      leitura1 = (char*) leitura1;
      binarioNaTela(leitura1);
      break;
    case 10:
      imprimeBinario(leitura1);
      break;
  }

  // libera a memoria alocada
  free(leitura1);
  free(leitura2);
  free(leitura3);

  return 0;
}