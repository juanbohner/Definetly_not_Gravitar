#include <math.h>

#include "fisicas.h"
#include "polilinea.h"

double computar_velocidad(double v, double a, double dt){
    return (v + a * dt);
}

double computar_posicion(double p, double vi, double dt){
    return (p + vi * dt);
}

double distancia_entre_puntos(double x1, double y1, double x2, double y2){
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double computar_angulo(float x_centro, float y_centro, float x_movil, float y_movil){
    return atan2((y_centro - y_movil), (x_centro - x_movil)) + PI;
}

double grados_a_rad(double grados){
    return grados * PI/180;
}

//Coloca la polilinea en la posicion x y
bool posicionar(figura_t *fig, float x, float y){
    for(size_t i = 0; i < figura_get_n_poli(fig); i++){
        polilinea_t *p = figura_get_polilinea(fig, i);
        if(p == NULL)   return false;

        polilinea_trasladar(p, x, y);
    }
    return true;
}