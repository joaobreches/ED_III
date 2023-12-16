#include "funcionalidades.h"
#include "funcoesFornecidas.h"
#include "registro.h"
#include "arquivo.h"
#include "algoritmos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Função para executar a funcionalidade 8
void recuperaDados8(char *nomeArquivo) {
    
  printf("entrou!");

    // Abrir o arquivo binário para leitura
    FILE *arquivo = abreBinarioLeitura(nomeArquivo);

    // Lê o número de registros no arquivo
    int numRegistros;
    fread(&numRegistros, sizeof(int), 1, arquivo);

    // Aloca espaço para os vértices
    Vertice *vertices = malloc(numRegistros * sizeof(Vertice));

    // Lê os registros do arquivo
    for (int i = 0; i < numRegistros; i++) {
    // Lê o vértice do arquivo
        if (fread(&vertices[i], sizeof(Vertice), 1, arquivo) != 1) {
            perror("Erro ao ler vértice do arquivo");
            exit(EXIT_FAILURE);
        }

        // Aloca espaço para as arestas do vértice
        vertices[i].arestas = malloc(vertices[i].numArestas * sizeof(Aresta));
        if (vertices[i].arestas == NULL) {
            perror("Erro ao alocar memória para arestas do vértice");
            exit(EXIT_FAILURE);
        }

        // Lê as arestas do arquivo
        if (fread(vertices[i].arestas, sizeof(Aresta), vertices[i].numArestas, arquivo) != vertices[i].numArestas) {
            perror("Erro ao ler arestas do vértice do arquivo");
            exit(EXIT_FAILURE);
        }
    }

    // Fecha o arquivo
    fclose(arquivo);

    // Ordena os nomes das tecnologias para a saída ordenada
    char **nomesOrdenados = (char**)malloc(numRegistros * sizeof(char*));
    for (int i = 0; i < numRegistros; i++) {
        nomesOrdenados[i] = vertices[i].nomeTecnologia;
    }
    qsort(nomesOrdenados, numRegistros, sizeof(char*), compararStrings);

    // Imprime os dados ordenados
    for (int i = 0; i < numRegistros; i++) {
        int indice = 0;
        // Encontra o índice correspondente ao nome ordenado
        while (strcmp(vertices[indice].nomeTecnologia, nomesOrdenados[i]) != 0) {
            indice++;
        }

        printf("%s, %d, %d, %d, %d, ", vertices[indice].nomeTecnologia,
               vertices[indice].grupo, vertices[indice].grauEntrada,
               vertices[indice].grauSaida, vertices[indice].grau);

        // Ordena as arestas pelo nomeTecDestino para a saída ordenada
        qsort(vertices[indice].arestas, vertices[indice].numArestas, sizeof(Aresta), compararArestas);

        // Imprime as arestas
        for (int j = 0; j < vertices[indice].numArestas; j++) {
            printf("%s, %d", vertices[indice].arestas[j].nomeTecDestino, vertices[indice].arestas[j].peso);
            if (j < vertices[indice].numArestas - 1) {
                printf(", ");
            }
        }

        printf("\n");
    }

    // Libera a memória alocada
    for (int i = 0; i < numRegistros; i++) {
        free(vertices[i].arestas);
    }
    free(vertices);
    free(nomesOrdenados);
}

// Função para executar a funcionalidade 10
void listaNomes(char *nomeArquivo, int n) {
    
  // Abrir o arquivo binário para leitura
  FILE *arquivo = abreBinarioLeitura(nomeArquivo);


  // Lê o número de registros no arquivo
  int numRegistros;
  fread(&numRegistros, sizeof(int), 1, arquivo);

  // Aloca espaço para os vértices
  Vertice *vertices = (Vertice*)malloc(numRegistros * sizeof(Vertice));

  // Lê os registros do arquivo
  for (int i = 0; i < numRegistros; i++) {
      fread(&vertices[i], sizeof(Vertice), 1, arquivo);
      // Aloca espaço para as arestas do vértice
      vertices[i].arestas = (Aresta*)malloc(vertices[i].numArestas * sizeof(Aresta));
      // Lê as arestas do arquivo
      fread(vertices[i].arestas, sizeof(Aresta), vertices[i].numArestas, arquivo);
  }

  // Fecha o arquivo
  fclose(arquivo);

  Registro registro;

    // Realiza a funcionalidade n vezes
    for (int k = 0; k < n; k++) {
        // Lê o nome da tecnologia passado como parâmetro
        char tecnologia[50];
        fgets(tecnologia, sizeof(tecnologia), stdin);
        tecnologia[strcspn(tecnologia, "\n")] = '\0'; // Remove o caractere de nova linha

        // Encontra o índice correspondente ao nome passado como parâmetro
        int indice = 0;
        while (strcmp(vertices[indice].nomeTecnologia, tecnologia) != 0) {
            indice++;
            if (indice >= numRegistros) {
                printf("Registro inexistente.\n\n");
                continue;
            }
        }

        // Imprime a tecnologia passada como parâmetro
        printf("\"%s\": ", tecnologia);

        // Ordena as arestas pelo nomeTecOrigem para a saída ordenada
        qsort(vertices[indice].arestas, vertices[indice].numArestas, sizeof(Aresta), compararArestas);

        // Imprime as tecnologias que originaram a tecnologia passada como parâmetro
        int primeiraTecnologia = 1;
        for (int j = 0; j < vertices[indice].numArestas; j++) {
            if (primeiraTecnologia) {
                printf("%s", vertices[indice].arestas[j].nomeTecOrigem);
                primeiraTecnologia = 0;
            } else {
                printf(", %s", vertices[indice].arestas[j].nomeTecOrigem);
            }
        }

        printf("\n\n");
    }

    // Libera a memória alocada
    for (int i = 0; i < numRegistros; i++) {
        free(vertices[i].arestas);
    }
    free(vertices);
}

