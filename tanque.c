#include "tanque.h"

#define X 0
#define Y 1

struct tanque{
	figura_t *figura_tanque;
	double coord[2];
	double angulo;
};

tanque_t *tanque_crear(double px, double py, double angulo){
	tanque_t *tanque = malloc(sizeof(tanque_t));
	if(tanque == NULL)return NULL;

	tanque->coord[X] = px;
	tanque->coord[Y] = py;
	tanque->angulo = angulo;

	tanque->figura_tanque = NULL;

	return tanque;
}

void tanque_destruir(tanque_t *t){
	if(t->figura_tanque != NULL){
		figura_destruir(t->figura_tanque);
	}
	free(t);
}

double tanque_get_coord_x(tanque_t *t){
    return t->coord[X];
}

double tanque_get_coord_y(tanque_t *t){
    return t->coord[Y];
}

double tanque_get_angulo(tanque_t *t){
    return t->angulo;
}

figura_t *tanque_get_figura(tanque_t *t){
	return t->figura_tanque;
}


void tanque_setear_figura(tanque_t *t, figura_t *f){
    //LIbero la memoria de la figura actual del tanque
	if(t->figura_tanque != NULL){
		figura_destruir(t->figura_tanque);
	}	
	//Le damos una figura a cada figura del TDA de la nave
    t->figura_tanque = figura_clonar(f);
    
    figura_polilinea_rototrasladar(t->figura_tanque, t->angulo, t->coord[X], t->coord[Y]);
}

void tanque_imprimir(tanque_t *tanque, SDL_Renderer *renderer, float factor, double dx, double dy){
        figura_imprimir_movida(tanque->figura_tanque, renderer, factor, dx, dy);  
}