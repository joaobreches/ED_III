#include "grafo.h"

// Função para inicializar um grafo com paremetros iniciais
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

        // libera arestas
        for(int j = 0; j < grafo.vertices[i]->grauSaida; j++){
            grafo.vertices[i]->ini = aresta->prox;
            free(aresta);
            aresta = grafo.vertices[i]->ini;
        }
        // libera strings e vertices
        free(grafo.vertices[i]->nomeTecnologia);
        free(grafo.vertices[i]);
    }
    // libera array de vertices
    free(grafo.vertices);
}

// imprime os valores do grafo de acordo com o formato exigido
void imprimeGrafo(Grafo grafo){
    for(int i = 0; i < grafo.numVertices; i++){ // percorre os vertices
        Vertice* v = grafo.vertices[i];
        Aresta* aresta = v->ini;

        for(int j = 0; j < v->grauSaida; j++){ // percorre as arestas
            printf("%s %d %d %d %d %s %d\n", v->nomeTecnologia, v->grupo, v->grauEntrada,  v->grauSaida, v->grau, aresta->destino->nomeTecnologia, aresta->peso);
            aresta = aresta->prox;
        }
    }
}

// adiciona um vertice ao grafo
void adicionaVertice(Grafo *grafo, char *nomeTecnologia, int grupo){
    // aloca estruturas e variaveis dinamicamente
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
    grafo->vertices[grafo->numVertices]->visitado = 0;
    grafo->vertices[grafo->numVertices]->ini = NULL;

    // aumenta a contagem de quantidade de vertices em 1
    grafo->numVertices++;
}

// Função para adicionar uma aresta ao grafo
void adicionaAresta(Vertice** vertices, int origem, int destino, int peso) {
    // aloca memoria dinamicamente
    Aresta* arestaNova = malloc(sizeof(Aresta));
    if(arestaNova == NULL){
        printf("Falha na execução da funcionalidade\n");
        exit(1);
    }

    // define parametros da aresta sendo criada
    arestaNova->destino = vertices[destino]; 
    arestaNova->peso = peso; 
    arestaNova->prox = NULL; 

    Aresta* arestaAtual = vertices[origem]->ini;
    if(arestaAtual == NULL) // verifica se a aresta eh a primeira do vertice
        vertices[origem]->ini = arestaNova;
    else {
        Aresta* arestaAnterior = NULL; // define aresta auxiliar
        while(arestaAtual != NULL){
            // busca posicao a ser inserida (insercao ordenada)
            if(strcmp(arestaAtual->destino->nomeTecnologia, vertices[destino]->nomeTecnologia) > 0)
                break;
            arestaAnterior = arestaAtual;
            arestaAtual = arestaAtual->prox;
        }

        if(arestaAnterior == NULL){ // verifica se a aresta sera inserida no inicio do encadeamento
            vertices[origem]->ini = arestaNova;
        }
        else {
            arestaAnterior->prox = arestaNova;
        }

        arestaNova->prox = arestaAtual; // insere a nova aresta na posicao correta
    }
        
    vertices[origem]->grauSaida++; // aumenta o grau de saida da tecnologia de origem em 1
    vertices[origem]->grau++; // aumenta o grau total da tecnologia de origem em 1
    
    vertices[destino]->grauEntrada++; // aumenta o grau de entrada da tecnologia de saida em 1
    vertices[destino]->grau++; // aumenta o grau total da tecnologia de saida em 1
}

// Função para criar um grafo (transposto ou nao)
Grafo criaGrafo(FILE* arquivo, bool transposto){
    fseek(arquivo, 0, SEEK_SET); // incia o ponteiro no começo do arquivo

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
        // lê registro do arquivo binario
        if(!leRegistroNaoNulo(arquivo, &registro))
            continue;
        
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
        
        // cria aresta e a adiciona ao grafo
        if(transposto) // se o grafo for transposto inverte as tecnologias de origem e destino
            adicionaAresta(grafo.vertices, verticeDestino, verticeOrigem, registro.peso);
        else
            adicionaAresta(grafo.vertices, verticeOrigem, verticeDestino, registro.peso);

        // libera memoria alocada
        free(registro.TecnologiaOrigem.string);
        free(registro.TecnologiaDestino.string);
    }

    // ordena os vertices do grafo
    quicksortVertice(grafo.vertices, grafo.numVertices); 

    return grafo;
}

