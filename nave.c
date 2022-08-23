#include "nave.h"
#include "config.h"
#include "fisicas.h"

struct nave{
    figura_t *figura_nave_sin_propulsor;
    figura_t *figura_nave_con_propulsor;
	figura_t *figura_escudo;
    figura_t *figura_escudo_nivel;
    
    //Para activar luego las figuras de mi nave
    bool estado_propulsor;
    bool estado_escudo;
    bool estado_escudo_nivel;

    double  angulo;
    double  angulo_escudo;
    double  aceleracion;
    double  velocidad[2];
    double  coord[2];
		
    int combustible;
};

/***********************************************************/
/***************    MANEJO DE MEMORIA      *****************/
/***********************************************************/
nave_t *nave_crear(void){
    static nave_t *n; 
    n = malloc(sizeof(nave_t));
    if(n == NULL) return NULL;

    //La inicializo con todo esto en 0.
    n->angulo = grados_a_rad(0);
    n->angulo_escudo = grados_a_rad(0);
    n->velocidad[X] = 0;
    n->velocidad[Y] = 0;
    
    n->coord[X] = 0;
    n->coord[Y] = 0;
    
    n->aceleracion = 0;
    n->combustible = CANTIDAD_COMBUSTIBLE_INICIAL;

    n->figura_escudo = NULL;
    n->figura_escudo_nivel = NULL;
    n->figura_nave_con_propulsor = NULL;
    n->figura_nave_sin_propulsor = NULL;

    n->estado_escudo = false;
    n->estado_escudo_nivel = false;
    n->estado_propulsor = false;
    
    return n;
}

//Debe recibir una figura ya inicializada.
void nave_destruir(nave_t *n){
    figura_destruir(n->figura_nave_con_propulsor);
    figura_destruir(n->figura_nave_sin_propulsor);
    figura_destruir(n->figura_escudo);
    figura_destruir(n->figura_escudo_nivel);
    free(n);
}

/*********************************************************************/
/**********************     GETERS      ******************************/
/*********************************************************************/

double nave_get_coord_x(nave_t *nave){
    return nave->coord[X];
}

double nave_get_coord_y(nave_t *nave){
    return nave->coord[Y];
}

void nave_get_coord(nave_t *n, double *x, double *y){
    *x = nave_get_coord_x(n);
    *y = nave_get_coord_y(n);
}



void nave_get_velocidad(nave_t *n, double *v_x, double *v_y){
    *v_x = n->velocidad[X];
    *v_y = n->velocidad[Y];
}

double nave_get_angulo(nave_t *n){
    return n->angulo;
}

int nave_get_combustible(nave_t *n){
    return n->combustible;
}

//devulve todo lo que tiene la nave por interfaz
void nave_get_all_data(nave_t *n, double *x, double *y, double *v_x, double *v_y, double *a){
    nave_get_coord(n, x, y);
    nave_get_velocidad(n, v_x, v_y);
    *a =nave_get_angulo(n);
}

figura_t *nave_get_figura_sin_chorro(nave_t *n){
    return n->figura_nave_sin_propulsor;
}

/*********************************************************************/
/*********************      SETERS      ******************************/
/*********************************************************************/

