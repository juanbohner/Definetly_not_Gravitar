#include <stdlib.h>


#include "lectura.h"


bool leer_encabezado(FILE *f, char nombre[], figura_tipo_t *tipo, bool *infinito, size_t *cantidad_polilineas){
    
    if(fread(nombre, sizeof(char), 20, f) != 20)
        return false;

    static uint8_t t; // xixx tttx.
    if(fread(&t, sizeof(uint8_t), 1, f) != 1)
        return false;
    
    
    //Obtengo el tipo y el bit de infinito.
    *tipo = (t >> 1) & MASK_TIPO;   
    *infinito = t & MASK_INFINITO;  


    static uint16_t aux;   //Uso esta variable auxiliar para leer para asegurarme que el tamaño de lo que leo es lo correcto. Luego la igualo a lo que me importa.
    if(fread(&aux, sizeof(uint16_t), 1, f) != 1) 
        return false;
    
    *cantidad_polilineas = aux;     //Se va a hacer un casteo automaticamente a size_t

    return true;
}


polilinea_t *leer_polilinea(FILE *f){
    static uint16_t encabezado_poli;

    if(fread(&encabezado_poli, sizeof(uint16_t), 1, f) != 1) return NULL;
    
    static color_t color;
    color = (encabezado_poli & MASK_COLOR_POLI) >> 13; //Es de la forma 0000 0000 0000 0xxx

    static size_t n_puntos;
    n_puntos = encabezado_poli & MASK_NUM_PUNTOS;    //Hay casteo.


    /*
        ***Creacion de la polilinea***
    */
    static polilinea_t *poli;
    poli = polilinea_crear_vacia(n_puntos); 
    if( poli == NULL) return NULL; //pedida de memoria, si falla devuelve NULL

    
    //Seteo el color
    polilinea_setear_color(poli, color);

    /* puntero a arreglos de 2 flotantes. pido memoria asigno los puntos de poli con lo guardado en el puntero. */
    static float (*p)[2];
    p = malloc(sizeof(float[2]) * n_puntos);
    if(p == NULL)   return NULL;

    if(fread(p, sizeof(float[2]), n_puntos, f) != n_puntos){
        polilinea_destruir(poli);
        free(p);
        return NULL;
    } 

    for(size_t i = 0; i < n_puntos; i++){
        if(!polilinea_setear_punto(poli, i, p[i][X], p[i][Y])){
            polilinea_destruir(poli);
            free(p);
            return NULL;
        }
    }
    free(p);
    return poli;
}