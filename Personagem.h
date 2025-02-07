#ifndef __PERSONAGEM__ 	
#define __PERSONAGEM__

#include <allegro5/allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include "Joystick.h"
#include "Ataque.h"

#define PASSOS_PERSONAGEM 10 // Em pixels
#define PULO_DURACAO 0.7 // Em segundos
#define PULO_ALTURA 50 // Em pixels
#define MOVIMENTO_VELOCIDADE 5 // Velocidade de movimento dos personagens

/* Estruturas de personagem e sprites */
typedef struct {
	unsigned char altura;
	unsigned char largura;
	unsigned char fronte;
	unsigned char hp;
	unsigned short x;
	unsigned short y;
	joystick *controle;
	ataque *ataque;	
	ALLEGRO_BITMAP *sprite_andando_1;
	ALLEGRO_BITMAP *sprite_andando_2;
	ALLEGRO_BITMAP *sprite_pulando;
	ALLEGRO_BITMAP *sprite_abaixando;
	ALLEGRO_BITMAP *sprite_atacando;
	ALLEGRO_BITMAP *sprite_atacando_baixo;
	ALLEGRO_BITMAP *sprite_atacando_alto;
    ALLEGRO_BITMAP *sprite_atual;
} personagem;

typedef struct {
    ALLEGRO_BITMAP* sprite_andandoI;
    ALLEGRO_BITMAP* sprite_andandoII;
    ALLEGRO_BITMAP* sprite_atacando;
    ALLEGRO_BITMAP* sprite_pulando;
    ALLEGRO_BITMAP* sprite_abaixado;
    ALLEGRO_BITMAP* sprite_atacando_baixo;
    ALLEGRO_BITMAP* sprite_atacando_alto;
} SpritesPersonagem;

/* Funções de personagem */
personagem* cria_personagem(unsigned char altura, unsigned char largura, unsigned char fronte, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, ALLEGRO_BITMAP *sprite_1, ALLEGRO_BITMAP *sprite_2, ALLEGRO_BITMAP *sprite_pulo, ALLEGRO_BITMAP *sprite_abaixando, ALLEGRO_BITMAP *sprite_atacando, ALLEGRO_BITMAP *sprite_atacando_baixo, ALLEGRO_BITMAP *sprite_atacando_alto);
void movimentacao_de_personagem(personagem *elemento, char passos, unsigned char caminho, unsigned short max_x, unsigned short max_y);
void ataque_de_personagem(personagem *elemento);
void reseta_personagem(personagem *elemento, int x_inicial, int y_inicial);
void destroi_personagem(personagem *elemento);
unsigned char verifica_colisao(personagem *player_1, personagem *player_2);
unsigned char verifica_derrota(personagem *atacante, personagem *atacado);
void atualiza_posicao(personagem *player_1, personagem *player_2, double dt);
SpritesPersonagem carregar_sprites_personagem(const char* diretorio_base);
void atualiza_dano(personagem *player);

#endif