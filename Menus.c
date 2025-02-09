#include "Menus.h"

/* Função para exibir o menu inicial */
int exibirMenuInicial(ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp) {
    int opcao = 0;
    int estadoMenu = 0;

    ALLEGRO_EVENT event;
    ALLEGRO_TIMER* menuTimer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* menuQueue = al_create_event_queue();    
    al_register_event_source(menuQueue, al_get_keyboard_event_source());
    al_register_event_source(menuQueue, al_get_timer_event_source(menuTimer));
    al_start_timer(menuTimer);

    while (1) {
        al_wait_for_event(menuQueue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (estadoMenu == 0) {
                if (opcao == 0) { // Menu principal
                    al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN / 2, Y_SCREEN / 2 - 30, ALLEGRO_ALIGN_CENTRE, "Iniciar Jogo");
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Ajuda");
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 30, ALLEGRO_ALIGN_CENTRE, "Sair");
                } else if (opcao == 1) {
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 - 30, ALLEGRO_ALIGN_CENTRE, "Iniciar Jogo");
                    al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Ajuda");
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 30, ALLEGRO_ALIGN_CENTRE, "Sair");
                } else if (opcao == 2) {
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 - 30, ALLEGRO_ALIGN_CENTRE, "Iniciar Jogo");
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Ajuda");
                    al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN / 2, Y_SCREEN / 2 + 30, ALLEGRO_ALIGN_CENTRE, "Sair");
                }
           
            } else if (estadoMenu == 1) { // Tela de ajuda
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 - 100, ALLEGRO_ALIGN_CENTRE, "CONTROLES:");
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 - 50, ALLEGRO_ALIGN_CENTRE, "(PLAYER_1)");
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 - 30, ALLEGRO_ALIGN_CENTRE, "W: PULA, S: ABAIXA");
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 - 20, ALLEGRO_ALIGN_CENTRE, "A: ESQUERDA, D: DIREITA, C: ATIRA");
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 30, ALLEGRO_ALIGN_CENTRE, "(PLAYER_2)");
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 50, ALLEGRO_ALIGN_CENTRE, "(UP): PULA, (DOWN): ABAIXA");
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 60, ALLEGRO_ALIGN_CENTRE, "(L): ESQUERDA, (R): DIREITA, SHIFT(DIR): ATIRA");
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 120, ALLEGRO_ALIGN_CENTRE, "PRESSIONE ESC PARA VOLTAR");
            }

            al_flip_display();
        
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (estadoMenu == 0) {
                
                if (event.keyboard.keycode == ALLEGRO_KEY_UP) // Move para cima
                    opcao = (opcao + 2) % 3;
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) // Move para baixo
                    opcao = (opcao + 1) % 3; // Move para baixo
                else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) { // Seleciona
                    if (opcao == 0) {
                        al_stop_timer(menuTimer);
                        al_destroy_timer(menuTimer);
                        al_destroy_event_queue(menuQueue);
                        return opcao; // Iniciar Jogo
                    
                    } else if (opcao == 1) {
                        estadoMenu = 1; // Vai para a tela de ajuda
                        opcao = 0; // Reseta a opção na tela de ajuda
                    
                    } else if (opcao == 2) { // Sai do jogo
                        al_stop_timer(menuTimer);
                        al_destroy_timer(menuTimer);
                        al_destroy_event_queue(menuQueue);
                        return -1; // Sair
                    }
                }
            } else if (estadoMenu == 1) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    estadoMenu = 0; // Volta para o menu principal
            }
        }
    }
    al_destroy_timer(menuTimer);
    al_destroy_event_queue(menuQueue);
    
    return opcao;
}

/* Função para exibir o menu de seleção de personagens */
int exibirMenuPersonagens(ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp, ALLEGRO_BITMAP* sprites[], ALLEGRO_BITMAP* spritesPB[], int numPersonagens, int* selecoes) {
    int opcao = 0;
    int player = 1;
    selecoes[0] = -1;
    selecoes[1] = -1;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER* menuTimer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* menuQueue = al_create_event_queue();

    al_register_event_source(menuQueue, al_get_keyboard_event_source());
    al_register_event_source(menuQueue, al_get_timer_event_source(menuTimer));
    al_start_timer(menuTimer);

    while (1) {
        al_wait_for_event(menuQueue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            int total_width = numPersonagens * al_get_bitmap_width(sprites[0]) + (numPersonagens - 1) * ESPACO_ENTRE_PERSONAGENS;
            int start_x = (X_SCREEN - total_width) / 2;
            // Desenha os personagens
            for (int i = 0; i < numPersonagens; i++) {
                int x = start_x + i * (al_get_bitmap_width(sprites[0]) + ESPACO_ENTRE_PERSONAGENS);
                al_draw_bitmap(sprites[i], x, Y_SCREEN / 2 - al_get_bitmap_height(sprites[i]) / 2, 0);
                if (opcao == i)
                    al_draw_rectangle(x - 5, Y_SCREEN / 2 - al_get_bitmap_height(sprites[i]) / 2 - 5, x + al_get_bitmap_width(sprites[i]) + 5, Y_SCREEN / 2 + al_get_bitmap_height(sprites[i]) / 2 + 5, al_map_rgb(255, 0, 0), 3);
            }
       
            // Se todos os players selecionaram seus personagens
            if (player == 3) {
                al_flip_display();                
                al_rest(1.0); // Espera 1 segundo                
                al_stop_timer(menuTimer);
                al_destroy_timer(menuTimer);
                al_destroy_event_queue(menuQueue);
                return 0;

            // Exibe a mensagem para que o player selecione seu personagem
            } else {
                al_draw_textf(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN - 30, ALLEGRO_ALIGN_CENTRE, "Jogador %d: Selecione seu personagem", player);
                al_flip_display();                
            }
            
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                opcao = (opcao + numPersonagens - 1) % numPersonagens;
            else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                opcao = (opcao + 1) % numPersonagens;
            else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                if (player == 1 || (player == 2 && opcao != selecoes[0])) {
                    selecoes[player-1] = opcao;
                    sprites[opcao] = spritesPB[opcao]; // Modifica o sprite para spritesPB
                    player++;                    
                }
            }
        }
    }
    return 0;
}