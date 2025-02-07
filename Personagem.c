#include "Personagem.h"
#include "Menus.h"

/* Função que cria um personagem */
personagem* cria_personagem(unsigned char altura, unsigned char largura, unsigned char fronte, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, ALLEGRO_BITMAP *sprite_andarI, ALLEGRO_BITMAP *sprite_andarII, ALLEGRO_BITMAP *sprite_pulo, ALLEGRO_BITMAP *sprite_abaixando, ALLEGRO_BITMAP *sprite_atacando, ALLEGRO_BITMAP *sprite_atacando_baixo, ALLEGRO_BITMAP *sprite_atacando_alto){

	if ((x - largura/2 < 0) || (x + largura/2 > max_x) || (y - altura/2 < 0) || (y + altura/2 > max_y)) 
		return NULL;
	if (fronte > 3) 
		return NULL;

	personagem *novo_personagem = (personagem*) malloc(sizeof(personagem));
	if (!novo_personagem) 
		return NULL;
	
	/*Se a alocação deu certo, preenche os campos da estrutura*/
	novo_personagem->altura = altura;
	novo_personagem->largura = largura;
	novo_personagem->fronte = fronte;
	novo_personagem->hp = 10;
	novo_personagem->x = x;
	novo_personagem->y = y;
	novo_personagem->controle = cria_joystick();
	novo_personagem->ataque = inicializa_ataque();
	novo_personagem->sprite_andando_1 = sprite_andarI;
	novo_personagem->sprite_andando_2 = sprite_andarII;
	novo_personagem->sprite_pulando = sprite_pulo;
	novo_personagem->sprite_abaixando = sprite_abaixando;
	novo_personagem->sprite_atacando = sprite_atacando;
	novo_personagem->sprite_atacando_baixo = sprite_atacando_baixo;
	novo_personagem->sprite_atacando_alto = sprite_atacando_alto;											
	
	return novo_personagem;
}

/* Função que movimenta um personagem */
void movimentacao_de_personagem(personagem *elemento, char passos, unsigned char caminho, unsigned short max_x, unsigned short max_y){

	if (!caminho){ 
		if ((elemento->x - passos*PASSOS_PERSONAGEM) - elemento->largura/2 >= 0) 
			elemento->x = elemento->x - passos*PASSOS_PERSONAGEM;
	
	} else if (caminho == 1){ 
		if ((elemento->x + passos*PASSOS_PERSONAGEM) + elemento->largura/2 <= max_x) 
			elemento->x = elemento->x + passos*PASSOS_PERSONAGEM;
	
	} else if (caminho == 2){ // Movimentação pra cima
		if ((elemento->y - passos*PASSOS_PERSONAGEM) - elemento->altura/2 >= 0) 
			elemento->y = elemento->y - passos*PASSOS_PERSONAGEM;
	
	} else if (caminho == 3){ // Movimentação pra baixo 
		if ((elemento->y + passos*PASSOS_PERSONAGEM) + elemento->altura/2 <= max_y) 
			elemento->y = elemento->y + (passos*PASSOS_PERSONAGEM)/2;
	}
}

/* Função que aplica o ataque de um personagem */
void ataque_de_personagem(personagem *elemento){
	dano *dano;
	if (!elemento->fronte) 
		dano = aplica_ataque(elemento->x - elemento->largura/2, elemento->y, elemento->fronte, elemento->ataque);
	else if (elemento->fronte == 1) 
		dano = aplica_ataque(elemento->x + elemento->largura/2, elemento->y, elemento->fronte, elemento->ataque);
	if (dano) 
		elemento->ataque->aplicacoes = dano;
}

/* Função que reseta um personagem após início de nova partida*/
void reseta_personagem(personagem *elemento, int x_inicial, int y_inicial){
	elemento->x = x_inicial;
	elemento->y = y_inicial;
	elemento->hp = 10;
	elemento->ataque->timer = 0;
	elemento->ataque->aplicacoes = NULL;
	elemento->controle->esquerda = 0;
	elemento->controle->direita = 0;
	elemento->controle->cima.pulando = 0;
	elemento->controle->baixo = 0;
	elemento->controle->ataca = 0;
}

/* Função que destroi um personagem */
void destroi_personagem(personagem *elemento){
	destroi_ataque(elemento->ataque);
	destroi_joystick(elemento->controle);
	free(elemento);
}

/* Função que verifica se houve colisão entre dois personagens */
unsigned char verifica_colisao(personagem *player_1, personagem *player_2) {
    if ((((player_2->y-player_2->altura/2 >= player_1->y-player_1->altura/2) && (player_1->y+player_1->altura/2 >= player_2->y-player_2->altura/2)) ||
        ((player_1->y-player_1->altura/2 >= player_2->y-player_2->altura/2) && (player_2->y+player_2->altura/2 >= player_1->y-player_1->altura/2))) && 
        (((player_2->x-player_2->largura/2 >= player_1->x-player_1->largura/2) && (player_1->x+player_1->largura/2 >= player_2->x-player_2->largura/2)) ||
        ((player_1->x-player_1->largura/2 >= player_2->x-player_2->largura/2) && (player_2->x+player_2->largura/2 >= player_1->x-player_1->largura/2)))) return 1;
    else return 0;
}

