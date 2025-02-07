#ifndef __JOYSTICK__
#define __JOYSTICK__

#include <stdbool.h>
#include <stdlib.h>

/* Estruturas de joystick */
typedef struct {
    bool pulando;
    double tempo_pulo;
    double altura_inicial;
} ControlePulo;

typedef struct {
    unsigned char direita;
    unsigned char esquerda;
    ControlePulo cima;
    unsigned char baixo;
    unsigned char ataca;
} joystick;

/* Funções de joystick */
joystick* cria_joystick();
void move_personagem_esquerda(joystick *elemento);
void move_personagem_direita(joystick *elemento);
void personagem_abaixa(joystick *elemento);
void personagem_pula(joystick *elemento);
void personagem_ataca(joystick *elemento);
void destroi_joystick(joystick *elemento);

#endif