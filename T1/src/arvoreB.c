#include "arvoreB.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// void imprimeRegistroDados(RegistroDados registro) {
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
void insereEmNoNaoCheio(RegistroDados *no, int chave, int RRNdoNo, int RRNdoNovoNo, FILE *arquivoIndice) {
    int i = no->nroChavesNo - 1;

    // Encontra a posição correta para a nova chave
    while (i >= 0 && chave < no->chave[i]) {
        no->chave[i + 1] = no->chave[i];
        no->RRNdoNo[i + 2] = no->RRNdoNo[i + 1];
        i--;
    }

    // Insere a nova chave e atualiza os ponteiros dos filhos
    no->chave[i + 1] = chave;
    no->RRNdoNo[i + 2] = RRNdoNovoNo;
    no->nroChavesNo++;

    // Atualiza o nó no arquivo
    fseek(arquivoIndice, RRNdoNo * sizeof(RegistroDados), SEEK_SET);
    fwrite(no, sizeof(RegistroDados), 1, arquivoIndice);
}



// Função auxiliar para dividir um nó durante a inserção
void particionaNo(int RRN, int i, int chave, int RRNdoNovoNo, FILE *arquivoIndice) {
    // Lê o nó a ser dividido
    fseek(arquivoIndice, RRN * sizeof(RegistroDados), SEEK_SET);
    RegistroDados no;
    fread(&no, sizeof(RegistroDados), 1, arquivoIndice);

    // Cria um novo nó
    RegistroDados novoNo;
    novoNo.nroChavesNo = (ORDEM_ARVORE_B - 1) / 2;
    novoNo.alturaNo = 1;

    // Copia metade das chaves e RRNs para o novo nó
    for (int j = 0; j < (ORDEM_ARVORE_B - 1) / 2; j++) {
        novoNo.chave[j] = no.chave[j + (ORDEM_ARVORE_B + 1) / 2];
        novoNo.RRNdoNo[j] = no.RRNdoNo[j + (ORDEM_ARVORE_B + 1) / 2];
    }
    
    // Atualiza a quantidade de chaves no nó original
    no.nroChavesNo = (ORDEM_ARVORE_B - 1) / 2;

    // Atualiza o RRN do próximo nó no nó original
    no.RRNdoNo[ORDEM_ARVORE_B / 2] = RRNdoNovoNo;

    // Atualiza o cabeçalho
    fseek(arquivoIndice, 0, SEEK_SET);
    RegistroCabecalho cabecalho;
    fread(&cabecalho, sizeof(RegistroCabecalho), 1, arquivoIndice);
    cabecalho.RRNproxNo++;
    fseek(arquivoIndice, 0, SEEK_SET);
    fwrite(&cabecalho, sizeof(RegistroCabecalho), 1, arquivoIndice);

    // Atualiza o nó original no arquivo
    fseek(arquivoIndice, RRN * sizeof(RegistroDados), SEEK_SET);
    fwrite(&no, sizeof(RegistroDados), 1, arquivoIndice);

    // Escreve o novo nó no arquivo
    fseek(arquivoIndice, RRNdoNovoNo * sizeof(RegistroDados), SEEK_SET);
    fwrite(&novoNo, sizeof(RegistroDados), 1, arquivoIndice);
}

int proximoRRNNo(FILE *arquivoIndice) {
    fseek(arquivoIndice, 0, SEEK_SET);
    RegistroCabecalho cabecalho;
    fread(&cabecalho, sizeof(RegistroCabecalho), 1, arquivoIndice);
    return cabecalho.RRNproxNo;
}

// Função auxiliar para inserir uma chave na árvore-B
void insereNaArvoreB(int chave, int RRN, FILE *arquivoIndice) {
    int chavePromovida;
    int RRNdoNovoNo;
    int houveSplit = insereNaArvoreBRecursivo(chave, RRN, 0, &chavePromovida, &RRNdoNovoNo, arquivoIndice);

    if (houveSplit) {
        // Cria um novo nó raiz
        RegistroDados novaRaiz;
        novaRaiz.nroChavesNo = 1;
        novaRaiz.alturaNo = 2;

        // Armazena o RRN do próximo nó no campo RRNdoNo[1] da nova raiz
        novaRaiz.RRNdoNo[0] = RRN;
        novaRaiz.RRNdoNo[1] = RRNdoNovoNo;
        novaRaiz.chave[0] = chavePromovida;

        // Atualiza o cabeçalho
        fseek(arquivoIndice, 0, SEEK_SET);
        RegistroCabecalho cabecalho;
        fread(&cabecalho, sizeof(RegistroCabecalho), 1, arquivoIndice);
        cabecalho.noRaiz = proximoRRNNo(arquivoIndice);
        cabecalho.RRNproxNo++;
        fseek(arquivoIndice, 0, SEEK_SET);
        fwrite(&cabecalho, sizeof(RegistroCabecalho), 1, arquivoIndice);

        // Escreve a nova raiz no arquivo
        fseek(arquivoIndice, novaRaiz.RRNdoNo[0] * sizeof(RegistroDados), SEEK_SET);
        fwrite(&novaRaiz, sizeof(RegistroDados), 1, arquivoIndice);
    }
}

// Função auxiliar para inserir uma chave na árvore-B recursivamente
int insereNaArvoreBRecursivo(int chave, int RRN, int nivel, int *chavePromovida, int *RRNdoNovoNo, FILE *arquivoIndice) {
    fseek(arquivoIndice, RRN * sizeof(RegistroDados), SEEK_SET);
    RegistroDados no;
    fread(&no, sizeof(RegistroDados), 1, arquivoIndice);

    int i = no.nroChavesNo - 1;

    // Encontra a posição correta para a nova chave
    while (i >= 0 && chave < no.chave[i]) {
        i--;
    }

    // Caso base: se o nó é uma folha
    if (no.alturaNo == 1) {
        if (no.nroChavesNo < ORDEM_ARVORE_B - 1) {
            // Se há espaço no nó, insere a chave
            insereEmNoNaoCheio(&no, chave, RRN, -1, arquivoIndice);
            return 0;
        } else {
            // Se o nó está cheio, divide-o
            particionaNo(RRN, i + 1, chave, *RRNdoNovoNo, arquivoIndice);
            return 1;
        }
    } else {
        // Caso recursivo: desce na árvore
        int houveSplit = insereNaArvoreBRecursivo(chave, no.RRNdoNo[i + 1], nivel + 1, chavePromovida, RRNdoNovoNo, arquivoIndice);

        // Se houve split no nível inferior, insere a chave promovida
        if (houveSplit) {
            if (no.nroChavesNo < ORDEM_ARVORE_B - 1) {
                // Se há espaço no nó, insere a chave promovida
                insereEmNoNaoCheio(&no, *chavePromovida, RRN, *RRNdoNovoNo, arquivoIndice);
                return 0;
            } else {
                // Se o nó está cheio, divide-o
                particionaNo(RRN, i + 1, *chavePromovida, *RRNdoNovoNo, arquivoIndice);
                return 1;
            }
        }

        return 0;
    }
}

// Função para buscar um registro na árvore-B
int buscaArvoreB(FILE *arquivoIndice, int RRN, int chave) {
    RegistroDados no;
    fseek(arquivoIndice, RRN * sizeof(RegistroDados), SEEK_SET);
    fread(&no, sizeof(RegistroDados), 1, arquivoIndice);

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

    // Desce para o próximo nível da árvore-B
    return buscaArvoreB(arquivoIndice, no.RRNdoNo[i], chave);
}