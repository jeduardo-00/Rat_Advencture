#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "paginas.h"
#include "lista.h"

// Inserir itém ao final da lista
void lista_final(Lista * list, usuario dado)
{
    No * novo = malloc(sizeof(No));
    novo->dados = dado;
    novo->prox = NULL;

    // Verificar se a lista está vazia
    if (list->inicio == NULL)
    {
        list->inicio = novo;
    }
    else
    {
        No * pi; // Ponteito para flutuar na lista
        for (pi = list->inicio; pi->prox!=NULL; pi = pi->prox);
        pi->prox = novo; // Faz o ponteiro que apontava pra nada coemçar a apontar para a struct do dado 
        

    }

}

// Mostrando a lista por completo
void mostrar_lista(Lista * lista, ALLEGRO_FONT * font)
{
    No * pi = lista->inicio;

    if (pi == NULL)
    {
        return;
    }
    else
    {
        int i = 0;
        int move = 0;
        for (pi = lista->inicio; pi != NULL && i < 3; pi = pi->prox)
        {
            al_draw_textf(font, al_map_rgb(255, 255, 255), 211, 163 + move, 0, "%d - ", i+1);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 250, 163 + move, 0, "%s", pi->dados.username);
            al_draw_textf(font, al_map_rgb(25, 55, 13), 470, 163 + move, 0, "%d", pi->dados.score);
            i++;
            move+=45;
        }
    }
    
}

// Irá ordenar a lista
void ordenar_lista(Lista * list)
{
    if (list->inicio == NULL)
    {
        return;
    }
    No * pi;
    No * pj;
    No * pfim = NULL;

    for (pi = list->inicio; pi->prox != NULL; pi = pi->prox)
    {
        for (pj = list->inicio; pj->prox != pfim; pj = pj->prox)
        {
            if (pj->dados.score < pj->prox->dados.score)
            {
                // Faz a movimentação entre estruturas 
                usuario temp = pj->dados;
                pj->dados = pj->prox->dados;
                pj->prox->dados = temp;
            }
            
        }
        // A cada giro concluido faz o pfim andar para esquerda
        pfim = pj;
    }
    
}

// Deletar lista
void deletar_lista(Lista * list)
{
    if (list->inicio == NULL)
    {
        return;
    }
    
    No * pi;
    while (list->inicio != NULL)
    {
        pi = list->inicio;
        list->inicio = pi->prox;
        free(pi);
    }
}