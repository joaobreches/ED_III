#include "aux.h"
#include <stdio.h> 
#include <stdlib.h>

void leBinario(){

    FILE *arq = fopen("arquivo/binario/indice1.bin", "rb");

    CabecalhoooArvoreB rC;
    No noAux;

    // Lendo o cabeçalho
    fread(&rC.status, sizeof(char), 1, arq);
    fread(&rC.noRaiz, sizeof(int), 1, arq);
    fread(&rC.RRNproxNo, sizeof(int), 1, arq);
    fread(rC.lixo, sizeof(char), LIXO_CABECALHO, arq);

    printf("\nRegistro de Cabecalho da Arvore B:");
    printf("\n\tStatus = %c\n\tRRN da Raiz = %d\n\tRRN do prox No = %d\n\tLixo = ", rC.status, rC.noRaiz, rC.RRNproxNo);
    for(int i = 0; i < LIXO_CABECALHO; i++) {
        printf("%c" , rC.lixo[i]);
    }

    for(int i = 0; i < rC.RRNproxNo; i++) {
        // Le um No 
        fread(&noAux.nroChavesNo, sizeof(int), 1, arq);
        fread(&noAux.AlturaNo, sizeof(int), 1, arq);
        fread(&noAux.RRNdoNo, sizeof(int), 1, arq);
        for(int j = 0; j < ORDEM - 1; j++) {
            fread(&noAux.P[j], sizeof(int), 1, arq);
            fread(noAux.dado[j].C, sizeof(char), 55, arq);
            fread(&noAux.dado[j].Pr, sizeof(int), 1, arq);
        }
        fread(&noAux.P[ORDEM - 1], sizeof(int), 1, arq);

        // Printa o No
        printf("\n\nRegistro de RRN %d:", i);
        printf("\n\tNumero de chaves do no = %d", noAux.nroChavesNo);
        printf("\n\tAltura do No = %d", noAux.AlturaNo);
        printf("\n\tRRN do no = %d", noAux.RRNdoNo);
        for(int k = 0; k < ORDEM - 1; k++) {
            printf("\n\tP%d = %d", k, noAux.P[k]);
            printf("\n\tC%d = ", k);
            for(int j = 0; j < 55; j++) {
                printf("%c", noAux.dado[k].C[j]);
            }
            printf("\n\tPr%d = %d", k, noAux.dado[k].Pr);
        }
        printf("\n\tP%d = %d", ORDEM-1, noAux.P[ORDEM - 1]);
    }
}