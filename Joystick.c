#include "Joystick.h"

/* Criação de joystick */
joystick* cria_joystick() {
	joystick *elemento = (joystick*) malloc (sizeof(joystick));
	
	if (!elemento) return NULL;
	elemento->direita = 0;
	elemento->esquerda = 0;
	elemento->cima.pulando = 0;
	elemento->baixo = 0;
	elemento->ataca = 0;

	return elemento;
}

/* Mover personagem para a esquerda */
void move_personagem_esquerda(joystick *elemento) { 
	elemento->esquerda = elemento->esquerda ^ 1;
}

/* Mover personagem para a direita */
void move_personagem_direita(joystick *elemento) { 
	elemento->direita = elemento->direita ^ 1;
}

/* Pulo de personagem */
void personagem_pula(joystick *elemento) {  
	elemento->cima.pulando = elemento->cima.pulando ^ 1;
}

/* Abaixar personagem */
void personagem_abaixa(joystick *elemento) { 
	elemento->baixo = elemento->baixo ^ 1;
}

/* Função de disparo */
void personagem_ataca(joystick *elemento) { 
	elemento->ataca = elemento->ataca ^ 1;
}

/* Destroi joystick */
void destroi_joystick(joystick *elemento) {
	free(elemento);
}