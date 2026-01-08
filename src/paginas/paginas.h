#ifndef PAGINAS_H
#define PAGINAS_H


#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

enum paginaGame {
    REGISTRO,
    MENU,
    SCORE,
    CONFIG,
    GAMER,
    PAUSE,
    GAMER_OVER,
    SAIR
};


typedef struct
{
    char username [10];
    int score;
    
}usuario;

#define SCREEN_W 720 // Largura da tela 
#define SCREEN_H 500 // Altura da tela
#define FPS 40

// SETOR DE MENU
int menu_registro(ALLEGRO_EVENT_QUEUE * queue, ALLEGRO_EVENT event, bool redraw, bool done, ALLEGRO_FONT * font);
int menu_principal(ALLEGRO_EVENT_QUEUE * queue, ALLEGRO_EVENT event, bool redraw, bool done, ALLEGRO_FONT * font, int atual);
int menu_score(ALLEGRO_EVENT_QUEUE * queue, ALLEGRO_EVENT event, bool redraw, bool done, ALLEGRO_FONT * font);
// int menu_configuracoes();

// //SETOR DE PARTIDA
// int inicio_gamer();
// int pausa();
// int gamer_over();


// FUNÇÃO TEMPORARIA
void must_init(bool test, const char *description);

#endif