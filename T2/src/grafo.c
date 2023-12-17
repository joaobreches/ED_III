#include "grafo.h"

// Função para inicializar um grafo
Grafo inicializarGrafo() {
    Grafo grafo;
    grafo.numVertices = 0;
    grafo.vertices = NULL;
    return grafo;
}

// libera toda a memoria alocada no grafo
void liberaGrafo(Grafo grafo) {
    for(int i = 0; i < grafo.numVertices; i++){
        Aresta* aresta = grafo.vertices[i]->ini;

        for(int j = 0; j < grafo.vertices[i]->grauSaida; j++){
            grafo.vertices[i]->ini = aresta->prox;
            free(aresta);
            aresta = grafo.vertices[i]->ini;
        }
        free(grafo.vertices[i]->nomeTecnologia);
        free(grafo.vertices[i]);
    }
    free(grafo.vertices);
}

// imprime os valores do grafo de acordo com o formato exigido
void imprimeGrafo(Grafo grafo){
    for(int i = 0; i < grafo.numVertices; i++){
        Vertice* v = grafo.vertices[i];
        Aresta* aresta = v->ini;

        for(int j = 0; j < v->grauSaida; j++){
            printf("%s %d %d %d %d %s %d\n", v->nomeTecnologia, v->grupo, v->grauEntrada,  v->grauSaida, v->grau, aresta->destino->nomeTecnologia, aresta->peso);
            aresta = aresta->prox;
        }
    }
}

// adiciona um vertice ao grafo
void adicionaVertice(Grafo *grafo, char *nomeTecnologia, int grupo){
    // if(strcmp(nomeTecnologia, "ANGULAR2") == 0 || strcmp(nomeTecnologia, "REACTJS") == 0)
    //     printf("------------------TEC %s TEM GRUPO %d\n", nomeTecnologia, grupo);

    grafo->vertices = (Vertice**)realloc(grafo->vertices, (grafo->numVertices + 1) * sizeof(Vertice));
    grafo->vertices[grafo->numVertices] = (Vertice*) malloc(sizeof(Vertice));
    grafo->vertices[grafo->numVertices]->nomeTecnologia = (char*) malloc(50 * sizeof(char));

    if(grafo->vertices == NULL || grafo->vertices[grafo->numVertices] == NULL || grafo->vertices[grafo->numVertices]->nomeTecnologia == NULL){
        perror("Falha na execução da funcionalidade");
        exit(1);
    }

    // define parametros iniciais do vertice
    strcpy(grafo->vertices[grafo->numVertices]->nomeTecnologia, nomeTecnologia);
    grafo->vertices[grafo->numVertices]->grupo = grupo;
    grafo->vertices[grafo->numVertices]->grauEntrada = 0;
    grafo->vertices[grafo->numVertices]->grauSaida = 0;
    grafo->vertices[grafo->numVertices]->grau = 0;
    grafo->vertices[grafo->numVertices]->numArestas = 0;
    grafo->vertices[grafo->numVertices]->visitado = 0;
    grafo->vertices[grafo->numVertices]->ini = NULL;

    // aumenta a contagem de quantidade de vertices em 1
    grafo->numVertices++;
}

