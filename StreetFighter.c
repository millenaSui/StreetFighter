#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Personagem.h"
#include "Menus.h"

#define NUMERO_PERSONAGENS 4 

ALLEGRO_BITMAP* sprites[NUMERO_PERSONAGENS]; // Array para armazenar os sprites dos personagens
ALLEGRO_BITMAP* spritesPB[NUMERO_PERSONAGENS]; // Array para armazenar os sprites dos personagens em preto e branco

/*Gerencia o sistema de partidas*/
int sistema_de_partidas(ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp, ALLEGRO_BITMAP* wallpaper, personagem* player1, personagem* player2, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_TIMER* timer, int vitoriasPlayer1, int vitoriasPlayer2) {
    unsigned char player1Derrotado = 0, player2Derrotado = 0;

    /*Registra os eventos*/
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    while (1) { // Loop principal
        al_wait_for_event(queue, &event);

        /*Verifica se algum jogador foi derrotado*/
        player1Derrotado = verificaDerrotaPersonagem(player2, player1);
        player2Derrotado = verificaDerrotaPersonagem(player1, player2);

        /*Exibe o resultado da partida*/
        if (player1Derrotado || player2Derrotado) {
            al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela
            if (player2Derrotado && player1Derrotado)
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2 - 40, Y_SCREEN / 2 - 15, 0, "EMPATE!"); // Desenha o texto de empate
            else if (player2Derrotado)
                al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN / 2 - 75, Y_SCREEN / 2 - 15, 0, "JOGADOR 1 GANHOU!"); // Desenha o texto de vitória do jogador 1
            else if (player1Derrotado)
                al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN / 2 - 75, Y_SCREEN / 2 - 15, 0, "JOGADOR 2 GANHOU!"); // Desenha o texto de vitória do jogador 2

            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2 - 110, Y_SCREEN / 2 + 5, 0, "PRESSIONE ESPAÇO PARA CONTINUAR"); // Instrução para continuar
            al_flip_display();

            /* Aguarda por dois segundos antes de iniciar a próxima partida */
            al_rest(2.0);

            /*Verifica os eventos de teclado e fechamento da janela*/
            while (1) {
                al_wait_for_event(queue, &event);
                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE)
                    break; // Sai do loop ao pressionar espaço
                else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    return -1; // Jogador fechou a janela
            }
            
            /*Reseta a posição dos personagens*/
            if (vitoriasPlayer1 < 2 && vitoriasPlayer2 < 2) {
                resetaPersonagem(player1, player1->x, player1->y);
                resetaPersonagem(player2, player2->x, player2->y);
            }
            if (player2Derrotado && !player1Derrotado) return 1; // Jogador 1 vence
            if (player1Derrotado && !player2Derrotado) return 2; // Jogador 2 vence
            
        } else {
            /*Evento do timer*/
            if (event.type == ALLEGRO_EVENT_TIMER) {
                atualizaPosicaoPersonagem(player1, player2, 1.0 / 30.0); // Atualiza a posição dos jogadores
                
                al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela
                al_draw_bitmap(wallpaper, 0, 0, 0); // Desenha o wallpaper
                
                /*Desenha os sprites do player 1*/
                if (!player1->controle->ataca) {
                    if (player1->controle->cima.pulo)
                        al_draw_bitmap(player1->spritePulando, player1->x - player1->largura / 2, player1->y - player1->altura / 2, 0);
                    else if (player1->controle->baixo)
                        al_draw_bitmap(player1->spriteAbaixando, player1->x - player1->largura / 2, player1->y - player1->altura / 2, 0);
                    else if (player1->controle->esquerda || player1->controle->direita)
                        al_draw_bitmap(player1->spriteAndando1, player1->x - player1->largura / 2, player1->y - player1->altura / 2, 0);
                    else 
                        al_draw_bitmap(player1->spriteAndando2, player1->x - player1->largura / 2, player1->y - player1->altura / 2, 0);
                } else if (player1->controle->ataca) {
                    if (player1->controle->cima.pulo)
                        al_draw_bitmap(player1->spriteAtacandoAlto, player1->x - player1->largura / 2, player1->y - player1->altura / 2, 0);
                    else if (player1->controle->baixo)
                        al_draw_bitmap(player1->spriteAtacandoBaixo, player1->x - player1->largura / 2, player1->y - player1->altura / 2, 0);
                    else if (player1->controle->esquerda || player1->controle->direita)
                        al_draw_bitmap(player1->spriteAtacando, player1->x - player1->largura / 2, player1->y - player1->altura / 2, 0);
                    else
                        al_draw_bitmap(player1->spriteAtacando, player1->x - player1->largura / 2, player1->y - player1->altura / 2, 0);
                }

                /*Desenha os sprites do player 2*/
                if (!player2->controle->ataca) {
                    if (player2->controle->cima.pulo)
                        al_draw_bitmap(player2->spritePulando, player2->x - player2->largura / 2, player2->y - player2->altura / 2, 0);
                    else if (player2->controle->baixo)
                        al_draw_bitmap(player2->spriteAbaixando, player2->x - player2->largura / 2, player2->y - player2->altura / 2, 0);
                    else if (player2->controle->esquerda || player2->controle->direita)
                        al_draw_bitmap(player2->spriteAndando1, player2->x - player2->largura / 2, player2->y - player2->altura / 2, 0);
                    else 
                        al_draw_bitmap(player2->spriteAndando2, player2->x - player2->largura / 2, player2->y - player2->altura / 2, 0);
                } else if (player2->controle->ataca) {
                    if (player2->controle->cima.pulo)
                        al_draw_bitmap(player2->spriteAtacandoAlto, player2->x - player2->largura / 2, player2->y - player2->altura / 2, 0);
                    else if (player2->controle->baixo)
                        al_draw_bitmap(player2->spriteAtacandoBaixo, player2->x - player2->largura / 2, player2->y - player2->altura / 2, 0);
                    else if (player2->controle->esquerda || player2->controle->direita)
                        al_draw_bitmap(player2->spriteAtacando, player2->x - player2->largura / 2, player2->y - player2->altura / 2, 0);
                    else
                        al_draw_bitmap(player2->spriteAtacando, player2->x - player2->largura / 2, player2->y - player2->altura / 2, 0);
                }

                if (player1->ataque->timer) player1->ataque->timer--; // Decrementa o timer do ataque do player 1
                if (player2->ataque->timer) player2->ataque->timer--; // Decrementa o timer do ataque do player 2
                
                /*Exibe HP dos players*/
                char hp_text[20];
                sprintf(hp_text, "HP: %d", player1->hp);
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, hp_text);
                sprintf(hp_text, "HP: %d", player2->hp);
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN - 80, 10, ALLEGRO_ALIGN_LEFT, hp_text);
                
                al_flip_display(); // Atualiza a tela

            } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                /*Verifica teclas pressionadas*/
                if (event.keyboard.keycode == ALLEGRO_KEY_A) player1->controle->esquerda = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_D) player1->controle->direita = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_W) {
                    if (!player1->controle->cima.pulo) {
                        player1->controle->cima.pulo = true;
                        player1->controle->cima.tempoPulo = DURACAO_PULO;
                        player1->controle->cima.alturaInicial = player1->y;
                    }
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_S) player1->controle->baixo = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) player2->controle->esquerda = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) player2->controle->direita = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                    if (!player2->controle->cima.pulo) {
                        player2->controle->cima.pulo = true;
                        player2->controle->cima.tempoPulo = DURACAO_PULO;
                        player2->controle->cima.alturaInicial = player2->y;
                    }
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) player2->controle->baixo = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_C) player1->controle->ataca = 1;                   
                else if (event.keyboard.keycode == ALLEGRO_KEY_RSHIFT) player2->controle->ataca = 1;
            
            } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
                /*Verifica teclas soltas*/
                if (event.keyboard.keycode == ALLEGRO_KEY_A) player1->controle->esquerda = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_D) player1->controle->direita = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_S) player1->controle->baixo = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) player2->controle->esquerda = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) player2->controle->direita = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) player2->controle->baixo = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_C) player1->controle->ataca = 0;                    
                else if (event.keyboard.keycode == ALLEGRO_KEY_RSHIFT) player2->controle->ataca = 0;
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                return -1; // Indica que o jogador fechou a janela
            }
        }
    }

    return 0; // Empate
}

