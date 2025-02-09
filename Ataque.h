#ifndef __PISTOL__
#define __PISTOL__

#include <stdlib.h>

#define COOLDOWN_ATAQUE 10
#define MOVE_DANO 5

/* Estruturas de ataque */
typedef struct {
	unsigned short x;
	unsigned short y;
	unsigned char trajetoria;
	struct dano *next;
} dano;		

typedef struct {
	unsigned char timer;
	int dano;
	dano *aplicacoes;	
} ataque;

/* Funções de ataque */
ataque* inicializaAtaque();
dano* inicializaDano(unsigned short x, unsigned short y, unsigned char trajetoria, dano *next);
dano* aplicaAtaque(unsigned short x, unsigned short y, unsigned char trajetoria, ataque *gun);
void aplicaDano(dano *elements);
void destroiAtaque(ataque *element);
void destroiDano(dano *element);

#endif