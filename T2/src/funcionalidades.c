#include "funcionalidades.h"

// Função para executar a funcionalidade 8 - cria grafo e imprime-o
void criaGrafo(char *nomeArquivo) {
    // abre arquivo binario para leitura
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Falha na execução da funcionalidade");
        fclose(arquivo);
        exit(1);
    }

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

        // cria aresta e a adiciona ao grafo
        adicionaAresta(grafo.vertices, verticeOrigem, verticeDestino, registro.peso);
    }

    quicksortVertice(grafo.vertices, grafo.numVertices); // ordena os vertices do grafo
    // for(int i = 0; i < grafo.numVertices; i++) // ordena a aresta de cada vertice do grafo
    //     quicksortAresta(grafo.vertices[i].arestas, grafo.vertices[i].grauSaida);
    imprimeGrafo(grafo);

    // Libera a memória alocada
    liberaGrafo(grafo);
    fclose(arquivo);
}

// Função para executar a funcionalidade 9 - cria grafo transposto e imprime-o
void criaGrafoTransposto(char *nomeArquivo) {
    // abre arquivo binario para leitura
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Falha na execução da funcionalidade");
        fclose(arquivo);
        exit(1);
    }

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

        // cria aresta e a adiciona ao grafo
        adicionaAresta(grafo.vertices, verticeDestino, verticeOrigem, registro.peso);
    }

    quicksortVertice(grafo.vertices, grafo.numVertices); // ordena os vertices do grafo
    // for(int i = 0; i < grafo.numVertices; i++) // ordena a aresta de cada vertice do grafo
    //     quicksortAresta(grafo.vertices[i].arestas, grafo.vertices[i].grauSaida);
    imprimeGrafo(grafo);

    // Libera a memória alocada
    liberaGrafo(grafo);
    fclose(arquivo);
}

// // Função para executar a funcionalidade 10
// void listaNomes(char *nomeArquivo, int n) {
    
//   // Abrir o arquivo binário para leitura
//   FILE *arquivo = abreBinarioLeitura(nomeArquivo);


//   // Lê o número de registros no arquivo
//   int numRegistros;
//   fread(&numRegistros, sizeof(int), 1, arquivo);

//   // Aloca espaço para os vértices
//   Vertice *vertices = (Vertice*)malloc(numRegistros * sizeof(Vertice));

//   // Lê os registros do arquivo
//   for (int i = 0; i < numRegistros; i++) {
//       fread(&vertices[i], sizeof(Vertice), 1, arquivo);
//       // Aloca espaço para as arestas do vértice
//       vertices[i].arestas = (Aresta*)malloc(vertices[i].numArestas * sizeof(Aresta));
//       // Lê as arestas do arquivo
//       fread(vertices[i].arestas, sizeof(Aresta), vertices[i].numArestas, arquivo);
//   }

//   // Fecha o arquivo
//   fclose(arquivo);

//   Registro registro;

//     // Realiza a funcionalidade n vezes
//     for (int k = 0; k < n; k++) {
//         // Lê o nome da tecnologia passado como parâmetro
//         char tecnologia[50];
//         fgets(tecnologia, sizeof(tecnologia), stdin);
//         tecnologia[strcspn(tecnologia, "\n")] = '\0'; // Remove o caractere de nova linha

//         // Encontra o índice correspondente ao nome passado como parâmetro
//         int indice = 0;
//         while (strcmp(vertices[indice].nomeTecnologia, tecnologia) != 0) {
//             indice++;
//             if (indice >= numRegistros) {
//                 printf("Registro inexistente.\n\n");
//                 continue;
//             }
//         }

//         // Imprime a tecnologia passada como parâmetro
//         printf("\"%s\": ", tecnologia);

//         // Ordena as arestas pelo nomeTecOrigem para a saída ordenada
//         qsort(vertices[indice].arestas, vertices[indice].numArestas, sizeof(Aresta), compararArestas);

//         // Imprime as tecnologias que originaram a tecnologia passada como parâmetro
//         int primeiraTecnologia = 1;
//         for (int j = 0; j < vertices[indice].numArestas; j++) {
//             if (primeiraTecnologia) {
//                 // printf("%s", vertices[indice].arestas[j].nomeTecOrigem);
//                 primeiraTecnologia = 0;
//             } else {
//                 // printf(", %s", vertices[indice].arestas[j].nomeTecOrigem);
//             }
//         }

//         printf("\n\n");
//     }

//     // Libera a memória alocada
//     for (int i = 0; i < numRegistros; i++) {
//         free(vertices[i].arestas);
//     }
//     free(vertices);
// }

// // Função para determinar se o grafo é fortemente conexo
// void fortementeConexo(char* nomeArquivo) {
//     // Abrir o arquivo binário para leitura
//     FILE* arquivo = abreBinarioLeitura(nomeArquivo);
//     if (arquivo == NULL) {
//         printf("Falha na execução da funcionalidade.\n");
//         return;
//     }

