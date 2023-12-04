#include "funcionalidades.h"
#include "funcoesFornecidas.h"
#include "registro.h"
#include "arvoreB.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void criaTabela(char* nomeArquivoCSV, char* nomeArquivoBinario) {
  /*
  Essa funcao le um arquivo csv e escreve os registros com campo fixo e variavel em um arquivo binario. 

  O arquivo inicialmente possui um cabecalho que indica o status do arquivo binario, o indice do proximo registro, 
  o numero de tecnologias distintas presentes e o numero de pares de tecnologias.

  Essa funcao representa a funcionalidade 1 do exercicio introdutorio.
  */

  // nomeArquivoCSV = diretorioArquivo(nomeArquivoCSV, 'c');

  // abre arquivo csv
  FILE* arquivoCSV = fopen(nomeArquivoCSV, "r");
  if (arquivoCSV == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // nomeArquivoBinario = diretorioArquivo(nomeArquivoBinario, 'b');

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
  // free(nomeArquivoCSV);
  // free(nomeArquivoBinario);
}

void imprimeArquivo(char* nomeArquivoBinario) {
  /*
  Essa funcao le um arquivo binario e imprime seus registros.

  Essa funcao representa a funcionalidade 2 do exercicio introdutorio.
  */
  
  // abre arquivo binario 
  // nomeArquivoBinario = diretorioArquivo(nomeArquivoBinario, 'b');

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
    leRegistro(arquivoBinario, &registro); 

    // imprime o registro
    imprimeRegistro(registro);

    // libera a memoria alocada
    free(registro.TecnologiaOrigem.string);
    free(registro.TecnologiaDestino.string);
  }

  // fecha o arquivo binario
  fclose(arquivoBinario);
  // free(nomeArquivoBinario);
}

bool filtroRegistro(FILE* arquivo, char nomeCampo[TAM_REGISTRO_VARIAVEL], char valorCampo[TAM_REGISTRO_FIXO]){
  /*
    Busca todos os registro e filtra os campos
  */

  // cria registro auxiliar
  Registro registro;

  // indica se o registro atende a condicao fornecida ou nao
    bool encontrado = 0;
    
    // reseta o arquivo para o inicio
    rewind(arquivo);

    // pula os bytes do cabecalho do arquivo
    int sucessoArquivo = skipCabecalho(arquivo);
    if (sucessoArquivo == 1){
      return encontrado;
    }

    // le os registros do arquivo e imprime-os
    while (1) {

      // verifica se o proximo registro existe
      if (fread(&registro.removido, sizeof(char), 1, arquivo) == 0) {
        break;
      }

      // le o registro
      leRegistro(arquivo, &registro); 

      // indica se o registro atende a condicao fornecida ou nao
      bool registrocondiz = 0;
      
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
      if (registrocondiz) {
        imprimeRegistro(registro);
      }

      // libera a memoria alocada
    }
  return encontrado;
}

// Funcionalidade 3 e 6
void busca(int caso, char *arquivoEntrada, char* arquivoIndice, int n) {
  /*
  Essa funcao le um arquivo binario e imprime os registros que se adequam a condicao fornecida.

  Essa funcao representa a funcionalidade 3 do exercicio introdutorio.
  */

  // abre arquivo binario 
  FILE *arquivo = abreBinarioLeitura(arquivoEntrada);
  FILE *arqIndice = abreBinarioLeitura(arquivoIndice);

  char statusEntrada, statusIndice;
  fread(&statusEntrada, sizeof(char), 1, arquivo);
  fread(&statusIndice, sizeof(char), 1, arqIndice);

  if(statusEntrada != '1' || statusIndice != '1'){
    printf("Falha no processamento do arquivo.\n");
    return;
  }  

  // loop para cada uma das n condicoes fornecidas como entrada
  for (int i = 0; i < n; i++) {
    char nomeCampo[TAM_REGISTRO_VARIAVEL];
    char valorCampo[TAM_REGISTRO_FIXO];

    // le o nome do campo e o valor a ser buscado
    scanf("%s %s", nomeCampo, valorCampo);

    // limpa o valorCampo se for uma string (remove as aspas)
    if(strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0 || strcmp(nomeCampo, "nomeTecnologiaOrigemDestino") == 0 || strcmp(nomeCampo, "nomeTecnologiaDestino") == 0){
      int i;
      for(i = 0; valorCampo[i] != '\0'; i++){
        valorCampo[i] = valorCampo[i + 1];
      }
      valorCampo[i - 2] = '\0';
    }

    bool encontrado = 0;

    if(strcmp(nomeCampo, "nomeTecnologiaOrigemDestino") == 0)
      encontrado = filtroArvore(arquivoEntrada, arqIndice, valorCampo);
    else
      encontrado = filtroRegistro(arquivo, nomeCampo, valorCampo);

    // imprime se nao encontrado nenhum registro em todo o arquivo
    if (!encontrado) {
      printf("Registro inexistente.\n");
    }
  }

  // fecha o arquivo binario
  fclose(arquivo);
  fclose(arqIndice);
}