// Função para buscar o indice correspondente de um vertice em um grafo
int buscaIndice(Grafo grafo, char* nomeTecnologia){
    int indice = 0;

    // busca vertice como mesmo nome de tecnologia do buscado
    while (strcmp(grafo.vertices[indice]->nomeTecnologia, nomeTecnologia) != 0){
        indice++;
        if(indice > grafo.numVertices) // se nao encontrado retorna -1
            return -1;
    }

    return indice; // retorna o indice do vertice
}

// Funções de busca em profundidade para algoritmo de Kosaraju
void dfs(Vertice* vertice, Pilha* pilha) {
    // marca o vertice como ja visitado
    vertice->visitado = 1;

    // desce as arestas do vertice recursivamente caso nao tenham sido visitadas
    Aresta* aresta = vertice->ini;
    while (aresta != NULL) {
        if (!aresta->destino->visitado) {
            dfs(aresta->destino, pilha);
        }
        aresta = aresta->prox;
    }
    
    // adiciona vertice na pilha
    empilhar(pilha, vertice);
}

// Função para calcular o caminho mais curto usando o algoritmo de Dijkstra
int dijkstra(Vertice** grafo, int numVertices, int origem, int destino) {
    // Fila para a BFS
    int* visitado = (int*)malloc(numVertices * sizeof(int));
    for (int i = 0; i < numVertices; i++) {
        visitado[i] = 0;
    }
    int frente = 0;
    int tras = 0;
    int* fila = (int*)malloc(numVertices * sizeof(int));
    fila[tras++] = origem;
    int* distancia = (int*)malloc(numVertices * sizeof(int));

    // Inicia a busca a partir do vértice de origem
    for (int i = 0; i < numVertices; i++) {
        distancia[i] = INT_MAX;
    }

    distancia[origem] = 0;

    while (frente != tras){
        int verticeAtual = fila[frente++];
        Vertice* v = grafo[verticeAtual];
        int vizinhoPos;
        Aresta* aresta = v->ini;

        visitado[verticeAtual] = 1;
        for(int j = 0; j < v->grauSaida; j++){
            Vertice* vizinho = aresta->destino;
            for(int k = 0; k < numVertices; k++){
                if(strcmp(grafo[k]->nomeTecnologia, vizinho->nomeTecnologia) == 0){
                    vizinhoPos = k;
                    break;
                }
            }
            if((distancia[verticeAtual] + aresta->peso) < distancia[vizinhoPos]){
                distancia[vizinhoPos] = distancia[verticeAtual] + aresta->peso;
            }
            aresta = aresta->prox;
        }

        int menorD = INT_MAX;
        int menorPos;
        for(int l = 0; l < numVertices; l++){
            if(distancia[l] <= menorD && visitado[l] != 1){
                menorD = distancia[l];
                menorPos = l;
            }
        }

        if(menorD < INT_MAX)
            fila[tras++] = menorPos;
    }



    int resultado = distancia[destino];

    free(distancia);
    free(fila);
    free(visitado);

    return resultado;
}

// Função de ordenação dos vertices do grafo (etapa recursiva)
// Implementada baseada nos trabalhos realizados na disciplina de Estrutura de Dados II
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

// Função de ordenação dos vertices do grafo (etapa geral)
// Implementada baseada nos trabalhos realizados na disciplina de Estrutura de Dados II
void quicksortVertice(Vertice **v, int n) {
    quicksortRecursivoVertice(v, 0, n - 1); // inicia recursividade
}

// Função para inicializar a pilha
Pilha* inicializarPilha(int tamanho) {
    // define parametros iniciais da pilha
    Pilha *pilha = (Pilha*)malloc(sizeof(Pilha));
    pilha->array = (Vertice*)malloc(tamanho * sizeof(Vertice));
    pilha->topo = -1;
    return pilha;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(Pilha* pilha) {
    return pilha->topo == -1;
}

// Função para empilhar um elemento na pilha
void empilhar(Pilha* pilha, Vertice* item) {
    if(pilha == NULL)
        return;
    pilha->array[++pilha->topo] = *item;
}

// Função para desempilhar um elemento da pilha
Vertice* desempilhar(Pilha* pilha) {
    if (!pilhaVazia(pilha)) {
        return &(pilha->array[pilha->topo--]);
    }
    return NULL; // Indica pilha vazia
}
