#include "funcionalidades.h"

// Função para executar a funcionalidade 8 e 9 - cria grafo (transposto ou nao) e imprime-o
void recuperaDadosGrafo(char *nomeArquivo, bool transposto) {
    // abre arquivo binario para leitura
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Falha na execução da funcionalidade");
        fclose(arquivo);
        exit(1);
    }

    Grafo grafo = criaGrafo(arquivo, transposto);
    imprimeGrafo(grafo);

    // Libera a memória alocada
    liberaGrafo(grafo);
    fclose(arquivo);
}

// Função para executar a funcionalidade 10
void listaNomes(char *nomeArquivo, int n) {
    // abre arquivo binario para leitura
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Falha na execução da funcionalidade");
        fclose(arquivo);
        exit(1);
    }

    Grafo grafoTransposto = criaGrafo(arquivo, 1);
    fclose(arquivo);

    // Realiza a funcionalidade n vezes
    for (int k = 0; k < n; k++) {
        char tecnologia[TAM_REGISTRO_FIXO];

        // le o nome da tecnologia a ser buscada
        scanf("%s", tecnologia);

        // limpa a tecnologia se for uma string (remove as aspas)
        int i;
        for(i = 0; tecnologia[i] != '\0'; i++){
            tecnologia[i] = tecnologia[i + 1];
        }
        tecnologia[i - 2] = '\0';

        // Encontra o índice correspondente ao nome passado como parâmetro
        int indice = 0;
        while (strcmp(grafoTransposto.vertices[indice]->nomeTecnologia, tecnologia) != 0) {
            indice++;
            if (indice > grafoTransposto.numVertices) {
                printf("Registro inexistente.\n\n");
                continue;
            }
        }

        // Imprime a tecnologia passada como parâmetro
        printf("%s: ", tecnologia);

        // Imprime as tecnologias que originaram a tecnologia passada como parâmetro
        bool primeiraTecnologia = 1;
        Aresta* aresta = grafoTransposto.vertices[indice]->ini;
        for (int j = 0; j < grafoTransposto.vertices[indice]->grauSaida; j++) {
            if (primeiraTecnologia) {
                printf("%s", aresta->destino->nomeTecnologia);
                primeiraTecnologia = 0;
            } else {
                printf(", %s", aresta->destino->nomeTecnologia);
            }
            aresta = aresta->prox;
        }
        printf("\n\n");
    }

    // Libera a memória alocada
    liberaGrafo(grafoTransposto);
}

// Função para determinar se o grafo é fortemente conexo
void fortementeConexo(char* nomeArquivo) {
    // Abrir o arquivo binário para leitura
    FILE* arquivo = abreBinarioLeitura(nomeArquivo);
    if (arquivo == NULL) {
        printf("Falha na execução da funcionalidade.\n");
        return;
    }

    // Inicializa o grafo
    Grafo grafo = criaGrafo(arquivo, 0);

    // Realiza a funcionalidade
    Pilha* pilha = inicializarPilha(grafo.numVertices);

    // Realiza a primeira DFS para preencher a pilha
    for (int i = 0; i < grafo.numVertices; i++) {
        if (!grafo.vertices[i]->visitado) {
            dfs(grafo.vertices[i], pilha);
        }
    }

    // Inicializa o grafo transposto (grafo reverso)
    Grafo grafoTransposto = criaGrafo(arquivo, 1);

    // Fecha o arquivo
    fclose(arquivo);

    // Inicializa um vetor para armazenar os componentes fortemente conexos
    // int *componente = (int*)malloc(grafo.numVertices * sizeof(int));

    // Realiza a DFS no grafo transposto para calcular os componentes fortemente conexos
    // int componenteAtual = 0;
    int numComponentes = 0;
    while (!pilhaVazia(pilha)) {
        Vertice* vertice = desempilhar(pilha);
        if (!vertice->visitado) {
            dfs(vertice, NULL);
            // componenteAtual++;
            numComponentes++;
        }
    }

    // Verifica quantos componentes fortemente conexos foram encontrados
    // for (int i = 0; i < grafo.numVertices; i++) {
    //     if (componente[i] > numComponentes) {
    //         numComponentes = componente[i];
    //     }
    // }

    // Verifica se o grafo é fortemente conexo
    if (numComponentes == 0) {
        printf("Nao, o grafo nao e fortemente conexo e possui 0 componentes.\n");
    } else {
        printf("Sim, o grafo e fortemente conexo e possui %d componente%s.\n", numComponentes + 1, (numComponentes == 0) ? "" : "s");
    }

    // Libera a memória alocada
    free(pilha->array);
    free(pilha);
    liberaGrafo(grafo);
    liberaGrafo(grafoTransposto);
    // free(componente);
}

// // Função para executar a funcionalidade 12void executarFuncionalidade12(const char* nomeArquivo, int n)
// void caminhoCurto(char* nomeArquivo, int n) {
//     // Abrir o arquivo binário para leitura
//     FILE *arquivo = fopen(nomeArquivo, "rb");
//     if (arquivo == NULL) {
//         printf("Falha na execução da funcionalidade.\n");
//         return;
//     }

//     // Lê o número de registros no arquivo
//     int numRegistros;
//     fread(&numRegistros, sizeof(int), 1, arquivo);

//     // Inicializa o grafo
//     Vertice *vertice;

//     // Lê os registros do arquivo
//     Aresta aresta;
//     Registro registro;
//     skipCabecalho(arquivo);

//     for (int i = 0; i < numRegistros; i++) {
//         //lê registro do arquivo
//         if(!leRegistroNaoNulo(arquivo, &registro))
//             continue;
//         imprimeRegistro(registro);
//     }


//     // Fecha o arquivo
//     fclose(arquivo);

//     // Realiza a funcionalidade
//     for (int i = 0; i < n; i++) {
//         char tecnologiaOrigem[TAM_NOME];
//         char tecnologiaDestino[TAM_NOME];

//         // Lê as tecnologias de origem e destino
//         scanf("%s %s", tecnologiaOrigem, tecnologiaDestino);

//         int origem = -1;
//         int destino = -1;

//         // Encontra os índices dos vértices correspondentes às tecnologias
//         for (int j = 0; j < numRegistros; j++) {
//             if (strcmp(vertice[j].nomeTecnologia, tecnologiaOrigem) == 0) {
//                 origem = j;
//             }
//             if (strcmp(vertice[j].nomeTecnologia, tecnologiaDestino) == 0) {
//                 destino = j;
//             }
//         }

//         // Verifica se as tecnologias foram encontradas
//         if (origem == -1 || destino == -1) {
//             printf("Registro inexistente.\n");
//         } else {
//             // Verifica se há caminho entre as tecnologias
//             if (bfs(vertice, numRegistros, origem, destino)) {
//                 // Calcula o caminho mais curto usando Dijkstra
//                 int pesoCaminho = dijkstra(vertice, numRegistros, origem, destino);
//                 printf("%s %s: %d\n", tecnologiaOrigem, tecnologiaDestino, pesoCaminho);
//             } else {
//                 printf("%s %s: CAMINHO INEXISTENTE\n", tecnologiaOrigem, tecnologiaDestino);
//             }
//         }
//     }
// }