// Funcionalidade 4 - Trabalho introdutorio
void recuperaRegistro(char *arquivoEntrada, int rrn) {
  /*
  Essa funcao le um arquivo binario e imprime o registro de acordo com o seu RRN.

  Essa funcao representa a funcionalidade 4 do exercicio introdutorio.
  */

  // abre arquivo binario 
  // arquivoEntrada = diretorioArquivo(arquivoEntrada, 'b');

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
  if(status != '1'){
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
  leRegistro(arquivo, &registro);
  imprimeRegistro(registro);

  // fecha o arquivo
  fclose(arquivo);
  // free(arquivoEntrada);
}

// Função principal para criar o índice da árvore-B a partir do arquivo de dados
void criaIndiceArvoreB(char *arquivoDados, char *arquivoIndice) {
  FILE *arqDados = abreBinarioLeitura(arquivoDados);
  FILE *arqIndice = abreBinarioEscritaLeitura(arquivoIndice);

  CabecalhoArvoreB cabecalho;
  cabecalho.status = '1';
  cabecalho.noRaiz = -1;
  cabecalho.RRNproxNo = 0;
  escreveCabecalhoArvoreB(arqIndice, cabecalho);

  if(skipCabecalho(arqDados)){
    fclose(arqDados);
    return;
  }

  Registro registroAtual;
  Chave chave;
  chave.ponteiroanterior = -1;
  chave.ponteiroproximo = -1;
  int RRN = -1;

  // le os registros do arquivo de dados
  while (1) {

    // verifica se o proximo registro existe
    if (fread(&registroAtual.removido, sizeof(char), 1, arqDados) == 0) {
      break;
    }

    // le o proximo registro
    leRegistro(arqDados, &registroAtual); 
    RRN++;

    // Verifica se o registro não foi removido logicamente
    if (registroAtual.removido == '0' && registroAtual.TecnologiaOrigem.tamanho != 0 && registroAtual.TecnologiaDestino.tamanho != 0) {
      // Insere a chave na árvore-B
      chave.nome = malloc(registroAtual.TecnologiaOrigem.tamanho + registroAtual.TecnologiaDestino.tamanho + 2);
      if(chave.nome == NULL){
        printf("Falha na alocacao de memoria\n");
        exit(1);
      }
      chave.nome[0] = '\0';
      strcat(chave.nome, registroAtual.TecnologiaOrigem.string);
      strcat(chave.nome, registroAtual.TecnologiaDestino.string);
      chave.ref = RRN;
      insereNaArvoreB(chave, -1, 1, arqIndice);
    }
  }

  fclose(arqDados);
  fechaIndiceEscrita(arqIndice);
}

bool filtroArvore(char* nomeArquivoDados, FILE* arquivoIndice, char* chave) {

  // Leitura do cabeçalho do índice árvore-B
  CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arquivoIndice);

  bool encontrado = 0;
  Pagina pagina;

  int RRN = buscaArvoreB(arquivoIndice, cabecalhoArvoreB.noRaiz, chave, &pagina);
  if(RRN == -1)
    return 0;

  pagina = lePagina(arquivoIndice, RRN);

  for(int i = 0; i < pagina.nroChavesNo; i++)
    free(pagina.chave[i].nome);

  if (RRN != -1) {
      // Se encontrado na árvore-B, recupera os dados do arquivo de dados
      recuperaRegistro(nomeArquivoDados, RRN);
      encontrado = 1;
  }
  return encontrado;
}