/* Função que verifica se um personagem foi derrotado */
unsigned char verifica_derrota(personagem *atacante, personagem *atacado) {
    dano *previo = NULL;
    for (dano *index = atacante->ataque->aplicacoes; index != NULL; index = (dano*) index->next) {
        if ((index->x >= atacado->x - atacado->largura/2) && (index->x <= atacado->x + atacado->largura/2) &&
            (index->y >= atacado->y - atacado->altura/2) && (index->y <= atacado->y + atacado->altura/2)) {
            atacado->hp--; // Reduz o HP do personagem atingido
            if (atacado->hp > 0) {
                if (previo) {
                    previo->next = index->next;
                    destroi_dano(index);
                    index = (dano*) previo->next;
                } else {
                    atacante->ataque->aplicacoes = (dano*) index->next;
                    destroi_dano(index);
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
void atualiza_posicao(personagem *player_1, personagem *player_2, double dt) {
    if (player_1->controle->esquerda) {
        player_1->x -= MOVIMENTO_VELOCIDADE;
        if (verifica_colisao(player_1, player_2)) {
            player_1->x += MOVIMENTO_VELOCIDADE;
            if (player_1->controle->ataca && !player_1->ataque->timer) {
                ataque_de_personagem(player_1);
                player_1->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player_1->controle->direita) {
        player_1->x += MOVIMENTO_VELOCIDADE;
        if (verifica_colisao(player_1, player_2)) {
            player_1->x -= MOVIMENTO_VELOCIDADE;
            if (player_1->controle->ataca && !player_1->ataque->timer) {
                ataque_de_personagem(player_1);
                player_1->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player_1->controle->cima.pulando) {
        player_1->controle->cima.tempo_pulo -= dt;
        if (player_1->controle->cima.tempo_pulo <= 0) {
            player_1->controle->cima.pulando = false;
            player_1->y = player_1->controle->cima.altura_inicial; // Volta para a altura inicial
        } else {
            double fator_pulo = (PULO_DURACAO - player_1->controle->cima.tempo_pulo) / PULO_DURACAO;
            player_1->y = player_1->controle->cima.altura_inicial - PULO_ALTURA * (1 - fator_pulo * fator_pulo); // Equação do movimento parabólico
        }
    }

    if (player_1->controle->baixo) {
        if (verifica_colisao(player_1, player_2)) {
            if (player_1->controle->ataca && !player_1->ataque->timer) {
                ataque_de_personagem(player_1);
                player_1->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player_2->controle->esquerda) {
        player_2->x -= MOVIMENTO_VELOCIDADE;
        if (verifica_colisao(player_2, player_1)) {
            player_2->x += MOVIMENTO_VELOCIDADE;
            if (player_2->controle->ataca && !player_2->ataque->timer) {
                ataque_de_personagem(player_2);
                player_2->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player_2->controle->direita) {
        player_2->x += MOVIMENTO_VELOCIDADE;
        if (verifica_colisao(player_2, player_1)) {
            player_2->x -= MOVIMENTO_VELOCIDADE;
            if (player_2->controle->ataca && !player_2->ataque->timer) {
                ataque_de_personagem(player_2);
                player_2->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }

    if (player_2->controle->cima.pulando) {
        player_2->controle->cima.tempo_pulo -= dt;
        if (player_2->controle->cima.tempo_pulo <= 0) {
            player_2->controle->cima.pulando = false;
            player_2->y = player_2->controle->cima.altura_inicial; // Volta para a altura inicial
        } else {
            double fator_pulo = (PULO_DURACAO - player_2->controle->cima.tempo_pulo) / PULO_DURACAO;
            player_2->y = player_2->controle->cima.altura_inicial - PULO_ALTURA * (1 - fator_pulo * fator_pulo); // Equação do movimento parabólico
        }
    }

    if (player_2->controle->baixo) {
        if (verifica_colisao(player_2, player_1)) {
            if (player_2->controle->ataca && !player_2->ataque->timer) {
                ataque_de_personagem(player_2);
                player_2->ataque->timer = COOLDOWN_ATAQUE;
            }
        }
    }
    atualiza_dano(player_1);
    atualiza_dano(player_2);
}

/* Função que carrega os sprites do personagem */
SpritesPersonagem carregar_sprites_personagem(const char* diretorio_base) {
    SpritesPersonagem sprites;
    char caminho[256];

    sprintf(caminho, "%s/1.png", diretorio_base);
    sprites.sprite_andandoI = al_load_bitmap(caminho);

    sprintf(caminho, "%s/2.png", diretorio_base);
    sprites.sprite_andandoII = al_load_bitmap(caminho);

    sprintf(caminho, "%s/3.png", diretorio_base);
    sprites.sprite_atacando = al_load_bitmap(caminho);

    sprintf(caminho, "%s/4.png", diretorio_base);
    sprites.sprite_pulando = al_load_bitmap(caminho);

    sprintf(caminho, "%s/5.png", diretorio_base);
    sprites.sprite_abaixado = al_load_bitmap(caminho);

    sprintf(caminho, "%s/6.png", diretorio_base);
    sprites.sprite_atacando_baixo = al_load_bitmap(caminho);

    sprintf(caminho, "%s/7.png", diretorio_base);
    sprites.sprite_atacando_alto = al_load_bitmap(caminho);

    return sprites;
}

void atualiza_dano(personagem *player){
    dano *previo = NULL;
    for (dano *index = player->ataque->aplicacoes; index != NULL;){
        if (!index->trajetoria) index->x -= MOVE_DANO;
        else if (index->trajetoria == 1) index->x += MOVE_DANO;

        if ((index->x < 0) || (index->x > X_SCREEN)){
            if (previo){
                previo->next = index->next;
                destroi_dano(index);
                index = (dano*) previo->next;
            } else {
                player->ataque->aplicacoes = (dano*) index->next;
                destroi_dano(index);
                index = player->ataque->aplicacoes;
            }
        } else{
            previo = index;
            index = (dano*) index->next;
        }
    }
}