#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "paginas.h"
#include "lista.h"
#include "gerais.h"

usuario individuo = {"", 0};

// Função para informar a quantidade de usuario no arquivo
int sizeof_vetor()
{

    //Abrindo o arquivo de score
    FILE * arq_temp = fopen("C:\\Project_gamer\\assets\\save\\score.dat","rb");

    //Verificando em casos do arquivo não abrir
    if(!arq_temp)
    {
        return 0;
    }

    int cont = 0;
    usuario temp; // Variável temporaria

    while (fread(&temp, sizeof(usuario), 1, arq_temp))
    {
        cont++;
    }

    fclose(arq_temp); // Fecha o arquivo

    return cont;
}

//Tela de registro de usuario
int menu_registro(ALLEGRO_EVENT_QUEUE * queue, ALLEGRO_EVENT event, bool redraw, bool done, ALLEGRO_FONT * font)
{
    // Variavel 
    float mover = 0;

    // Area da fonte
    ALLEGRO_FONT* font_registro = al_load_font("C:\\Project_gamer\\assets\\fontes\\font_2.otf", 35, 0);
    must_init(font_registro, "font");

    // Area da imagem
    ALLEGRO_BITMAP* registro_img = al_load_bitmap("C:\\Project_gamer\\assets\\imagens\\registro.png");
    must_init(registro_img, "registro_img");

    //Abertura de arquivo
    FILE * lista_playersb = fopen("C:\\Project_gamer\\assets\\save\\lista_players.dat","wb");

    if (!lista_playersb)
    {
        printf("Problema ao abrir arquivo\n");
        return SAIR;
    }
    
    while (1)
    {
        al_wait_for_event(queue, &event);
    
            switch(event.type)
            {
                case ALLEGRO_EVENT_TIMER:
                    // Lógica do jogo se precisar
                    redraw = true;
                    break;
    
                case ALLEGRO_EVENT_KEY_DOWN:
                    if (event.keyboard.keycode == ALLEGRO_KEY_ENTER|| event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
                    {
                        if (strlen(individuo.username) > 0)
                        {
                            fwrite(&individuo, sizeof(usuario), 1, lista_playersb);
                            fclose(lista_playersb);
                            return MENU;
                        }
                    }
                    
                    break;
                
                case ALLEGRO_EVENT_KEY_CHAR:
                    if (strlen(individuo.username) < 10)
                    {
                        char temp[] = {event.keyboard.unichar, '\0'};

                        // Logica para digitar palavras e aparecer na tela
                        if (event.keyboard.unichar == ' ' ||
                            (event.keyboard.unichar >= '0' && event.keyboard.unichar <= '9')||
                            (event.keyboard.unichar >= 'A' && event.keyboard.unichar <= 'Z')||
                            (event.keyboard.unichar >= 'a' && event.keyboard.unichar <= 'z'))
                        {
                            minusculo(temp); // Deixa a palavra minuscula
                            strcat(individuo.username,temp);
                            mover+=4.5;
                        }
                        
                        
                    }
                    // Lógica para apagar palavra
                    if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(individuo.username) != 0)
                    {
                        individuo.username[strlen(individuo.username) - 1] = '\0'; // Ele vai deletar a letra da palavra
                        mover-=4.5;
                    }
                    
                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }
    
            if(done)
                return SAIR;
    
            if(redraw && al_is_event_queue_empty(queue))
            {
                
                al_draw_bitmap(registro_img, 0,0,0);
                if (strlen(individuo.username) > 0)
                {
                    al_draw_text(font_registro, al_map_rgb(255, 255, 255), 355 - mover, 232, 0, individuo.username);
                }
                al_flip_display();
    
                
                redraw = false;
            }

    }

    al_destroy_font(font_registro);
    al_destroy_bitmap(registro_img);
}

