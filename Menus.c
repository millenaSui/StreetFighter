#include "Menus.h"

/* Função para exibir o menu inicial */
int exibir_menu_inicial(ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp) {
    int opcao = 0;
    int estado_menu = 0;

    ALLEGRO_EVENT event;
    ALLEGRO_TIMER* menu_timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* menu_queue = al_create_event_queue();    
    al_register_event_source(menu_queue, al_get_keyboard_event_source());
    al_register_event_source(menu_queue, al_get_timer_event_source(menu_timer));
    al_start_timer(menu_timer);

    while (1) {
        al_wait_for_event(menu_queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (estado_menu == 0) {
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
           
            } else if (estado_menu == 1) { // Tela de ajuda
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
            if (estado_menu == 0) {
                
                if (event.keyboard.keycode == ALLEGRO_KEY_UP) // Move para cima
                    opcao = (opcao + 2) % 3;
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) // Move para baixo
                    opcao = (opcao + 1) % 3; // Move para baixo
                else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) { // Seleciona
                    if (opcao == 0) {
                        al_stop_timer(menu_timer);
                        al_destroy_timer(menu_timer);
                        al_destroy_event_queue(menu_queue);
                        return opcao; // Iniciar Jogo
                    
                    } else if (opcao == 1) {
                        estado_menu = 1; // Vai para a tela de ajuda
                        opcao = 0; // Reseta a opção na tela de ajuda
                    
                    } else if (opcao == 2) { // Sai do jogo
                        al_stop_timer(menu_timer);
                        al_destroy_timer(menu_timer);
                        al_destroy_event_queue(menu_queue);
                        return -1; // Sair
                    }
                }
            } else if (estado_menu == 1) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    estado_menu = 0; // Volta para o menu principal
            }
        }
    }
    al_destroy_timer(menu_timer);
    al_destroy_event_queue(menu_queue);
    
    return opcao;
}

/* Função para exibir o menu de seleção de personagens */
int exibir_menu_personagens(ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp, ALLEGRO_BITMAP* sprites[], ALLEGRO_BITMAP* spritesPB[], int num_personagens, int* selecoes) {
    int opcao = 0;
    int jogador = 1;
    selecoes[0] = -1; // Initialize with an invalid index
    selecoes[1] = -1; // Initialize with an invalid index
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER* menu_timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* menu_queue = al_create_event_queue();

    al_register_event_source(menu_queue, al_get_keyboard_event_source());
    al_register_event_source(menu_queue, al_get_timer_event_source(menu_timer));
    al_start_timer(menu_timer);

    while (1) {
        al_wait_for_event(menu_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            int total_width = num_personagens * al_get_bitmap_width(sprites[0]) + (num_personagens - 1) * ESPACO_ENTRE_PERSONAGENS;
            int start_x = (X_SCREEN - total_width) / 2;
            // Desenha os personagens
            for (int i = 0; i < num_personagens; i++) {
                int x = start_x + i * (al_get_bitmap_width(sprites[0]) + ESPACO_ENTRE_PERSONAGENS);
                al_draw_bitmap(sprites[i], x, Y_SCREEN / 2 - al_get_bitmap_height(sprites[i]) / 2, 0);
                if (opcao == i)
                    al_draw_rectangle(x - 5, Y_SCREEN / 2 - al_get_bitmap_height(sprites[i]) / 2 - 5, x + al_get_bitmap_width(sprites[i]) + 5, Y_SCREEN / 2 + al_get_bitmap_height(sprites[i]) / 2 + 5, al_map_rgb(255, 0, 0), 3);
            }
       
            // Se todos os jogadores selecionaram seus personagens
            if (jogador == 3) {
                al_flip_display();                
                al_rest(1.0); // Espera 1 segundo                
                al_stop_timer(menu_timer);
                al_destroy_timer(menu_timer);
                al_destroy_event_queue(menu_queue);
                return 0;

            // Exibe a mensagem para que o jogador selecione seu personagem
            } else {
                al_draw_textf(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN - 30, ALLEGRO_ALIGN_CENTRE, "Jogador %d: Selecione seu personagem", jogador);
                al_flip_display();                
            }
            
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                opcao = (opcao + num_personagens - 1) % num_personagens;
            else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                opcao = (opcao + 1) % num_personagens;
            else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                if (jogador == 1 || (jogador == 2 && opcao != selecoes[0])) {
                    selecoes[jogador-1] = opcao;
                    sprites[opcao] = spritesPB[opcao]; // Modifica o sprite para spritesPB
                    jogador++;                    
                }
            }
        }
    }
    return 0;
}