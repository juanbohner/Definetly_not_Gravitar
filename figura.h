#ifndef FIGURA_H
#define FIGURA_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "polilinea.h"


/*********************************************/
//mover a config?
#define MASK_TIPO 0x07     // 0000 0111
#define MASK_INFINITO 0x40 // 0100 0000
/*********************************************/

// Representa los tipos de figuras posibles
typedef enum figura_tipo{
    ICONO = 0,
    NIVEL = 1,
    SPRITE = 2,
    PLANETA = 3,
    BASE = 4,
    COMBUSTIBLE = 5,
    TORRETA = 6,
    REACTOR = 7,
} figura_tipo_t;

typedef struct figura figura_t;


// Convierte el tipo de figura que recibe en un cadena.
const char *figura_tipo_a_cadena(figura_tipo_t figura);

/***********************************************************************/
/***********************************************************************/
/***********    MANEJO DE MEMORIA DE FIGURAS    ************************/
/***********************************************************************/
/***********************************************************************/

/*
    Reserva memoria para la figura, la inicializa con un nombre, un tipo, si es infinita o no.

    Devuelve NULL si no pudo crear una figura.
*/
figura_t *figura_crear(const char nombre[], figura_tipo_t tipo, bool inf, size_t cant_polilineas);

//en la figura arma el vector. Si falla indica con false. Cambia el largo del vectror si falla.
bool figura_armar_vector_de_polilineas(FILE *fi, figura_t *figura);

/*
    Libera la memoria de la figura. 
*/
void figura_destruir(figura_t *figura);

void _figura_destruir(void *figura);

/*
    *Devuelve un clon de la figura recibida.
*/
figura_t *figura_clonar(const figura_t *figura);


/****************************************/
/***********   GETERS   *****************/
/****************************************/
char *figura_get_nombre(figura_t *figura);

figura_tipo_t figura_get_tipo(figura_t *figura);

polilinea_t *figura_get_polilinea(figura_t *figura, size_t i);

size_t figura_get_n_poli(figura_t *figura);


/****************************************/
/***********   SETERS   *****************/
/****************************************/
/*
    Setea el arreglo de polineas de la figura
    Precondicion de uso: figura_t->n_poli debe ser igual a la longitud de polilineas
*/
bool figura_setear_polilinea(figura_t *figura, polilinea_t **polilinea);

/*
    Traslada las polilineas de la figura en (dx,dy)
*/
void figura_polilinea_trasladar(figura_t *figura, int dx, int dy);

/*
    Rota las polilineas de una figura con respecto del origen.
	recibe los grados en radianes y un punto de referencia con respecto del origen
*/
void figura_polilinea_rotar(figura_t *figura, double rad);

/*
    *Aplica primero una rotacion en phi grados y luego una traslacion en dx y dy.
*/
void figura_polilinea_rototrasladar(figura_t *f, double phi, double dx, double dy);


/****************************************/
/****   IMPRESION DE FIGURAS   **********/
/****************************************/

/*
    *Indica si un punto que se le pasa intersecta con la figura.
*/
bool figura_interseccion(figura_t *f, float distancia, double px, double py, float factor/* , double traslacion_x, double traslacion_y */);

double figura_min_x(figura_t *figura);

double figura_min_y(figura_t *figura);

double figura_max_x(figura_t *figura);

double figura_max_y(figura_t *figura);

double figura_ancho(figura_t *figura);

double figura_alto(figura_t *figura);


//Para imprimir las figuras de la lista
/*
    *Imprime la figura para un planeta que se esta moviendo.
*/
void figura_imprimir_movida(figura_t *figura, SDL_Renderer *renderer, float factor, double dx, double dy);

/*
    *Imprime la figura en un planeta fijo.
*/
void figura_imprimir(figura_t *figura, SDL_Renderer *renderer, float factor, double dx, double dy);


#endif