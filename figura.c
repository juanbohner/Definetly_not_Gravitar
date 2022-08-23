#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "figura.h"
#include "lectura.h"
#include "color.h"
#include "polilinea.h"

#define MASK_TIPO 0x07     // 0000 0111
#define MASK_INFINITO 0x40 // 0100 0000

// Tabla de busqueda
static const char *tipo_str[] = {
    [ICONO] = "Icono",
    [NIVEL] = "Nivel",
    [SPRITE] = "Sprite",
    [PLANETA] = "Planeta",
    [BASE] = "Base",
    [COMBUSTIBLE] = "Combustible",
    [TORRETA] = "Torreta",
    [REACTOR] = "Reactor",
};

struct figura{
    char nombre[20];
    figura_tipo_t tipo;
    bool infinito;

    // vector de polilinea_t *poliliena de largo n_poli
    polilinea_t **polilinea;
    size_t n_poli;
};

const char *figura_tipo_a_cadena(figura_tipo_t tipo){
    return tipo_str[tipo];
}

bool figura_armar_vector_de_polilineas(FILE *fi, figura_t *figura){
    //vector de polilineas
    polilinea_t **vect = malloc(sizeof(polilinea_t *) * figura->n_poli);
    if(vect == NULL)    return false;

    figura->polilinea = vect;

    for(size_t i = 0; i < figura->n_poli; i++){
        polilinea_t *p = leer_polilinea(fi);
        if(p == NULL){ //falla la lectrua de la i-esima polilinea.
            //Destruyo la figura que no pude leer
            figura->n_poli = i; 
            figura_destruir(figura); 
            free(p);
            return false; //destruyo la figura y devuelvo falso para indicarlo.
        }
        (figura->polilinea)[i] = p;
    }
    return true;
}


//Crea una figura con todas la variables inciializadas, menos el vector de polilineas pero no tiene inicializado el vectro de polilineas.
figura_t *figura_crear(const char nombre[], figura_tipo_t tipo, bool inf, size_t cant_polilineas){
    static figura_t *f;
    f = malloc(sizeof(figura_t));
    if (f == NULL)
        return NULL;
    strcpy(f->nombre, nombre);
    f->tipo = tipo;
    f->n_poli = cant_polilineas;
    f->infinito = inf;
	f->polilinea = NULL;
	
    return f;
}


void figura_destruir(figura_t *figura){
    for (size_t i = 0; i < (figura->n_poli); i++){
        polilinea_destruir(figura->polilinea[i]); // Se destruye cada una de las polilineas del arreglo.
    }
    free(figura->polilinea);
    free(figura);
}

void _figura_destruir(void *figura){
    figura_destruir(figura);
}


figura_t *figura_clonar(const figura_t *figura){
    //Creo una figura nueva que es la que devuelvo
    figura_t *f = figura_crear(figura->nombre, figura->tipo, figura->infinito, figura->n_poli);
    if(f == NULL){
        return NULL;
    }

    polilinea_t **vect_poli = malloc(sizeof(polilinea_t *) * f->n_poli);

    if(vect_poli == NULL){
        figura_destruir(f);
        return NULL;
    }
    size_t i;
    for(i = 0; i < f->n_poli; i++){
        //Creo cada polilinea del vector de polilineas_t *poli

        vect_poli[i] = polilinea_clonar(figura->polilinea[i]); 

        if(vect_poli[i] == NULL){
            for(size_t j = 0; j < i; j++){
                polilinea_destruir(vect_poli[j]);
            }
            
            free(vect_poli);
            figura_destruir(f);
            return NULL;
        }
    }
    
    if(!figura_setear_polilinea(f, vect_poli)){
        for(size_t j = 0; j < i; j++){
            polilinea_destruir(vect_poli[j]);
        }
            
        free(vect_poli);
        figura_destruir(f);
        return NULL;
    }
    return f;
}



//geters
char *figura_get_nombre(figura_t *figura){
    return figura->nombre;
}

figura_tipo_t figura_get_tipo(figura_t *figura){
    return figura->tipo;
}

polilinea_t *figura_get_polilinea(figura_t *figura, size_t i){
    return (figura->polilinea)[i];
}

size_t figura_get_n_poli(figura_t *figura){
    return figura->n_poli;
}

//Seter

bool figura_setear_polilinea(figura_t *figura, polilinea_t **polilinea){
    if((figura->polilinea = polilinea) == NULL)
        return false;
    return true;
}


void figura_polilinea_trasladar(figura_t *figura, int dx, int dy){
    for(size_t i = 0; i < figura->n_poli; i++){
        polilinea_trasladar((figura->polilinea)[i], dx, dy);
    }
}

void figura_polilinea_rotar(figura_t *figura, double rad){
    for(size_t i = 0; i < figura->n_poli; i++){
        polilinea_rotar((figura->polilinea)[i], rad);   //La roto
    }
}

void figura_polilinea_rototrasladar(figura_t *f, double phi, double dx, double dy){
        figura_polilinea_rotar(f,phi); 
        figura_polilinea_trasladar(f,dx,dy);
}


bool figura_interseccion(figura_t *f, float dist,double px, double py, float factor/*, double traslacion_x_figura, double traslacion_y, double angulo */){
    figura_t *clon = figura_clonar(f);
    for(size_t i = 0; i < f->n_poli; i++){
        polilinea_cambiar_factor(clon->polilinea[i], factor);
        if(polilinea_distancia(clon->polilinea[i], polilinea_cantidad_puntos(clon->polilinea[i]), px, py) <= dist){
            figura_destruir(clon);
            return true;
        }
    }
    figura_destruir(clon);
    return false;
}

double figura_min_x(figura_t *figura){
    double min = polilinea_min_x(figura->polilinea[0]);
    for(size_t i = 1; i < figura->n_poli; i++){
        double min_i = polilinea_min_x(figura->polilinea[i]);
        if(min > min_i)
            min = min_i;
    }
    return min;
}

double figura_min_y(figura_t *figura){
    double min = polilinea_min_y(figura->polilinea[0]);
    for(size_t i = 1; i < figura->n_poli; i++){
        double min_i = polilinea_min_y(figura->polilinea[i]);
        if(min > min_i)
            min = min_i;
    }
    return min;
}

double figura_max_x(figura_t *figura){
    double max = polilinea_max_x(figura->polilinea[0]);
    for(size_t i = 1; i < figura->n_poli; i++){
        double max_i = polilinea_max_x(figura->polilinea[i]);
        if(max < max_i)
            max = max_i;
    }
    return max;
}

double figura_max_y(figura_t *figura){
    double max = polilinea_max_y(figura->polilinea[0]);
    for(size_t i = 1; i < figura->n_poli; i++){
        double max_i = polilinea_max_y(figura->polilinea[i]);
        if(max < max_i)
            max = max_i;
    }
    return max;
}

double figura_ancho(figura_t *figura){
    return figura_max_x(figura) - figura_min_x(figura);
}

double figura_alto(figura_t *figura){
    return figura_max_y(figura) - figura_min_y(figura);
}

//Impresion de la polinea de la figura
void figura_imprimir_movida(figura_t *figura, SDL_Renderer *renderer, float factor, double dx, double dy){
    for(size_t i = 0; i < figura->n_poli; i++){
        polilinea_imprimir_movida(figura->polilinea[i],renderer, factor, dx, dy);
    }
}

void figura_imprimir(figura_t *figura, SDL_Renderer *renderer, float factor, double dx, double dy){
    for(size_t i = 0; i < figura->n_poli; i++){
        polilinea_imprimir(figura->polilinea[i],renderer, factor, dx, dy);
    }
}



