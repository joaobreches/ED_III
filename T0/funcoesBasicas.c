#include "funcoesBasicas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define TAM_REGISTRO_VARIAVEL 55
#define TAM_CABECALHO 13

typedef struct _Cabecalho {
  char status; // 0 para arquivo inconsistente e 1 para arquivo consistente
  int proxRRN;
  int nroTecnologias;
  int nroParesTecnologias;
} Cabecalho;

typedef struct _StringVariavel {
  int tamanho;
  char *string;
} StringVariavel;

typedef struct _Registro {
  char removido;

  int grupo;
  int popularidade;
  int peso;

  StringVariavel TecnologiaOrigem;
  StringVariavel TecnologiaDestino;
} Registro;

void escreveCabecalho(FILE *arquivo, Cabecalho cabecalho) {
  /*
  Escreve o cabecalho de  arquivo binario referente aos dados presentes no arquivo
  
  Essa funcao eh chamada na funcao "criaTabela" do cabecalho funcoesBasicas.h 
  */
  
  fseek(arquivo, 0, SEEK_SET); //inicia a escrita no começo do arquivo

  //escreve os dados do cabecalho
  fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
  fwrite(&cabecalho.proxRRN, sizeof(int), 1, arquivo);
  fwrite(&cabecalho.nroTecnologias, sizeof(int), 1, arquivo);
  fwrite(&cabecalho.nroParesTecnologias, sizeof(int), 1, arquivo);
}

void printCabecalho(FILE *arquivo) {
  /*
  Imprime o cabecalho de um arquivo binario
  
  Essa eh uma funcao auxiliar usada para verificacao dos valores do cabecalho e nao foi implementada nas funcoes principais do projeto
  */
  
  fseek(arquivo, 0, SEEK_SET); //inicia a leitura no começo do arquivo

  //armazena os dados do cabecalho em uma struct cabecalho e imprime seus valores
  Cabecalho cabecalho;
  fread(&cabecalho.status, sizeof(char), 1, arquivo);
  fread(&cabecalho.proxRRN, sizeof(int), 1, arquivo);
  fread(&cabecalho.nroTecnologias, sizeof(int), 1, arquivo);
  fread(&cabecalho.nroParesTecnologias, sizeof(int), 1, arquivo);
  printf("%c %d %d %d\n", cabecalho.status, cabecalho.proxRRN, cabecalho.nroTecnologias, cabecalho.nroParesTecnologias);
}

int skipCabecalho(FILE *arquivoBinario){
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
    return 1;
  } 

  // verifica se ha registros
  int proxRRN;
  fread(&proxRRN, sizeof(int), 1, arquivoBinario);
  if (proxRRN < 1) {
    printf("Registro inexistente.\n");
    return 1;
  }

  // pula para o byteoffsset do primeiro registro
  fseek(arquivoBinario, 8, SEEK_CUR);
  
  return 0;
}

void imprimeRegistro(Registro registro) {
  /*
  Essa funcao imprime o registro no formato:
  
  nomTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso

  imprimindo os campos inteiros nulos (-1) como NULO

  Essa funcao eh chamada nas funcoes "imprimeArquivo", "recuperaDados" e "recuperaRegistro" do cabecalho funcoesBasicas.h
  */
  
  // imprime o registro, se o campo for nulo (-1) imprime NULO
  printf("%s, ", registro.TecnologiaOrigem.string);
  if(registro.grupo == -1)
    printf("NULO, ");
  else
    printf("%d, ", registro.grupo);
  if(registro.popularidade == -1)
    printf("NULO, ");
  else
    printf("%d, ", registro.popularidade);
  printf("%s, ", registro.TecnologiaDestino.string);
  if(registro.peso == -1)
    printf("NULO\n");
  else
    printf("%d\n", registro.peso);
}

