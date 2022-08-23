#include <stdlib.h>


#include "planeta.h"
#include "fisicas.h"

struct planeta{
    figura_t *f_planeta;
    double coord[2];
};

//Crea una planeta sin una posicion especifica en pantalla
planeta_t *planeta_crear(figura_t *f){
    planeta_t *p = malloc(sizeof(planeta_t));
    if(p == NULL)   return NULL;

    p->f_planeta = figura_clonar(f);
    if(p->f_planeta == NULL){
        figura_destruir(p->f_planeta);
        free(p);
        return NULL;
    }
    
    return p;
}

void planeta_destruir(planeta_t *p){
    figura_destruir(p->f_planeta);
    free(p);    
}

//seters
void planeta_set_posicion(planeta_t *p, double x, double y){
    p->coord[X] = x;
    p->coord[Y] = y;
}

void planeta_trasladar(planeta_t *planeta,  int dx, int dy){
    planeta_set_posicion(planeta,dx,dy);
    figura_polilinea_trasladar(planeta->f_planeta, dx, dy);
}

//geters
double planeta_get_coord_x(planeta_t *p){
    return p->coord[X];
}

double planeta_get_coord_y(planeta_t *p){
    return p->coord[Y];
}

figura_t *planeta_get_figura(const planeta_t *p){
    return p->f_planeta;
}

bool planeta_interseccion(planeta_t *p,float dist, double px, double py, float factor){
    return figura_interseccion(p->f_planeta, dist, px, py, factor);
}

//Dibuja el planeta. Hace un clon de la figura, la traslada a donde tiene que ir y luego la imprime y libera la memoria de la figura 
void planeta_dibujar(SDL_Renderer *renderer, const planeta_t *planeta, float fact){
    figura_t *planeta_fig = figura_clonar(planeta->f_planeta);

    //figura_polilinea_trasladar(planeta_fig, planeta->coord[X], planeta->coord[Y]);
    figura_imprimir(planeta_fig,renderer,  fact, planeta->coord[X],planeta->coord[Y]);
    figura_destruir(planeta_fig);
}
