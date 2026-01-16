#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define SCREEN_W 720 // Largura da tela 
#define SCREEN_H 500 // Altura da tela
#define FPS 40


enum paginaGame {REGISTRO, MENU, SCORE, CONFIG, GAMER, PAUSE, GAMER_OVER, SAIR}

// Função void para verificar a funcionalidade dos comandos init
void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}


int tela_inicial(ALLEGRO_EVENT_QUEUE * queue, ALLEGRO_EVENT event, bool redraw, bool done)
{
    while (1)
    {
        al_wait_for_event(queue, &event);
    
            switch(event.type)
            {
                case ALLEGRO_EVENT_TIMER:
                    // game logic goes here.
                    redraw = true;
                    break;
    
                case ALLEGRO_EVENT_KEY_DOWN:
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }
    
            if(done)
                break;
    
            if(redraw && al_is_event_queue_empty(queue))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "REGISTRO DE PERSONAGEM");
                al_flip_display();
    
                redraw = false;
            }

            return SAIR;
    }
    
}



int main()
{
    // Variavel
    float x = 0.0, y = 0.0;
    int momento = 0;
    //Variável de T e F
    bool done = false;
    bool redraw = true;

     // ------------------------------------ÁREA DE INSTALAÇÃO----------------------------
    // Implementação allegro
    must_init(al_init(), "allegro");
    
    // Instalando função de entrada
    must_init(al_install_keyboard(), "keyboard");

    // Implementando fontes
    must_init(al_init_font_addon(), "fonte");
    must_init(al_init_ttf_addon(), "texto_fonte");

    // Area do time
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    must_init(timer, "timer");
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");
    
    // Area do display 
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(SCREEN_W, SCREEN_H);
    must_init(disp, "display");
    al_set_window_title(disp, "Rat Adventure");

    // Area da fonte
    ALLEGRO_FONT* font = al_load_font("C:\\Project_gamer\\assets\\fontes\\font.ttf", 20, 0);
    must_init(font, "font");

    ALLEGRO_EVENT event;
    // ----------------------------Registrando eventos--------------------------------------------
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer); //inicia o tempo

    while(!SAIR)
    {
        if (momento == REGISTRO)
        { 
            tela_inicial(queue,event,redraw,done);
        }
        else
        {
            momento = SAIR;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}