void nave_setear_figuras(nave_t *nave, const figura_t *fig_nave_sin_propulsor, const figura_t *fig_nave_con_propulsor, const figura_t *fig_escudo, const figura_t *fig_escudo_nivel){
    //Destruyo las figuras de la nave, reemplazarlas por unas nuevas clonando.
    if(nave->figura_nave_sin_propulsor != NULL){
        figura_destruir(nave->figura_nave_sin_propulsor);
    }
    if(nave->figura_nave_con_propulsor != NULL){
        figura_destruir(nave->figura_nave_con_propulsor);
    }
    if(nave->figura_escudo != NULL){
        figura_destruir(nave->figura_escudo);
    }
    if(nave->figura_escudo_nivel != NULL){
        figura_destruir(nave->figura_escudo_nivel);
    }

    //Le damos una figura a cada figura del TDA de la nave
    nave->figura_nave_sin_propulsor = figura_clonar(fig_nave_sin_propulsor);
    nave->figura_nave_con_propulsor = figura_clonar(fig_nave_con_propulsor);
    nave->figura_escudo = figura_clonar(fig_escudo);
    nave->figura_escudo_nivel = figura_clonar(fig_escudo_nivel);

    //Posicionamos cada figura en su respectivo lugar y angulo
    figura_polilinea_rototrasladar(nave->figura_nave_sin_propulsor, nave->angulo, nave->coord[X], nave->coord[Y]);
    figura_polilinea_rototrasladar(nave->figura_nave_con_propulsor, nave->angulo, nave->coord[X], nave->coord[Y]);
    figura_polilinea_rototrasladar(nave->figura_escudo, nave->angulo_escudo, nave->coord[X], nave->coord[Y]);
    figura_polilinea_rototrasladar(nave->figura_escudo_nivel, nave->angulo_escudo, nave->coord[X], nave->coord[Y]);
}

void nave_set_propulsor_escudo_escudonivel(nave_t *n, bool propulsor, bool escudo, bool escudo_nivel){
    n->estado_propulsor = propulsor;
    n->estado_escudo = escudo;
    n->estado_escudo_nivel = escudo_nivel;

}

void nave_set_coord(nave_t *n, double x, double y){
    n->coord[X] = x;
    n->coord[Y] = y;
}

void nave_set_vel(nave_t *n, double v_x, double v_y){
    n->velocidad[X] = v_x;
    n->velocidad[Y] = v_y;
}

void nave_set_acel(nave_t *n, double aceleracion){
    n->aceleracion = aceleracion;
}

void nave_set_acel_sumar(nave_t *n, double da){
    n->aceleracion += da;
    
}

void nave_modificar_cobustible(nave_t *nave, int combustible){
    nave->combustible = combustible;
}

void nave_sumar_cobustible(nave_t *nave, int combustible){
    nave->combustible += combustible;
}

void nave_set_escudo_angulo(nave_t *n, double angulo_escudo){
    n->angulo_escudo = angulo_escudo;

    while(n->angulo_escudo > 2 * PI)
        n->angulo_escudo -= (2 * PI);
    
    while(n->angulo_escudo < 0)
        n->angulo_escudo += (2 * PI);
}

void nave_reiniciar(nave_t *n, double coord_x, double coord_y){
    n->estado_propulsor = false;
    n->estado_escudo = false;
    n->estado_escudo_nivel = false;

    n->angulo = 0;
    n->angulo_escudo = 0;
    n->aceleracion = 0;
    n->velocidad[X] = 0;
    n->velocidad[Y] = 0;

    n->coord[X] = coord_x;
    n->coord[Y] = coord_y;
}


/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
//FUNCIONES RELACIONADAS CON LA INTERACCION DE LA NAVE EN EL JUEGO

//FUNCIONES PRIVADAS
static void nave_computar_velocidad(nave_t *nave,double aceleracion, double rad, double dt){
    nave->velocidad[X] = computar_velocidad(nave->velocidad[X], aceleracion * cos(rad), dt);
    nave->velocidad[Y] = computar_velocidad(nave->velocidad[Y], aceleracion * sin(rad), dt);
}

static void nave_computar_coord(nave_t *n, double dt){
    n->coord[X] = computar_posicion(n->coord[X], n->velocidad[X], dt);
    n->coord[Y] = computar_posicion(n->coord[Y], n->velocidad[Y], dt);
}

static void nave_figuras_trasladar(nave_t *nave, double dx, double dy){
    //Muevo las polilineas de las figuras de la nave    
    figura_polilinea_trasladar(nave->figura_nave_sin_propulsor, dx, dy);
    figura_polilinea_trasladar(nave->figura_nave_con_propulsor, dx, dy);
    //Actualizo las coordenadas de mi nave
    //nave_set_coord(nave, dx, dy);
}

