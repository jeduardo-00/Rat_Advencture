#ifndef GAMER_H
#define GAMER_H


#define KEY_SEEN     1
#define KEY_DOWN     2
// #define NUM_ELEMENTOS 2
#define SCREEN_W 720 // Largura da tela 
#define SCREEN_H 500 // Altura da tela
#define RATIM_W 80 // Largura personagem
#define RATIM_H 80 // Altura personagem
#define BLOCK_W 72
#define BLOCK_H 68
#define SPAWN_Y 380 // Ponto de inicio do boneco
#define SPAWN_X 150
#define SPAWN_BLOCK_X 0
#define SPAWN_BLOCK_Y 400
#define SPAWN_CLOCK_X 0
#define SPAWN_CLOCK_Y 350
#define JOGANDO 0
#define GAME_OVER 1
#define FPS 40
#define TEMPO 30.0

enum paginaGame {REGISTRO, MENU, SCORE, CONFIG, GAMER, PAUSE, GAMER_OVER, SAIR};

// //SETOR DE PARTIDA
int inicio_gamer(ALLEGRO_EVENT_QUEUE * queue, ALLEGRO_EVENT event, bool redraw, bool done, ALLEGRO_TIMER* timer);
// int pausa();
// int gamer_over();

#endif