// Função que recebe os valores a serem inseridos enquanto lida com nulos
Registro lidaNulos(){
  /*
    Essa função recebe as entradas dos valores a serem inseridos no registro lidando com os casos nulos

    Essa função é usada na função insereResgistro, referente à funcionalidade 7
  */

  Registro registro;

  //recebe os valores que serão inseridos pelo usuario (repete as 5 vezes)
  char* reg1 = malloc(sizeof(char) * 100);  // recebe o valor 1
  scanf("%s", reg1);
  reg1 = strtok(reg1, ",");
  if(strcmp(reg1, "NULO") == 0){  // lida com o caso nulo
    free(reg1);
    char* resg1 = malloc(2);
    resg1[0] = '\0';
    registro.TecnologiaOrigem.string = resg1;
    registro.TecnologiaOrigem.tamanho = 0;
  } else{
    registro.TecnologiaOrigem.string = reg1;
    registro.TecnologiaOrigem.tamanho = strlen(reg1);
  }

  char* reg2 = malloc(sizeof(char)* 100); //recebe o valor 2
  scanf("%s", reg2);
  reg2 = strtok(reg2, ",");
  if(strcmp(reg2, "NULO") == 0){
    free(reg2);
    registro.grupo = -1;
  } else{
    registro.grupo = atoi(reg2);
    free(reg2);
  }

  char* reg3 = malloc(sizeof(char)* 100);
  scanf("%s", reg3);
  reg3 = strtok(reg3, ",");
  if(strcmp(reg3, "NULO") == 0){
    free(reg3);
    registro.popularidade = -1;
  } else{
    registro.popularidade = atoi(reg3);
    free(reg3);
  }

  char* reg4 = malloc(sizeof(char)* 100);
  scanf("%s", reg4);
  reg4 = strtok(reg4, ",");
  if(strcmp(reg4, "NULO") == 0){
    free(reg4);
    char* resg2 = malloc(2);
    resg2[0] = '\0';
    registro.TecnologiaDestino.string = resg2;
    registro.TecnologiaDestino.tamanho = 0;
  } else{
    registro.TecnologiaDestino.string = reg4;
    registro.TecnologiaDestino.tamanho = strlen(reg4);
  }

  char* reg5 = malloc(sizeof(char)* 100);
  scanf("%s", reg5);
  if(strcmp(reg5, "NULO") == 0){
    free(reg5);
    registro.peso = -1;
  } else{
    registro.peso = atoi(reg5);
    free(reg5);
  }
  
  return registro;
}

