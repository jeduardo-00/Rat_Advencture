#ifndef LISTA_H      // Se não estiver definido…
#define LISTA_H      // Define o cabeçalho


#include "paginas.h"

// Estrutura de cada elemnto dentro da lista
typedef struct No
{
    usuario dados; // Informa o dado
    struct No * prox; // Informa o endereço a qual aponta

}No;

// Lista que contém NÓ que contem dado + end a qual aponta
typedef struct 
{
    No * inicio; // Inicio da lista

}Lista;

// FUNÇÕES

void lista_final(Lista * list, usuario dado);
void mostrar_lista(Lista * lista, ALLEGRO_FONT * font);
void ordenar_lista(Lista * list);
void deletar_lista(Lista * list);


#endif