void nave_rotar(nave_t *nave, double phi){
    
    //Muevo la nave al origen
    nave_figuras_trasladar(nave, -nave->coord[X], -nave->coord[Y]);

    //Roto las figuras que hay que rotar
    figura_polilinea_rotar(nave->figura_nave_sin_propulsor, phi);
    figura_polilinea_rotar(nave->figura_nave_con_propulsor, phi);
    
    //Traslado nuevamente la figura a su posicion 
    nave_figuras_trasladar(nave, nave->coord[X], nave->coord[Y]);

    //Actualizo en angulo
    nave->angulo += phi;


    while(nave->angulo > 2 * PI)
        nave->angulo -= (2 * PI);

    while(nave->angulo < 0)
        nave->angulo += (2 * PI);
}

void nave_mover_adelante(nave_t *n, double aceleracion, double dt){
    nave_computar_velocidad(n, aceleracion, n->angulo,dt);
    nave_computar_coord(n, dt);
}

void nave_acercar_a_posicion(nave_t *nave, double aceleracion, double p_x, double p_y, double dt){
    static double angulo;
    angulo = computar_angulo(nave->coord[X], nave->coord[Y], p_x, p_y);
    nave_computar_velocidad(nave, aceleracion, angulo, dt);
    nave_computar_coord(nave, dt);
}

void nave_gravedad_piso(nave_t *nave, double aceleracion, double dt){
    nave_computar_velocidad(nave, aceleracion, -PI/2, dt);
    nave_computar_coord(nave, dt);
}

void nave_invertir_vel_x(nave_t *n){
    n->velocidad[X] = -n->velocidad[X];
}

void nave_invertir_vel_y(nave_t *n){
    n->velocidad[Y] = -n->velocidad[Y];
}


/************************************************************************************/
/****************************   IMPRESION DE LA NAVE   ******************************/
/************************************************************************************/
void nave_imprimir_movida(nave_t *nave, SDL_Renderer *renderer, float factor, double dx, double dy){
    if(nave->estado_propulsor){
        figura_imprimir_movida(nave->figura_nave_con_propulsor, renderer, factor, dx, dy);
    }
    else{
        figura_imprimir_movida(nave->figura_nave_sin_propulsor, renderer, factor, dx, dy);    
    }
    if(nave->estado_escudo){
        figura_imprimir_movida(nave->figura_escudo, renderer, factor, dx, dy);
    }
    if(nave->estado_escudo_nivel){
        figura_imprimir_movida(nave->figura_escudo_nivel, renderer, factor, dx, dy);    
    }

}


void nave_imprimir(nave_t *nave, SDL_Renderer *renderer, float factor){
    if(nave->estado_propulsor){
        figura_imprimir(nave->figura_nave_con_propulsor, renderer, factor, nave->coord[X], nave->coord[Y]);
    }
    else{
        figura_imprimir(nave->figura_nave_sin_propulsor, renderer, factor, nave->coord[X], nave->coord[Y]);    
    }
    
    if(nave->estado_escudo){
        figura_imprimir(nave->figura_escudo, renderer, factor, nave->coord[X], nave->coord[Y]);
    }
    if(nave->estado_escudo_nivel){
        figura_imprimir(nave->figura_escudo_nivel, renderer, factor, nave->coord[X], nave->coord[Y]);    
    }
}

void nave_vidas_mostrar(figura_t *figura, SDL_Renderer *renderer, char vidas){
    figura_t *clon = figura_clonar(figura);
    
    figura_polilinea_rotar(clon, PI/2);
    for(size_t n = 0; n < vidas; n++){
            if(n >= 5){
                figura_imprimir_movida(figura, renderer, 2, 50 + 30*(n - 5) , VENTANA_ALTO -80);
            } else{
                figura_imprimir_movida(figura, renderer, 2, 50 + 30*n , VENTANA_ALTO -40);
            }
    }

    figura_destruir(clon);
}