#ifndef TANQUE_H
#define TANQUE_H

#include <stddef.h>
#include <SDL2/SDL.h>

#include "figura.h"

typedef struct tanque tanque_t;

tanque_t *tanque_crear(double px, double py, double angulo);

void tanque_destruir(tanque_t *t);

double tanque_get_coord_x(tanque_t *t);

double tanque_get_coord_y(tanque_t *t);

figura_t *tanque_get_figura(tanque_t *t);


/*
    *Setea la figura del tanque
*/
void tanque_setear_figura(tanque_t *t, figura_t *f);

/*
    *Imprime la figura del tanque.
*/
void tanque_imprimir(tanque_t *tanque, SDL_Renderer *renderer, float factor, double dx, double dy);

#endif