// Funcionalidade 7 - insere os registros
void insereRegistro(char *arquivoDados, char *arquivoIndice, int n) {
    /*
      Função relacionada à funcionalidade 7;

      Insere os valores de entrada no registroe na arvore-B
    */
  // Realize as inserções
  FILE *dados = fopen(arquivoDados, "rb+");
  FILE *indice = fopen(arquivoIndice, "rb+");

  // Verifica se o aquivo existe
  if (dados == NULL || indice == NULL) {  
    printf("Falha no processamento do arquivo.\n");
    return;
  }


  Cabecalho cabecalho;

  // le o cabecalho
  fseek(dados, 0, SEEK_SET);
  fread(&cabecalho.status, sizeof(char), 1, dados);
  fread(&cabecalho.proxRRN, sizeof(int), 1, dados);
  fread(&cabecalho.nroTecnologias, sizeof(int), 1, dados);
  fread(&cabecalho.nroParesTecnologias, sizeof(int), 1, dados);

  fseek(dados, cabecalho.proxRRN * TAM_REGISTRO, SEEK_SET);

  Registro registroAtual;
  Chave chave;
  chave.ponteiroanterior = -1;
  chave.ponteiroproximo = -1;
  int RRN;


  for (int i = 0; i < n; i++) {
      // Solicite ao usuário os valores do novo registro
      registroAtual = lidaNulos();
      registroAtual.removido = '0';
      int similar1 = 0;
      int similar2 = 0;

      // conta tecnologias;
      for(int j =0; j < cabecalho.proxRRN; j++){
        fseek(dados, TAM_CABECALHO, SEEK_SET);
        Registro newReg;
        fread(&newReg.removido, sizeof(char), 1, dados);
        fread(&newReg.grupo, sizeof(int), 1, dados);
        fread(&newReg.popularidade, sizeof(int), 1, dados);
        fread(&newReg.peso, sizeof(int), 1, dados);
        fread(&newReg.TecnologiaOrigem.tamanho, sizeof(int), 1, dados);
        newReg.TecnologiaOrigem.string = malloc(newReg.TecnologiaOrigem.tamanho + 1);
        if(newReg.TecnologiaOrigem.tamanho != 0)
          fread(newReg.TecnologiaOrigem.string, sizeof(char), newReg.TecnologiaOrigem.tamanho, dados);

        newReg.TecnologiaOrigem.string[newReg.TecnologiaOrigem.tamanho] = '\0';
        fread(&newReg.TecnologiaDestino.tamanho, sizeof(int), 1, dados);

        newReg.TecnologiaDestino.string = malloc(newReg.TecnologiaDestino.tamanho + 1);
        if(newReg.TecnologiaDestino.tamanho != 0)
          fread(newReg.TecnologiaDestino.string, sizeof(char), newReg.TecnologiaDestino.tamanho, dados);

        newReg.TecnologiaDestino.string[newReg.TecnologiaDestino.tamanho] = '\0';

        if(newReg.removido == '1')
          continue;
        
        if(newReg.TecnologiaOrigem.tamanho != 0 && registroAtual.TecnologiaOrigem.tamanho != 0){
          if(strcmp(newReg.TecnologiaOrigem.string, registroAtual.TecnologiaOrigem.string) == 0){
            similar1 = 1;
          }
        }

        if(newReg.TecnologiaOrigem.tamanho != 0 && registroAtual.TecnologiaDestino.tamanho != 0){
          if(strcmp(newReg.TecnologiaOrigem.string, registroAtual.TecnologiaDestino.string) == 0){
            similar2 = 1;
          }
        }

        if(newReg.TecnologiaDestino.tamanho != 0 && registroAtual.TecnologiaOrigem.tamanho != 0){
          if(strcmp(newReg.TecnologiaDestino.string, registroAtual.TecnologiaOrigem.string) == 0){
            similar1 = 1;
          }
        }

        if(newReg.TecnologiaDestino.tamanho != 0 && registroAtual.TecnologiaDestino.tamanho != 0){
          if(strcmp(newReg.TecnologiaDestino.string, registroAtual.TecnologiaDestino.string) == 0){
            similar2 = 1;
          }
        }

        free(newReg.TecnologiaOrigem.string);
        free(newReg.TecnologiaDestino.string);
      }
      
      if(similar1 == 1){
        cabecalho.nroTecnologias++;
      }
      if(similar2 == 1){
        cabecalho.nroTecnologias++;
      }
      if(registroAtual.TecnologiaDestino.tamanho != 0 && registroAtual.TecnologiaOrigem.tamanho != 0){
        cabecalho.nroParesTecnologias++;
      }
      // fim da contagem de tecnologias

      // escreve campos no arquivo binario
      fwrite(&registroAtual.removido, sizeof(char), 1, dados);
      fwrite(&registroAtual.grupo, sizeof(int), 1, dados);
      fwrite(&registroAtual.popularidade, sizeof(int), 1, dados);
      fwrite(&registroAtual.peso, sizeof(int), 1, dados);
      fwrite(&registroAtual.TecnologiaOrigem.tamanho, sizeof(int), 1, dados);
      if(registroAtual.TecnologiaOrigem.tamanho != 0)
        fwrite(registroAtual.TecnologiaOrigem.string, sizeof(char), registroAtual.TecnologiaOrigem.tamanho, dados);
      
      fwrite(&registroAtual.TecnologiaDestino.tamanho, sizeof(int), 1, dados);
      if(registroAtual.TecnologiaDestino.tamanho != 0)
        fwrite(registroAtual.TecnologiaDestino.string, sizeof(char), registroAtual.TecnologiaDestino.tamanho, dados);
      
      for(int j=0; j < TAM_REGISTRO - 21 - registroAtual.TecnologiaOrigem.tamanho - registroAtual.TecnologiaDestino.tamanho; j++){
        char lixo = '$';
        fwrite(&lixo, sizeof(char), 1, dados);
      }

      // Verifica se o registro não foi removido logicamente
      if (registroAtual.removido == '0' && registroAtual.TecnologiaOrigem.tamanho != 0 && registroAtual.TecnologiaDestino.tamanho != 0) {
      // Insere a chave na árvore-B
      chave.nome = malloc(registroAtual.TecnologiaOrigem.tamanho + registroAtual.TecnologiaDestino.tamanho + 2);
      if(chave.nome == NULL){
        printf("Falha na alocacao de memoria\n");
        exit(1);
      }
      chave.nome[0] = '\0';
      strcat(chave.nome, registroAtual.TecnologiaOrigem.string);
      strcat(chave.nome, registroAtual.TecnologiaDestino.string);
      chave.ref = RRN;
      insereNaArvoreB(chave, -1, 1, indice);
      }
    cabecalho.proxRRN++;
  }

  // escreve o cabecalho no arquivo
  escreveCabecalho(dados, cabecalho);

  fclose(dados);
  fechaIndiceEscrita(indice);

  // Exibe o conteúdo dos arquivos na tela
  binarioNaTela(arquivoDados);

  binarioNaTela(arquivoIndice);
}