// Função para determinar se o grafo é fortemente conexo
void fortementeConexo(char* nomeArquivo) {
    // Abrir o arquivo binário para leitura
    FILE* arquivo = abreBinarioLeitura(nomeArquivo);
    if (arquivo == NULL) {
        printf("Falha na execução da funcionalidade.\n");
        return;
    }

    // Lê o número de registros no arquivo
    int numRegistros;
    fread(&numRegistros, sizeof(int), 1, arquivo);

    // Inicializa o grafo
    Vertice* grafo = inicializarGrafo(numRegistros);

    // Lê os registros do arquivo
    for (int i = 0; i < numRegistros; i++) {
        fread(&grafo[i], sizeof(Vertice), 1, arquivo);
        // Aloca espaço para as arestas do vértice
        grafo[i].arestas = (Aresta*)malloc(grafo[i].numArestas * sizeof(Aresta));
        // Lê as arestas do arquivo
        fread(grafo[i].arestas, sizeof(Aresta), grafo[i].numArestas, arquivo);
    }

    // Fecha o arquivo
    fclose(arquivo);

    // Realiza a funcionalidade
    Pilha* pilha = inicializarPilha(numRegistros);

    // Realiza a primeira DFS para preencher a pilha
    for (int i = 0; i < numRegistros; i++) {
        if (!grafo[i].visitado) {
            dfs(grafo, i, pilha);
        }
    }

    // Inicializa o grafo transposto (grafo reverso)
    Vertice* grafoTransposto = inicializarGrafo(numRegistros);

    // Inicializa um vetor para armazenar os componentes fortemente conexos
    int *componente = (int*)malloc(numRegistros * sizeof(int));

    // Realiza a DFS no grafo transposto para calcular os componentes fortemente conexos
    int componenteAtual = 0;
    while (!pilhaVazia(pilha)) {
        int vertice = desempilhar(pilha);
        if (!grafoTransposto[vertice].visitado) {
            dfsTransposto(grafo, vertice, componente, componenteAtual);
            componenteAtual++;
        }
    }

    // Verifica quantos componentes fortemente conexos foram encontrados
    int numComponentes = 0;
    for (int i = 0; i < numRegistros; i++) {
        if (componente[i] > numComponentes) {
            numComponentes = componente[i];
        }
    }

    // Verifica se o grafo é fortemente conexo
    if (numComponentes == 0) {
        printf("Nao, o grafo nao e fortemente conexo e possui 0 componentes.\n");
    } else {
        printf("Sim, o grafo e fortemente conexo e possui %d componente%s.\n", numComponentes + 1, (numComponentes == 0) ? "" : "s");
    }

    // Libera a memória alocada
    free(pilha->array);
    free(pilha);
    free(grafoTransposto);
    free(componente);
}

// Função para executar a funcionalidade 12void executarFuncionalidade12(const char* nomeArquivo, int n)
void caminhoCurto(char* nomeArquivo, int n) {
    // Abrir o arquivo binário para leitura
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Falha na execução da funcionalidade.\n");
        return;
    }

    // Lê o número de registros no arquivo
    int numRegistros;
    fread(&numRegistros, sizeof(int), 1, arquivo);

    // Inicializa o grafo
    Vertice* grafo = inicializarGrafo(numRegistros);

    // Lê os registros do arquivo
    for (int i = 0; i < numRegistros; i++) {
        fread(&grafo[i], sizeof(Vertice), 1, arquivo);
        // Aloca espaço para as arestas do vértice
        grafo[i].arestas = (Aresta*)malloc(grafo[i].numArestas * sizeof(Aresta));
        // Lê as arestas do arquivo
        fread(grafo[i].arestas, sizeof(Aresta), grafo[i].numArestas, arquivo);
    }

    // Fecha o arquivo
    fclose(arquivo);

    // Realiza a funcionalidade
    for (int i = 0; i < n; i++) {
        char tecnologiaOrigem[100];
        char tecnologiaDestino[100];

        // Lê as tecnologias de origem e destino
        scanf("%s %s", tecnologiaOrigem, tecnologiaDestino);

        int origem = -1;
        int destino = -1;

        // Encontra os índices dos vértices correspondentes às tecnologias
        for (int j = 0; j < numRegistros; j++) {
            if (strcmp(grafo[j].nomeTecnologia, tecnologiaOrigem) == 0) {
                origem = j;
            }
            if (strcmp(grafo[j].nomeTecnologia, tecnologiaDestino) == 0) {
                destino = j;
            }
        }

        // Verifica se as tecnologias foram encontradas
        if (origem == -1 || destino == -1) {
            printf("Registro inexistente.\n");
        } else {
            // Verifica se há caminho entre as tecnologias
            if (bfs(grafo, numRegistros, origem, destino)) {
                // Calcula o caminho mais curto usando Dijkstra
                int pesoCaminho = dijkstra(grafo, numRegistros, origem, destino);
                printf("%s %s: %d\n", tecnologiaOrigem, tecnologiaDestino, pesoCaminho);
            } else {
                printf("%s %s: CAMINHO INEXISTENTE\n", tecnologiaOrigem, tecnologiaDestino);
            }
        }
    }

    // Libera a memória alocada
    for (int i = 0; i < numRegistros; i++) {
        free(grafo[i].arestas);
    }
    free(grafo);
}