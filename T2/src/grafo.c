#include "grafo.h"

// Função para inicializar um grafo
Grafo inicializarGrafo() {
    Grafo grafo;
    grafo.numVertices = 0;
    grafo.vertices = malloc(sizeof(Vertice));
    return grafo;
}

// libera toda a memoria alocada no grafo
void liberaGrafo(Grafo grafo) {
    for (int i = 0; i < grafo.numVertices; i++) {
        free(grafo.vertices[i].arestas);
    }
    free(grafo.vertices);
}

// imprime os valores do grafo de acordo com o formato exigido
void imprimeGrafo(Grafo grafo){
    for(int i = 0; i < grafo.numVertices; i++)
        for(int j = 0; j < grafo.vertices[i].grauSaida; j++)
            printf("origem %s, grupo %d, entrada %d, saida %d, grau %d, destino %s, peso %d\n", grafo.vertices[i].nomeTecnologia, grafo.vertices[i].grupo, grafo.vertices[i].grauEntrada,  grafo.vertices[i].grauSaida, grafo.vertices[i].grau, grafo.vertices[i].arestas[j].destino->nomeTecnologia, grafo.vertices[i].arestas[j].peso);
}

// adiciona um vertice ao grafo
void adicionaVertice(Grafo *grafo, char *nomeTecnologia, int grupo){
    grafo->vertices = (Vertice*)realloc(grafo->vertices, (grafo->numVertices + 1) * sizeof(Vertice));
    if(grafo->vertices == NULL){
        perror("Falha na execução da funcionalidade");
        exit(1);
    }
    
    // define parametros iniciais do vertice
    strcpy(grafo->vertices[grafo->numVertices].nomeTecnologia, nomeTecnologia);
    grafo->vertices[grafo->numVertices].grupo = grupo;
    grafo->vertices[grafo->numVertices].grauEntrada = 0;
    grafo->vertices[grafo->numVertices].grauSaida = 0;
    grafo->vertices[grafo->numVertices].grau = 0;
    grafo->vertices[grafo->numVertices].numArestas = 0;
    grafo->vertices[grafo->numVertices].visitado = 0;

    // aumenta a contagem de quantidade de vertices em 1
    grafo->numVertices++;
}

// Função para adicionar uma aresta ao grafo
void adicionaAresta(Vertice* vertices, int origem, int destino, int peso) {
    vertices[origem].arestas = (Aresta*)realloc(vertices[origem].arestas, (vertices[origem].grauSaida + 1) * sizeof(Aresta)); // aloca memoria para a nova aresta
    
    vertices[origem].arestas[vertices[origem].grauSaida].destino->nomeTecnologia = &vertices[destino]; // define destino da aresta
    printf("tec destino: %s\n", vertices[origem].arestas[vertices[origem].grauSaida].destino->nomeTecnologia);
    vertices[origem].arestas[vertices[origem].grauSaida].peso = peso; // define peso da aresta
    
    vertices[origem].grauSaida++; // aumenta o grau de saida da tecnologia de origem em 1
    vertices[origem].grau++; // aumenta o grau total da tecnologia de origem em 1
    
    vertices[destino].grauEntrada++; // aumenta o grau de entrada da tecnologia de saida em 1
    vertices[destino].grau++; // aumenta o grau total da tecnologia de saida em 1
}

