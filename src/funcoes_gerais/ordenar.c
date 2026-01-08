#include <stdio.h>
#include <stdlib.h>
#include "paginas.h"


int particao(usuario v[], int inicio, int fim) {
    int pivo = v[fim].score;  // pivô = pontuação do último
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        // ordem DECRESCENTE  (por isso > em vez de <)
        if (v[j].score > pivo) {
            i++;
            usuario temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }

    // coloca o pivô na posição correta
    usuario temp = v[i + 1];
    v[i + 1] = v[fim];
    v[fim] = temp;

    return i + 1;
}

void quicksort(usuario v[], int inicio, int fim) {
    if (inicio < fim) {
        int p = particao(v, inicio, fim);

        quicksort(v, inicio, p - 1);
        quicksort(v, p + 1, fim);
    }
}