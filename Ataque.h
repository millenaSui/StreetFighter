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
ataque* inicializa_ataque();
dano* inicializa_dano(unsigned short x, unsigned short y, unsigned char trajetoria, dano *next);
dano* aplica_ataque(unsigned short x, unsigned short y, unsigned char trajetoria, ataque *gun);
void aplica_dano(dano *elements);
void destroi_ataque(ataque *element);
void destroi_dano(dano *element);

#endif