int comparaTecnologias(FILE *arquivoBinario, StringVariavel novaTecOrigem, StringVariavel novaTecDestino) {
  /*
  Compara tecnologias e par de tecnologias com os demais dados de um arquivo binario. O inteiro retornado indica se ha tecnologias e/ou pares de tecnologias novos  
  
  Essa funcao eh chamada na funcao "contaTecnologias" do cabecalho funcoesBasicas.h 
  
  retornos:

  0: nada novo;
  1: apenas par novo;
  2: apenas destino e par novos;
  3: apenas origem e par novos;
  4: origem, destino e par novos;
  5: destino novo e origem nulo;
  6: origem nova e destino nulo.
  */

  // cria um registro auxiliar e define inteiros que indicam se as tecnologias atuais ja foram encontradas (0 como padrao e 1 para se encontrada)
  Registro registro;
  int tecRepetidaOrigem = 0;
  int tecRepetidaDestino = 0;

  // inicia a leitura do arquivo pulando o cabecalho
  if (fseek(arquivoBinario, TAM_CABECALHO, SEEK_SET) != 0) {
    printf("Falha no processamento do arquivo.\n");
    fclose(arquivoBinario);
    return -1;
  }

  // le o arquivo ate o final
  while (1) {
    if (fread(&registro.removido, sizeof(char), 1, arquivoBinario) == 0) {
      break;
    }

    // verifica se o registro foi removido e passa para a proxima interacao se verdadeiro
    if (registro.removido == '1') {
      fseek(arquivoBinario, TAM_REGISTRO - 1, SEEK_CUR);
      continue;
    }

    // os primeiros campos do registro (grupo, popularidade e peso) e vai ate os campos das tecnologias
    fseek(arquivoBinario, 12, SEEK_CUR);

    // le a tecnologia de origem e armazena no registro auxiliar
    fread(&registro.TecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBinario);

    registro.TecnologiaOrigem.string = malloc((registro.TecnologiaOrigem.tamanho + 1) * sizeof(char));
    fread(registro.TecnologiaOrigem.string, sizeof(char), registro.TecnologiaOrigem.tamanho, arquivoBinario);

    // le a tecnologia de destino e armazena no registro auxiliar
    fread(&registro.TecnologiaDestino.tamanho, sizeof(int), 1, arquivoBinario);

    registro.TecnologiaDestino.string = malloc((registro.TecnologiaDestino.tamanho + 1) * sizeof(char));
    fread(registro.TecnologiaDestino.string, sizeof(char), registro.TecnologiaDestino.tamanho, arquivoBinario);

    // pula o lixo do registro com o ponteiro do arquivo 
    int TAM_BITES = sizeof(registro.grupo) + sizeof(registro.popularidade) + sizeof(registro.peso) + sizeof(registro.TecnologiaOrigem.tamanho) + sizeof(registro.TecnologiaDestino.tamanho) + sizeof(char) + registro.TecnologiaOrigem.tamanho + registro.TecnologiaDestino.tamanho;
    fseek(arquivoBinario, TAM_REGISTRO - TAM_BITES + 1, SEEK_CUR);

    // finaliza as strings das tecnologias com '\0'
    registro.TecnologiaOrigem.string[registro.TecnologiaOrigem.tamanho] = '\0';
    registro.TecnologiaDestino.string[registro.TecnologiaDestino.tamanho] = '\0';

    // compara as tecnologias lidas anteriormente com as tecnologias novas fornecidas como parametro da funcao
    if (strcmp(registro.TecnologiaOrigem.string, novaTecOrigem.string) == 0) {
      if (strcmp(registro.TecnologiaDestino.string, novaTecDestino.string) == 0) {
        return 0; // retorna se as tecnologias de origem e destino sao as mesmas e o par eh o mesmo tambem
      }
      tecRepetidaOrigem = 1; // tecnologia de origem eh a mesma
    }
    if (tecRepetidaDestino == 0 && strcmp(registro.TecnologiaDestino.string, novaTecDestino.string) == 0) {
      tecRepetidaDestino = 1; // tecnologia de destino eh a mesma
    }
    if (tecRepetidaDestino == 0 && strcmp(registro.TecnologiaOrigem.string, novaTecDestino.string) == 0) {
      tecRepetidaDestino = 1; // nova tecnologia de destino eh a mesma da tecnologia de origem lida
    }
    if (tecRepetidaOrigem == 0 && strcmp(registro.TecnologiaDestino.string, novaTecOrigem.string) == 0) {
      tecRepetidaOrigem = 1; // nova tecnologia de origem eh a mesma da tecnologia de destino lida
    }

    // libera a memoria alocada pelo registro auxiliar
    free(registro.TecnologiaOrigem.string);
    free(registro.TecnologiaDestino.string);
  }

  // tratamento de campos nulos
  if((novaTecOrigem.tamanho == 0 && novaTecDestino.tamanho == 0) || (novaTecOrigem.tamanho == 0 && tecRepetidaDestino == 1) || (novaTecDestino.tamanho == 0 && tecRepetidaOrigem == 1)){
    return 0; // retorna se os dois sao nulos ou um eh nulo e o outro eh repetido  
  }
  if(novaTecOrigem.tamanho == 0 && tecRepetidaDestino == 0){
    return 5; // retorna se a tecnologia de origem eh nula e a de destino eh nova
  }
  if(novaTecDestino.tamanho == 0 && tecRepetidaOrigem == 0){
    return 6; // retorna se a tecnologia de destino eh nula e a de origem eh nova
  }

  // trata os demais casos de retorno
  if (tecRepetidaOrigem == 1) {
    if (tecRepetidaDestino == 1) {
      return 1; // retorna se as tecnologias sao repetidas mas o par eh novo
    } else
      return 2; // retorna se a tecnologia de destino eh nova (consequentemente o par eh novo)
  }
  if (tecRepetidaDestino == 1) {
    return 3; // retorna se a tecnologia de origem eh nova (consequentemente o par eh novo)
  }
  return 4; // retorna se as tecnologias sao novas (consequentement o par eh novo)
}

