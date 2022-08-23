#include "auxiliar.h"
#include "torreta.h"
#include "nave.h"
#include "tanque.h"


// Recibe la lista de disparos y los dibuja, fijandose si es un nivel infinito o no

bool auxiliar_disparo_dibujar(lista_t *lista_disparos, SDL_Renderer *renderer, figura_t *disparo_fig, double factor, bool infinito, double dx, double dy){
    lista_iter_t *iterador = lista_iter_crear(lista_disparos);
    if(iterador == NULL)    return false;

    //Itero sobre la lista para ver si un disparo debe ser impreso o no.
    for(size_t i = 0; i < lista_largo(lista_disparos); i++){
        disparo_t *act = lista_iter_ver_actual(iterador);
        disparo_sumar_dt(act, DT);

        if(disparo_get_tiempo_vida(act) >= MAXIMO_TIEMPO_DISPARO){
            disparo_destruir(act);
            lista_iter_borrar(iterador);
            continue;
        }

        if(!disparo_setear_figura(act, disparo_fig)){
            fprintf(stderr, "Error seteando la figura de un disparo\n");
            return false;
        }

        disparo_mover_adelante(act, DT);
        if(infinito){
            disparo_imprimir_movido(act, renderer, factor, dx, dy);
        } else{
            disparo_imprimir(act, renderer, factor);

        }

        lista_iter_avanzar(iterador);
    }

    lista_iter_destruir(iterador);
    return true;
}

// Se fija si el disparo intersecta con la figura recibida. Si el disparo salio de esa misma figura, no intersecta

bool auxiliar_disparo_interseccion(lista_t *lista_disparos, figura_t *fig, double distancia, bool disparo_nave, float factor){
    lista_iter_t *iterador = lista_iter_crear(lista_disparos);
    /* if(iterador == NULL)    return false; */

    //Itero sobre la lista para ver si un disparo debe ser impreso o no.
    for(size_t i = 0; i < lista_largo(lista_disparos); i++){
        disparo_t *act = lista_iter_ver_actual(iterador);

        if(disparo_get_pertenece_nave(act) ^ disparo_nave){
            if(figura_interseccion(fig, distancia, disparo_get_coord_x(act), disparo_get_coord_y(act), factor) == true){
                disparo_destruir(act);
                lista_iter_borrar(iterador);
                lista_iter_destruir(iterador);

                return true;
            }
        }

        lista_iter_avanzar(iterador);
    }

    lista_iter_destruir(iterador);
    return false;
}

#define DISTANCIA_INTERSECCION  1
#define TORRETA_COOLDOWN        5
#define DISTANCIA_TERRENO       5

//Manejo de los disparos de las torretas y la impresion. Ademas si la torreta choca con un disparo de la nave, se elimina.
bool auxiliar_recorrer_lista_torretas(lista_t *l_torretas, lista_t *l_disparos, SDL_Renderer *renderer, nave_t *nave, nivel_t *nivel, figura_t *torreta_fig, figura_t *torreta_disparo_fig, float factor, bool infinito, double planeta_ancho, double movido_dx, double movido_dy, double *puntaje){
    lista_iter_t *iterador = lista_iter_crear(l_torretas);
    if(iterador == NULL)    return false;

    bool disparando;

    for(size_t i = 0; i < lista_largo(l_torretas); i++){
        torreta_t *actual = lista_iter_ver_actual(iterador);

        torreta_setear_figuras(actual, torreta_fig, torreta_disparo_fig);
        if(auxiliar_disparo_interseccion(l_disparos, torreta_get_torreta_sin_disparo(actual), 3, false, factor)){
            torreta_destruir(actual);
            lista_iter_borrar(iterador);
            *puntaje += 250;
            continue;
        }
        
        if(torreta_get_cooldown(actual) <= 0){
            if(figura_interseccion(torreta_get_torreta_sin_disparo(actual), DISTANCIA_TORRETA_DISPARAR, nave_get_coord_x(nave), nave_get_coord_y(nave), factor)){
                 
                torreta_apuntar(actual, nave_get_coord_x(nave) / factor, nave_get_coord_y(nave) / factor);
                
                double angulo_apuntado = torreta_get_angulo_apuntado(actual);
                double angulo = torreta_get_angulo(actual);
                double ang = (angulo > (3*PI / 2))? angulo - 3*PI/2: angulo + PI/2 ;
                
                if((ang-angulo_apuntado <= PI/2 && ang-angulo_apuntado >=  -PI/2) || (ang-angulo_apuntado >= 3*PI/2)){ 
                    if(!lista_insertar_ultimo(l_disparos, disparo_crear(angulo_apuntado, (torreta_get_coordenada_x(actual) * factor + factor * 15 * cos(angulo + PI/2) ), (torreta_get_coordenada_y(actual) * factor + factor * 15 * sin(angulo + PI/2)), (VELOCIDAD_DISPARO * cos(angulo_apuntado)), (VELOCIDAD_DISPARO * sin(angulo_apuntado)), false))){
                        torreta_destruir(actual);
                        lista_iter_destruir(iterador);
                        return false;
                    }
                }
                torreta_set_cooldown(actual, TORRETA_COOLDOWN);

            }
        
        auxiliar_disparo_interseccion(l_disparos, nivel_get_figura(nivel), 1, true, factor);
        }

        if(torreta_get_cooldown(actual) < 0.2 && torreta_get_cooldown(actual) > 0 )
            disparando = true;
        else
            disparando = false;

        torreta_imprimir(actual,renderer,factor, disparando , movido_dx, movido_dy);
            
        if(infinito){
            torreta_imprimir(actual,renderer,factor,disparando, movido_dx - planeta_ancho*factor , movido_dy);
            torreta_imprimir(actual,renderer,factor,disparando, movido_dx + planeta_ancho*factor , movido_dy);
        }
        
        
        torreta_incrementar_cooldown(actual, -DT);

        lista_iter_avanzar(iterador);
    }
    lista_iter_destruir(iterador);
    
    if(lista_largo(l_torretas) == 0){
        *puntaje += nivel_get_bonus(nivel);
        nivel_setear_bonus(nivel,0);
    }
    
    return true;
}

