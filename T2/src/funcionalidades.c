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

    // cria e imprime grafo
    Grafo grafo = criaGrafo(arquivo, transposto);
    imprimeGrafo(grafo);

    // Libera a memória alocada
    liberaGrafo(grafo);
    fclose(arquivo);
}

// Função para executar a funcionalidade 10
void listaNomes(char *nomeArquivo, int n, char** strings) {
    // abre arquivo binario para leitura
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Falha na execução da funcionalidade");
        fclose(arquivo);
        exit(1);
    }

    // cria grafo transposto e libera memoria alocada
    Grafo grafoTransposto = criaGrafo(arquivo, 1);
    fclose(arquivo);

    // Realiza a funcionalidade n vezes
    for (int k = 0; k < n; k++) {
        char* tecnologia = strings[k+3];

        // limpa a string da tecnologia (remove as aspas)
        int i;
        for(i = 0; tecnologia[i] != '\0'; i++){
            tecnologia[i] = tecnologia[i + 1];
        }
        tecnologia[i - 2] = '\0';

        // Busca o indice correspondente ao nome passado como parâmetro
        int indice = buscaIndice(grafoTransposto, tecnologia);
        if (indice == -1) { // tecnologia nao encontrada
            printf("Registro inexistente.\n\n");
            continue;
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

        // a tecnologia nao foi gerada por outra
        if (grafoTransposto.vertices[indice]->grauSaida == 0)
            printf("Registro inexistente.");

        printf("\n\n");
    }

    // Libera a memória alocada
    liberaGrafo(grafoTransposto);
}

// Função para determinar se o grafo é fortemente conexo
void fortementeConexo(char* nomeArquivo) {
    printf("%s\n", nomeArquivo); 
    // Abrir o arquivo binário para leitura
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Falha na execução da funcionalidade");
        fclose(arquivo);
        exit(1);
    }

    // Inicializa o grafo
    Grafo grafo = criaGrafo(arquivo, 0);

    // Inicializa a pilha
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

    // Inicializa contagem de componentes fortemente conexos
    int numComponentes = 0;

    // Realiza a DFS no grafo transposto desempilhando-o
    while (!pilhaVazia(pilha)) {
        Vertice* vertice = desempilhar(pilha); // desempilha o vertice usado no grafo original

        int indice = buscaIndice(grafoTransposto, vertice->nomeTecnologia); // busca o vertice correspondente no grafo transposto

        vertice = grafoTransposto.vertices[indice]; // atribui o vertice no grafo transposto ao vertice utilizado
        
        // realiza a DFS no grafo transposto e atualiza a contagem de componentes
        if (!vertice->visitado) {
            dfs(vertice, NULL);
            numComponentes++;
        }
    }

    // Verifica se o grafo é fortemente conexo
    if (numComponentes > 1) {
        printf("Não, o grafo não é fortemente conexo e possui %d componentes.\n", numComponentes);
    } else {
        printf("Sim, o grafo é fortemente conexo e possui 1 componente.\n");
    }

    // Libera a memória alocada
    free(pilha->array);
    free(pilha);
    liberaGrafo(grafo);
    liberaGrafo(grafoTransposto);
}

// Função para executar a funcionalidade 12void executarFuncionalidade12(const char* nomeArquivo, int n)
void caminhoCurto(char* nomeArquivo, int n, char** strings) {
    // Abrir o arquivo binário para leitura
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Falha na execução da funcionalidade.\n");
        return;
    }

    // Inicializa o grafo
    Grafo grafo = criaGrafo(arquivo, 0);
    Vertice** vertice = grafo.vertices;

    
    // Realiza a funcionalidade
    for (int i = 0; i < n; i++) {
        char* tecnologiaOrigem = strings[3 + i *2];
        char* tecnologiaDestino = strings[2* i + 4];

        int origem = -1;
        int destino = -1;

        int k;
        for(k = 0; tecnologiaDestino[k] != '\0'; k++){
            tecnologiaDestino[k] = tecnologiaDestino[k + 1];
        }
        tecnologiaDestino[k - 2] = '\0';

        for(k = 0; tecnologiaOrigem[k] != '\0'; k++){
            tecnologiaOrigem[k] = tecnologiaOrigem[k + 1];
        }
        tecnologiaOrigem[k - 2] = '\0';

        // Encontra os índices dos vértices correspondentes às tecnologias
        for (int j = 0; j < grafo.numVertices; j++) {
            if (strcmp(vertice[j]->nomeTecnologia, tecnologiaOrigem) == 0) {
                origem = j;
            }
            if (strcmp(vertice[j]->nomeTecnologia, tecnologiaDestino) == 0) {
                destino = j;
            }
        }

        // Verifica se as tecnologias foram encontradas
        if (origem == -1 || destino == -1) {
            printf("Registro inexistente.\n");
        } else {
            int pesoCaminho = dijkstra(vertice, grafo.numVertices, origem, destino);
            // Verifica se há caminho entre as tecnologias
            if (pesoCaminho < INT_MAX) {
                // Calcula o caminho mais curto usando Dijkstra
                printf("%s %s: %d\n", tecnologiaOrigem, tecnologiaDestino, pesoCaminho);
            } else {
                printf("%s %s: CAMINHO INEXISTENTE.\n", tecnologiaOrigem, tecnologiaDestino);
            }
        }
    }

    fclose(arquivo);
    liberaGrafo(grafo);
}