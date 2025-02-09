#ifndef __JOYSTICK__
#define __JOYSTICK__

#include <stdbool.h>
#include <stdlib.h>

/* Estruturas de joystick */
typedef struct {
    bool pulo;
    double tempoPulo;
    double alturaInicial;
} ControlePulo;

typedef struct {
    unsigned char direita;
    unsigned char esquerda;
    ControlePulo cima;
    unsigned char baixo;
    unsigned char ataca;
} joystick;

/* Funções de joystick */
joystick* criaJoystick();
void movePersonagemEsquerda(joystick *elemento);
void movePersonagemDireita(joystick *elemento);
void personagemPula(joystick *elemento);
void personagemAbaixa(joystick *elemento);
void personagemAtaca(joystick *elemento);
void destroiJoystick(joystick *elemento);

#endif