//Manejo de los tanques y su impresion. Ademas si la nave con escudo agarra el tanque, se elimina.

bool auxiliar_recorrer_lista_tanques( lista_t *l_tanques, SDL_Renderer *renderer, nave_t *nave, figura_t *tanque_fig, bool escudo, float factor, bool infinito, double planeta_ancho, double movido_dx, double movido_dy, double *combustible){
    lista_iter_t *iterador = lista_iter_crear(l_tanques);
    if(iterador == NULL)    return false;

    for(size_t i = 0; i < lista_largo(l_tanques); i++){
        tanque_t *actual = lista_iter_ver_actual(iterador);

        tanque_setear_figura(actual, tanque_fig);
        if(figura_interseccion(tanque_get_figura(actual), 40, nave_get_coord_x(nave), nave_get_coord_y(nave), factor) && escudo){
            tanque_destruir(actual);
            lista_iter_borrar(iterador);
            *combustible += 3000;
            continue;
        }

        if(infinito){
            tanque_imprimir(actual,renderer,factor, movido_dx - planeta_ancho * factor, movido_dy);
            tanque_imprimir(actual,renderer,factor, movido_dx + planeta_ancho * factor, movido_dy);
        }
        
        tanque_imprimir(actual,renderer,factor, movido_dx, movido_dy);

        lista_iter_avanzar(iterador);
    }
    lista_iter_destruir(iterador);
    
    return true;
}

//Manejo de los reactores y su impresion. Ademas si la nave le dispara al reactor, se elimina.

bool auxiliar_recorrer_lista_tanques_reactor(lista_t *l_tanques, lista_t *l_disparos, SDL_Renderer *renderer, nave_t *nave, nivel_t *nivel, figura_t *reactor_fig, float factor, double planeta_ancho, double movido_dx, double movido_dy, double *puntaje){
    lista_iter_t *iterador = lista_iter_crear(l_tanques);
    if(iterador == NULL)    return false;

    for(size_t i = 0; i < lista_largo(l_tanques); i++){
        tanque_t *actual = lista_iter_ver_actual(iterador);

        tanque_setear_figura(actual, reactor_fig);
        if(auxiliar_disparo_interseccion(l_disparos, tanque_get_figura(actual), 3, false, factor)){
            tanque_destruir(actual);
            lista_iter_borrar(iterador);
            *puntaje += nivel_get_bonus(nivel);
            nivel_setear_bonus(nivel, 0);
            continue;
        }

        tanque_imprimir(actual,renderer,factor, movido_dx, movido_dy);
            
        lista_iter_avanzar(iterador);
    }
    lista_iter_destruir(iterador);
    
    return true;
}

//Setea todas las figuras de cada nivel y las traslada a su posicion y las

void auxiliar_setear_listas_torretas_y_tanques_nivel(nivel_t *nivel, uint8_t n){
    if(n == 1){ 
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(1064, 13, 0));
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(1685, 113, 0));
        
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(916, 75, -0.66));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(1425, 159, 0.66));
    }
    else if(n == 2){ 
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(482, 94, 0));
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(1751, 247, 0));

        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(423, 195, 2 * PI - 0.66));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(806, 215,  2 * PI-0.33));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(1254, 153, 0.66));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(1587, 223, 2.23));
    }
    else if(n == 3){ 
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(820, 46, 0));
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear (1196, 68, 0));
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(1602, 46, 0));
        
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(70, 46, 0));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(506, 12, 0));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(952, 12, 0));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(1385, 12, 0));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(757, 210, 3.14));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(1161, 210, 3.14));
    }
    else if(n == 4){ 
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(188, 429, 0));
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(667, 600, 0));
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(1054, 404, 3.14));
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(574, 344, 3.14));
        
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(257 , 440, 0.66));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(719, 674, 2.23));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(985, 565, 0));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(1125, 417, 3.8));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(862, 163, 3.8));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(626, 323, 2.23));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(505, 331, 3.8));
        lista_insertar_primero(nivel_get_torretas(nivel), torreta_crear(378, 296, 2.23));
    }
    else if(n == 5){
        lista_insertar_primero(nivel_get_tanques(nivel), tanque_crear(815, 309, 0));
    }
}


figura_t *auxiliar_vect_figuras_buscar_figura_por_nombre(figura_t **vect, const char *nombre, size_t n){
    figura_t *f;
    for(size_t i = 0; i < n - 1; i++){
        if(strcmp(figura_get_nombre(vect[i]), nombre) == 0)
            return f = vect[i];
    }
    return NULL; //Devuelve NULL si no lo encuentra
}


void auxiliar_vector_figuras_destruir(figura_t **vect, size_t n){
    for(size_t i = 0; i < n -1; i++){
        figura_destruir(vect[i]);//Libero la memoria de cada figura del vector
    }
    free(vect); //libero el vector de figuras
}


void auxiliar_vector_de_planetas_destruir(planeta_t **planetas, size_t n){
        for(size_t i = 0; i < n; i++){
            planeta_destruir(planetas[i]);
        }
    }

void auxiliar_vector_de_niveles_destruir(nivel_t **niveles, size_t n){
    for(size_t i = 1; i < n; i++){
        nivel_destruir(niveles[i]);
    }
}
