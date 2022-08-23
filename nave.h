#ifndef NAVE_H
#define NAVE_H

#include <SDL2/SDL.h>

#include "figura.h"
/*
    *Estructura de la nave
        Esta va a poseer la figura de la nave, otra para la nave con el propulsor encencido. El angulo de la nave, su posicion y su velocidad.
*/
typedef struct nave nave_t;


/***********************************************************/
/***************    MANEJO DE MEMORIA      *****************/
/***********************************************************/

/*  
    *Crea una nave con todo en 0. 
    *ERROR -> NULL
*/
nave_t *nave_crear(void);


/*
    *Destruye la nave, libera la memoria necesaria
*/
void nave_destruir(nave_t *n);

/*********************************************************************/
/************************   GETERS   *********************************/
/*********************************************************************/

double nave_get_coord_x(nave_t *nave);

double nave_get_coord_y(nave_t *nave);

/*
    *Devuelve las coord. (retornan por interfaz)
*/
void nave_get_coord(nave_t *n, double *x, double *y);


/*
    *Devuelve las componentes de la v. (Retornan por interfaz)
*/
void nave_get_velocidad(nave_t *n, double *v_x, double *v_y);

/*
    *Devuelve el angulo
*/
double nave_get_angulo(nave_t *n);

/*
    *Devuelve la cantidad de combustible
*/
int nave_get_combustible(nave_t *n);

/*
    *Devuelve todos los anteriores (Por interfaz)
*/
void nave_get_all_data(nave_t *n, double *x, double *y, double *v_x, double *v_y, double *a);

/*
    *Devuelve la figura de la nave sin el chorro.
*/
figura_t *nave_get_figura_sin_chorro(nave_t *n);

/*
    *Devuelve el estado del escudo de la nave
*/
bool nave_get_status_escudo(nave_t *n);

/*
    *Devuelve el estado del escudo en el nivel
*/
bool nave_get_status_escudo_nivel(nave_t *n);




/*********************************************************************/
/************************   SETERS   *********************************/
/*********************************************************************/

/*  
    *Setear las figuras de la nave.
    Las pone en memoria y las coloca en su respectiva posicion con el angulo correcto 
*/
void nave_setear_figuras(nave_t *nave, const figura_t *fig_nave_sin_propulsor, const figura_t *fig_nave_con_propulsor, const figura_t *fig_escudo, const figura_t *fig_escudo_nivel);

/*
    *Setea la coordenadas
*/
void nave_set_coord(nave_t *n, double x, double y);

/*
    Cambia los estados de si está encendido el propulsor, el escudo y el escudo de nivel.
*/
void nave_set_propulsor_escudo_escudonivel(nave_t *n, bool propulsor, bool escudo, bool escudo_nivel);


/*
    *Setea la aceleracion de la nave. El TDA nave ya traquea por su cuenta la aceleracion que posee. 
*/
void nave_set_aceleracion(nave_t *n, double aceleracion);

/*
    *Modifica la celeracion del TDA nave en da. 
    Para agregar celeracion es poner un valor positivo.
    Para disminuir la aceleracion es poner un valor negativo
*/
void nave_set_acel_sumar(nave_t *n, double da);


/*
    *Setea la velocidad de la nave
*/
void nave_set_vel(nave_t *n, double v_x, double v_y);

/*
    *Cambia la cantidad total de combustible de la nave a c
*/
void nave_modificar_combustible(nave_t *n, int c);

/*
    *Agrega la cantidad c de combustible al actual
*/
void nave_sumar_combustible(nave_t *n, int c);


/*
    *Setea el angulo del escudo.
*/
void nave_set_escudo_angulo(nave_t *n, double angulo_escudo);

/*
    *Setea todo en 0 y los bool en falso, excepto la posicion que la setea donde se indica.
*/
void nave_reiniciar(nave_t *n, double coord_x, double coord_y);


/*********************************************************************/
/*****    FUNCIONES RELACIONADAS CON LA IMPRESION DE LA NAVE    ******/
/*********************************************************************/
/*
    *Rota la nave un cierto angulo respecto del angulo actual que posee la nave. 
    (Actualiza sus valores actuales)
*/
void nave_rotar(nave_t *n, double angulo);


/*
    *Actualiza las cooredenadas y la velocidad de la nave. Teniendo en cuenta la posicion donde está apuntado. Todo recibiendo una cierta acelaracion en un intevalo de tiempo dt.
*/
void nave_mover_adelante(nave_t *n, double aceleracion, double dt);


/*
    *Acerca la nave hacia el centro, con una aceleracion en un intervalo dt.
*/
void nave_acercar_a_posicion(nave_t *nave, double aceleracion, double p_x, double p_y, double dt);

/*
    *Acerca la nave en el sentido del angulo rad, con una aceleracion en un intervalo dt.
*/
void nave_gravedad_piso(nave_t *nave, double aceleracion, double dt);



/*
    *invierten las velocidades de la nave
*/
void nave_invertir_vel_x(nave_t *n);
void nave_invertir_vel_y(nave_t *n);



/************************************************************************************/
/****************************   IMPRESION DE LA NAVE   ******************************/
/************************************************************************************/
/*
    *Recibe el estado del propulsor y del escudo. 
    Imprime todas las figuras a la vez en la posicion actual de la nave para un planeta fijo.
*/
void nave_imprimir(nave_t *nave, SDL_Renderer *renderer, float factor);

/*
    *Recibe el estado del propulsor y del escudo. 
    Imprime todas las figuras a la vez en la posicion actual de la nave para un planeta que se esta moviendo.
*/
void nave_imprimir_movida(nave_t *nave, SDL_Renderer *renderer, float factor, double dx, double dy);

/*
    *Imprime la cantidad de vidas.
*/
void nave_vidas_mostrar(figura_t *figura, SDL_Renderer *renderer, char vidas);

#endif