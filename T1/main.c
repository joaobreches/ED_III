#include "../include/funcoesFornecidas.h"
#include "../include/funcionalidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULO "-1"
#define LIXO "$"

#define REMOVIDO '1'
#define NAO_REMOVIDO '0'

int main() {
  
  char *leitura1, *leitura2, *linha;
  int funcionalidade;

  // aloca memoria para as strings
  linha = (char *) malloc(200 * sizeof(char));
  leitura1 = (char *) malloc(100 * sizeof(char));
  leitura2 = (char *) malloc(100 * sizeof(char));

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
  free(linha);

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
      recuperaDados(leitura1, atoi(leitura2));
      break;
    case 4:   
      recuperaRegistro(leitura1, atoi(leitura2));
      break;
  }

  // libera a memoria alocada
  free(leitura1);
  free(leitura2);

  return 0;
}