// Função para adicionar uma aresta ao grafo
void adicionaAresta(Vertice** vertices, int origem, int destino, int peso) {
    Aresta* arestaNova = malloc(sizeof(Aresta));
    if(arestaNova == NULL){
        printf("Falha na execução da funcionalidade\n");
        exit(1);
    }

    arestaNova->destino = vertices[destino]; 
    arestaNova->peso = peso; 
    arestaNova->prox = NULL; 

    Aresta* arestaAtual = vertices[origem]->ini;
    if(arestaAtual == NULL)
        vertices[origem]->ini = arestaNova;
    
    else {
        Aresta* arestaAnterior = NULL;
        while(arestaAtual != NULL){
            if(strcmp(arestaAtual->destino->nomeTecnologia, vertices[destino]->nomeTecnologia) > 0)
                break;
            arestaAnterior = arestaAtual;
            arestaAtual = arestaAtual->prox;
        }

        if(arestaAnterior == NULL){
            vertices[origem]->ini = arestaNova;
        }
        else {
            arestaAnterior->prox = arestaNova;
        }

        arestaNova->prox = arestaAtual;
    }
        
    vertices[origem]->grauSaida++; // aumenta o grau de saida da tecnologia de origem em 1
    vertices[origem]->grau++; // aumenta o grau total da tecnologia de origem em 1
    
    vertices[destino]->grauEntrada++; // aumenta o grau de entrada da tecnologia de saida em 1
    vertices[destino]->grau++; // aumenta o grau total da tecnologia de saida em 1

    // if(strcmp(vertices[origem]->nomeTecnologia, "AZURE") == 0){
    //     Vertice* v = vertices[origem];
    //     Aresta* aresta = v->ini;

    //     for(int j = 0; j < v->grauSaida; j++){
    //         printf("\n\n%s %d %d %d %d %s %d\n\n", v->nomeTecnologia, v->grupo, v->grauEntrada,  v->grauSaida, v->grau, aresta->destino->nomeTecnologia, aresta->peso);
    //         aresta = aresta->prox;
    //     }
    // }
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
        // imprimeRegistro(registro);
        
        // verifica se as tecnologias de origem e de destinos ja tem vertices, se tiver identifica quais sao os vertices, se nao cria vertices para elas 
        int verticeOrigem = -1;
        int verticeDestino = -1;

        for(int j = 0; j < grafo.numVertices; j++){
            if(strcmp(grafo.vertices[j]->nomeTecnologia, registro.TecnologiaOrigem.string) == 0){
                verticeOrigem = j;
                if(grafo.vertices[verticeOrigem]->grupo == -1)
                    grafo.vertices[verticeOrigem]->grupo = registro.grupo;
            }
            if(strcmp(grafo.vertices[j]->nomeTecnologia, registro.TecnologiaDestino.string) == 0)
                verticeDestino = j;
            if(verticeOrigem != -1 && verticeDestino != -1)
                break;
        }

        if(verticeOrigem == -1){
            adicionaVertice(&grafo, registro.TecnologiaOrigem.string, registro.grupo);
            verticeOrigem = grafo.numVertices - 1;
        }

        if(verticeDestino == -1){
            adicionaVertice(&grafo, registro.TecnologiaDestino.string, -1);
            verticeDestino = grafo.numVertices - 1;
        }
        
        // printf("ANTES vertice %s tem %d arestas\n", grafo.vertices[verticeOrigem].nomeTecnologia, grafo.vertices[verticeOrigem].grauSaida);
        // cria aresta e a adiciona ao grafo
        if(transposto)
            adicionaAresta(grafo.vertices, verticeDestino, verticeOrigem, registro.peso);
        else
            adicionaAresta(grafo.vertices, verticeOrigem, verticeDestino, registro.peso);
        // printf("DEPOIS vertice %s tem %d arestas\n\n", grafo.vertices[verticeOrigem].nomeTecnologia, grafo.vertices[verticeOrigem].grauSaida);

        free(registro.TecnologiaOrigem.string);
        free(registro.TecnologiaDestino.string);
    }

    quicksortVertice(grafo.vertices, grafo.numVertices); // ordena os vertices do grafo

    return grafo;
}

// // Função para comparar arestas (usada para ordenação)
// int compararArestas(const void *a, const void *b) {
//     return strcmp(((Aresta*)a)->nomeTecDestino, ((Aresta*)b)->nomeTecDestino);
// }

// Função para realizar uma DFS no grafo
// void dfs(Vertice* vertice, Pilha* pilha) {
//     vertice->visitado = 1;

//     Aresta* atual = vertice->ini;
//     while (atual != NULL) {
//         Vertice* destino = atual->destino;
//         if (!destino->visitado) {
//             dfs(destino, pilha);
//         }
//         atual = atual->prox;
//     }

//     empilhar(pilha, vertice);
// }

