#include "nivel.h"
#include "torreta.h"
#include "tanque.h"

struct nivel{
	figura_t *figura_nivel;

    lista_t *torretas;
    lista_t *tanques;
    
    bool infinito;
    double bonus;
};

nivel_t *nivel_crear(figura_t *fig){
    nivel_t *n = malloc(sizeof(nivel_t));
    if(n == NULL)   return NULL;

    n->figura_nivel = figura_clonar(fig);

    n->torretas = NULL;
    n->tanques = NULL;

    n->infinito = false;

    return n;
}

void nivel_destruir(nivel_t *n){
    figura_destruir(n->figura_nivel);
    lista_destruir(n->torretas, (void (*)(void *)) torreta_destruir);
    lista_destruir(n->tanques, (void (*)(void *)) tanque_destruir);

    free(n);
}

void nivel_set_listas(nivel_t *nivel, lista_t *lista_torretas, lista_t *lista_tanques){
    nivel->torretas = lista_torretas;
    nivel->tanques = lista_tanques;
}

figura_t *nivel_get_figura(nivel_t *n){
    return n->figura_nivel;
}

lista_t *nivel_get_torretas(nivel_t *nivel){
    return nivel->torretas;
}

lista_t *nivel_get_tanques(nivel_t *nivel){
    return nivel->tanques;
}

bool nivel_get_infinito(nivel_t *nivel){
    return nivel->infinito;
}

double nivel_get_bonus(nivel_t *nivel){
    return nivel->bonus;
}

void nivel_setear_infinito(nivel_t *n, bool inf){
    n->infinito = inf;
}

void nivel_setear_bonus(nivel_t *nivel, double bonus){
    nivel->bonus = bonus;
}


double nivel_min_x(nivel_t *nivel){
    return figura_min_x(nivel->figura_nivel);
}

double nivel_min_y(nivel_t *nivel){
    return figura_min_y(nivel->figura_nivel);
}

double nivel_max_x(nivel_t *nivel){
    return figura_max_x(nivel->figura_nivel);
}

double nivel_max_y(nivel_t *nivel){
    return figura_max_y(nivel->figura_nivel);
}

double nivel_ancho(nivel_t *nivel){
    return nivel_max_x(nivel)- nivel_min_x(nivel);
}

double nivel_alto(nivel_t *nivel){
    return nivel_max_y(nivel)- nivel_min_y(nivel);
}

void nivel_imprimir(nivel_t *nivel,SDL_Renderer *renderer, float factor, double dx, double dy){
    
    figura_imprimir_movida(nivel->figura_nivel, renderer, factor, dx, dy);
    
    if(nivel->infinito){
        float corrimiento_der = figura_ancho(nivel->figura_nivel) * factor;
        float corrimiento_izq = - corrimiento_der;

        figura_imprimir_movida(nivel->figura_nivel, renderer, factor, dx + corrimiento_der, dy);
        figura_imprimir_movida(nivel->figura_nivel, renderer, factor, dx + corrimiento_izq, dy);
    }
}