Grafo criaGrafo(FILE* arquivo, bool transposto){
    fseek(arquivo, 0, SEEK_SET);

    // Verifica o status do arquivo
    char statusArquivo;
    fread(&statusArquivo, sizeof(char), 1, arquivo);
    if (statusArquivo == '0') {
        perror("Falha na execução da funcionalidade");
        fclose(arquivo);
        exit(1);
    }

    // le proxRRN (quantidade de registros)
    int numRegistros;
    fread(&numRegistros, sizeof(int), 1, arquivo);

    // inicializa grafo
    Grafo grafo = inicializarGrafo();

    // pula o cabecalho do arquivo binario (deixa o ponteiro no inicio do primeiro registro)
    Registro registro;
    skipCabecalho(arquivo);

    // adiciona registros no grafo
    for (int i = 0; i < numRegistros; i++) {
        // int trava;
        // scanf("%d", &trava);
        // lê registro do arquivo binario
        if(!leRegistroNaoNulo(arquivo, &registro))
            continue;
        imprimeRegistro(registro);
        
        // verifica se as tecnologias de origem e de destinos ja tem vertices, se tiver identifica quais sao os vertices, se nao cria vertices para elas 
        int verticeOrigem = -1;
        int verticeDestino = -1;

        for(int j = 0; j < grafo.numVertices; j++){
            if(strcmp(grafo.vertices[j].nomeTecnologia, registro.TecnologiaOrigem.string) == 0)
                verticeOrigem = j;
            if(strcmp(grafo.vertices[j].nomeTecnologia, registro.TecnologiaDestino.string) == 0)
                verticeDestino = j;
            if(verticeOrigem != -1 && verticeDestino != -1)
                break;
        }

        if(verticeOrigem == -1){
            adicionaVertice(&grafo, registro.TecnologiaOrigem.string, registro.grupo);
            verticeOrigem = grafo.numVertices - 1;
        }

        if(verticeDestino == -1){
            adicionaVertice(&grafo, registro.TecnologiaDestino.string, registro.grupo);
            verticeDestino = grafo.numVertices - 1;
        }

        printf("ANTES vertice %s tem %d arestas\n", grafo.vertices[verticeOrigem].nomeTecnologia, grafo.vertices[verticeOrigem].grauSaida);
        // cria aresta e a adiciona ao grafo
        if(transposto)
            adicionaAresta(grafo.vertices, verticeDestino, verticeOrigem, registro.peso);
        else
            adicionaAresta(grafo.vertices, verticeOrigem, verticeDestino, registro.peso);

        printf("DEPOIS vertice %s tem %d arestas\n", grafo.vertices[verticeOrigem].nomeTecnologia, grafo.vertices[verticeOrigem].grauSaida);
    }

    // quicksortVertice(grafo.vertices, grafo.numVertices); // ordena os vertices do grafo
    // for(int i = 0; i < grafo.numVertices; i++) // ordena a aresta de cada vertice do grafo
    //     quicksortAresta(grafo.vertices[i].arestas, grafo.vertices[i].grauSaida);

    return grafo;
}

// Função para comparar arestas (usada para ordenação)
int compararArestas(const void *a, const void *b) {
    // return strcmp(((Aresta*)a)->nomeTecDestino, ((Aresta*)b)->nomeTecDestino);
}

// Função para realizar uma DFS no grafo
void dfs(Vertice* grafo, int vertice, Pilha* pilha) {
    grafo[vertice].visitado = 1;

    for (int i = 0; i < grafo[vertice].numArestas; i++) {
        int destino = grafo[vertice].arestas[i].destino;
        if (!grafo[destino].visitado) {
            dfs(grafo, destino, pilha);
        }
    }

    empilhar(pilha, vertice);
}

// Função para realizar uma DFS no grafo reverso (transposto)
void dfsTransposto(Vertice* grafo, int vertice, int* componente, int componenteAtual) {
    grafo[vertice].visitado = 1;
    componente[vertice] = componenteAtual;

    for (int i = 0; i < grafo[vertice].numArestas; i++) {
        int destino = grafo[vertice].arestas[i].destino;
        if (!grafo[destino].visitado) {
            dfsTransposto(grafo, destino, componente, componenteAtual);
        }
    }
}

// Função para realizar uma busca em largura no grafo
int bfs(Vertice* grafo, int numVertices, int origem, int destino) {
    int* visitado = (int*)malloc(numVertices * sizeof(int));
    for (int i = 0; i < numVertices; i++) {
        visitado[i] = 0;
    }

    // Fila para a BFS
    int* fila = (int*)malloc(numVertices * sizeof(int));
    int frente = 0;
    int tras = 0;

    // Inicia a busca a partir do vértice de origem
    visitado[origem] = 1;
    fila[tras++] = origem;

    while (frente != tras) {
        int verticeAtual = fila[frente++];
        for (int i = 0; i < grafo[verticeAtual].numArestas; i++) {
            int vizinho = grafo[verticeAtual].arestas[i].destino;
            if (!visitado[vizinho]) {
                visitado[vizinho] = 1;
                fila[tras++] = vizinho;
            }
        }
    }

    int alcancavel = visitado[destino];

    free(visitado);
    free(fila);

    return alcancavel;
}

// Função para calcular o caminho mais curto usando o algoritmo de Dijkstra
int dijkstra(Vertice* grafo, int numVertices, int origem, int destino) {
    int* distancia = (int*)malloc(numVertices * sizeof(int));
    int* visitado = (int*)malloc(numVertices * sizeof(int));

    for (int i = 0; i < numVertices; i++) {
        distancia[i] = INT_MAX;
        visitado[i] = 0;
    }

    distancia[origem] = 0;

    for (int i = 0; i < numVertices - 1; i++) {
        int u = -1;
        for (int j = 0; j < numVertices; j++) {
            if (!visitado[j] && (u == -1 || distancia[j] < distancia[u])) {
                u = j;
            }
        }

        visitado[u] = 1;

        for (int j = 0; j < grafo[u].numArestas; j++) {
            int v = grafo[u].arestas[j].destino;
            int peso = grafo[u].arestas[j].peso;

            if (distancia[u] != INT_MAX && distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
            }
        }
    }

    int resultado = distancia[destino];

    free(distancia);
    free(visitado);

    return resultado;
}