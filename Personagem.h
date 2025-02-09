#ifndef __PERSONAGEM__ 	
#define __PERSONAGEM__

#include <allegro5/allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include "Joystick.h"
#include "Ataque.h"

#define PASSOS 10 // Em pixels
#define DURACAO_PULO 0.7 // Em segundos
#define ALTURA_PULO 50 // Em pixels
#define VELOCIDADE_MOVIMENTO 5 // Velocidade de movimento dos personagens

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
	ALLEGRO_BITMAP *spriteAndando1;
	ALLEGRO_BITMAP *spriteAndando2;
	ALLEGRO_BITMAP *spritePulando;
	ALLEGRO_BITMAP *spriteAbaixando;
	ALLEGRO_BITMAP *spriteAtacando;
	ALLEGRO_BITMAP *spriteAtacandoBaixo;
	ALLEGRO_BITMAP *spriteAtacandoAlto;
    ALLEGRO_BITMAP *spriteAtual;
} personagem;

/* Funções de personagem */
personagem* criaPersonagem(unsigned char altura, unsigned char largura, unsigned char fronte, unsigned short x, unsigned short y, unsigned short maximoX, unsigned short maximoY, ALLEGRO_BITMAP *spriteAndando1, ALLEGRO_BITMAP *spriteAndando2, ALLEGRO_BITMAP *spritePulando, ALLEGRO_BITMAP *spriteAbaixando, ALLEGRO_BITMAP *spriteAtacando, ALLEGRO_BITMAP *spriteAtacandoBaixo, ALLEGRO_BITMAP *spriteAtacandoAlto);
void movimentaPersonagem(personagem *elemento, char passos, unsigned char caminho, unsigned short maximoX, unsigned short maximoY);
void atacaPersonagem(personagem *elemento);
void resetaPersonagem(personagem *elemento, int xInicial, int yInicial);
void destroiPersonagem(personagem *elemento);
unsigned char verificaColisaoPersonagens(personagem *player1, personagem *player2);
unsigned char verificaDerrotaPersonagem(personagem *atacante, personagem *atacado);
void atualizaPosicaoPersonagem(personagem *player1, personagem *player2, double dt);
void atualizaDanoPersonagem(personagem *player);

#endif