void contaTecnologias(FILE* arquivoBinario, Registro registroAtual, Cabecalho* cabecalho) {
  /*
  Atualiza o cabecalho contando as tecnologias do arquivo. Essa funcao recebe um registro e compara as tecnologias com as existentes num arquivo binario. Caso encontre tecnologias ou par de tecnologia novos atualiza-se o contador.
  
  Essa funcao eh chamada na "criaTabela" do cabecalho funcoesBasicas.h
  */
  
  int novaTec = comparaTecnologias(arquivoBinario, registroAtual.TecnologiaOrigem, registroAtual.TecnologiaDestino);

  switch (novaTec) {
    case 0:
      break;
    case 1:
      cabecalho->nroParesTecnologias++;
      break;
    case 2:
      cabecalho->nroParesTecnologias++;
      cabecalho->nroTecnologias++;
      break;
    case 3:
      cabecalho->nroParesTecnologias++;
      cabecalho->nroTecnologias++;
      break;
    case 4:
      cabecalho->nroParesTecnologias++;
      cabecalho->nroTecnologias += 2;
      break;
    case 5:
      cabecalho->nroTecnologias++;
      break;
    case 6:
      cabecalho->nroTecnologias++;
      break;
  }
}

Registro trabalhaRegistros(FILE *arquivo, Registro *registro){
  /* 
  Essa função le o registro e lida com as suas questões gerais, como verificar se o mesmo existe, lidar com nulos, lidar com lixo e le os campos do registro, armazenando a leitura no registro auxiliar

  Essa funcao eh chamada nas funcoes "imprimeArquivo", "recuperaDados" e "recuperaRegistro" do cabecalho funcoesBasicas.h
  */

  // verifica se o registro existe
  if (registro->removido == '1') {
    fseek(arquivo, TAM_REGISTRO - 1, SEEK_CUR);
    return *registro;
  }

  // le os campos do registro e aramazena no registro auxiliar
  fread(&registro->grupo, sizeof(int), 1, arquivo);
  fread(&registro->popularidade, sizeof(int), 1, arquivo);
  fread(&registro->peso, sizeof(int), 1, arquivo);
  fread(&registro->TecnologiaOrigem.tamanho, sizeof(int), 1, arquivo);

  if (registro->TecnologiaOrigem.tamanho != 0) {
    registro->TecnologiaOrigem.string = malloc((registro->TecnologiaOrigem.tamanho + 1) * sizeof(char));
    if(registro->TecnologiaOrigem.string == NULL){
      printf("Falha no processamento do arquivo\n");
      fclose(arquivo);
      return *registro;
    }

    fread(registro->TecnologiaOrigem.string, sizeof(char), registro->TecnologiaOrigem.tamanho, arquivo);
    registro->TecnologiaOrigem.string[registro->TecnologiaOrigem.tamanho] = '\0';
  } else {
    registro->TecnologiaOrigem.string = strdup("NULO"); // imprime NULO se o campo da string for nulo
  }

  fread(&registro->TecnologiaDestino.tamanho, sizeof(int), 1, arquivo);

  if (registro->TecnologiaDestino.tamanho != 0) {
    registro->TecnologiaDestino.string = malloc((registro->TecnologiaDestino.tamanho + 1) * sizeof(char));
    if(registro->TecnologiaDestino.string == NULL){
      printf("Falha no processamento do arquivo\n");
      fclose(arquivo);
      return *registro;
    }

    fread(registro->TecnologiaDestino.string, sizeof(char), registro->TecnologiaDestino.tamanho, arquivo);
    registro->TecnologiaDestino.string[registro->TecnologiaDestino.tamanho] = '\0';
  } else {
    registro->TecnologiaDestino.string = strdup("NULO"); // imprime NULO se o campo da string for nulo
  }

  // verifica o a quantidade de lixo restante nesse registro e pula o ponteiro do arquivo para o proximo registro
  int tam_bytes =
      sizeof(registro->grupo) + sizeof(registro->popularidade) +
      sizeof(registro->peso) + sizeof(registro->TecnologiaOrigem.tamanho) +
      sizeof(registro->TecnologiaDestino.tamanho) + sizeof(char) +
      registro->TecnologiaOrigem.tamanho + registro->TecnologiaDestino.tamanho;
  fseek(arquivo, TAM_REGISTRO - tam_bytes, SEEK_CUR);


  return *registro;
}

