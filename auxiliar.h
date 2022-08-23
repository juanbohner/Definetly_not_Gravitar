#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <SDL2/SDL.h>

#include "figura.h"
#include "disparo.h"
#include "lista_enlazada.h"
#include "nivel.h"
#include "torreta.h"
#include "nave.h"
#include "tanque.h"
#include "planeta.h"

//Es un TDA auxiliar creado por barbara para tener algunas funciones que va a usar en el main y hacer que sea mas compacto el main.

/*
    *Se hace la impresion del disparo de la lista. En caso de falla devuelve false. 
*/
bool auxiliar_disparo_dibujar(lista_t *lista_disparos, SDL_Renderer *renderer, figura_t *disparo_fig, double factor,  bool infinito, double dx, double dy);

/*
    *Devuelve true si algun disparo de la lista interseccta la figura que se le pasa
*/
bool auxiliar_disparo_interseccion(lista_t *lista_disparos, figura_t *fig, double distancia, bool disparo_nave, float factor);



/*
    *Retorna la figura que tenga el nombre que se le pasa. En caso de pertenecer al vector de figuras
*/
figura_t *auxiliar_vect_figuras_buscar_figura_por_nombre(figura_t **vect, const char *nombre, size_t n);

//Manejo de los disparos de las torretas y la impresion. Ademas si la torreta choca con un disparo de la nave, se elimina.
bool auxiliar_recorrer_lista_torretas(lista_t *l_torretas, lista_t *l_disparos, SDL_Renderer *renderer, nave_t *nave, nivel_t *nivel, figura_t *torreta_fig, figura_t *torreta_disparo_fig, float factor, bool infinito, double planeta_ancho, double movido_dx, double movido_dy, double *puntaje);

//Manejo de los tanques y su impresion. Ademas si la nave con escudo agarra el tanque, se elimina.
bool auxiliar_recorrer_lista_tanques( lista_t *l_tanques, SDL_Renderer *renderer, nave_t *nave, figura_t *tanque_fig, bool escudo, float factor, bool infinito, double planeta_ancho, double movido_dx, double movido_dy, double *combustible);

//Manejo de los reactores y su impresion. Ademas si la nave le dispara al reactor, se elimina.
bool auxiliar_recorrer_lista_tanques_reactor(lista_t *l_tanques, lista_t *l_disparos, SDL_Renderer *renderer, nave_t *nave, nivel_t *nivel, figura_t *reactor_fig, float factor, double planeta_ancho, double movido_dx, double movido_dy, double *puntaje);

//Funcion para destruir el vector de figuras que se crea en main.c
void auxiliar_vector_figuras_destruir(figura_t **vect, size_t n);

//Crea las listas para cada nivel, ya con su cantidad de torretas y tanques de combustible 
void auxiliar_setear_listas_torretas_y_tanques_nivel(nivel_t *nivel, uint8_t n);

//
void auxiliar_vector_de_planetas_destruir(planeta_t **planetas, size_t n);

//Destruccion del arreglo de punteros nivele_t
void auxiliar_vector_de_niveles_destruir(nivel_t **niveles, size_t n);



#endif