int menu_principal(ALLEGRO_EVENT_QUEUE * queue, ALLEGRO_EVENT event, bool redraw, bool done, ALLEGRO_FONT * font, int atual)
{
    // Area da imagem
    ALLEGRO_BITMAP* menu_img = al_load_bitmap("C:\\Project_gamer\\assets\\imagens\\menu.png");
    must_init(menu_img, "menu_img");

    FILE * file_b = fopen("C:\\Project_gamer\\assets\\save\\score.dat","rb+");

    if (!file_b)
    {
        file_b = fopen("C:\\Project_gamer\\assets\\save\\score.dat","wb+");
    }
    
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
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    strcpy(individuo.username, "");
                    return atual - 1;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_1 || event.keyboard.keycode == ALLEGRO_KEY_PAD_1)
                {
                    // 1. Garante que o ponteiro está no inicio para começar a procura
                    rewind(file_b); 
                    
                    usuario temp_score = {"",0};
                    bool usuario_encontrado = false;

                    // 2. Procura se o usuário já existe
                    while (fread(&temp_score, sizeof(usuario), 1, file_b) == 1)
                    {
                        if (strcmp(temp_score.username, individuo.username) == 0)
                        {
                            printf("USUARIO ENCONTRADO\n");
                            usuario_encontrado = true;
                            break; // Para de procurar, já achamos
                        }
                    }

                    // 3. Se percorreu tudo e NÃO achou, adiciona no final
                    if (!usuario_encontrado)
                    {
                        printf("USUARIO nao ENCONTRADO\n");
                        fseek(file_b, 0, SEEK_END); // Vai para o final do arquivo
                        
                        usuario novo_registro;
                        strcpy(novo_registro.username, individuo.username);
                        novo_registro.score = individuo.score;
                        
                        fwrite(&novo_registro, sizeof(usuario), 1, file_b);
                    }

                    // Importante: Fechar o arquivo antes de sair da função
                    fclose(file_b);
                    
                    return GAMER;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_2 || event.keyboard.keycode == ALLEGRO_KEY_PAD_2)
                {
                    return SCORE;
                }
                
                else if (event.keyboard.keycode == ALLEGRO_KEY_3 || event.keyboard.keycode == ALLEGRO_KEY_PAD_3)
                {
                    return SAIR;
                }
            
                else
                {
                    break;
                }
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }
    
            if(done)
                return SAIR;
    
            if(redraw && al_is_event_queue_empty(queue))
            {
                al_draw_bitmap(menu_img, 0,0,0);
                al_flip_display();
    
                redraw = false;
            }

    }

    al_destroy_bitmap(menu_img);
}

int menu_score(ALLEGRO_EVENT_QUEUE * queue, ALLEGRO_EVENT event, bool redraw, bool done, ALLEGRO_FONT * font)
{

    // Area da imagem
    ALLEGRO_BITMAP* score_img = al_load_bitmap("C:\\Project_gamer\\assets\\imagens\\score.png");
    must_init(score_img, "score_img");

    //Area fonte
    ALLEGRO_FONT* font_score = al_load_font("C:\\Project_gamer\\assets\\fontes\\font_2.otf", 35, 0);
    must_init(font_score, "font");

    ALLEGRO_FONT* font_score_2 = al_load_font("C:\\Project_gamer\\assets\\fontes\\font_2.otf", 50, 0);
    must_init(font_score_2, "font");

    FILE * arq_ind = fopen("C:\\Project_gamer\\assets\\save\\lista_players.dat","rb");
    if (!arq_ind)
    {
        printf("Score indisponível\n");
        return MENU;
    }
    usuario infor_particular = {"", 0};
    
    while (fread(&infor_particular, sizeof(usuario), 1, arq_ind) == 1){}
    fclose(arq_ind);

    FILE * arq_b = fopen("C:\\Project_gamer\\assets\\save\\score.dat","rb");
    if (!arq_b)
    {
        printf("Score indisponível\n");
        return MENU;
    }
    
    bool existencia = true;

    int n = sizeof_vetor(); // Informa o tamanho do vetor

    // Criação de lista de dados para ordenar score
    Lista lista_dados;
    lista_dados.inicio = NULL;
    
    if (n == 0)
    {
        existencia = false;
    }
    else
    {
        usuario temp_score;
        rewind(arq_b);
        
        int c = 0;

        while (fread(&temp_score, sizeof(usuario), 1, arq_b) == 1)
        {
            lista_final(&lista_dados, temp_score);

            if (strcmp(temp_score.username,infor_particular.username) == 0)
            {
                infor_particular = temp_score;
            }
        }

        // Ordenar em descrescente
        ordenar_lista(&lista_dados);
        
    }
    fclose(arq_b);
    
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
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    {
                        deletar_lista(&lista_dados);
                        return MENU;
                    }
                    
                    break;
                
                case ALLEGRO_EVENT_KEY_CHAR:

                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }
    
            if(done)
                return SAIR;
    
            if(redraw && al_is_event_queue_empty(queue))
            {
                if (existencia)
                {
                    al_draw_bitmap(score_img,0,0,0);
                    mostrar_lista(&lista_dados, font_score_2);
                    al_draw_textf(font_score, al_map_rgb(25, 55, 13), 216, 400, 0, "PLAYER: %s", infor_particular.username);
                    al_draw_textf(font_score, al_map_rgb(25, 55, 13), 420, 400, 0, "SCORE: %d", infor_particular.score);

                    al_flip_display();
                    
                    
                }
                else
                {
                    al_clear_to_color(al_map_rgb(50, 100, 0));
                    al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "PAGINA DE SCORE VAZIA");
                }
                al_flip_display();
                
                redraw = false;
            }

    }

}

