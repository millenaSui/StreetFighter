#include "Personagem.h"
#include "Menus.h"

/* Função que cria um personagem */
personagem* criaPersonagem(unsigned char altura, unsigned char largura, unsigned char fronte, unsigned short x, unsigned short y, unsigned short maximoX, unsigned short maximoY, ALLEGRO_BITMAP *spriteAndando1, ALLEGRO_BITMAP *spriteAndando2, ALLEGRO_BITMAP *spritePulando, ALLEGRO_BITMAP *spriteAbaixando, ALLEGRO_BITMAP *spriteAtacando, ALLEGRO_BITMAP *spriteAtacandoBaixo, ALLEGRO_BITMAP *spriteAtacandoAlto) {

	if ((x - largura/2 < 0) || (x + largura/2 > maximoX) || (y - altura/2 < 0) || (y + altura/2 > maximoY)) 
		return NULL;
	if (fronte > 3) 
		return NULL;

	personagem *novoPersonagem = (personagem*) malloc(sizeof(personagem));
	if (!novoPersonagem) 
		return NULL;
	
	/*Se a alocação deu certo, preenche os campos da estrutura*/
	novoPersonagem->altura = altura;
	novoPersonagem->largura = largura;
	novoPersonagem->fronte = fronte;
	novoPersonagem->hp = 10;
	novoPersonagem->x = x;
	novoPersonagem->y = y;
	novoPersonagem->controle = criaJoystick();
	novoPersonagem->ataque = inicializaAtaque();
	novoPersonagem->spriteAndando1 = spriteAndando1;
	novoPersonagem->spriteAndando2 = spriteAndando2;
	novoPersonagem->spritePulando = spritePulando;
	novoPersonagem->spriteAbaixando = spriteAbaixando;
	novoPersonagem->spriteAtacando = spriteAtacando;
	novoPersonagem->spriteAtacandoBaixo = spriteAtacandoBaixo;
	novoPersonagem->spriteAtacandoAlto = spriteAtacandoAlto;											
	
	return novoPersonagem;
}

/* Função que movimenta um personagem */
void movimentaPersonagem(personagem *elemento, char passos, unsigned char caminho, unsigned short maximoX, unsigned short maximoY) {

	if (!caminho){ 
		if ((elemento->x - passos*PASSOS) - elemento->largura/2 >= 0) 
			elemento->x = elemento->x - passos*PASSOS;
	
	} else if (caminho == 1){ 
		if ((elemento->x + passos*PASSOS) + elemento->largura/2 <= maximoX) 
			elemento->x = elemento->x + passos*PASSOS;
	
	} else if (caminho == 2){ // Movimentação pra cima
		if ((elemento->y - passos*PASSOS) - elemento->altura/2 >= 0) 
			elemento->y = elemento->y - passos*PASSOS;
	
	} else if (caminho == 3){ // Movimentação pra baixo 
		if ((elemento->y + passos*PASSOS) + elemento->altura/2 <= maximoY) 
			elemento->y = elemento->y + (passos*PASSOS)/2;
	}
}

/* Função que aplica o ataque de um personagem */
void atacaPersonagem(personagem *elemento) {
	dano *dano;
	if (!elemento->fronte) 
		dano = aplicaAtaque(elemento->x - elemento->largura/2, elemento->y, elemento->fronte, elemento->ataque);
	else if (elemento->fronte == 1) 
		dano = aplicaAtaque(elemento->x + elemento->largura/2, elemento->y, elemento->fronte, elemento->ataque);
	if (dano) 
		elemento->ataque->aplicacoes = dano;
}

/* Função que reseta um personagem após início de nova partida*/
void resetaPersonagem(personagem *elemento, int xInicial, int yInicial) {
	elemento->x = xInicial;
	elemento->y = yInicial;
	elemento->hp = 10;
	elemento->ataque->timer = 0;
	elemento->ataque->aplicacoes = NULL;
	elemento->controle->esquerda = 0;
	elemento->controle->direita = 0;
	elemento->controle->cima.pulo = 0;
	elemento->controle->baixo = 0;
	elemento->controle->ataca = 0;
}

/* Função que destroi um personagem */
void destroiPersonagem(personagem *elemento) {
	destroiAtaque(elemento->ataque);
	destroiJoystick(elemento->controle);
	free(elemento);
}

/* Função que verifica se houve colisão entre dois personagens */
unsigned char verificaColisaoPersonagens(personagem *player1, personagem *player2) {
    if ((((player2->y-player2->altura/2 >= player1->y-player1->altura/2) && (player1->y+player1->altura/2 >= player2->y-player2->altura/2)) ||
        ((player1->y-player1->altura/2 >= player2->y-player2->altura/2) && (player2->y+player2->altura/2 >= player1->y-player1->altura/2))) && 
        (((player2->x-player2->largura/2 >= player1->x-player1->largura/2) && (player1->x+player1->largura/2 >= player2->x-player2->largura/2)) ||
        ((player1->x-player1->largura/2 >= player2->x-player2->largura/2) && (player2->x+player2->largura/2 >= player1->x-player1->largura/2)))) return 1;
    else return 0;
}

