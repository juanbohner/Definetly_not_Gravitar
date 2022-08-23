#ifndef PLANETA_H
#define PLANETA_H

#include <stdio.h>
#include "figura.h"
#include "nave.h"


typedef enum {
        BASE1   ,
        ESTRELLA,
        PLANETA1,
        PLANETA2,
        PLANETA3,
        PLANETA4,
        PLANETA5,
} planetas_nombres_t;



//estructura con los datos de un planeta y funciones relacionadas con el planeta
typedef struct planeta planeta_t;

/********************************************************************/
/*********************  MEMORIA  ************************************/
/********************************************************************/


/*
    hace la reserva de memoria de la estructira del planeta
*/
planeta_t *planeta_crear(figura_t *f);

void planeta_destruir(planeta_t *planeta);

/********************************************************************/
/*********************  SETERS  *************************************/
/********************************************************************/


void planeta_set_posicion(planeta_t *p, double x, double y);

void planeta_trasladar(planeta_t *planeta,  int dx, int dy);


/********************************************************************/
/*********************  GETERS  *************************************/
/********************************************************************/

double planeta_get_coord_x(planeta_t *p);

double planeta_get_coord_y(planeta_t *p);

/*
    *Devuelve la figura asociada al planeta.
*/
figura_t *planeta_get_figura(const planeta_t *p);


/*
    *Devuelve si la posicion esta dentro de un radio(dist) desde el centro del planeta, o no.
*/
bool planeta_interseccion(planeta_t *p, float dist, double px, double py, float factor);


/*
    *Devuelve la distancia entre la nave y un planeta
*/
double planeta_distancia(planeta_t *planeta, nave_t *nave);

/********************************************************************/
/*********************  IMPRESION  *************************************/
/********************************************************************/

/*
    *Imprime el planeta.
*/
void planeta_dibujar(SDL_Renderer *renderer, const planeta_t *planeta, float escala);


#endif