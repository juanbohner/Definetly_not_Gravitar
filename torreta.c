#include "torreta.h"

#define X 0
#define Y 1


struct torreta{
    figura_t *figura_torreta_sin_disparo;
    figura_t *figura_torreta_con_disparo;
    double angulo;
    double coord[2];
    double angulo_apuntado;
    double cooldown;
};

torreta_t *torreta_crear(double px, double py, double angulo){
    torreta_t *torreta = malloc(sizeof(torreta_t));
    if(torreta == NULL) return NULL;

    torreta->angulo = angulo;
    torreta->coord[X] = px;
    torreta->coord[Y] = py;
    torreta->angulo_apuntado = 0;
    torreta->cooldown = COOLDOWN_TORRETA;

    torreta->figura_torreta_con_disparo = NULL;
    torreta->figura_torreta_sin_disparo = NULL;

    return torreta;
}

void torreta_destruir(torreta_t *t){
    if(t->figura_torreta_con_disparo != NULL){
        figura_destruir(t->figura_torreta_con_disparo);
    }
    if(t->figura_torreta_sin_disparo != NULL){
        figura_destruir(t->figura_torreta_sin_disparo);
    }
    
    free(t);
}

/********************************************************************/
/*********************  GETERS  *************************************/
/********************************************************************/

figura_t *torreta_get_torreta_sin_disparo(torreta_t *torreta){
    return torreta->figura_torreta_sin_disparo;
}

figura_t *torreta_get_torreta_con_disparo(torreta_t *torreta){
    return torreta->figura_torreta_con_disparo;
}

double torreta_get_angulo(torreta_t *torreta){
    return torreta->angulo;
}

double torreta_get_coordenada_x(torreta_t *torreta){
    return torreta->coord[X];
}

double torreta_get_coordenada_y(torreta_t *torreta){
    return torreta->coord[Y];
}

double torreta_get_angulo_apuntado(torreta_t *torreta){
    return torreta->angulo_apuntado;
}

double torreta_get_cooldown(torreta_t *torreta){
    return torreta->cooldown;
}

/********************************************************************/
/*********************  SETERS  *************************************/
/********************************************************************/

void torreta_setear_figuras(torreta_t *torreta, figura_t *figura_torreta_sin_disparo, figura_t *figura_torreta_con_disparo){
    if(torreta->figura_torreta_con_disparo != NULL){
        figura_destruir(torreta->figura_torreta_con_disparo);
    }
    if(torreta->figura_torreta_sin_disparo != NULL){
        figura_destruir(torreta->figura_torreta_sin_disparo);
    }
    torreta->figura_torreta_sin_disparo = figura_clonar(figura_torreta_sin_disparo);
    torreta->figura_torreta_con_disparo = figura_clonar(figura_torreta_con_disparo);

    figura_polilinea_rototrasladar(torreta->figura_torreta_sin_disparo, torreta->angulo, torreta->coord[X],torreta->coord[Y]);
    figura_polilinea_rototrasladar(torreta->figura_torreta_con_disparo, torreta->angulo, torreta->coord[X],torreta->coord[Y]);
}

void torreta_set_cooldown(torreta_t *t, double cooldown){
    t->cooldown = cooldown;
}

void torreta_incrementar_cooldown(torreta_t *t, double dt){
    t->cooldown += dt;
}

void torreta_set_coord(torreta_t *t, double angulo, double px, double py){
    t->angulo = angulo;
    t->coord[X] = px;
    t->coord[Y] = py;
}

void torreta_apuntar(torreta_t *t, double coord_x_nave, double coord_y_nave){
    t->angulo_apuntado = atan2((coord_y_nave - t->coord[Y]) , (coord_x_nave - t->coord[X]));
}
/********************************************************************/
/*********************  IMPRESION  **********************************/
/********************************************************************/

void torreta_imprimir(torreta_t *torreta, SDL_Renderer *renderer, float factor, bool disparo, double dx, double dy){
    if(disparo){
        figura_imprimir_movida(torreta->figura_torreta_con_disparo, renderer, factor, dx, dy);
    }
    else{
        figura_imprimir_movida(torreta->figura_torreta_sin_disparo, renderer, factor, dx, dy);
    }    
}
