#include "disparo.h"
#include "nave.h"
#include "fisicas.h"


struct disparo{
    figura_t    *fig;
    double      coord[2];
    double      velocidad[2];
    double      angulo;
    double      tiempo_vida;      

    bool        propio;
};

/**********************************/
//Manejo de memoria
disparo_t *disparo_crear(double angulo, double px, double py, double vx, double vy, bool propio){
    disparo_t *d = malloc(sizeof(disparo_t));
    if(d == NULL)   return NULL;

    d->angulo = angulo;

    d->coord[X] = px +  5 *cos(angulo);
    d->coord[Y] = py +  5 *sin(angulo);

    d->velocidad[X] = vx;
    d->velocidad[Y] = vy;

    d->tiempo_vida = 0;

    d->propio = propio;

    d->fig = NULL;


    return d;
}

void disparo_destruir(disparo_t *d){
    if(d != NULL){
        if(d->fig != NULL)
            figura_destruir(d->fig);
        
        free(d);
    }
}

/*********************************************************************/
//GETERS

bool disparo_get_pertenece_nave(disparo_t *d){
    return d->propio;
}

double disparo_get_coord_x(disparo_t *d){
    return d->coord[X];
}

double disparo_get_coord_y(disparo_t *d){
    return d->coord[Y];
}

double disparo_get_tiempo_vida(disparo_t *d){
    return d->tiempo_vida;
}


/*********************************************************************/
//Seters

void disparo_mover_adelante(disparo_t *d, double dt){
    d->coord[X] = computar_posicion(d->coord[X], d->velocidad[X], dt);
    d->coord[Y] = computar_posicion(d->coord[Y], d->velocidad[Y], dt);
}

bool disparo_setear_figura(disparo_t *d, figura_t *f){
    //Libero la memoria de la figura vieja
    if(d->fig != NULL){
        figura_destruir(d->fig);
    }    
    d->fig = figura_clonar(f);
    
    if(d->fig == NULL){
        disparo_destruir(d);
        return false;
    }

    figura_polilinea_rototrasladar(d->fig, d->angulo, d->coord[X], d->coord[Y]);
    return true;
}

void disparo_sumar_dt(disparo_t *d, double dt){
    d->tiempo_vida += dt;
}

double disparo_interseccion(disparo_t *d, double px, double py){
    return distancia_entre_puntos(px, py, d->coord[X], d->coord[Y]);
}

void disparo_imprimir(disparo_t *d, SDL_Renderer *renderer, float factor){
    figura_imprimir(d->fig, renderer, factor, d->coord[X], d->coord[Y]);
}

void disparo_imprimir_movido(disparo_t *d, SDL_Renderer *renderer, float factor, double dx, double dy){
    figura_imprimir_movida(d->fig, renderer, factor, dx, dy);
}

