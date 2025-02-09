#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#define X_SCREEN 600 // Largura da tela
#define Y_SCREEN 303 // Altura da tela
#define ESPACO_ENTRE_PERSONAGENS 20

/* Funções de menu */
int exibirMenuInicial(ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp);
int exibirMenuPersonagens(ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp, ALLEGRO_BITMAP* sprites[], ALLEGRO_BITMAP* spritesPB[], int num_personagens, int* selecoes);