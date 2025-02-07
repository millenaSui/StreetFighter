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
int sistema_de_partidas(ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp, ALLEGRO_BITMAP* wallpaper, personagem* player_1, personagem* player_2, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_TIMER* timer, int vitorias_player_1, int vitorias_player_2) {
    unsigned char player_1_derrotado = 0, player_2_derrotado = 0;

    /*Registra os eventos*/
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    while (1) { // Loop principal
        al_wait_for_event(queue, &event);

        /*Verifica se algum jogador foi derrotado*/
        player_1_derrotado = verifica_derrota(player_2, player_1);
        player_2_derrotado = verifica_derrota(player_1, player_2);

        /*Exibe o resultado da partida*/
        if (player_1_derrotado || player_2_derrotado) {
            al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela
            if (player_2_derrotado && player_1_derrotado)
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2 - 40, Y_SCREEN / 2 - 15, 0, "EMPATE!"); // Desenha o texto de empate
            else if (player_2_derrotado)
                al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN / 2 - 75, Y_SCREEN / 2 - 15, 0, "JOGADOR 1 GANHOU!"); // Desenha o texto de vitória do jogador 1
            else if (player_1_derrotado)
                al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN / 2 - 75, Y_SCREEN / 2 - 15, 0, "JOGADOR 2 GANHOU!"); // Desenha o texto de vitória do jogador 2

            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2 - 110, Y_SCREEN / 2 + 5, 0, "PRESSIONE ESPAÇO PARA CONTINUAR"); // Instrução para continuar
            al_flip_display();

            /* Aguarda por cinco segundos antes de iniciar a próxima partida */
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
            if (vitorias_player_1 < 2 && vitorias_player_2 < 2) {
                reseta_personagem(player_1, player_1->x, player_1->y);
                reseta_personagem(player_2, player_2->x, player_2->y);
            }
            if (player_2_derrotado && !player_1_derrotado) return 1; // Jogador 1 vence
            if (player_1_derrotado && !player_2_derrotado) return 2; // Jogador 2 vence
            
        } else {
            /*Evento do timer*/
            if (event.type == ALLEGRO_EVENT_TIMER) {
                atualiza_posicao(player_1, player_2, 1.0 / 30.0); // Atualiza a posição dos jogadores
                
                al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela
                al_draw_bitmap(wallpaper, 0, 0, 0); // Desenha o wallpaper
                
                /*Desenha os sprites do player 1*/
                if (!player_1->controle->ataca) {
                    if (player_1->controle->cima.pulando)
                        al_draw_bitmap(player_1->sprite_pulando, player_1->x - player_1->largura / 2, player_1->y - player_1->altura / 2, 0);
                    else if (player_1->controle->baixo)
                        al_draw_bitmap(player_1->sprite_abaixando, player_1->x - player_1->largura / 2, player_1->y - player_1->altura / 2, 0);
                    else if (player_1->controle->esquerda || player_1->controle->direita)
                        al_draw_bitmap(player_1->sprite_andando_1, player_1->x - player_1->largura / 2, player_1->y - player_1->altura / 2, 0);
                    else 
                        al_draw_bitmap(player_1->sprite_andando_2, player_1->x - player_1->largura / 2, player_1->y - player_1->altura / 2, 0);
                } else if (player_1->controle->ataca) {
                    if (player_1->controle->cima.pulando)
                        al_draw_bitmap(player_1->sprite_atacando_alto, player_1->x - player_1->largura / 2, player_1->y - player_1->altura / 2, 0);
                    else if (player_1->controle->baixo)
                        al_draw_bitmap(player_1->sprite_atacando_baixo, player_1->x - player_1->largura / 2, player_1->y - player_1->altura / 2, 0);
                    else if (player_1->controle->esquerda || player_1->controle->direita)
                        al_draw_bitmap(player_1->sprite_atacando, player_1->x - player_1->largura / 2, player_1->y - player_1->altura / 2, 0);
                    else
                        al_draw_bitmap(player_1->sprite_atacando, player_1->x - player_1->largura / 2, player_1->y - player_1->altura / 2, 0);
                }

                /*Desenha os sprites do player 2*/
                if (!player_2->controle->ataca) {
                    if (player_2->controle->cima.pulando)
                        al_draw_bitmap(player_2->sprite_pulando, player_2->x - player_2->largura / 2, player_2->y - player_2->altura / 2, 0);
                    else if (player_2->controle->baixo)
                        al_draw_bitmap(player_2->sprite_abaixando, player_2->x - player_2->largura / 2, player_2->y - player_2->altura / 2, 0);
                    else if (player_2->controle->esquerda || player_2->controle->direita)
                        al_draw_bitmap(player_2->sprite_andando_1, player_2->x - player_2->largura / 2, player_2->y - player_2->altura / 2, 0);
                    else 
                        al_draw_bitmap(player_2->sprite_andando_2, player_2->x - player_2->largura / 2, player_2->y - player_2->altura / 2, 0);
                } else if (player_2->controle->ataca) {
                    if (player_2->controle->cima.pulando)
                        al_draw_bitmap(player_2->sprite_atacando_alto, player_2->x - player_2->largura / 2, player_2->y - player_2->altura / 2, 0);
                    else if (player_2->controle->baixo)
                        al_draw_bitmap(player_2->sprite_atacando_baixo, player_2->x - player_2->largura / 2, player_2->y - player_2->altura / 2, 0);
                    else if (player_2->controle->esquerda || player_2->controle->direita)
                        al_draw_bitmap(player_2->sprite_atacando, player_2->x - player_2->largura / 2, player_2->y - player_2->altura / 2, 0);
                    else
                        al_draw_bitmap(player_2->sprite_atacando, player_2->x - player_2->largura / 2, player_2->y - player_2->altura / 2, 0);
                }

                if (player_1->ataque->timer) player_1->ataque->timer--; // Decrementa o timer do ataque do player 1
                if (player_2->ataque->timer) player_2->ataque->timer--; // Decrementa o timer do ataque do player 2
                
                /*Exibe HP dos players*/
                char hp_text[20];
                sprintf(hp_text, "HP: %d", player_1->hp);
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, hp_text);
                sprintf(hp_text, "HP: %d", player_2->hp);
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN - 80, 10, ALLEGRO_ALIGN_LEFT, hp_text);
                
                al_flip_display(); // Atualiza a tela

            } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                /*Verifica teclas pressionadas*/
                if (event.keyboard.keycode == ALLEGRO_KEY_A) player_1->controle->esquerda = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_D) player_1->controle->direita = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_W) {
                    if (!player_1->controle->cima.pulando) {
                        player_1->controle->cima.pulando = true;
                        player_1->controle->cima.tempo_pulo = PULO_DURACAO;
                        player_1->controle->cima.altura_inicial = player_1->y;
                    }
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_S) player_1->controle->baixo = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) player_2->controle->esquerda = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) player_2->controle->direita = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                    if (!player_2->controle->cima.pulando) {
                        player_2->controle->cima.pulando = true;
                        player_2->controle->cima.tempo_pulo = PULO_DURACAO;
                        player_2->controle->cima.altura_inicial = player_2->y;
                    }
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) player_2->controle->baixo = 1;
                else if (event.keyboard.keycode == ALLEGRO_KEY_C) player_1->controle->ataca = 1;                   
                else if (event.keyboard.keycode == ALLEGRO_KEY_RSHIFT) player_2->controle->ataca = 1;
            
            } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
                /*Verifica teclas soltas*/
                if (event.keyboard.keycode == ALLEGRO_KEY_A) player_1->controle->esquerda = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_D) player_1->controle->direita = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_S) player_1->controle->baixo = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) player_2->controle->esquerda = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) player_2->controle->direita = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) player_2->controle->baixo = 0;
                else if (event.keyboard.keycode == ALLEGRO_KEY_C) player_1->controle->ataca = 0;                    
                else if (event.keyboard.keycode == ALLEGRO_KEY_RSHIFT) player_2->controle->ataca = 0;
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                return -1; // Indica que o jogador fechou a janela
            }
        }
    }

    return 0; // Empate
}