//     // Lê o número de registros no arquivo
//     int numRegistros;
//     fread(&numRegistros, sizeof(int), 1, arquivo);

//     // Inicializa o grafo
//     Vertice* grafo = inicializarGrafo(numRegistros);

//     // Lê os registros do arquivo
//     for (int i = 0; i < numRegistros; i++) {
//         fread(&grafo[i], sizeof(Vertice), 1, arquivo);
//         // Aloca espaço para as arestas do vértice
//         grafo[i].arestas = (Aresta*)malloc(grafo[i].numArestas * sizeof(Aresta));
//         // Lê as arestas do arquivo
//         fread(grafo[i].arestas, sizeof(Aresta), grafo[i].numArestas, arquivo);
//     }

//     // Fecha o arquivo
//     fclose(arquivo);

//     // Realiza a funcionalidade
//     Pilha* pilha = inicializarPilha(numRegistros);

//     // Realiza a primeira DFS para preencher a pilha
//     for (int i = 0; i < numRegistros; i++) {
//         if (!grafo[i].visitado) {
//             dfs(grafo, i, pilha);
//         }
//     }

//     // Inicializa o grafo transposto (grafo reverso)
//     Vertice* grafoTransposto = inicializarGrafo(numRegistros);

//     // Inicializa um vetor para armazenar os componentes fortemente conexos
//     int *componente = (int*)malloc(numRegistros * sizeof(int));

//     // Realiza a DFS no grafo transposto para calcular os componentes fortemente conexos
//     int componenteAtual = 0;
//     while (!pilhaVazia(pilha)) {
//         int vertice = desempilhar(pilha);
//         if (!grafoTransposto[vertice].visitado) {
//             dfsTransposto(grafo, vertice, componente, componenteAtual);
//             componenteAtual++;
//         }
//     }

//     // Verifica quantos componentes fortemente conexos foram encontrados
//     int numComponentes = 0;
//     for (int i = 0; i < numRegistros; i++) {
//         if (componente[i] > numComponentes) {
//             numComponentes = componente[i];
//         }
//     }

//     // Verifica se o grafo é fortemente conexo
//     if (numComponentes == 0) {
//         printf("Nao, o grafo nao e fortemente conexo e possui 0 componentes.\n");
//     } else {
//         printf("Sim, o grafo e fortemente conexo e possui %d componente%s.\n", numComponentes + 1, (numComponentes == 0) ? "" : "s");
//     }

//     // Libera a memória alocada
//     free(pilha->array);
//     free(pilha);
//     free(grafoTransposto);
//     free(componente);
// }

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
//     Vertice *grafo = inicializarGrafo(numRegistros);

//     // Lê os registros do arquivo
//     Aresta aresta;
//     for (int i = 0; i < numRegistros; i++) {
//         fread(&grafo[i], sizeof(Vertice), 1, arquivo);
//         // Aloca espaço para as arestas do vértice
//         grafo[i].arestas = (Aresta*)malloc(grafo[i].numArestas * (TAM_NOME * sizeof(char) + sizeof(int)));
//         // Lê as arestas do arquivo
//         fread(aresta.nomeTecDestino, TAM_NOME * sizeof(char) + sizeof(int), grafo[i].numArestas, arquivo);
//         fread(aresta.nomeTecOrigem, TAM_NOME * sizeof(char) + sizeof(int), grafo[i].numArestas, arquivo);
//         fread(aresta.peso, TAM_NOME * sizeof(int) + sizeof(int), grafo[i].numArestas, arquivo);
//         fread(aresta.destino, TAM_NOME * sizeof(int) + sizeof(int), grafo[i].numArestas, arquivo);
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
//             if (strcmp(grafo[j].nomeTecnologia, tecnologiaOrigem) == 0) {
//                 origem = j;
//             }
//             if (strcmp(grafo[j].nomeTecnologia, tecnologiaDestino) == 0) {
//                 destino = j;
//             }
//         }

//         // Verifica se as tecnologias foram encontradas
//         if (origem == -1 || destino == -1) {
//             printf("Registro inexistente.\n");
//         } else {
//             // Verifica se há caminho entre as tecnologias
//             if (bfs(grafo, numRegistros, origem, destino)) {
//                 // Calcula o caminho mais curto usando Dijkstra
//                 int pesoCaminho = dijkstra(grafo, numRegistros, origem, destino);
//                 printf("%s %s: %d\n", tecnologiaOrigem, tecnologiaDestino, pesoCaminho);
//             } else {
//                 printf("%s %s: CAMINHO INEXISTENTE\n", tecnologiaOrigem, tecnologiaDestino);
//             }
//         }
//     }

//     // Libera a memória alocada
//     liberaGrafo(grafo);
// }