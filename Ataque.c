#include <stdlib.h>
#include "Ataque.h"

/* Função que inicializa um ataque */
ataque* inicializa_ataque(){
	ataque *novo_ataque = (ataque*) malloc(sizeof(ataque));
	if (!novo_ataque) return NULL;
	novo_ataque->timer = 0;
	novo_ataque->aplicacoes = NULL;
	return novo_ataque;
}

/* Função que inicializa um dano */
dano* inicializa_dano(unsigned short x, unsigned short y, unsigned char trajetoria, dano *proximo){
	if ((trajetoria < 0) || (trajetoria > 1)) return NULL;
	dano *novo_dano = (dano*) malloc(sizeof(dano));
	if (!novo_dano) return NULL;
	novo_dano->x = x;
	novo_dano->y = y;
	novo_dano->trajetoria = trajetoria;
	novo_dano->next = (struct dano*) proximo;
	return novo_dano;
}

/* Função que aplica um ataque */
dano* aplica_ataque(unsigned short x, unsigned short y, unsigned char trajetoria, ataque *novo_ataque){
	dano *novo_dano = inicializa_dano(x, y, trajetoria, novo_ataque->aplicacoes);
	if (!novo_dano) return NULL;
	return novo_dano;
}

/* Função que aplica o dano de um ataque */
void aplica_dano(dano *elementos){
	for (dano *index = elementos; index != NULL; index = (dano*) index->next){
		if (!index->trajetoria) index->x = index->x - MOVE_DANO;
		else index->x = index->x + MOVE_DANO;
	}
}

/* Função que destroi um ataque */
void destroi_ataque(ataque *elemento){
	dano *sentinela;
	for (dano *index = elemento->aplicacoes; index != NULL; index = sentinela){
		sentinela = (dano*) index->next;
		destroi_dano(index);
	}
	free(elemento);
}

/* Função que destroi um dano */
void destroi_dano(dano *elemento){
	free(elemento);
}