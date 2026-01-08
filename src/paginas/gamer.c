#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "gamer.h"
#include "paginas.h"

//Estrutura 

typedef struct
{
    float x, y, posx,posy,posw,posh, mini_w, mini_h;
    int clock;
}obj;

// Função void para verificar a funcionalidade dos comandos init
void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

// Função de colisão
bool colidir(obj* personagem,float*vx, float*vy, bool * on_ground, int block_x, int block_y, int * score, int * cont, float * ac)
{
    
    // Hitboxes do rato e do bloco
    float rato_esquerda = personagem->x; 
    float rato_direita = personagem->x + personagem->mini_w;
    float rato_topo = personagem->y;
    float rato_base = personagem->y + personagem->mini_h;

    float block_esquerda = block_x;
    float block_direita = block_x + BLOCK_W;
    float block_topo = block_y;
    float block_base = block_y + BLOCK_H;


    // Verificar colisão de sobreposição
    if (rato_direita > block_esquerda &&
        rato_esquerda < block_direita &&
        rato_base > block_topo &&
        rato_topo < block_base)
    {
        float overlap_x1 = rato_direita - block_esquerda;
        float overlap_x2 = block_direita - rato_esquerda;
        float overlap_y1 = rato_base - block_topo;
        float overlap_y2 = block_base - rato_topo;

        float overlap_x = 0;
        float overlap_y = 0;

        // Verificando a colisão pelas laterais 
        if (overlap_x1 < overlap_x2){
            overlap_x = overlap_x1;
        }
        else{
            overlap_x = overlap_x2;
        }

        // Verificando a colisão pela vertical
        if (overlap_y1 < overlap_y2){
            overlap_y = overlap_y1;
        }
        else{
            overlap_y = overlap_y2;
        }

        if (overlap_x < overlap_y)
        {
            // A colisão é HORIZONTAL.
            if (overlap_x1 < overlap_x2) // O rato veio da esquerda
            {
                personagem->x = block_esquerda - personagem->mini_w; // Empurra o rato para a esquerda
            }
            else // O rato veio da direita
            {
                personagem->x = block_direita; // Empurra o rato para a direita
            }
            // *vx = 0; // Para a velocidade horizontal
            *vy = 0;
        }
        else
        {
            // A colisão é VERTICAL.
            if (overlap_y1 < overlap_y2) // O rato veio de cima (pousando)
            {
                *cont+=1;
                personagem->y = block_topo - personagem->mini_h; // Pousa em cima do bloco
                *vy = 0; // Para a velocidade vertical
                // *vx = 0;
                *on_ground = true; // Avisa que o jogador está no chão
                if (*cont == 1 && *vx >= BLOCK_W)
                {
                    *score+=1;
                    *ac += 0.0001; 
                }
                
                
                
            }
            else // O rato veio de baixo (batendo a cabeça)
            {
                personagem->y = block_base; // Empurra para baixo
                *vy = 0; // Para a velocidade vertical
                // *vx = 0;
            }
        }
        return true; // Sim, uma colisão foi resolvida
  
    }

    return false; // Nenhuma colisão ocorreu

}

// Função de cadastrar blocos
void cadastrar_blocos (obj* blocos, int n_block)
{
    int i;
    // Verifica o último vazio
    bool bloco_vazio = false;

    for (i = 0; i < n_block; i++)
    {

        blocos[i].x = i * BLOCK_W; // Posiciona o bloco no espaço

        // Garante uma plataforma inicial para segurança do player
        if (i<3)
        {
            blocos[i].y = SPAWN_BLOCK_Y;
            bloco_vazio = false;
        }
        else
        {
            // Verificar se o bloco anterior foi vazio
            if (bloco_vazio)
            {
                blocos[i].y = SPAWN_BLOCK_Y;
                bloco_vazio = false;    
            }
            else
            {
                // Criar momentos para criar buracos com rand, gerador de numeros aleatorios
                if (rand()%4 ==0)
                {
                    blocos[i].y = SCREEN_H; // Move aquele bloco muito para baixo para sumir
                    bloco_vazio = true;
                }
                else
                {
                    blocos[i].y = SPAWN_BLOCK_Y;
                    bloco_vazio = false;
                }
                
            }
        }
        

        // Adicione aqui os posx, posy do seu sprite de bloco
        blocos[i].posx = 212;
        blocos[i].posy = 315;
        // Adicionar dimensão
        blocos[i].posw = BLOCK_W;
        blocos[i].posh = BLOCK_H;
        // Presença do relógio 
        blocos[i].clock = 0;
    }
}