/*Libera os recursos alocados durante as partidas*/
void liberar_recursos(ALLEGRO_BITMAP* wallpaper_1, ALLEGRO_BITMAP* wallpaper_2, ALLEGRO_BITMAP* wallpaper_3, ALLEGRO_BITMAP* sprites[], int num_personagens, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, personagem* player_1, personagem* player_2) {
    al_destroy_bitmap(wallpaper_1);
    al_destroy_bitmap(wallpaper_2);
    al_destroy_bitmap(wallpaper_3);
    for (int i = 0; i < num_personagens; i++)
        al_destroy_bitmap(sprites[i]);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    destroi_personagem(player_1);
    destroi_personagem(player_2);
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
        int screen_width = info.x2 - info.x1;
        int screen_height = info.y2 - info.y1;
        al_set_window_position(disp, (screen_width - X_SCREEN) / 2, (screen_height - Y_SCREEN) / 2);
    }
    /*Verifica se os elementos foram criados corretamente*/
    if (!timer || !queue || !font || !disp) {
        fprintf(stderr, "Falha ao criar elementos principais da Allegro.\n");
        return -1;
    }

    /*Carrega wallpapers*/
    srand(time(NULL)); // Inicializa gerador de números aleatórios
    ALLEGRO_BITMAP* wallpaper_1 = al_load_bitmap("./wallpapers/wallpaper_1.jpg");
    ALLEGRO_BITMAP* wallpaper_2 = al_load_bitmap("./wallpapers/wallpaper_2.jpg");
    ALLEGRO_BITMAP* wallpaper_3 = al_load_bitmap("./wallpapers/wallpaper_3.jpg");
    if (!wallpaper_1 || !wallpaper_2 || !wallpaper_3) {
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
    int opcao = exibir_menu_inicial(font, disp);
    if (opcao == 0) {  // Iniciar jogo
        int selecoes[2] = {0, 0};
        exibir_menu_personagens(font, disp, sprites, spritesPB, NUMERO_PERSONAGENS, selecoes);

        /*Registra os eventos*/
        al_register_event_source(queue, al_get_keyboard_event_source());
        al_register_event_source(queue, al_get_display_event_source(disp));
        al_register_event_source(queue, al_get_timer_event_source(timer));

        /*Carrega os sprites do player 1*/
        SpritesPersonagem sprites_player_1;
        SpritesPersonagem sprites_player_2;

        if (selecoes[0] == 0)
            sprites_player_1 = carregar_sprites_personagem("./sprites/player1/ryu");
        else if (selecoes[0] == 1)
            sprites_player_1 = carregar_sprites_personagem("./sprites/player1/cammy");
        else if (selecoes[0] == 2)
            sprites_player_1 = carregar_sprites_personagem("./sprites/player1/chun-li");
        else if (selecoes[0] == 3)
            sprites_player_1 = carregar_sprites_personagem("./sprites/player1/zangief");
        
        /*Carrega os sprites do player 2*/
        if (selecoes[1] == 0)
            sprites_player_2 = carregar_sprites_personagem("./sprites/player2/ryu");
        else if (selecoes[1] == 1)
            sprites_player_2 = carregar_sprites_personagem("./sprites/player2/cammy");
        else if (selecoes[1] == 2)
            sprites_player_2 = carregar_sprites_personagem("./sprites/player2/chun-li");
        else if (selecoes[1] == 3)
            sprites_player_2 = carregar_sprites_personagem("./sprites/player2/zangief");

        if (!sprites_player_1.sprite_andandoI || !sprites_player_1.sprite_andandoII || !sprites_player_1.sprite_atacando || !sprites_player_1.sprite_pulando || !sprites_player_1.sprite_abaixado || !sprites_player_1.sprite_atacando_baixo || !sprites_player_1.sprite_atacando_alto ||
            !sprites_player_2.sprite_andandoI || !sprites_player_2.sprite_andandoII || !sprites_player_2.sprite_atacando || !sprites_player_2.sprite_pulando || !sprites_player_2.sprite_abaixado || !sprites_player_2.sprite_atacando_baixo || !sprites_player_2.sprite_atacando_alto) {
            fprintf(stderr, "Falha ao carregar sprites dos personagens.\n");
            return -1;
        }

        /*Cria os personagens player_1 e player_2*/
        personagem* player_1 = cria_personagem(150, 90, 1, 150, Y_SCREEN - 90, X_SCREEN, Y_SCREEN, sprites_player_1.sprite_andandoI, sprites_player_1.sprite_andandoII, sprites_player_1.sprite_pulando, sprites_player_1.sprite_abaixado, sprites_player_1.sprite_atacando, sprites_player_1.sprite_atacando_baixo, sprites_player_1.sprite_atacando_alto);
        if (!player_1) {
            fprintf(stderr, "Falha ao criar o jogador 1.\n");
            return 1;
        }
        personagem* player_2 = cria_personagem(150, 90, 0, X_SCREEN - 150, Y_SCREEN - 90, X_SCREEN, Y_SCREEN, sprites_player_2.sprite_andandoI, sprites_player_2.sprite_andandoII, sprites_player_2.sprite_pulando, sprites_player_2.sprite_abaixado, sprites_player_2.sprite_atacando, sprites_player_2.sprite_atacando_baixo, sprites_player_2.sprite_atacando_alto);
        if (!player_2) {
            fprintf(stderr, "Falha ao criar o jogador 2.\n");
            return 2;
        }

        int vitorias_player_1 = 0, vitorias_player_2 = 0;
        int resultado;
        /*Loop das partidas, até que um jogador vença duas vezes*/
        while (vitorias_player_1 < 2 && vitorias_player_2 < 2) {

            /*Recarrega o wallpaper*/
            ALLEGRO_BITMAP* wallpaper;
            int random_wallpaper = rand() % 3;
            if (random_wallpaper == 0)
                wallpaper = wallpaper_1;
            else if (random_wallpaper == 1)
                wallpaper = wallpaper_2;
            else
                wallpaper = wallpaper_3;
            
            /*Trata o resultado da partida*/
            resultado = sistema_de_partidas(font, disp, wallpaper, player_1, player_2, queue, timer, vitorias_player_1, vitorias_player_2);
            if (resultado == 1) 
                vitorias_player_1++;
            else if (resultado == 2) 
                vitorias_player_2++;
            else if (resultado == -1) break; // Jogador fechou a janela
        }

        /*Exibe o wallpaper do resultado da batalha*/
        al_clear_to_color(al_map_rgb(0, 0, 0));
        ALLEGRO_BITMAP* wallpaper;
        int random_wallpaper = rand() % 3;
        if (random_wallpaper == 0)
            wallpaper = wallpaper_1;
        else if (random_wallpaper == 1)
            wallpaper = wallpaper_2;
    }
    return 0;
}