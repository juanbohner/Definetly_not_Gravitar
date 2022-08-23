#ifndef DISPARO_H
#define DISPARO_H

#include <stddef.h>
#include <SDL2/SDL.h>

#include "figura.h"

typedef struct disparo disparo_t;

/*********************************************************************/
/*******************    MANEJO DE MEMORIA   **************************/
/*********************************************************************/
/*
    *Reserva memoria para el disparo, inicializando su velocidad, su coordenada de donde se dispara y un angulo con respecto a la tangente. 
*/
disparo_t *disparo_crear(double angulo, double px, double py, double vx, double vy, bool propio);


/*
    *Se encarga de liberar memoria usada por el disparo. Hay que usarla para cuando el tiempo de vida del disparo es mayor al maximo o si toca algo.
*/
void disparo_destruir(disparo_t *d);


/*********************************************************************/
/*************************    GETERS   *******************************/
/*********************************************************************/
/*
    Para get las coordenadas
*/
double disparo_get_coord_x(disparo_t *d);
double disparo_get_coord_y(disparo_t *d);


/*
    *Si el disparo es de la nave es true y si es de una torreta es false
*/
bool disparo_get_pertenece_nave(disparo_t *d);

double disparo_get_tiempo_vida(disparo_t *d);

/*********************************************************************/
/*************************    SETERS   *******************************/
/*********************************************************************/

/*
    *Mueve la figura del disparo en la direccion a la que apunta.
*/
void disparo_mover_adelante(disparo_t *d, double dt);

/*
    *Setea la figura del disparo. Hay que usarlo en cada ciclo para actualizar la figura del disparo.
*/
bool disparo_setear_figura(disparo_t *d, figura_t *f);

/*
    *Le suma el intevalo dt al tiempo de vida del disparo.
*/
void disparo_sumar_dt(disparo_t *d, double dt);

/*
    *Devuelve la distancia entre el disparo y la posicion indicada.
*/
double disparo_interseccion(disparo_t *d, double px, double py);

/*
    *Imprime el disparo en un planeta fijo.
*/
void disparo_imprimir(disparo_t *d, SDL_Renderer *renderer, float factor);

/*
    *Imprime el disparo para un planeta que se esta moviendo.
*/
void disparo_imprimir_movido(disparo_t *d, SDL_Renderer *renderer, float factor, double dx, double dy);


#endif