void criaTabela(char* nomeArquivoCSV, char* nomeArquivoBinario) {
  /*
  Essa funcao le um arquivo csv e escreve os registros com campo fixo e variavel em um arquivo binario. O arquivo inicialmente possui um cabecalho que indica o status do arquivo binario, o indice do proximo registro, o numero de tecnologias distintas presentes e o numero de pares de tecnologias.

  Essa funcao representa a funcionalidade 1 do exercicio introdutorio.
  */

  // abre arquivo csv
  FILE* arquivoCSV = fopen(nomeArquivoCSV, "r");
  if (arquivoCSV == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // abre arquivo binario
  FILE *arquivoBinario = fopen(nomeArquivoBinario, "wb+");
  if (arquivoBinario == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // inicializa o cabecalho do arquivo binario
  Cabecalho cabecalho;
  cabecalho.status = '0';
  cabecalho.proxRRN = 0;
  cabecalho.nroTecnologias = 0;
  cabecalho.nroParesTecnologias = 0;
  escreveCabecalho(arquivoBinario, cabecalho);

  // aloca memoria para leitura de uma linha
  char *linha = malloc(sizeof(char) * TAM_REGISTRO);
  if (linha == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // inicializa registro auxiliar para armazenar os valores lidos no csv e colunas para ler o que cada coluna do csv representa
  Registro registroAtual;
  char *colunas[5];

  // le a primeira linha e separa os tokens por virgula, armazena em no vetor de string coluna
  fgets(linha, TAM_REGISTRO, arquivoCSV);
  char *col = strtok(linha, ",");

  for (int i = 0; i < 5; i++) {
    if (i == 4)
      col[strlen(col) - 2] = '\0';
    colunas[i] = strdup(col);
    col = strtok(NULL, ",");
  }

  // le o arquivo csv ate o fim e escreve no arquivo binario fazendo os devidos tratamentos
  while (1) {
    // le o arquivo csv ate o fim
    if (fgets(linha, TAM_REGISTRO, arquivoCSV) == NULL) {
      break;
    }
    else {
      fseek(arquivoCSV, (-1)*strlen(linha), SEEK_CUR);
    }

    // define o registro atual como nao removido
    registroAtual.removido = '0';

    // le os cinco campos do registro char a char para tratar campos nulos
    for (int i = 0; i < 5; i++) {

      char c;
      char *campo = malloc(sizeof(char) * 1);
      int j = 0;

      // le cada char ate encontrar o uma virgula, fim da linha ou fim do arquivo
      do {
        c = fgetc(arquivoCSV);
        campo[j++] = c;
        campo = realloc(campo, sizeof(char) * (j + 1));
      } while (c != ',' && c != '\n' && c != EOF);

      // campo = realloc(campo, sizeof(char) * (j - 1));
      // finaliza a string do campo com '\0'
      if(campo[j - 1] == ',' || campo[j - 1] == EOF) {
        // campo = realloc(campo, sizeof(char) * (j - 2));
        campo[j - 1] = '\0';
      }
      else {
        // campo = realloc(campo, sizeof(char) * (j - 3));
        campo[j - 2] = '\0';
      }

      // armazena o campo no registro atual de acordo com a ordem das colunas
      if (strcmp(colunas[i], "nomeTecnologiaOrigem") == 0) {
        registroAtual.TecnologiaOrigem.tamanho = strlen(campo); // campo nulo = 0
        registroAtual.TecnologiaOrigem.string = strdup(campo);
      } else if (strcmp(colunas[i], "nomeTecnologiaDestino") == 0) {
        registroAtual.TecnologiaDestino.tamanho = strlen(campo); // campo nulo = 0
        registroAtual.TecnologiaDestino.string = strdup(campo);
      } else if (strcmp(colunas[i], "grupo") == 0) {
        if (strcmp(campo, "") == 0)
          registroAtual.grupo = -1; // campo nulo
        else
          registroAtual.grupo = atoi(campo);
      } else if (strcmp(colunas[i], "popularidade") == 0) {
        if (strcmp(campo, "") == 0)
          registroAtual.popularidade = -1; // campo nulo
        else
          registroAtual.popularidade = atoi(campo);
      } else if (strcmp(colunas[i], "peso") == 0) {
        if (strcmp(campo, "") == 0 || strcmp(campo, " ") == 0)
          registroAtual.peso = -1; // campo nulo
        else
          registroAtual.peso = atoi(campo);
      }
      // libera a memoria alocada
      free(campo);
      campo = NULL;
    }

    // atualiza a contagem de tecnologias e pares de tecnologia
    contaTecnologias(arquivoBinario, registroAtual, &cabecalho);

    // escreve campos no arquivo binario
    fwrite(&registroAtual.removido, sizeof(char), 1, arquivoBinario);
    fwrite(&registroAtual.grupo, sizeof(int), 1, arquivoBinario);
    fwrite(&registroAtual.popularidade, sizeof(int), 1, arquivoBinario);
    fwrite(&registroAtual.peso, sizeof(int), 1, arquivoBinario);
    fwrite(&registroAtual.TecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBinario);
    fwrite(registroAtual.TecnologiaOrigem.string, sizeof(char), registroAtual.TecnologiaOrigem.tamanho, arquivoBinario);
    fwrite(&registroAtual.TecnologiaDestino.tamanho, sizeof(int), 1, arquivoBinario);
    fwrite(registroAtual.TecnologiaDestino.string, sizeof(char), registroAtual.TecnologiaDestino.tamanho, arquivoBinario);

    // conta a quantidade de memoria ocupada pelo registro
    int tam_bytes = sizeof(registroAtual.grupo) + sizeof(registroAtual.popularidade) + sizeof(registroAtual.peso) + sizeof(registroAtual.TecnologiaOrigem.tamanho) + sizeof(registroAtual.TecnologiaDestino.tamanho) + sizeof(char) + strlen(registroAtual.TecnologiaDestino.string) + strlen(registroAtual.TecnologiaOrigem.string);

    // completa o tamanho fixo do registro com lixo
    for (int i = tam_bytes; i < TAM_REGISTRO; i++) {
      char lixo = '$';
      fwrite(&lixo, sizeof(char), 1, arquivoBinario);
    }

    // libera a memoria alocada
    free(registroAtual.TecnologiaDestino.string);
    free(registroAtual.TecnologiaOrigem.string);
    
    // atualiza o indice do proximo registro 
    cabecalho.proxRRN++;
  }

  // libera a memoria alocada
  free(linha);

  // atualiza o cabecalho e fecha os arquivos abertos
  cabecalho.status = '1';
  escreveCabecalho(arquivoBinario, cabecalho);
  fclose(arquivoBinario);
  fclose(arquivoCSV);

  // libera a memoria alocada
  for(int i = 0; i < 5; i++) {
    free(colunas[i]);
  }
}

// incluir mensagens de erro - erro de nao registros ja incluido
void imprimeArquivo(char* nomeArquivoBinario) {
  /*
  Essa funcao le um arquivo binario e imprime seus registros.

  Essa funcao representa a funcionalidade 2 do exercicio introdutorio.
  */
  
  // abre arquivo binario 
  FILE *arquivoBinario = fopen(nomeArquivoBinario, "rb");
  if (arquivoBinario == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // cria registro auxiliar
  Registro registro;

  int sucessoArquivo = skipCabecalho(arquivoBinario);
  if (sucessoArquivo == 1){
    return;
  }

  // le os registros do arquivo e imprime-os
  while (1) {

    // verifica se o proximo registro existe
    if (fread(&registro.removido, sizeof(char), 1, arquivoBinario) == 0) {
      break;
    }

    // le o proximo registro
    trabalhaRegistros(arquivoBinario, &registro); 

    // imprime o registro
    imprimeRegistro(registro);

    // libera a memoria alocada
    free(registro.TecnologiaOrigem.string);
    free(registro.TecnologiaDestino.string);
  }

  // fecha o arquivo binario
  fclose(arquivoBinario);
}

void recuperaDados(char *arquivoEntrada, int n) {
  /*
  Essa funcao le um arquivo binario e imprime os registros que se adequam a condicao fornecida seus registros.

  Essa funcao representa a funcionalidade 3 do exercicio introdutorio.
  */

  // abre arquivo binario 
  FILE *arquivo = fopen(arquivoEntrada, "rb");
  if (arquivo == NULL) {
    printf("Falha no processamento do arquivo. \n");
    return;
  }

  // cria registro auxiliar
  Registro registro;

  // loop para cada uma das n condicoes fornecidas como entrada
  for (int i = 0; i < n; i++) {
    char nomeCampo[TAM_REGISTRO_VARIAVEL];
    char valorCampo[TAM_REGISTRO_FIXO];

    // le o nome do campo e o valor a ser buscado
    scanf("%s %s", nomeCampo, valorCampo);

    // limpa o valorCampo se for uma string (remove as aspas)
    if(strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0 || strcmp(nomeCampo, "nomeTecnologiaDestino") == 0){
      int i;
      for(i = 0; valorCampo[i] != '\0'; i++){
        valorCampo[i] = valorCampo[i + 1];
      }
      valorCampo[i - 2] = '\0';
    }

    // indica se o registro atende a condicao fornecida ou nao
    int encontrado = 0;
    
    // reseta o arquivo para o inicio
    rewind(arquivo);

    // pula os bytes do cabecalho do arquivo
    int sucessoArquivo = skipCabecalho(arquivo);
    if (sucessoArquivo == 1){
      return;
    }

    // le os registros do arquivo e imprime-os
    while (1) {

      // verifica se o proximo registro existe
      if (fread(&registro.removido, sizeof(char), 1, arquivo) == 0) {
        break;
      }

      // le o registro
      trabalhaRegistros(arquivo, &registro); 

      // indica se o registro atende a condicao fornecida ou nao
      int registrocondiz = 0;
      
      // compara se o campo eh igual a condicao fornecida
      if (registro.removido == '0') {
          if (strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0 && strcmp(registro.TecnologiaOrigem.string, valorCampo) == 0) {
            encontrado = 1;
            registrocondiz = 1;
            } else if (strcmp(nomeCampo, "grupo") == 0 && registro.grupo == atoi(valorCampo)) {
            encontrado = 1;
            registrocondiz = 1;
          } else if (strcmp(nomeCampo, "popularidade") == 0 && registro.popularidade == atoi(valorCampo)) {
            encontrado = 1;
            registrocondiz = 1;
          } else if (strcmp(nomeCampo, "nomeTecnologiaDestino") == 0 && strcmp(registro.TecnologiaDestino.string, valorCampo) == 0) {
            encontrado = 1;
            registrocondiz = 1;
          } else if (strcmp(nomeCampo, "peso") == 0 && registro.peso == atoi(valorCampo)) {
            encontrado = 1;
            registrocondiz = 1;
          }
      }

      // imprime o registro se encontrado
      if (registrocondiz == 1) {
        imprimeRegistro(registro);
        // break;
      }

      // libera a memoria alocada
      free(registro.TecnologiaOrigem.string);
      free(registro.TecnologiaDestino.string);
    }

    // imprime se nao encontrado nenhum registro em todo o arquivo
    if (encontrado == 0) {
      printf("Registro inexistente.\n");
    }
  }

  // fecha o arquivo binario
  fclose(arquivo);
}

void recuperaRegistro(const char *arquivoEntrada, int rrn) {
  /*
  Essa funcao le um arquivo binario e imprime o registro de acordo com o seu RRN.

  Essa funcao representa a funcionalidade 4 do exercicio introdutorio.
  */

  // abre arquivo binario 
  FILE *arquivo = fopen(arquivoEntrada, "rb");
  if (arquivo == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // cria registro auxiliar
  Registro registro;

  // verifica o status do arquivo binario
  char status;
  fread(&status, sizeof(char), 1, arquivo);
  if(status == '0'){
    printf("Falha no processamento do arquivo.\n");
    return;
  }  
  
  // le o proxrrn presente no cabecalho do arquivo
  int proxrrn;
  fread(&proxrrn, sizeof(int), 1, arquivo);

  // verifica se ha um registro com o rrn desejado no arquivo
  if(proxrrn - 1 < rrn){
    printf("Registro inexistente.\n");
    fclose(arquivo);
    return;
  }

  // pula o ponteiro do arquivo para o byteoffset do registro com rrn desejado
  int byte_offset = TAM_CABECALHO - 5 + ((rrn) * TAM_REGISTRO);
  fseek(arquivo, byte_offset, SEEK_CUR);

  // verifica se o registro desejado foi removido
  fread(&registro.removido, sizeof(char), 1, arquivo);
  if (registro.removido == '1') {
    printf("Registro inexistente.\n");
    fclose(arquivo);
    return;
  }

  // le o registro desejado e imprime-o
  trabalhaRegistros(arquivo, &registro);
  imprimeRegistro(registro);

  // fecha o arquivo
  fclose(arquivo);
}