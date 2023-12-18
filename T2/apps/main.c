#include "funcionalidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULO "-1"
#define LIXO "$"

#define REMOVIDO '1'
#define NAO_REMOVIDO '0'

int main() {
  char linha[500];
  char *token;
  char *strings[50];

  // le a linha
  fgets(linha, sizeof(linha), stdin);

  // remove \n da linha
  size_t length = strlen(linha);
  if (length > 0 && linha[length - 1] == '\n') {
      linha[length - 1] = '\0';
  }

  // separa a linha em strings a cada espaço
  token = strtok(linha, " ");

  int i = 0;
  // Armazena as strings no vetor
  while (token != NULL && i < sizeof(strings) / sizeof(strings[0])) {
      strings[i] = strdup(token);  // strdup aloca memória para a nova string
      token = strtok(NULL, " ");
      i++;
  }

  int funcionalidade = atoi(strings[0]); 

  // chama cada funcionalidade de acordo com a entrada no terminal
  switch (funcionalidade){
    case 8:
      recuperaDadosGrafo(strings[1], 0); // cria grafo (transposto = FALSE = 0)
      break;
    case 9:   
      recuperaDadosGrafo(strings[1], 1); // cria grafo transposto (transposto = TRUE = 1)
      break;
    case 10:   
      listaNomes(strings[1], atoi(strings[2]), strings);
      break;
    case 11:   
      fortementeConexo(strings[1], 0);
      break;
    case 12:   
      caminhoCurto(strings[1], atoi(strings[2]));
      break;
  }

  // Libera a memória alocada para as strings
  for (int j = 0; j < i; j++) {
      free(strings[j]);
  }

  return 0;
}