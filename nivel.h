#ifndef NIVEL_H
#define NIVEL_H

#include <stddef.h>
#include <SDL2/SDL.h>

#include "figura.h"
#include "lista_enlazada.h"

typedef struct nivel nivel_t;


/*
    Crea un nivel, con la cantidad de torretas y de tanques de combustible incializados en 0.
*/
nivel_t *nivel_crear(figura_t *f);

void nivel_destruir(nivel_t *n);

void nivel_set_listas(nivel_t *nivel, lista_t *lista_torretas, lista_t *lista_tanques);
/*
    *libera la memoria consumida por las entidades que habian en el nivel.
    Destruye las figuras del nivel, tambien las listas de disparos y de torretas
*/
void nivel_destruir(nivel_t *nivel);

/*********************************************************************************/
/**************************     GETERS   *****************************************/
/*********************************************************************************/
/*
    *Devuelve la figura del nivel
*/
figura_t *nivel_get_figura(nivel_t *nivel);

/*
    *Devuelve la lista de torretas que hay en el nivel
*/
lista_t *nivel_get_torretas(nivel_t *nivel);

/*
    *Devuelve la lista de combustibles que hay en el nivel
*/
lista_t *nivel_get_tanques(nivel_t *nivel);

bool nivel_get_infinito(nivel_t *nivel);

/*
    *Devuelve el puntaje bonus asociado al nivel.
*/
double nivel_get_bonus(nivel_t *nivel);

/*********************************************************************************/
/**************************     SETERS   *****************************************/
/*********************************************************************************/


void nivel_setear_infinito(nivel_t *n, bool inf);

void nivel_setear_bonus(nivel_t *, double bonus);



double nivel_min_x(nivel_t *nivel);

double nivel_min_y(nivel_t *nivel);

double nivel_max_x(nivel_t *nivel);

double nivel_max_y(nivel_t *nivel);

double nivel_ancho(nivel_t *nivel);

double nivel_alto(nivel_t *nivel);

/*
    Imprime la figura del nivel.
*/
void nivel_imprimir(nivel_t *nivel,SDL_Renderer *renderer, float factor, double dx, double dy); 

#endif