// // Função para realizar uma DFS no grafo reverso (transposto)
// void dfsTransposto(Vertice* vertice, int* componente, int componenteAtual) {
//     vertice->visitado = 1;
//     componente[vertice] = componenteAtual;

//     Aresta* atual = vertice->ini;

//     for (int i = 0; i < vertice->numArestas; i++) {
//         Vertice* destino = atual->destino;
//         if (!destino->visitado) {
//             dfsTransposto(destino, componente, componenteAtual);
//         }
//         atual = atual->prox;
//     }
// }

// Função para realizar uma busca em largura no grafo
// int bfs(Vertice* vertice, int numVertices, int origem, int destino) {
//     int* visitado = (int*)malloc(numVertices * sizeof(int));
//     for (int i = 0; i < numVertices; i++) {
//         visitado[i] = 0;
//     }

//     // Fila para a BFS
//     int* fila = (int*)malloc(numVertices * sizeof(int));
//     int frente = 0;
//     int tras = 0;

//     // Inicia a busca a partir do vértice de origem
//     visitado[origem] = 1;
//     fila[tras++] = origem;

//     while (frente != tras) {
//         int verticeAtual = fila[frente++];
//         for (int i = 0; i < vertice[verticeAtual].numArestas; i++) {
//             Vertice* vizinho = verticeAtual.destino;
//             if (!visitado[vizinho]) {
//                 visitado[vizinho] = 1;
//                 fila[tras++] = vizinho;
//             }
//         }
//     }

//     int alcancavel = visitado[destino];

//     free(visitado);
//     free(fila);

//     return alcancavel;
// }

// // Função para calcular o caminho mais curto usando o algoritmo de Dijkstra
// int dijkstra(Vertice* grafo, int numVertices, int origem, int destino) {
//     int* distancia = (int*)malloc(numVertices * sizeof(int));
//     int* visitado = (int*)malloc(numVertices * sizeof(int));
//     Aresta grafo;
//     for (int i = 0; i < numVertices; i++) {
//         distancia[i] = INT_MAX;
//         visitado[i] = 0;
//     }

//     distancia[origem] = 0;

//     for (int i = 0; i < numVertices - 1; i++) {
//         int u = -1;
//         for (int j = 0; j < numVertices; j++) {
//             if (!visitado[j] && (u == -1 || distancia[j] < distancia[u])) {
//                 u = j;
//             }
//         }

//         visitado[u] = 1;

//         for (int j = 0; j < grafo[u].numArestas; j++) {
//             int v = grafo[u].arestas[j].destino;
//             int peso = grafo[u].arestas[j].peso;

//             if (distancia[u] != INT_MAX && distancia[u] + peso < distancia[v]) {
//                 distancia[v] = distancia[u] + peso;
//             }
//         }
//     }

//     int resultado = distancia[destino];

//     free(distancia);
//     free(visitado);

//     return resultado;
// }

void quicksortRecursivoVertice(Vertice **v, int ini, int fim) {
    Vertice *pivo, *aux;
    int meio, i, j;
    
    if (ini >= fim)
        return; // caso trivial, vetor unitario
    
    // define o pivo
    meio = ini + (fim - ini) / 2;
    pivo = v[meio];
    
    // faz iteracao para colocar o pivo no lugar
    i = ini;
    j = fim;
    while (1) {
        // andar o i
        for (; strcmp(v[i]->nomeTecnologia, pivo->nomeTecnologia) < 0; i++);
        
        // andar o j
        for (; strcmp(v[j]->nomeTecnologia, pivo->nomeTecnologia) > 0; j--);
        
        // verificar se i encontrou j
        if (i == j)
            break; // ambos se encontraram; iteracao termina
        
        // troca
        aux = v[i];
        v[i] = v[j];
        v[j] = aux;
    }
    
    // processa as metades inferior e superior recursivamente
    quicksortRecursivoVertice(v, ini, i - 1);
    quicksortRecursivoVertice(v, i + 1, fim);
}

void quicksortVertice(Vertice **v, int n) {
    quicksortRecursivoVertice(v, 0, n - 1); // inicia recursividade
}