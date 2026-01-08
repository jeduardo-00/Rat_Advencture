#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "gamer.h"
#include "paginas.h"


int main()
{
    // Variavel
    int momento = REGISTRO;


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

    //Audio 
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    // Imagem 
    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* icon = al_load_bitmap("C:\\Project_gamer\\assets\\imagens\\icon.png");
    must_init(icon, "icon");

    
    // Uso das formas geometrica
    must_init(al_init_primitives_addon(), "primitives");
    
    // Area do time
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    must_init(timer, "timer");
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); // Variável responsável pela fila de espera 
    must_init(queue, "queue");
    
    // Area do display 
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    
    ALLEGRO_DISPLAY* disp = al_create_display(SCREEN_W, SCREEN_H);
    must_init(disp, "display");
    al_set_window_title(disp, "Rat Adventure");
    
    // Logo
    al_set_display_icon(disp,icon);
    // Area da fonte
    ALLEGRO_FONT* font = al_load_font("C:\\Project_gamer\\assets\\fontes\\font_2.otf", 70, 0);
    must_init(font, "font");

    ALLEGRO_EVENT event;
    // ----------------------------Registrando eventos--------------------------------------------
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer); //inicia o tempo

    while(momento != SAIR)
    {
        switch(momento)
        {
            case REGISTRO:
                printf("Entrando em REGISTRO...\n"); // Para debug
                momento = menu_registro(queue,event,redraw,done,font);
                printf("Saindo de REGISTRO, indo para momento = %d\n", momento); // Para debug
                break;
            
            case MENU:
                printf("Entrando em MENU PRINCIPAL...\n"); // Para debug
                momento = menu_principal(queue,event,redraw,done,font,momento);
                printf("Saindo de MENU PRINCIPAL, indo para momento = %d\n", momento); // Para debug
                break;
            
            case SCORE:
                printf("Entrando em SCORE...\n");
                momento = menu_score(queue,event,redraw,done,font);
                printf("Saindo de SCORE, indo para momento = %d\n", momento);

                break;
            
            case GAMER:
                printf("Entrando em PARTIDA...\n"); // Para debug
                momento = inicio_gamer(queue,event,redraw,done,timer);
                printf("Saindo de PARTIDA, indo para momento = %d\n", momento); // Para debug
                break;

            case SAIR:
                momento = SAIR;
                break;
        }

    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}