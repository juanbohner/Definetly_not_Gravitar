#ifndef POLILINEA_H
#define POLILINEA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "color.h"
#include "config.h"

#define MASK_NUM_PUNTOS         0x03ff      //0000 0011 1111 1111
#define MASK_COLOR_POLI         0xe000      //1110 0000 0000 0000

#define LARGO_FILA                      2
#define X                               0
#define Y                               1

/*
    *Estructura que contendrá la cantidad de puntos y las coordenadas de la polilinea.
*/
typedef struct polilinea polilinea_t;



/*
    *cree una nueva polilínea con n puntos pero sin inicializar sus coordenadas. 
    *La función devuelve la polilínea creada o NULL en caso de falla.
*/
polilinea_t *polilinea_crear_vacia(size_t n);

/*
    *Crea la polilinea inicilaizandola con los puntos_recibidos
*/
polilinea_t *polilinea_crear(const float puntos_recibidos[][2], size_t num,  color_t color);

/*
    *Libera la memoria asociada a la polilinea previamente creada. 
*/
void polilinea_destruir(polilinea_t *polilinea);

/*
    Hace una copia de la polilinea
*/
polilinea_t *polilinea_clonar(const polilinea_t *polilinea);


/***********************************************************************/
//Seters y geters

/*
    En pos de la polilinea asigna al punto el valor (x,y)
*/
bool polilinea_setear_punto(polilinea_t *polilinea, size_t pos, float x, float y);

/*
    *Asigna un color a la polilinea
    retorna false si falla. 
*/
void polilinea_setear_color(polilinea_t *polilinea, color_t color);

/*
    cant puntos
*/
size_t polilinea_cantidad_puntos(const polilinea_t *polilinea);

/*
    por interfaz en x e y obtiene los valores del punto en la polilinea
    si falla devuelve flase.
*/
bool polilinea_get_punto(const polilinea_t *polilinea, size_t pos, float *x, float *y);

/*
    *Devuelve la norma entre los dos puntos introducidos
*/



/*
    Funciones devuelven el valor de las componentes del color de la polilinea
*/
uint8_t polilinea_get_color_r(const polilinea_t *p);
uint8_t polilinea_get_color_g(const polilinea_t *p);
uint8_t polilinea_get_color_b(const polilinea_t *p);


/***********************************/
//Transformaciones

/*
    traslada la polilinea en dx e dy
*/
void polilinea_trasladar(polilinea_t *p, float dx, float dy);

/*
    rota RESPECTO DEL ORIGEN, recibe angulo en radianes.
*/
void polilinea_rotar(polilinea_t *p, double rad);


/*
    *Cambia el factor de escala de la polilinea
*/
void polilinea_cambiar_factor(polilinea_t *p, float factor);


/*
    *Camlcula la distancia entre polilinea y un punto
*/
double polilinea_interseccion(polilinea_t *p, double px, double py);

/*
    Devuelve la distancia entre un punto y el punto mas cercano de la polilinea
*/
double polilinea_distancia(polilinea_t *p, size_t n, float px, float py);

double polilinea_min_x(polilinea_t *polilinea);

double polilinea_min_y(polilinea_t *polilinea);

double polilinea_max_x(polilinea_t *polilinea);

double polilinea_max_y(polilinea_t *polilinea);

double polilinea_ancho(polilinea_t *polilinea);

double polilinea_alto(polilinea_t *polilinea);

/*
    *Imprime la polilinea en un planeta fijo.
*/
void polilinea_imprimir(polilinea_t *p, SDL_Renderer *renderer, float factor, double dx, double dy);

/*
    *Imprime la polilinea para un planeta que se esta moviendo.
*/
void polilinea_imprimir_movida(polilinea_t *p, SDL_Renderer *renderer, float factor, double dx, double dy);

#endif