/*Libera os recursos alocados durante as partidas*/
void liberar_recursos(ALLEGRO_BITMAP* wallpaper1, ALLEGRO_BITMAP* wallpaper2, ALLEGRO_BITMAP* wallpaper3, ALLEGRO_BITMAP* sprites[], int numPersonagens, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, personagem* player1, personagem* player2) {
    al_destroy_bitmap(wallpaper1);
    al_destroy_bitmap(wallpaper2);
    al_destroy_bitmap(wallpaper3);
    for (int i = 0; i < numPersonagens; i++)
        al_destroy_bitmap(sprites[i]);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    destroiPersonagem(player1);
    destroiPersonagem(player2);
}

int main() {
    /*Inicializa dependências e indica possíveis erros*/
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar primitivas.\n");
        return -1;
    }
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar imagens.\n");
        return -1;
    }
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return -1;
    }
    if (!al_init_font_addon()) {
        fprintf(stderr, "Falha ao inicializar fontes.\n");
        return -1;
    }

    /*Inicializa elementos principais da Allegro*/
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_FONT* font = al_create_builtin_font();
    // Inicializa o display ao centro da tela 
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);
    if (disp) {
        ALLEGRO_MONITOR_INFO info;
        al_get_monitor_info(0, &info);
        int larguraTela = info.x2 - info.x1;
        int alturaTela = info.y2 - info.y1;
        al_set_window_position(disp, (larguraTela - X_SCREEN) / 2, (alturaTela - Y_SCREEN) / 2);
    }
    /*Verifica se os elementos foram criados corretamente*/
    if (!timer || !queue || !font || !disp) {
        fprintf(stderr, "Falha ao criar elementos principais da Allegro.\n");
        return -1;
    }

    /*Carrega wallpapers*/
    srand(time(NULL)); // Inicializa gerador de números aleatórios
    ALLEGRO_BITMAP* wallpaper1 = al_load_bitmap("./wallpapers/wallpaper1.jpg");
    ALLEGRO_BITMAP* wallpaper2 = al_load_bitmap("./wallpapers/wallpaper2.jpg");
    ALLEGRO_BITMAP* wallpaper3 = al_load_bitmap("./wallpapers/wallpaper3.jpg");
    if (!wallpaper1 || !wallpaper2 || !wallpaper3) {
        fprintf(stderr, "Falha ao carregar wallpapers.\n");
        return -1;
    }

    /*Carrega sprites dos personagens*/
    sprites[0] = al_load_bitmap("./sprites/select/ryu.png");
    sprites[1] = al_load_bitmap("./sprites/select/cammy.png");
    sprites[2] = al_load_bitmap("./sprites/select/chun-li.png");
    sprites[3] = al_load_bitmap("./sprites/select/zangief.png");
    spritesPB[0] = al_load_bitmap("./sprites/select/ryu-PB.png");
    spritesPB[1] = al_load_bitmap("./sprites/select/cammy-PB.png");
    spritesPB[2] = al_load_bitmap("./sprites/select/chun-li-PB.png");
    spritesPB[3] = al_load_bitmap("./sprites/select/zangief-PB.png");
    
    /*Verifica se os sprites foram carregados corretamente*/
    for (int i = 0; i < NUMERO_PERSONAGENS; i++) {
        if (!sprites[i]) {
            fprintf(stderr, "Falha ao carregar sprite do personagem %d\n", i + 1);
            return -1;
        }
    }

    /* Inicializa o áudio */
    if (!al_install_audio()) {
        fprintf(stderr, "Falha ao inicializar o áudio.\n");
        return -1;
    }
    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return -1;
    }
    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Falha ao reservar canais de áudio.\n");
        return -1;
    }

    ALLEGRO_SAMPLE* musica = al_load_sample("./songs/Eagle.flac");
    if (!musica) {
        fprintf(stderr, "Falha ao carregar a música.\n");
        return -1;
    }

    ALLEGRO_SAMPLE_ID musica_id;
    if (!al_play_sample(musica, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musica_id)) {
        fprintf(stderr, "Falha ao reproduzir a música.\n");
        return -1;
    }

    /* Exibe o menu inicial e inicia o jogo */
    int opcao = exibirMenuInicial(font, disp);
    if (opcao == 0) {  // Iniciar jogo
        int selecoes[2] = {0, 0};
        exibirMenuPersonagens(font, disp, sprites, spritesPB, NUMERO_PERSONAGENS, selecoes);

        /*Registra os eventos*/
        al_register_event_source(queue, al_get_keyboard_event_source());
        al_register_event_source(queue, al_get_display_event_source(disp));
        al_register_event_source(queue, al_get_timer_event_source(timer));

        /*Carrega os sprites do player 1*/
        ALLEGRO_BITMAP*player1SpriteAndando1;
        ALLEGRO_BITMAP*player1SpriteAndando2;
        ALLEGRO_BITMAP*player1SpriteAtacando;
        ALLEGRO_BITMAP*player1SpritePulando;
        ALLEGRO_BITMAP*player1SpriteAbaixando;
        ALLEGRO_BITMAP*player1SpriteAtacandoBaixo;
        ALLEGRO_BITMAP*player1SpriteAtacandoAlto;

        if (selecoes[0] == 0) { // Sprites Ryu
            player1SpriteAndando1 = al_load_bitmap("./sprites/player1/ryu/1.png");
            player1SpriteAndando2 = al_load_bitmap("./sprites/player1/ryu/2.png");
            player1SpriteAtacando = al_load_bitmap("./sprites/player1/ryu/3.png");
            player1SpritePulando = al_load_bitmap("./sprites/player1/ryu/4.png");
            player1SpriteAbaixando = al_load_bitmap("./sprites/player1/ryu/5.png");
            player1SpriteAtacandoBaixo = al_load_bitmap("./sprites/player1/ryu/6.png");
            player1SpriteAtacandoAlto = al_load_bitmap("./sprites/player1/ryu/7.png");
        } else if (selecoes[0] == 1) { // Sprites Cammy
            player1SpriteAndando1 = al_load_bitmap("./sprites/player1/cammy/1.png");
            player1SpriteAndando2 = al_load_bitmap("./sprites/player1/cammy/2.png");
            player1SpriteAtacando = al_load_bitmap("./sprites/player1/cammy/3.png");
            player1SpritePulando = al_load_bitmap("./sprites/player1/cammy/4.png");
            player1SpriteAbaixando = al_load_bitmap("./sprites/player1/cammy/5.png");
            player1SpriteAtacandoBaixo = al_load_bitmap("./sprites/player1/cammy/6.png");
            player1SpriteAtacandoAlto = al_load_bitmap("./sprites/player1/cammy/7.png");
        } else if (selecoes[0] == 2) { // Sprites Chun-li
            player1SpriteAndando1 = al_load_bitmap("./sprites/player1/chun-li/1.png");
            player1SpriteAndando2 = al_load_bitmap("./sprites/player1/chun-li/2.png");
            player1SpriteAtacando = al_load_bitmap("./sprites/player1/chun-li/3.png");
            player1SpritePulando = al_load_bitmap("./sprites/player1/chun-li/4.png");
            player1SpriteAbaixando = al_load_bitmap("./sprites/player1/chun-li/5.png");
            player1SpriteAtacandoBaixo = al_load_bitmap("./sprites/player1/chun-li/6.png");
            player1SpriteAtacandoAlto = al_load_bitmap("./sprites/player1/chun-li/7.png");
        } else if (selecoes[0] == 3) { // Sprites Zangief
            player1SpriteAndando1 = al_load_bitmap("./sprites/player1/zangief/1.png");
            player1SpriteAndando2 = al_load_bitmap("./sprites/player1/zangief/2.png");
            player1SpriteAtacando = al_load_bitmap("./sprites/player1/zangief/3.png");
            player1SpritePulando = al_load_bitmap("./sprites/player1/zangief/4.png");
            player1SpriteAbaixando = al_load_bitmap("./sprites/player1/zangief/5.png");
            player1SpriteAtacandoBaixo = al_load_bitmap("./sprites/player1/zangief/6.png");
            player1SpriteAtacandoAlto = al_load_bitmap("./sprites/player1/zangief/7.png");}

        /*Carrega os sprites do player 2*/
        ALLEGRO_BITMAP*player2SpriteAndando1;
        ALLEGRO_BITMAP*player2SpriteAndando2;
        ALLEGRO_BITMAP*player2SpriteAtacando;
        ALLEGRO_BITMAP*player2SpritePulando;
        ALLEGRO_BITMAP*player2SpriteAbaixando;
        ALLEGRO_BITMAP*player2SpriteAtacandoBaixo;
        ALLEGRO_BITMAP*player2SpriteAtacandoAlto;

        if (selecoes[1] == 0) { // Sprites Ryu
            player2SpriteAndando1 = al_load_bitmap("./sprites/player2/ryu/1.png");
            player2SpriteAndando2 = al_load_bitmap("./sprites/player2/ryu/2.png");
            player2SpriteAtacando = al_load_bitmap("./sprites/player2/ryu/3.png");
            player2SpritePulando = al_load_bitmap("./sprites/player2/ryu/4.png");
            player2SpriteAbaixando = al_load_bitmap("./sprites/player2/ryu/5.png");
            player2SpriteAtacandoBaixo = al_load_bitmap("./sprites/player2/ryu/6.png");
            player2SpriteAtacandoAlto = al_load_bitmap("./sprites/player2/ryu/7.png");
        } else if (selecoes[1] == 1) { // Sprites Cammy
            player2SpriteAndando1 = al_load_bitmap("./sprites/player2/cammy/1.png");
            player2SpriteAndando2 = al_load_bitmap("./sprites/player2/cammy/2.png");
            player2SpriteAtacando = al_load_bitmap("./sprites/player2/cammy/3.png");
            player2SpritePulando = al_load_bitmap("./sprites/player2/cammy/4.png");
            player2SpriteAbaixando = al_load_bitmap("./sprites/player2/cammy/5.png");
            player2SpriteAtacandoBaixo = al_load_bitmap("./sprites/player2/cammy/6.png");
            player2SpriteAtacandoAlto = al_load_bitmap("./sprites/player2/cammy/7.png");
        } else if (selecoes[1] == 2) { // Sprites Chun-li
            player2SpriteAndando1 = al_load_bitmap("./sprites/player2/chun-li/1.png");
            player2SpriteAndando2 = al_load_bitmap("./sprites/player2/chun-li/2.png");
            player2SpriteAtacando = al_load_bitmap("./sprites/player2/chun-li/3.png");
            player2SpritePulando = al_load_bitmap("./sprites/player2/chun-li/4.png");
            player2SpriteAbaixando = al_load_bitmap("./sprites/player2/chun-li/5.png");
            player2SpriteAtacandoBaixo = al_load_bitmap("./sprites/player2/chun-li/6.png");
            player2SpriteAtacandoAlto = al_load_bitmap("./sprites/player2/chun-li/7.png");
        } else if (selecoes[1] == 3) { // Sprites Zangief
            player2SpriteAndando1 = al_load_bitmap("./sprites/player2/zangief/1.png");
            player2SpriteAndando2 = al_load_bitmap("./sprites/player2/zangief/2.png");
            player2SpriteAtacando = al_load_bitmap("./sprites/player2/zangief/3.png");
            player2SpritePulando = al_load_bitmap("./sprites/player2/zangief/4.png");
            player2SpriteAbaixando = al_load_bitmap("./sprites/player2/zangief/5.png");
            player2SpriteAtacandoBaixo = al_load_bitmap("./sprites/player2/zangief/6.png");
            player2SpriteAtacandoAlto = al_load_bitmap("./sprites/player2/zangief/7.png");}

        /*Cria os personagens player1 e player2*/  
        personagem* player1 = criaPersonagem(150, // altura
            90, // largura
            1, // fronte
            150,
            Y_SCREEN - 90, 
            X_SCREEN, 
            Y_SCREEN, 
            player1SpriteAndando1, 
            player1SpriteAndando2, 
            player1SpritePulando, 
            player1SpriteAbaixando, 
            player1SpriteAtacando, 
            player1SpriteAtacandoBaixo, 
            player1SpriteAtacandoAlto);

        if (!player1) {
            fprintf(stderr, "Falha ao criar o jogador 1.\n");
            return 1;
        }

        personagem* player2 = criaPersonagem(150, // altura
            90, // largura
            0, // fronte
            X_SCREEN - 150,
            Y_SCREEN - 90, 
            X_SCREEN, 
            Y_SCREEN, 
            player2SpriteAndando1, 
            player2SpriteAndando2, 
            player2SpritePulando, 
            player2SpriteAbaixando, 
            player2SpriteAtacando, 
            player2SpriteAtacandoBaixo, 
            player2SpriteAtacandoAlto);
        if (!player2) {
            fprintf(stderr, "Falha ao criar o jogador 2.\n");
            return 1;
        }

        int vitoriasPlayer1 = 0, vitoriasPlayer2 = 0;
        int resultado;
        /*Loop das partidas, até que um jogador vença duas vezes*/
        while (vitoriasPlayer1 < 2 && vitoriasPlayer2 < 2) {

            /*Recarrega o wallpaper*/
            ALLEGRO_BITMAP* wallpaper;
            int wallpaperAleatorio = rand() % 3;
            if (wallpaperAleatorio == 0)
                wallpaper = wallpaper1;
            else if (wallpaperAleatorio == 1)
                wallpaper = wallpaper2;
            else
                wallpaper = wallpaper3;
            
            /*Trata o resultado da partida*/
            resultado = sistema_de_partidas(font, disp, wallpaper, player1, player2, queue, timer, vitoriasPlayer1, vitoriasPlayer2);
            if (resultado == 1) 
                vitoriasPlayer1++;
            else if (resultado == 2) 
                vitoriasPlayer2++;
            else if (resultado == -1) break; // Jogador fechou a janela
        }

        /*Exibe o wallpaper do resultado da batalha*/
        al_clear_to_color(al_map_rgb(0, 0, 0));
        ALLEGRO_BITMAP* wallpaper;
        int wallpaperAleatorio = rand() % 3;
        if (wallpaperAleatorio == 0)
            wallpaper = wallpaper1;
        else if (wallpaperAleatorio == 1)
            wallpaper = wallpaper2;
        else if (wallpaperAleatorio == 2)
            wallpaper = wallpaper3;
    }
    return 0;
}