#ifndef FISICAS_H
#define FISICAS_H

#include <stdbool.h>

#include "figura.h"

/*
    *Coloca la polilinea en la posicion x y. Devuelve false si la polilinea == NULL.
*/
bool posicionar(figura_t *figura, float x, float y);

double computar_velocidad(double v, double a, double dt);

double computar_posicion(double p, double vi, double dt);

double computar_angulo(float x_centro, float y_centro, float x_movil, float y_movil);

double grados_a_rad(double grados);

double distancia_entre_puntos(double x1, double y1, double x2, double y2);




#endif