#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesBasicas.c"
#include "funcoesFornecidas.c"

#define ORDEM_ARVORE_B 4


// Estrutura para representar um registro de cabeçalho no arquivo de índice
typedef struct {
    char status;
    int noRaiz;
    int RRNproxNo;
} RegistroCabecalho;

// Estrutura para representar um nó na árvore-B
typedef struct {
    int removidoLogico;
    int RRN;
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;
    int chave[ORDEM_ARVORE_B - 1];
    int RRNdoNo[ORDEM_ARVORE_B];
} RegistroDados;


//-------------------------------------funcionalidade 5 (1)--------------------------------------------------------

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
void divideNo(int RRN, int i, int chave, int RRNdoNovoNo, FILE *arquivoIndice) {
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

int proximoRRNNo(FILE *arquivoIndice) {
    fseek(arquivoIndice, 0, SEEK_SET);
    RegistroCabecalho cabecalho;
    fread(&cabecalho, sizeof(RegistroCabecalho), 1, arquivoIndice);
    return cabecalho.RRNproxNo;
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

// Função para exibir o conteúdo do arquivo binário na tela
void binarioNaTela(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        exit(1);
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);
    rewind(arquivo);

    char *conteudo = (char *)malloc(tamanhoArquivo);
    if (conteudo == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    fread(conteudo, 1, tamanhoArquivo, arquivo);
    fclose(arquivo);

    printf("%s\n", conteudo);

    free(conteudo);
}

// Função principal para criar o índice da árvore-B a partir do arquivo de dados
void criarIndiceArvoreB(const char *arquivoDados, const char *arquivoIndice) {
    FILE *dados = fopen(arquivoDados, "rb");
    FILE *indice = fopen(arquivoIndice, "wb");

    if (dados == NULL || indice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    RegistroDados registro;

    while (fread(&registro, sizeof(RegistroDados), 1, dados) == 1) {
        // Verifica se o registro não foi removido logicamente
        if (registro.removidoLogico == 0) {
            // Insere a chave na árvore-B
            insereNaArvoreN(registro.chave, ftell(dados) / sizeof(RegistroDados) - 1, indice);
        }
    }

    fclose(dados);
    fclose(indice);
}

//-----------------------------FUNCIONALIDADE 6 (2)-----------------------------------------------------------

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

// Função para exibir os dados de um registro
void exibirRegistro(Registro registro) {
    printf("%s, %d, %d, %s, %lf\n",
           registro.TecnologiaOrigem.string,
           registro.grupo,
           registro.popularidade,
           registro.TecnologiaDestino.string,
           registro.peso);
}

// Função principal para realizar a funcionalidade [6]
void funcionalidade6(char *arquivoDados, char *arquivoIndice, int n, char **campos, char **valores) {
    FILE *arqDados = fopen(arquivoDados, "rb");
    FILE *arqIndice = fopen(arquivoIndice, "rb");

    if (arqDados == NULL || arqIndice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    // Leitura do cabeçalho do índice árvore-B
    RegistroCabecalho cabecalhoIndice;
    fread(&cabecalhoIndice, sizeof(RegistroCabecalho), 1, arqIndice);

    // Verifica a consistência do índice
    if (cabecalhoIndice.status != '1') {
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    // Para cada busca
    for (int i = 0; i < n; i++) {
        int chave;
        if (strcmp(campos[i], "nomeTecnologiaOrigemDestino") == 0) {
            // Se o campo for a chave de busca, usa o índice árvore-B
            chave = atoi(valores[i]);
            int RRN = buscaArvoreB(arqIndice, cabecalhoIndice.noRaiz, chave);

            if (RRN != -1) {
                // Se encontrado na árvore-B, recupera os dados do arquivo de dados
                fseek(arqDados, RRN * sizeof(RegistroDados), SEEK_SET);
                Registro registro;
                fread(&registro, sizeof(RegistroDados), 1, arqDados);
                exibirRegistro(registro);
            } else {
                printf("Registro inexistente.\n");
            }
        } else {
            // Se o campo não for a chave de busca, usa a funcionalidade [3]
            buscaFuncionalidade3(arqDados, campos[i], valores[i]);
        }
    }

    fclose(arqDados);
    fclose(arqIndice);
}

//-------------------------------------funcionalidade 7 (3)----------------------------------------------------------

// Insere um registro no arquivo de dados
void inserirNoArquivoDados(FILE *arquivoDados, Registro registro){
    fseek(arquivoDados, 0, SEEK_END);
    fwrite(&registro, sizeof(RegistroDados), 1, arquivoDados);
}

// Exibir os dados de um registro
void exibirRegistro(Registro registro){
    printf("%s, %d, %d, %s, %lf\n", registro.TecnologiaOrigem.string, registro.grupo, registro.popularidade, registro.TecnologiaDestino.string, registro.peso);
}

void insereRegistro(char *arquivoDados, char *arquivoIndice, int n, char **campos){
    arquivoDados = diretorioArquivo(arquivoDados, 'b');
    FILE *arqDados = fopen(arquivoDados, "rb");

    arquivoIndice = diretorioArquivo(arquivoIndice, 'b');
    FILE *arqIndice = fopen(arquivoIndice, "rb");

    if(arqDados == NULL || arqIndice == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    // Leitura do cabeçalho do índice arvore-B
    RegistroCabecalho cabecalhoIndice;
    fread(&cabecalhoIndice, sizeof(RegistroCabecalho), 1, arqIndice);

    // Para cada inserção
    for (int i = 0; i < n; i++){
        // Lê os valores
        char *nomeTecnologiaOrigem = campos[i++];
        int grupo = (strcmp(campos[i++], "NULO") == 0) ? -1 : atoi(campos[i - 1]);
        int popularidade = (strcmp(campos[i++], "NULO") == 0) ? -1 : atoi(campos[i - 1]);
        char *nomeTecnologiaDestino = campos[i++];
        double peso = (strcmp(campos[i], "NULO") == 0) ? -1 : atof(campos[i]);

        // Cria o registro
        Registro novoRegistro;
        strcpy(novoRegistro.TecnologiaOrigem.string, nomeTecnologiaOrigem);
        novoRegistro.grupo = grupo;
        novoRegistro.popularidade = popularidade;
        strcpy(novoRegistro.TecnologiaDestino.string, nomeTecnologiaDestino);
        novoRegistro.peso = peso;

        // Insere o registro no arquivo de dados
        inserirNoArquivoDados(arqDados, novoRegistro);

        // Insere a chave na árvore-B
        inserirNaArvoreB(arqIndice, novoRegistro.grupo, ftell(arqDados) / sizeof(Registro) - 1);
    }

    // Atualiza o status no cabeçalho do índice
    fseek(arqIndice, 0, SEEK_SET);
    cabecalhoIndice.status = '0';  // Marca como inconsistente
    fwrite(&cabecalhoIndice, sizeof(RegistroCabecalho), 1, arqIndice);

    // Exibe os dados dos arquivos
    printf("Arquivo de dados:\n");
    binarioNaTela(arquivoDados);

    printf("Arquivo de índice:\n");
    binarioNaTela(arquivoIndice);

    fclose(arqDados);
    fclose(arqIndice);

}

