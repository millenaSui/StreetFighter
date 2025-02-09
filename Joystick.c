#include "Joystick.h"

/* Criação de joystick */
joystick* criaJoystick() {
	joystick *elemento = (joystick*) malloc (sizeof(joystick));
	
	if (!elemento) return NULL;
	elemento->direita = 0;
	elemento->esquerda = 0;
	elemento->cima.pulo = 0;
	elemento->baixo = 0;
	elemento->ataca = 0;

	return elemento;
}

/* Mover personagem para a esquerda */
void movePersonagemEsquerda(joystick *elemento) { 
	elemento->esquerda = elemento->esquerda ^ 1;
}

/* Mover personagem para a direita */
void movePersonagemDireita(joystick *elemento) { 
	elemento->direita = elemento->direita ^ 1;
}

/* Pulo de personagem */
void personagemPula(joystick *elemento) {  
	elemento->cima.pulo = elemento->cima.pulo ^ 1;
}

/* Abaixar personagem */
void personagemAbaixa(joystick *elemento) { 
	elemento->baixo = elemento->baixo ^ 1;
}

/* Função de disparo */
void personagemAtaca(joystick *elemento) { 
	elemento->ataca = elemento->ataca ^ 1;
}

/* Destroi joystick */
void destroiJoystick(joystick *elemento) {
	free(elemento);
}