// Função colidir com relogio
bool colidir_relogio(obj *personagem, int relogio_w, int relogio_h, int bloco_x, int bloco_y)
{
    // Hitboxes do rato e do bloco
    float rato_esquerda = personagem->x; 
    float rato_direita = personagem->x + personagem->mini_w;
    float rato_topo = personagem->y;
    float rato_base = personagem->y + personagem->mini_h;

    float relogio_esquerda = bloco_x + 15;
    float relogio_direita = relogio_esquerda + relogio_w;
    float relogio_topo = bloco_y - relogio_h;
    
    if (rato_esquerda < relogio_direita &&
        rato_direita > relogio_esquerda &&
        rato_base > relogio_topo)
    {
        return true;
    }
    return false;
}

int inicio_gamer(ALLEGRO_EVENT_QUEUE * queue, ALLEGRO_EVENT event, bool redraw, bool done, ALLEGRO_TIMER* timer)
{
    srand(time(NULL));

    //---------------------------VARIÁVEIS-------------------------------------------- 

    //variável Básica
    
    int i = 0;
    float vx = 0, vy = 0;     // velocidades
    float gravity = 2.5;   // força da gravidade
    float x = SPAWN_X; // Posicao na tela em x
    float y = SPAWN_Y; // Posicao na tela em y
    int frame_index = 0; // qual quadro desenhar
    float mini_w = 40; // dimensão pequena do rato em w
    float mini_h = 40; // dimensão pequena do rato em h
    int n_bloco = (SCREEN_W/BLOCK_W)+1; // Quantidade de blocos na tela
    float world_offset = 0.0; // Controla o quanto o mundo já "andou" para a esquerda
    float camera_x = 0.0;
    float camera_alvo_offset = 0.0; // Posição ALVO da câmera (para onde ela quer ir)
    float suavidade = 0.15;
    int estado_jogo = JOGANDO;
    int score = 0;
    int cont = 0;
    float tempo_jogo = TEMPO;
    float acelera = 0.0;
    
    
    // Variavel de estrutura
    obj player = {SPAWN_X,SPAWN_Y,0,144,159,144,55,70,0}; // Rato
    obj bloco[n_bloco]; // Pode usar alocação dinamica com malloc 
    obj relogio = {SPAWN_CLOCK_X, SPAWN_CLOCK_Y, 0,0,47,45,0,0};
    obj barra_tempo = {0,50,0,0,SCREEN_W,20};
    usuario dados;
    usuario cadastro;


    //Variável de T e F
    bool jumping = false;  // controle de pulo
    bool on_ground = false; // Variável para checar se o personagem está no chão
    bool pouso = true;
    bool time = false;
    bool encontrado = false;
    bool salva_arquivo = false;
    
    // Variável
    
    
    // ------------------------------------ÁREA DE INSTALAÇÃO----------------------------

    //Area audio
    ALLEGRO_AUDIO_STREAM* music = al_load_audio_stream("C:\\Project_gamer\\assets\\sons\\musica_main.wav", 4, 2048);
    must_init(music, "music");
    // Define que ela deve tocar em loop
    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
    // Define o volume da musica
    al_set_audio_stream_gain(music, 0.2);
    // Anexa ao mixer padrão (isso já começa a tocar)
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer());

    ALLEGRO_SAMPLE* jump = al_load_sample("C:\\Project_gamer\\assets\\sons\\som_jump.wav");
    must_init(jump, "jump");

    ALLEGRO_SAMPLE* tiktak = al_load_sample("C:\\Project_gamer\\assets\\sons\\som_relogio.wav");
    must_init(tiktak, "tiktak");

    ALLEGRO_SAMPLE* queda = al_load_sample("C:\\Project_gamer\\assets\\sons\\caindo_agua.wav");
    must_init(queda, "queda");

    
    
    // Area da fonte
    ALLEGRO_FONT* font_1 = al_load_font("C:\\Project_gamer\\assets\\fontes\\font_2.otf", 30, 0);
    must_init(font_1, "font");

    ALLEGRO_FONT* font_2 = al_load_font("C:\\Project_gamer\\assets\\fontes\\font_2.otf", 70, 0);
    must_init(font_2, "font");

    ALLEGRO_FONT* font_3 = al_load_font("C:\\Project_gamer\\assets\\fontes\\font_2.otf", 40, 0);
    must_init(font_3, "font");
    
    // Carregando imagem
    ALLEGRO_BITMAP* ratim = al_load_bitmap("C:\\Project_gamer\\assets\\imagens\\ratim_sprite.png");
    must_init(ratim, "ratim");

    ALLEGRO_BITMAP* cenario = al_load_bitmap("C:\\Project_gamer\\assets\\imagens\\cenario.png");
    must_init(cenario, "cenario");

    ALLEGRO_BITMAP* game_over = al_load_bitmap("C:\\Project_gamer\\assets\\imagens\\gameover.png");
    must_init(game_over, "game_over");

    ALLEGRO_BITMAP* icon = al_load_bitmap("C:\\Project_gamer\\assets\\imagens\\icon.png");
    must_init(icon, "icon");
    
    // inicializando blocos
    cadastrar_blocos(bloco, n_bloco);
    

    
    unsigned char key[ALLEGRO_KEY_MAX]; //cria um vetor para armazenar o estado de todas as teclas possíveis.
    memset(key, 0, sizeof(key)); // marca todas as teclas como não pressionadas no início.

    al_start_timer(timer); //inicia o tempo
    
    while(1)
    {
        al_wait_for_event(queue, &event); //espera até ocorrer um evento
        
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
            {
                if (estado_jogo == JOGANDO)
                {
                    if (time)
                    {
                        
                        tempo_jogo -= (1.0/FPS)+acelera;

                        if (tempo_jogo < 0)
                        {
                            tempo_jogo = 0;
                            estado_jogo = GAME_OVER;

                        }
                        else if (tempo_jogo > TEMPO)
                        {
                            tempo_jogo = TEMPO;
                        }
                        
                        
                    }
                    
                    //--------------------------CONTROLE--------------------------
                    if (on_ground)
                    {
                        if(key[ALLEGRO_KEY_LEFT])  player.x -= 5;
                    }

                    if (on_ground && !jumping) // ESTÁ NO CHÃO
                    {
                        
                        bool inicia_pulo = false;
                        
                        
                        if (key[ALLEGRO_KEY_RIGHT]) {
                            vy = -20;    // impulso vertical
                            vx = BLOCK_W;      // deslocamento horizontal pequeno
                            al_play_sample(jump, 5.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            inicia_pulo = true;
                            time = true;
                            player.posx = player.posw;
                            cont = 0;
                        }
                        if (key[ALLEGRO_KEY_SPACE]) {
                            vy = -27;    // impulso vertical mais forte
                            vx = BLOCK_W*2;      // deslocamento horizontal maior
                            al_play_sample(jump, 5.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            inicia_pulo = true;
                            time = true;
                            player.posx = player.posw*2+10.5;
                            cont = 0;
                            }
                        
                        if (inicia_pulo)
                        {
                            jumping = true;
                            on_ground = false; // Saiu do chão
                            camera_alvo_offset += vx; // Define o novo alvo da câmera
                            
                        }
                    }
                    
                    // ----------------------------FISICA APLICADA------------------------- 
                    
                    // Aplica gravidade na velocidade vertical
                    if (!on_ground)
                    {
                        vy += gravity;
                    }
                    
                    // Atualiza a posição com base nas velocidades
                    player.y += vy;
                    player.x = SPAWN_X;
                    on_ground = false;
                    
                    for (i = 0; i <  n_bloco; i++)
                    {
                        // BLOCOS EXISTENTES
                        
                        if (colidir(&player,&vx, &vy, &on_ground, bloco[i].x - world_offset, bloco[i].y, &score, &cont, &acelera)) 
                        {
                            player.posx = 0;
                                 
                        }
                        
                    }
                    
                    for (i = 0; i < n_bloco; i++)
                    {
                        if (bloco[i].clock == 1)
                        {

                            if (colidir_relogio(&player, relogio.posw, relogio.posh, bloco[i].x - world_offset, bloco[i].y))
                            {
                                bloco[i].clock = 0;
                                al_play_sample(tiktak, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                tempo_jogo += 10; 

                            }

                        }
                    }
                    
                    // MOVIMENTO SUAVE DA PLATAFORMA

                    camera_x+=(camera_alvo_offset-camera_x)*suavidade; // Suavisa cada movimento
                    world_offset = camera_x; // Implementa a cada frame a nova posiçãodo bloco
                    
                    // Atualiza o estado para o próximo frame
                    pouso = !on_ground;

                    if (on_ground)
                    {
                        jumping = false;
                    }
                    
                    // Loop para RECICLAR os blocos que saíram da tela
                    for (i = 0; i < n_bloco; i++)
                    {
                        // Se a posição REAL do bloco na tela (bloco.x - world_offset) está fora, à esquerda...
                        if ((bloco[i].x - world_offset) < -BLOCK_W)
                        {
                            // ...movemos ele para o final da fila, à direita
                            bloco[i].x += n_bloco * BLOCK_W;

                            int indice_anterior;

                            if (i == 0)
                            {
                                // Se 'i' for 0 (o primeiro bloco), o anterior é o último.
                                indice_anterior = n_bloco - 1;
                            }
                            else
                            {
                                // Para qualquer outro bloco, o anterior é simplesmente i - 1.
                                indice_anterior = i - 1;
                            }
                            
                            if (bloco[indice_anterior].y > SPAWN_BLOCK_Y)
                            {
                                //CRIA UM BLOCO
                                bloco[i].y = SPAWN_BLOCK_Y;
                            }
                            else
                            {
                                if (rand() % 4 == 0)
                                {
                                    bloco[i].y = SCREEN_H;
                                    bloco[i].clock = 0;
                                }
                                else
                                {
                                    bloco[i].y = SPAWN_BLOCK_Y;

                                    if (rand()%8 == 0)
                                    {
                                        bloco[i].clock = 1;
                                    }
                                    else
                                    {
                                        bloco[i].clock = 0;
                                    }
                                }
                            
                            }
                            
                        }
                    }
                }
                else if (estado_jogo == GAME_OVER)
                {
                    // EXTRAIR INFORMAÇÕES DA LISTA_PLAYERS
                    if (!salva_arquivo)
                    {
                        FILE * ler_b =fopen("C:\\Project_gamer\\assets\\save\\lista_players.dat","rb");
                        FILE * score_b = fopen("C:\\Project_gamer\\assets\\save\\score.dat","rb+");
                        if (fread(&dados, sizeof(usuario), 1, ler_b) == 1)
                        {
                            // Atribui o conteúdo para a variável dados
                        }
                        else
                        {
                            // Se não conseguir ler, deu erro. Melhor sair.
                            printf("Erro: Nao consegui ler o jogador de lista_players.dat\n");
                            fclose(ler_b);
                            fclose(score_b);
                            
                            return SAIR;
                        }
                        
                        fclose(ler_b);
                        
                        // Começo a fazer o processo de armazenar pontuação do usuario existente e não existente
                        
                        rewind(score_b); // Garante que a leitura será feita do inicio do arquivo 
                        
                        while (fread(&cadastro, sizeof(usuario), 1, score_b)==1)
                        {
                            if (strcmp(cadastro.username,dados.username)==0)
                            {
                                encontrado = true;
                                printf("Player já possui cadastro\n");
    
                                if (score > cadastro.score)
                                {
                                    printf("Pontuação ANTIGA: %d\n", cadastro.score);
                                    cadastro.score = score;
                                    fseek(score_b, -(long)sizeof(usuario), SEEK_CUR); // Comando responsável por mover o dado no arquivo
                                    fwrite(&cadastro, sizeof(usuario), 1, score_b);
                                    fflush(score_b); // Forçar a escrita
                                    printf("NOVO Nome: %s\n", cadastro.username);
                                    printf("NOVO Score: %d\n", cadastro.score);
                                }
                                break;
                            }
                            
                        }
    
                        if (!encontrado)
                        {
                            printf("Jogador '%s' nao encontrado. Adicionando...\n", dados.username);
    
                            dados.score = score;
    
                            fseek(score_b, sizeof(usuario), SEEK_END); // Comando que adiciona o dado no final do arquivo
                            fwrite(&cadastro, sizeof(usuario), 1, score_b);
    
                        }
                        
                        fclose(score_b);
                        salva_arquivo = true;
                    }
                    

                }
                



                // Trava nos limites da tela
                if (player.x < 0) player.x = 0;
                if (player.y < 0) {player.y = 0; vy = 0;}
                if (player.x + RATIM_W > SCREEN_W) player.x = SCREEN_W - RATIM_W;
                if (estado_jogo == JOGANDO && player.y + player.mini_h >= SCREEN_H) {
                    player.y = SCREEN_H-player.mini_h;
                    al_play_sample(queda, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    cont = 0;
                    score -=1;
                    estado_jogo = GAME_OVER;
                }
                
                
                // Ao clicar esc o programa finaliza
                if(key[ALLEGRO_KEY_ESCAPE])
                {
                    al_destroy_font(font_1);
                    al_destroy_font(font_2);
                    al_destroy_font(font_3);
                    al_destroy_bitmap(ratim);
                    al_destroy_bitmap(cenario);
                    al_destroy_bitmap(game_over);
                    al_destroy_bitmap(icon);
                    al_destroy_audio_stream(music);
                    al_destroy_sample(jump);
                    al_destroy_sample(queda);
                    al_destroy_sample(tiktak);
                    return MENU;
                }
                
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                {
                    key[i] &= ~KEY_SEEN;
                }
                redraw = true;
                break;
            }   
            
            // CASE RESPONSÁVEL PELA TECLA PRECIONADA
            case ALLEGRO_EVENT_KEY_DOWN:
            {
                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;

                // Verifica se deu game over para um possível reinicio
                if (estado_jogo == GAME_OVER && event.keyboard.keycode == ALLEGRO_KEY_ENTER||event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
                {
                    // reconstrução dos blocos
                    cadastrar_blocos(bloco,n_bloco);

                    // personagem reposicionado velocidades 0, posição inicial, tempo
                    vy = 0;
                    vx = 0;
                    player.x = SPAWN_X;
                    player.y = SPAWN_Y;
                    score = 0;
                    tempo_jogo = TEMPO;
                    acelera = 0.0;

                    // Resetar estados
                    on_ground = true;
                    jumping = false;
                    pouso = false;
                    time = false;
                    salva_arquivo = false;

                    // mundo movendo
                    world_offset = 0.0;
                    camera_x = 0.0;
                    camera_alvo_offset = 0.0;

                    // ESTADO DO JOGO
                    estado_jogo = JOGANDO;


                }
                
                break;
            }
            
            // CASE RESPONSÁVEL PELA FINALIZAÇÃO DO TOQUE NA TECLA
            case ALLEGRO_EVENT_KEY_UP:
            {
                key[event.keyboard.keycode] &= ~KEY_DOWN;
                break;
            }

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
            {
                done = true;
                break;
            }    
        }
                
        if(done)
        break;
        
        if(redraw && al_is_event_queue_empty(queue))
        {
            if (estado_jogo == JOGANDO)
            {
            
                
                float porcentagem = tempo_jogo/TEMPO;
                float barra_progresso = barra_tempo.posw*porcentagem;
                
                // Desenho do cenario 
                al_draw_bitmap(cenario, 0,0,0);
                // Desenho do fundo da barra
                al_draw_filled_rectangle(
                    barra_tempo.x, barra_tempo.y,
                    barra_tempo.x+barra_tempo.posw, barra_tempo.y+barra_tempo.posh,
                    al_map_rgb(40,40,40)
                );
                
                // Progresso
                if (porcentagem > 0.5)
                {
                    
                    al_draw_filled_rectangle(
                        barra_tempo.x, barra_tempo.y,
                        barra_tempo.x+barra_progresso, barra_tempo.y+barra_tempo.posh,
                        al_map_rgb(0,200,0)
                    );
                    
                }
                else if (porcentagem > 0.1)
                {
                    al_draw_filled_rectangle(
                        barra_tempo.x, barra_tempo.y,
                        barra_tempo.x+barra_progresso, barra_tempo.y+barra_tempo.posh,
                        al_map_rgb(250,250,0)
                    );
                }
                else
                {
                    al_draw_filled_rectangle(
                        barra_tempo.x, barra_tempo.y,
                        barra_tempo.x+barra_progresso, barra_tempo.y+barra_tempo.posh,
                        al_map_rgb(250,0,0)
                    );
                }
                
                al_draw_rectangle(
                    barra_tempo.x, barra_tempo.y,
                    barra_tempo.x+barra_tempo.posw, barra_tempo.y+barra_tempo.posh,
                    al_map_rgb(250,250,250),1
                );
                
                al_draw_textf(font_1, al_map_rgb(255, 255, 255), 0, 0, 0, "SCORE: %d", score);
                al_draw_textf(font_1,al_map_rgb(255, 255, 255), 0, barra_tempo.y-5, 0, "TEMPO");
                

                //Adicionar os blocos
                for (i = 0; i < n_bloco; i++) // quantidade de blocos até preencher largura
                {

                    
                    al_draw_bitmap_region(
                        ratim,
                        bloco[i].posx, bloco[i].posy, // Posição (x,y) do sprite na sua imagem
                        BLOCK_W, BLOCK_H,             // O tamanho do sprite na imagem (que é o mesmo final)
                        bloco[i].x - world_offset,    // Posição X na tela
                        bloco[i].y,                   // Posição Y na tela
                        0
                    );

                    if (bloco[i].clock)
                    {
                        al_draw_bitmap_region(
                            ratim,
                            relogio.posx, relogio.posy, 
                            relogio.posw, relogio.posh,
                            // Posição X: Centralizado em cima do bloco
                            (bloco[i].x - world_offset) + 10, 
                            // Posição Y: Flutuando um pouco acima do bloco
                            bloco[i].y-relogio.posh,
                            0
                        );
                    }

                }
                
                

                al_draw_scaled_bitmap(
                    ratim,          // bitmap original
                    player.posx, player.posy, player.posw, player.posh,  // região original (sx,sy,sw,sh)
                    player.x, player.y,           // posição na tela
                    RATIM_W, RATIM_H,         // largura e altura desejadas na tela
                    0               // flags
                );                            

            }
            else if (estado_jogo == GAME_OVER)
            {
                al_draw_bitmap(game_over, 0,0,0);
                al_draw_filled_rectangle(223, 260,223+293, 260+35,al_map_rgb(25,99,73));
                al_draw_filled_rectangle(0, 50,0+SCREEN_W, 50+55,al_map_rgb(255,20,60));
                al_draw_rectangle(223, 260,223+293, 260+35,al_map_rgb(0,0,0),4);
                al_draw_rectangle(0, 50,0+SCREEN_W, 50+55,al_map_rgb(0,0,0),4);
                al_draw_textf(font_2, al_map_rgb(255, 255, 255), 278, 45, 0,"GAME OVER");
                al_draw_textf(font_3, al_map_rgb(255, 255, 255), 230, 260, 0,"Aperte ENTER para reiniciar");
                al_draw_textf(font_1, al_map_rgb(255, 255, 255), 335, 295, 0,"SCORE: %d", score);
            }
            
            // MOSTRA O QUE FOI DESENHADO DEVE OCORRER EM AMBOS EVENTOS
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font_1);
    al_destroy_font(font_2);
    al_destroy_font(font_3);
    al_destroy_bitmap(ratim);
    al_destroy_bitmap(cenario);
    al_destroy_bitmap(game_over);
    al_destroy_bitmap(icon);
    al_destroy_audio_stream(music);
    al_destroy_sample(jump);
    al_destroy_sample(queda);
    al_destroy_sample(tiktak);

    return SAIR;
}