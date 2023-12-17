#include "algoritmos.h"

// void quicksortRecursivoAresta(Aresta *a, int ini, int fim) {
//     printf("na sort aresta recursivo\n");
//     Aresta pivo, aux;
//     int meio, i, j;
    
//     if (ini >= fim)
//         return; // caso trivial, vetor unitario
    
//     // define o pivo
//     meio = ini + (fim - ini) / 2;
//     pivo = a[meio];
    
//     // faz iteracao para colocar o pivo no lugar
//     i = ini;
//     j = fim;
//     while (1) {
//         int trava;
//         scanf("%d", &trava);
//         printf("i %d: %s, j %d: %s, pivo %d: %s\n", i, a[i].destino->nomeTecnologia, j, a[j].destino->nomeTecnologia, meio, pivo.destino->nomeTecnologia);
//         // andar o i
//         for (; strcmp(a[i].destino->nomeTecnologia, pivo.destino->nomeTecnologia) < 0; i++);
        
//         // andar o j
//         for (; strcmp(a[j].destino->nomeTecnologia, pivo.destino->nomeTecnologia) > 0; j--);
        
//         // verificar se i encontrou j
//         if (i == j)
//             break; // ambos se encontraram; iteracao termina
        
//         // troca
//         aux = a[i];
//         a[i] = a[j];
//         a[j] = aux;
//     }
    
//     // processa as metades inferior e superior recursivamente
//     quicksortRecursivoAresta(a, ini, i - 1);
//     quicksortRecursivoAresta(a, i + 1, fim);
// }

// void quicksortAresta(Aresta *a, int n) {
//     quicksortRecursivoAresta(a, 0, n - 1); // inicia recursividade
// }

// Função para comparar strings (usada para ordenação)
int compararStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}