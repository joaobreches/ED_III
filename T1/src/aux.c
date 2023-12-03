#include "aux.h"
#include <stdio.h> 
#include <stdlib.h>

void leBinario(){

    FILE *arq = fopen("arquivo/binario/i1.bin", "rb");
    FILE *txt = fopen("arquivo/txt/gerado1.txt", "w");

    CabecalhoooArvoreB rC;
    No noAux;

    // Lendo o cabeçalho
    fread(&rC.status, sizeof(char), 1, arq);
    fread(&rC.noRaiz, sizeof(int), 1, arq);
    fread(&rC.RRNproxNo, sizeof(int), 1, arq);
    fread(rC.lixo, sizeof(char), LIXO_CABECALHO, arq);

    fprintf(txt, "\nRegistro de Cabecalho da Arvore B:");
    fprintf(txt, "\n\tStatus = %c\n\tRRN da Raiz = %d\n\tRRN do prox No = %d\n\tLixo = ", rC.status, rC.noRaiz, rC.RRNproxNo);
    for(int i = 0; i < LIXO_CABECALHO; i++) {
        fprintf(txt, "%c" , rC.lixo[i]);
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
        fprintf(txt, "\n\nRegistro de RRN %d:", i);
        fprintf(txt, "\n\tNumero de chaves do no = %d", noAux.nroChavesNo);
        fprintf(txt, "\n\tAltura do No = %d", noAux.AlturaNo);
        fprintf(txt, "\n\tRRN do no = %d", noAux.RRNdoNo);
        for(int k = 0; k < ORDEM - 1; k++) {
            fprintf(txt, "\n\tP%d = %d", k, noAux.P[k]);
            fprintf(txt, "\n\tC%d = ", k);
            for(int j = 0; j < 55; j++) {
                fprintf(txt, "%c", noAux.dado[k].C[j]);
            }
            fprintf(txt, "\n\tPr%d = %d", k, noAux.dado[k].Pr);
        }
        fprintf(txt, "\n\tP%d = %d", ORDEM-1, noAux.P[ORDEM - 1]);
    }

    fclose(arq);
    fclose(txt);
}


void comparaBinarios() {
    FILE *arqMonitor = fopen("arquivo/binario/indice1.bin", "rb");
    FILE *arqGerado = fopen("arquivo/binario/i1.bin", "rb");
    FILE *txt = fopen("arquivo/txt/comp1.txt", "w");

    char ger, monitor;
    int i = 0;

    while(fread(&monitor, 1, 1, arqMonitor) != EOF && fread(&ger, 1, 1, arqGerado) != EOF) {
        if(monitor != ger) {
            fprintf(txt, "\nO byte %d eh diferente nos dois arquivos!", i);
            fprintf(txt, "\n\tByte gerado = %c", ger);
            fprintf(txt, "\n\tByte do monitor = %c", monitor);
        }
        i++;
    }

    fclose(arqMonitor);
    fclose(arqGerado);
    fclose(txt);
}