/* Função que verifica se um personagem foi derrotado */
unsigned char verificaDerrotaPersonagem(personagem *atacante, personagem *atacado) {
    dano *previo = NULL;
    for (dano *index = atacante->ataque->aplicacoes; index != NULL; index = (dano*) index->next) {
        if ((index->x >= atacado->x - atacado->largura/2) && (index->x <= atacado->x + atacado->largura/2) &&
            (index->y >= atacado->y - atacado->altura/2) && (index->y <= atacado->y + atacado->altura/2)) {
            atacado->hp--; // Reduz o HP do personagem atingido
            if (atacado->hp > 0) {
                if (previo) {
                    previo->next = index->next;
                    destroiDano(index);
                    index = (dano*) previo->next;
                } else {
                    atacante->ataque->aplicacoes = (dano*) index->next;
                    destroiDano(index);
                    index = atacante->ataque->aplicacoes;
                }
                return 0;
            } else return 1; // O personagem foi derrotado
        }
        previo = index;
    }
    return 0;
}

/* Função que atualiza a posição dos personagens */
void atualizaPosicaoPersonagem(personagem *player1, personagem *player2, double dt) {
    if (player1->controle->esquerda) {
        player1->x -= VELOCIDADE_MOVIMENTO;
        if (verificaColisaoPersonagens(player1, player2)) {
            player1->x += VELOCIDADE_MOVIMENTO;
            if (player1->controle->ataca && !player1->ataque->timer) {
                atacaPersonagem(player1);
                player1->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player1->controle->direita) {
        player1->x += VELOCIDADE_MOVIMENTO;
        if (verificaColisaoPersonagens(player1, player2)) {
            player1->x -= VELOCIDADE_MOVIMENTO;
            if (player1->controle->ataca && !player1->ataque->timer) {
                atacaPersonagem(player1);
                player1->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player1->controle->cima.pulo) {
        player1->controle->cima.tempoPulo -= dt;
        if (player1->controle->cima.tempoPulo <= 0) {
            player1->controle->cima.pulo = false;
            player1->y = player1->controle->cima.alturaInicial; // Volta para a altura inicial
        } else {
            double fatorPulo = (DURACAO_PULO - player1->controle->cima.tempoPulo) / DURACAO_PULO;
            player1->y = player1->controle->cima.alturaInicial - ALTURA_PULO * (1 - fatorPulo * fatorPulo); // Equação do movimento parabólico
        }
    }

    if (player1->controle->baixo) {
        if (verificaColisaoPersonagens(player1, player2)) {
            if (player1->controle->ataca && !player1->ataque->timer) {
                atacaPersonagem(player1);
                player1->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player2->controle->esquerda) {
        player2->x -= VELOCIDADE_MOVIMENTO;
        if (verificaColisaoPersonagens(player2, player1)) {
            player2->x += VELOCIDADE_MOVIMENTO;
            if (player2->controle->ataca && !player2->ataque->timer) {
                atacaPersonagem(player2);
                player2->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player2->controle->direita) {
        player2->x += VELOCIDADE_MOVIMENTO;
        if (verificaColisaoPersonagens(player2, player1)) {
            player2->x -= VELOCIDADE_MOVIMENTO;
            if (player2->controle->ataca && !player2->ataque->timer) {
                atacaPersonagem(player2);
                player2->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player2->controle->cima.pulo) {
        player2->controle->cima.tempoPulo -= dt;
        if (player2->controle->cima.tempoPulo <= 0) {
            player2->controle->cima.pulo = false;
            player2->y = player2->controle->cima.alturaInicial; // Volta para a altura inicial
        } else {
            double fatorPulo = (DURACAO_PULO - player2->controle->cima.tempoPulo) / DURACAO_PULO;
            player2->y = player2->controle->cima.alturaInicial - ALTURA_PULO * (1 - fatorPulo * fatorPulo); // Equação do movimento parabólico
        }
    }

    if (player2->controle->baixo) {
        if (verificaColisaoPersonagens(player2, player1)) {
            if (player2->controle->ataca && !player2->ataque->timer) {
                atacaPersonagem(player2);
                player2->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }
    atualizaDanoPersonagem(player1);
    atualizaDanoPersonagem(player2);
}

void atualizaDanoPersonagem(personagem *player){
    dano *previo = NULL;
    for (dano *index = player->ataque->aplicacoes; index != NULL;){
        if (!index->trajetoria) index->x -= MOVE_DANO;
        else if (index->trajetoria == 1) index->x += MOVE_DANO;

        if ((index->x < 0) || (index->x > X_SCREEN)){
            if (previo){
                previo->next = index->next;
                destroiDano(index);
                index = (dano*) previo->next;
            } else {
                player->ataque->aplicacoes = (dano*) index->next;
                destroiDano(index);
                index = player->ataque->aplicacoes;
            }
        } else{
            previo = index;
            index = (dano*) index->next;
        }
    }
}