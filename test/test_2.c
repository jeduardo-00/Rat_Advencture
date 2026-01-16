#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

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
#define SPAWN_X 100
#define SPAWN_BLOCK_X 0
#define SPAWN_BLOCK_Y 400
#define JOGANDO 0
#define GAME_OVER 1


//Estrutura 

typedef struct
{
    float x, y, posx,posy,posw,posh, mini_w, mini_h;
}obj;


// Função void para verificar a funcionalidade dos comandos init
void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

// Função de colisão
bool colidir(obj* personagem, float*vy, bool * on_ground, int block_x, int block_y, int * score, int * cont)
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
                if (*cont == 1 && personagem->x > SPAWN_X +45)
                {
                    *score+=1; 
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
    }
}

int main()
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
    int estado_jogo = JOGANDO;
    int * score = malloc(sizeof(int));
    int * cont = malloc(sizeof(int));
    
    
    // Variavel de estrutura
    obj player = {SPAWN_X,SPAWN_Y,0,144,159,144,55,70}; // Rato
    obj bloco[n_bloco]; // Pode usar alocação dinamica com malloc 
    
    //Variável de T e F
    bool done = false;
    bool redraw = true;
    bool jumping = false;  // controle de pulo
    bool on_ground = false; // Variável para checar se o personagem está no chão
    bool pouso = true;
    
    
    // ------------------------------------ÁREA DE INSTALAÇÃO----------------------------
    // Implementação allegro
    must_init(al_init(), "allegro");
    
    // Instalando função de entrada
    must_init(al_install_keyboard(), "keyboard");
    
    // Area do time
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");
    
    // Area do display 
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    
    ALLEGRO_DISPLAY* disp = al_create_display(SCREEN_W, SCREEN_H);
    must_init(disp, "display");
    
    // Area da fonte
    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");
    
    // Carregando imagem
    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* ratim = al_load_bitmap("C:\\Project_gamer\\assets\\imagens\\ratim_sprite.png");
    must_init(ratim, "ratim");
    
    // Uso das formas geometrica
    must_init(al_init_primitives_addon(), "primitives");
    
    // inicializando blocos
    cadastrar_blocos(bloco, n_bloco);
    
    ALLEGRO_EVENT event;
    // ----------------------------Registrando eventos--------------------------------------------
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    
    
    unsigned char key[ALLEGRO_KEY_MAX]; //cria um vetor para armazenar o estado de todas as teclas possíveis.
    memset(key, 0, sizeof(key)); // marca todas as teclas como não pressionadas no início.
    
    al_start_timer(timer); //inicia o tempo
    
    *score = 0;
    *cont = 0;

    while(1)
    {
        al_wait_for_event(queue, &event); //espera até ocorrer um evento
        
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
            {
                if (estado_jogo == JOGANDO)
                {

                    //--------------------------CONTROLE--------------------------
                    if(key[ALLEGRO_KEY_LEFT])  player.x -= 5;

                    if (!jumping) {
                        if (key[ALLEGRO_KEY_RIGHT]) {
                            vy = -15;    // impulso vertical
                            vx = BLOCK_W;      // deslocamento horizontal pequeno
                            jumping = true;
                            player.posx = player.posw;
                            *cont = 0;
                        }
                        if (key[ALLEGRO_KEY_SPACE]) {
                            vy = -15;    // impulso vertical mais forte
                            vx = BLOCK_W*2;      // deslocamento horizontal maior
                            jumping = true;
                            player.posx = player.posw*2;
                            *cont = 0;
                        }
                    }
                    
                    // ----------------------------FISICA APLICADA------------------------- 
                    
                    // Aplica gravidade na velocidade vertical
                    if (!on_ground)
                    {
                        vy += gravity;
                    }
                    
                    // Atualiza a posição com base nas velocidades
                    // player.x += vx;
                    player.y += vy;
                    on_ground = false;
                    
                    for (i = 0; i <  n_bloco; i++)
                    {
                        // BLOCOS EXISTENTES
                        
                        if (colidir(&player, &vy, &on_ground, bloco[i].x - world_offset, bloco[i].y, score, cont)) 
                        {
                            player.posx = 0;
                            break;     
                        }
                        
                    }
                    
                    if (on_ground && pouso) // Ele ACABOU de pousar!
                    {
                        
                        // Calcula o quanto precisamos mover o mundo
                        float scroll_amount = vx;
                        // float scroll_amount = player.x - SPAWN_X;
                        world_offset += scroll_amount; // Adiciona ao deslocamento total

                        // "Puxa" o rato de volta para sua posição inicial na tela
                        // player.x -= scroll_amount;
                        vx = 0;
                    }
                    
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
                                }
                                else
                                {
                                    bloco[i].y = SPAWN_BLOCK_Y;
                                }
                            
                            }
                            
                        }
                    }
                }

                // Trava nos limites da tela
                if (player.x < 0) player.x = 0;
                if (player.y < 0) {player.y = 0; vy = 0;}
                if (player.x + RATIM_W > SCREEN_W) player.x = SCREEN_W - RATIM_W;
                if (estado_jogo == JOGANDO && player.y + player.mini_h >= SCREEN_H) {
                    player.y = SCREEN_H-player.mini_h;
                    vy = 0;
                    vx = 0;
                    jumping = false;
                    on_ground = true;
                    player.posx = 0;
                    estado_jogo = GAME_OVER;
                    

                }
                
                
                // Ao clicar esc o programa finaliza
                if(key[ALLEGRO_KEY_ESCAPE])
                {
                    done = true;
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

                    // personagem reposicionado velocidades 0, posição inicial
                    vy = 0;
                    vx = 0;
                    player.x = SPAWN_X;
                    player.y = SPAWN_Y;
                    *score = 0;

                    // Resetar estados
                    on_ground = false;
                    jumping = false;
                    pouso = true;

                    // mundo movendo
                    world_offset = 0.0;

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
                /* code */
            
            
                al_clear_to_color(al_map_rgb(60, 60, 60));
                al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "SCORE: %d/COLISÃO: %d", *score, *cont);
                
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
                al_clear_to_color(al_map_rgb(228, 247, 155));
                al_draw_textf(font, al_map_rgb(97, 76, 22), 320, 250, 0,"GAME OVER");
                al_draw_textf(font, al_map_rgb(97, 76, 22), 247, 260, 0,"Aperte ENTER para reiniciar");
            }
            
            // MOSTRA O QUE FOI DESENHADO DEVE OCORRER EM AMBOS EVENTOS
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(ratim);

    return 0;
}
