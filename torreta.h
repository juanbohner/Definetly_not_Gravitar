#ifndef TORRETA_H
#define TORRETA_H

#include <stdbool.h>
#include <SDL2/SDL.h>


#include "figura.h"
#include "config.h"

typedef struct torreta torreta_t;

/********************************************************************/
/*********************  MANEJO DE MEMORIA  **************************/
/********************************************************************/

torreta_t *torreta_crear(double px, double py, double angulo);

void torreta_destruir(torreta_t *t);

/********************************************************************/
/*********************  GETERS  *************************************/
/********************************************************************/


figura_t *torreta_get_torreta_sin_disparo(torreta_t *torreta);

figura_t *torreta_get_torreta_con_disparo(torreta_t *torreta);

double torreta_get_angulo(torreta_t *torreta);

double torreta_get_coordenada_x(torreta_t *torreta);

double torreta_get_coordenada_y(torreta_t *torreta);

double torreta_get_angulo_apuntado(torreta_t *torreta);

double torreta_get_cooldown(torreta_t *torreta);

/********************************************************************/
/*********************  SETERS  *************************************/
/********************************************************************/

/*
    *Setea las figuras de la torreta en su respectiva posicion.
*/
void torreta_setear_figuras(torreta_t *torreta, figura_t *figura_torreta_sin_disparo, figura_t *figura_torreta_con_disparo);

/*
    *Setea el cooldown de la torreta
*/
void torreta_set_cooldown(torreta_t *t, double cooldown);

/*
    *Cambia el cooldown de la nave en += dt
*/
void torreta_incrementar_cooldown(torreta_t *t, double dt);

/*
    *Setea el angulo y la posicion de la torreta
*/
void torreta_set_coord(torreta_t *t, double angulo, double px, double py);

void torreta_apuntar(torreta_t *t, double coord_x_nave, double coord_y_nave);

void torreta_roto_trasladar(torreta_t *torreta, double dx, double dy, double angulo);



/********************************************************************/
/*********************  IMPRESION  **********************************/
/********************************************************************/

/*
    *Imprime la figura del tanque.
*/
void torreta_imprimir(torreta_t *torreta, SDL_Renderer *renderer, float factor, bool disparo, double dx, double dy);


#endif