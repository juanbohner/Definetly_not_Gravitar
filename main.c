#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define MAX_FIGURAS_USADAS  9

typedef enum{
    NAVE_SIN_CHORRO_FIG = 0,
    NAVE_CON_CHORRO_FIG,
    ESCUDO_FIG,
    ESCUDO2_FIG,
    DISPARO_FIG,
    TORRETA_FIG,
    TORRETA_CON_DISPARO_FIG,
    TANQUE_FIG,
    REACTOR_FIG,
}figuras_usadas_t;

typedef enum{
    PLANETA_BASE        = 0,
    PLANETA_1           = 1,
    PLANETA_2           = 2,
    PLANETA_3           = 3,
    PLANETA_4           = 4,
    PLANETA_5           = 5,
    PLANETA_ESTRELLA    = 6,
}planetas_usados_t;


#include "config.h"
/* #include "errors.h" */
#include "lista_enlazada.h"
#include "polilinea.h"
#include "figura.h"
#include "lectura.h"
#include "planeta.h"
#include "caracteres.h"
#include "nave.h"
#include "nivel.h" 
#include "torreta.h"
#include "tanque.h"
#include "disparo.h"
#include "auxiliar.h"


const double consumo_combustible_porpulsor_por_tick = JUEGO_COMBUSTIBLE_POT_X_SEG/JUEGO_FPS;
const double consumo_combustible_escudo_por_tick = JUEGO_COMBUSTIBLE_ESC_X_SEG/JUEGO_FPS;

int main(void){

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Gravitar");

    int dormir = 0;

    /********************************************************************/
    /********************************************************************/
    /********************************************************************/
    //BEGIN Poner en memoria al archivo binario
    
    FILE *fi = fopen("figuras.bin", "rb");
    if(fi == NULL){
        fprintf(stderr, "error al abrir el archivo binario de figuras\n");
        return 1;
    }

    //Guardo en memoria la figuras
    size_t cant_figuras = 1;    
    figura_t **vect_figuras = malloc(sizeof(figura_t*) * cant_figuras);
    if(vect_figuras == NULL){
        fprintf(stderr, "error Al crear el vector de figuras\n");
        return 1;
    }

    //Comienzo a guardar figuras en mi vector
    //Leo de a una figura hasta que no pueda leer el encabezado 
    while(true){
        char nombre[20];
        figura_tipo_t tipo;
        bool inf;
        size_t cantidad_polilinea;
        if(!leer_encabezado(fi, nombre, &tipo, &inf, &cantidad_polilinea))
            break; //Si no puedo leer salgo. EOF.

        //Creo figura con lo leido
        figura_t *fig = figura_crear((const char *) nombre, tipo, inf, cantidad_polilinea);
        if(fig == NULL){
            auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
            fclose(fi);
            return 1;
        }

        //Leo las polilinea de la figura, armo el vector de polilinea de la figura.
        if(!figura_armar_vector_de_polilineas(fi, fig)){
            auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
            //free(fig);
            fclose(fi);
            return 1;
        }

        //Ya tengo creada la figura
        vect_figuras[cant_figuras - 1] = fig;
        cant_figuras++;

        //Agrando el vector para guardar la proxima figura
        figura_t **aux = realloc(vect_figuras, sizeof(figura_t *) * cant_figuras);
        if(aux == NULL){
            auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
            fclose(fi);            
            return 1;
        }
        vect_figuras = aux;
    }
    //YA TENGO LAS FIGURAS EN UN VECTOR DE FIGURAS EN MEMORIA
    //Cierro archibo binario 
    fclose(fi);

    //END Poner en memoria al archivo binario
	/****************************************************/
	/****************************************************/
	
    /****************************************************/
	/****************************************************/
	//BEGIN Creacion de entidades


	//Creación de la nave y sus figuras    
    nave_t *nave = nave_crear();
    if(nave == NULL){
        auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
        //Ver si hay que liberar algo de SDL2
        /*
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        */
        return 1;
    }
    nave_reiniciar(nave, 388, 218);

    #define MAX_LEN_NOMBRE_FIGURA   20  //Es el caso de TORRETA+DISPARO

    const char *figuras_nombres[MAX_LEN_NOMBRE_FIGURA] = {
        [NAVE_SIN_CHORRO_FIG] = "NAVE",
        [NAVE_CON_CHORRO_FIG] = "NAVE+CHORRO",
        [ESCUDO_FIG] = "ESCUDO",
        [ESCUDO2_FIG] = "ESCUDO2",
        [DISPARO_FIG] = "DISPARO",
        [TORRETA_FIG] = "TORRETA",
        [TORRETA_CON_DISPARO_FIG] = "TORRETA+DISPARO",
        [TANQUE_FIG] = "COMBUSTIBLE",
        [REACTOR_FIG] = "REACTOR",
    };

    

    figura_t *figuras_a_usar[MAX_FIGURAS_USADAS];

    for(size_t i = 0; i < MAX_FIGURAS_USADAS; i++) {
        figuras_a_usar[i] = auxiliar_vect_figuras_buscar_figura_por_nombre(vect_figuras, figuras_nombres[i], cant_figuras);
        if(figuras_a_usar[i] == NULL) {
            auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
            //Ver si hay que liberar algo de SDL2
            /*
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            */
        // Acá, como lo programan una única vez y no copian y pegan 10 veces pueden abortar correctamente liberando las cosas que tienen que liberar.
            return 1;
        }
    }

    /************************************************************************************/
    /************************************************************************************/
    //Creacion de los planetas del nivel base
    #define MAX_PLANETAS   7

    /* const char *nombres_planetas[] ={
        [BASE] = "BASE",
        [ESTRELLA] = "ESTRELLA",
        [PLANETA_1] = "PLANETA_1",
        [PLANETA_2] = "PLANETA_1",
        [PLANETA_3] = "PLANETA_1",
        [PLANETA_4] = "PLANETA_1",
        [PLANETA_5] = "PLANETA_1",
    }; */
    
    struct planetas_usados {
        double coord_x, coord_y;
        //bool es_infinito;
        //int puntaje;
        char figura[20];
        int recompensa;
        double coord_desvio_caracteres_x, coord_desvio_caracteres_y;
    };

    struct planetas_usados tabla_planetas[] = {
        [PLANETA_BASE]          = {388, 218,/*  true, 2000, */ "BASE"    , 0, 0, 0},
        [PLANETA_ESTRELLA]      = {457, 364,/*  true, 2000, */ "ESTRELLA", 0, 0, 0},
        [PLANETA_1]     = {663, 437,/*  true, 2000, */ "PLANETA1", 2000, 0, 40},
        [PLANETA_2]     = {671, 145,/*  true, 2000, */ "PLANETA2", 4000, 0,-40},
        [PLANETA_3]     = {110, 79, /*  true, 2000, */ "PLANETA3", 6000, 0,-40},
        [PLANETA_4]     = {204, 455,/*  true, 2000, */ "PLANETA4", 8000, -40, 30},
        [PLANETA_5]     = {111, 307,/*  true, 2000, */ "PLANETA5", 9000, -30, 50},
    };


    /************************************************************/
    /********   VER DE MOVER ESTO A AUXILIAR.C    ***************/
    /************************************************************/
    

    
    /************************************************************/
    /************************************************************/
    /************************************************************/


    planeta_t *planetas[MAX_PLANETAS];

    for(size_t i = PLANETA_BASE; i < MAX_PLANETAS; i++){
        planetas[i] = planeta_crear(auxiliar_vect_figuras_buscar_figura_por_nombre(vect_figuras, (tabla_planetas[i]).figura, cant_figuras));
        if(planetas[i] == NULL){
            //Destruyo los planetas que acabo de crear
            auxiliar_vector_de_planetas_destruir(planetas, i);
            nave_destruir(nave);
            auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);

            //Ver si hay que liberar algo de SDL2
            /*
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            */
            return 1;
        }
        planeta_trasladar(planetas[i], (tabla_planetas[i]).coord_x, (tabla_planetas[i]).coord_y);
    }
    
    /************************************************************************************/
    /***********************************************************************************/
    /***********************************************************************************/
    //Creacion de niveles

    //Listas para los niveles. Hago esto para poder hacer mas facilmente la parte donde cambio de niveles
    typedef enum{
        NIVEL_BASE= 0,
        NIVEL_1   = 1,
        NIVEL_2   = 2,
        NIVEL_3   = 3,
        NIVEL_4   = 4,
        NIVEL_5   = 5,
    }niveles_usados_t;
    
    struct niveles_usados {
        double coord_summoning_x, coord_summoning_y;
        bool es_infinito;
        char figura_nombre[20];
        figura_t *fig;
    };

    struct niveles_usados tabla_niveles[] = {
        [NIVEL_BASE]= {388,             218,                false, ""},
        [NIVEL_1]   = {VENTANA_ANCHO/2, 400,                true,  "NIVEL1NE"},
        [NIVEL_2]   = {VENTANA_ANCHO/2, 400,                true,  "NIVEL1SE"},
        [NIVEL_3]   = {VENTANA_ANCHO/2, 400,                true,  "NIVEL1SW"},
        [NIVEL_4]   = {VENTANA_ANCHO/2, VENTANA_ALTO - 100, false, "NIVEL1NW"},
        [NIVEL_5]   = {VENTANA_ANCHO/2, VENTANA_ALTO - 100, false, "NIVEL1R"},
    };

    #define MAX_NIVELES 6

    nivel_t *niveles[MAX_NIVELES];

    for(niveles_usados_t i = NIVEL_1; i < MAX_NIVELES; i++){
        (tabla_niveles[i]).fig = auxiliar_vect_figuras_buscar_figura_por_nombre(vect_figuras, (tabla_niveles[i]).figura_nombre, cant_figuras);
        niveles[i] = nivel_crear((tabla_niveles[i]).fig);
        if(niveles[i] == NULL){
            auxiliar_vector_de_niveles_destruir(niveles, i);

            nave_destruir(nave);
            auxiliar_vector_de_planetas_destruir(planetas, MAX_PLANETAS);
            auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
            //Cosas de SDL2
            return 1;
        }
        nivel_setear_infinito(niveles[i], (tabla_niveles[i]).es_infinito);

        lista_t *lista_tanques = lista_crear();
        if(lista_tanques == NULL){
            auxiliar_vector_de_niveles_destruir(niveles, i);

            nave_destruir(nave);
            auxiliar_vector_de_planetas_destruir(planetas, MAX_PLANETAS);
            auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
            //Cosas de SDL2
            return 1;
        }
        lista_t *lista_torretas = lista_crear();
        if(lista_torretas == NULL){
            auxiliar_vector_de_niveles_destruir(niveles, i);

            nave_destruir(nave);
            auxiliar_vector_de_planetas_destruir(planetas, MAX_PLANETAS);
            auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
            //Cosas de SDL2
            return 1;
        }
        nivel_set_listas(niveles[i], lista_tanques, lista_torretas);
        auxiliar_setear_listas_torretas_y_tanques_nivel(niveles[i], i);

        nivel_setear_bonus(niveles[i], (tabla_planetas[i]).recompensa);
    }


    //END   Creacion de entidades
	/****************************************************/
	/****************************************************/
	/****************************************************/
	/****************************************************/
    
    //Lista de dispaparos
    lista_t *lista_disparos = lista_crear();
    if(lista_disparos == NULL){
        fprintf(stderr, "Error al crear la lista de disparos\n");
        return 1;
    }
    
    /*******************************************************************/
    /*******************************************************************/
    //Variables que se usaran en el ciclo para el manejo del juego

    niveles_usados_t nivel = NIVEL_BASE;
    float factor = 1;
    float centro = 0;
    
    int vidas = 3;

    double combustible = CANTIDAD_COMBUSTIBLE_INICIAL;

    double puntaje = 0;
    int puntaje_checkpoint = PUNTAJE_CHECKPOINT;

    //int cantidad_niveles_ganados = 0;

    //Para el nivel 5
    double tiempo = TIEMPO_REACTOR;

    //Para GAME OVER
    double tiempo_game_over = 0;
    
    /********************************/
    /*Estados de las cosas*/
    
    //Chorro
    bool chorro_prendido = false;
    bool escudo_prendido = false;

    //Disparos nave
    bool disparo_activado = false;
    bool disparo_habilitado = true;
    double disparo_cooldown = COOLDOWN_NAVE;


    bool rotar_horario = false;
    bool rotar_antihorario = false;

    bool mover_adelante = false;

    bool nivel_infinito = false;
    
    bool juego_terminar = false;

    /*********************************************************/
    /*********************************************************/
    /*********************************************************/
    //Ciclo donde ocurre el juego

    unsigned int ticks = SDL_GetTicks();
    while(1) {
        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;
            // BEGIN código del alumno
            if (event.type == SDL_KEYDOWN) {
                // Se apretó una tecla
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        if(combustible > 0){
                            
                            combustible -= consumo_combustible_porpulsor_por_tick;
                            // Prendemos el chorro:
                            mover_adelante = true;
                            chorro_prendido = true;
                        }
                        break;
                    //DEBUGGING Frena la nave
                    //case SDLK_DOWN:
                        //nave_set_vel(nave, 0, 0);  
                        //break;
                    case SDLK_RIGHT:
                        rotar_horario = true;
                        break;
                    case SDLK_LEFT:
                        rotar_antihorario = true;
                        break;
                    case SDLK_SPACE:
                        combustible -= consumo_combustible_escudo_por_tick;
                        escudo_prendido = true;
                        break;
                    /* case SDLK_1:                        
                        nivel_setear_bonus(niveles[NIVEL_1], 0);
                        break;
                    case SDLK_2:                        
                        nivel_setear_bonus(niveles[NIVEL_2], 0);
                        break;
                    case SDLK_3:                        
                        nivel_setear_bonus(niveles[NIVEL_3], 0);
                        break;
                    case SDLK_4:                        
                        nivel_setear_bonus(niveles[NIVEL_4], 0);
                        break;
                    case SDLK_5:                        
                        nivel_setear_bonus(niveles[NIVEL_5], 10);
                        break; */
                    case SDLK_LALT:
                        disparo_activado = true;
                        break;
                    //DEBUGGING Agrega 1000 puntos
                    // case SDLK_1:
                    //     puntaje += 1000;
                    //     break;
                    case SDLK_ESCAPE:
                        juego_terminar = true;
                        break;
                    
                }
            }
            else if (event.type == SDL_KEYUP) {
                // Se soltó una tecla
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        // Apagamos el chorro:
                        mover_adelante = false;
                        chorro_prendido = false;
                        break;
                    //DEBBUGING
                    // case SDLK_DOWN:
                    //     break;
                    case SDLK_RIGHT:
                        rotar_horario = false;
                        break;
                    case SDLK_LEFT:
                        rotar_antihorario = false;
                        break;
                    case SDLK_SPACE:
                        escudo_prendido = false;
                        break;
                    case SDLK_LALT:
                        disparo_activado = false;
                        break;

                    break;
                }
            }
            continue;
        }

        if(juego_terminar == true){
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);



        double planeta_ancho = 0;
        double planeta_alto = 0;
        
        double posicion_nave_x = 0;
        double posicion_nave_y = 0;

        double planeta_x_min = 0;
        double planeta_x_max = 0;


        //Actualizacion de las figuras de la nave
        nave_setear_figuras(nave, figuras_a_usar[NAVE_SIN_CHORRO_FIG], figuras_a_usar[NAVE_CON_CHORRO_FIG], figuras_a_usar[ESCUDO_FIG], figuras_a_usar[ESCUDO2_FIG]);

        
        /********** MENSAJES FIJOS **************/
        
        char d[10];
        char s[10];
        char p[10];
        char c[10];
        char v[10];
    
        nave_vidas_mostrar(figuras_a_usar[NAVE_SIN_CHORRO_FIG], renderer, vidas);

        sprintf(p,"%.0f", puntaje);
        caracter_multiple_cadena_imprimir(p, 3,true,true,false, (VENTANA_ANCHO/2) - 200,VENTANA_ALTO -40, renderer);
        caracter_multiple_cadena_imprimir("SCORE", 3,true,true,true, (VENTANA_ANCHO/2) - 45,VENTANA_ALTO -40, renderer);
        
        sprintf(c,"%.0f", combustible>0? combustible:0);
        caracter_multiple_cadena_imprimir(c, 3,true,true,false, (VENTANA_ANCHO/2) - 200,VENTANA_ALTO -70, renderer);
        caracter_multiple_cadena_imprimir("FUEL", 3,true,true,true, (VENTANA_ANCHO/2) - 35,VENTANA_ALTO -70, renderer);
        
        sprintf(v,"%d", puntaje_checkpoint);
        caracter_multiple_cadena_imprimir(v, 3,true,true,false, (VENTANA_ANCHO/2) + 30,VENTANA_ALTO -100, renderer);
        caracter_multiple_cadena_imprimir("NEXT LIFE", 3,true,true,true, (VENTANA_ANCHO/2) - 120,VENTANA_ALTO -100, renderer);

        /*****************************************************/
        /*****************************************************/
        /*****************************************************/
        /*****************************************************/
        //BEGIN Logica de niveles
       
        //nivel == 0 es un caso particular        
        if(nivel == NIVEL_BASE){
            factor = 1;
            
            //Impresion de los planetas en el nivel 0
            for(size_t i = 0; i < MAX_PLANETAS; i++){
                planeta_dibujar(renderer, planetas[i], factor);
                auxiliar_disparo_interseccion(lista_disparos, planeta_get_figura(planetas[i]),1, false, factor);
            }

            //Impresion de los mensajes de los niveles
            for(size_t i = NIVEL_1; i < MAX_PLANETAS; i++){
                if(i == PLANETA_ESTRELLA) continue;
                sprintf(d,"%d", (tabla_planetas[i]).recompensa);
                caracter_multiple_cadena_imprimir(d, 2,true,true,true, planeta_get_coord_x(planetas[i]) + (tabla_planetas[i]).coord_desvio_caracteres_x,planeta_get_coord_y(planetas[i]) + (tabla_planetas[i]).coord_desvio_caracteres_y, renderer);
            }    

            //Verifiacion nave dentro de limites validos
            if(nave_get_coord_x(nave) < 0 || nave_get_coord_x(nave) > VENTANA_ANCHO){
                nave_invertir_vel_x(nave);
            }
            if(nave_get_coord_y(nave) < 0 || nave_get_coord_y(nave) > VENTANA_ALTO){
                nave_invertir_vel_y(nave);
            }

            //Verifico si la nave toca los planetas. i empieza en 1 porque no me importa la interseccion con la base
            for(size_t i = PLANETA_1; i < MAX_PLANETAS; i++){
                if(planeta_interseccion(planetas[i], 5,nave_get_coord_x(nave), nave_get_coord_y(nave), factor)){
                    if(i == PLANETA_ESTRELLA){ //Es la estrella
                        nave_reiniciar(nave, planeta_get_coord_x(planetas[PLANETA_BASE]), planeta_get_coord_y(planetas[PLANETA_BASE]));
                        vidas--;

                        lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);
                        lista_disparos = lista_crear();
                        if(lista_disparos == NULL){
                            //Las demas cosas que hay que destruir 
                            auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
                            //Lo de SDL2
                            return 1;
                        }
                        break;
                    }

                    //En caso de que mi nave toque algun otro planeta
                    nivel = i;
                    nave_reiniciar(nave, (tabla_niveles[i]).coord_summoning_x, (tabla_niveles[i]).coord_summoning_y);
                    if(i == 2 || i == 3 || i == 4){
                        centro = VENTANA_ANCHO/2;
                        //nivel_infinito = true;
                    }

                    lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);
                    lista_disparos = lista_crear();
                    if(lista_disparos == NULL){
                        fprintf(stderr, "Error de memoria\n");
                        return 1;
                    }
                }
            }

            nave_acercar_a_posicion(nave, G, planeta_get_coord_x(planetas[PLANETA_ESTRELLA]), planeta_get_coord_y(planetas[PLANETA_ESTRELLA]), DT);
             
            nave_imprimir(nave, renderer, factor);
        }

        /*****************************************************/
        /*****************************************************/
        
        if(nivel == NIVEL_1 || nivel == NIVEL_2 || nivel == NIVEL_3 || nivel == NIVEL_4){
            //Hago cosas duferente en el caso de que sea nivel inifnito o no
            if(nivel_get_infinito(niveles[nivel])){
                planeta_ancho = nivel_ancho(niveles[nivel]);
                posicion_nave_x = nave_get_coord_x(nave);
                posicion_nave_y = nave_get_coord_y(nave);

                if(posicion_nave_y > VENTANA_ALTO * MARGEN_ALTURA)
                factor = VENTANA_ALTO * MARGEN_ALTURA / posicion_nave_y;
                if(factor < ESCALA_MINIMA)
                    factor = ESCALA_MINIMA;

                if((posicion_nave_x - centro) * factor > VENTANA_ANCHO / 2 * MARGEN_ANCHO)
                    centro = posicion_nave_x - VENTANA_ANCHO / 2 * MARGEN_ANCHO / factor;
                else if((centro - posicion_nave_x) * factor > VENTANA_ANCHO / 2 * MARGEN_ANCHO)
                    centro = posicion_nave_x + VENTANA_ANCHO / 2 * MARGEN_ANCHO / factor;

                if(posicion_nave_x < 0){
                    nave_set_coord(nave,posicion_nave_x + planeta_ancho, posicion_nave_y);
                    centro+=planeta_ancho;
                }
                if(posicion_nave_x > planeta_ancho){
                    nave_set_coord(nave,posicion_nave_x - planeta_ancho, posicion_nave_y);                
                    centro-=planeta_ancho;
                }

            } 
            else {
                factor = 1;
               //nivel_infinito = true;
                planeta_alto = nivel_alto(niveles[nivel]);

                posicion_nave_x = nave_get_coord_x(nave);
                posicion_nave_y = nave_get_coord_y(nave);

                planeta_x_max = nivel_max_x(niveles[nivel]);
                
                planeta_x_min = nivel_min_x(niveles[nivel]);

                factor = VENTANA_ALTO * 1.0 / planeta_alto;

                if(VENTANA_ANCHO * 1.0 / (planeta_x_max + planeta_x_min) < factor)
                    factor = VENTANA_ANCHO * 1.0 / (planeta_x_max + planeta_x_min);
                centro = (planeta_x_max + planeta_x_min) / 2;
            }

            //Se recorre lista de torretas para ver si se elimina alguna
            if(!auxiliar_recorrer_lista_torretas(nivel_get_torretas(niveles[nivel]),lista_disparos, renderer, nave, niveles[nivel], figuras_a_usar[TORRETA_FIG], figuras_a_usar[TORRETA_CON_DISPARO_FIG], factor, nivel_get_infinito(niveles[nivel]), planeta_ancho, (-centro + VENTANA_ANCHO / 2 / factor)*factor, 0,&puntaje)){
                fprintf(stderr, "Error al recorrer lista de torretas\n");
                return 1;
            }

            //Se recorre lista de tanques para ver si se elimina alguno
            if(!auxiliar_recorrer_lista_tanques(nivel_get_tanques(niveles[nivel]), renderer, nave, figuras_a_usar[TANQUE_FIG], escudo_prendido, factor, nivel_get_infinito(niveles[nivel]), planeta_ancho, (-centro + VENTANA_ANCHO / 2 / factor)*factor, 0, &combustible)){
                fprintf(stderr, "Error al recorrer lista de tanques\n");
                return 1;
            }

            //Si el disparo choca contra el terreno
            auxiliar_disparo_interseccion(lista_disparos, (tabla_niveles[nivel]).fig, 5, false, factor);

            //Nave es golpeada por un disparo de una torreta
            if(auxiliar_disparo_interseccion(lista_disparos,nave_get_figura_sin_chorro(nave), 3, true, 1) == true){
                if(escudo_prendido == false){
                    nivel = NIVEL_BASE;
                    nave_reiniciar(nave, planeta_get_coord_x(planetas[PLANETA_BASE]), planeta_get_coord_y(planetas[PLANETA_BASE]));
                    nivel_infinito = false;
                    vidas--;

                    lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);
                    lista_disparos = lista_crear();
                    if(lista_disparos == NULL){
                    fprintf(stderr, "Error de memoria\n");
                        return 1;
                    }
                    continue;
                }else{
                    combustible -= 200;
                }
            }

            //Nave choca contra el suelo del nivel
            if(figura_interseccion((tabla_niveles[nivel]).fig, 5, nave_get_coord_x(nave), nave_get_coord_y(nave), factor) == true){
                nivel = 0;
                nave_reiniciar(nave, planeta_get_coord_x(planetas[PLANETA_BASE]), planeta_get_coord_y(planetas[PLANETA_BASE]));
                nivel_infinito = false;
                vidas--;

                lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);
                lista_disparos = lista_crear();
                if(lista_disparos == NULL){
                    fprintf(stderr, "Error de memoria\n");
                    //Destruir correctamentre lo que hay que destruir
                    return 1;
                }
                continue;
            }
            
            //Salgo del nivel
            if(nivel_get_infinito(niveles[nivel])){
                if(posicion_nave_y >= VENTANA_ALTO){
                    nivel = 0;
                    nave_reiniciar(nave, planeta_get_coord_x(planetas[PLANETA_BASE]), planeta_get_coord_y(planetas[PLANETA_BASE]));
                    nivel_infinito = false;
                    

                    lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);
                    lista_disparos = lista_crear();
                    if(lista_disparos == NULL){
                        //Destruir correctamentre lo que hay que destruir
                        fprintf(stderr, "Error de memoria\n");
                        return 1;
                    }
                    continue;
                }
            }
            else{
                if(posicion_nave_y >= VENTANA_ALTO || posicion_nave_y <= 0 || posicion_nave_x >= VENTANA_ANCHO || posicion_nave_x <= 0 ){
                    nivel = 0;
                    nave_reiniciar(nave, planeta_get_coord_x(planetas[PLANETA_BASE]), planeta_get_coord_y(planetas[PLANETA_BASE]));
                    
                    lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);
                    lista_disparos = lista_crear();
                    if(lista_disparos == NULL){
                        //Destruir correctamentre lo que hay que destruir
                        fprintf(stderr, "Error de memoria\n");
                        return 1;
                    }
                    continue;
                }
            }

            nave_gravedad_piso(nave, G, DT);
            
            //Impresion
            if(nivel_get_infinito(niveles[nivel])){
                nivel_imprimir(niveles[nivel], renderer, factor, (-centro + VENTANA_ANCHO / 2 / factor)*factor, 0);
                nave_imprimir_movida(nave, renderer, factor, (-centro + VENTANA_ANCHO / 2 / factor)*factor, 0);   
            }
            else{
                nivel_imprimir(niveles[nivel], renderer, factor, 0, 0);
                 nave_imprimir(nave, renderer, factor);
            }
        }

        /**************************************************/
        /**************************************************/
        //nivel == 5 es un caso particular
        if(nivel == NIVEL_5){

            //Manejo del tiempo disponible para destruir el reactor
            sprintf(s,"%.0f", tiempo);
            caracter_multiple_cadena_imprimir(s, 3,true,true,true, planeta_get_coord_x(planetas[PLANETA_5]) - 30 ,planeta_get_coord_y(planetas[PLANETA_5]) + 50, renderer); 
            
            tiempo -= DT;

            planeta_alto = nivel_alto(niveles[NIVEL_5]);

            posicion_nave_x = nave_get_coord_x(nave);
            posicion_nave_y = nave_get_coord_y(nave);

            planeta_x_max = nivel_max_x(niveles[NIVEL_5]);
            
            planeta_x_min = nivel_min_x(niveles[NIVEL_5]);

            factor = VENTANA_ALTO * 1.0 / planeta_alto;
            if(VENTANA_ANCHO * 1.0 / (planeta_x_max + planeta_x_min) < factor)
                factor = VENTANA_ANCHO * 1.0 / (planeta_x_max + planeta_x_min);
            centro = (planeta_x_max + planeta_x_min) / 2;
            
            //Verifico que un disparo no choque contra el reactor
            if(!auxiliar_recorrer_lista_tanques_reactor(nivel_get_tanques(niveles[NIVEL_5]),lista_disparos, renderer, nave, niveles[NIVEL_5], figuras_a_usar[REACTOR_FIG], factor, planeta_ancho, (-centro + VENTANA_ANCHO / 2 / factor)*factor, 0, &puntaje)){
                fprintf(stderr, "ERROR en recorrer lista de tanques nivel 5\n");
                return 1;
            }

            //Chocar contra el suelo
            if(figura_interseccion(tabla_niveles[NIVEL_5].fig, 5, nave_get_coord_x(nave), nave_get_coord_y(nave),factor) == true){
                factor = 1;
                nivel = 0;
                nave_reiniciar(nave, planeta_get_coord_x(planetas[PLANETA_BASE]), planeta_get_coord_y(planetas[PLANETA_BASE]));
                vidas--;
                tiempo = TIEMPO_REACTOR;

                lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);
                lista_disparos = lista_crear();
                if(lista_disparos == NULL){
                    fprintf(stderr, "Error de memoria\n");
                    return 1;
                }
            }

            //Salir del nivel
            if(posicion_nave_y >= VENTANA_ALTO ){
                factor = 1;
                nivel = 0;
                tiempo = TIEMPO_REACTOR;
                nave_reiniciar(nave, planeta_get_coord_x(planetas[PLANETA_BASE]), planeta_get_coord_y(planetas[PLANETA_BASE]));
                
                lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);
                lista_disparos = lista_crear();
                if(lista_disparos == NULL){
                    fprintf(stderr, "Error de memoria\n");
                    return 1;
                }
            
            }

            //Se acaba el tiempo
            if(tiempo <= 0){
                tiempo = TIEMPO_REACTOR;
                factor = 1;
                nivel = 0;
                vidas--;
                nave_reiniciar(nave, planeta_get_coord_x(planetas[PLANETA_BASE]), planeta_get_coord_y(planetas[PLANETA_BASE]));
            
                lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);
                lista_disparos = lista_crear();
                if(lista_disparos == NULL){
                    fprintf(stderr, "Error de memoria\n");
                    return 1;
                }
            }

            //Interseccion del disparo con el piso del nivel
            auxiliar_disparo_interseccion(lista_disparos, tabla_niveles[NIVEL_5].fig, 5, false, factor);
            
            nivel_imprimir(niveles[NIVEL_5], renderer, factor,0,0);

            nave_imprimir(nave, renderer, factor);
        
            nave_gravedad_piso(nave, G, DT);
        }

        //END. Logica de niveles
        /**********************************************************/
        /**********************************************************/


        /**********************************************************/
        /**********************************************************/
        //BEGIN. Actualizacion de variables

        //Check de incrementar vidas por puntaje
        if(puntaje >= puntaje_checkpoint){
            puntaje_checkpoint += PUNTAJE_CHECKPOINT;
            vidas++;
        }

        //Movimiento de la nave
        if(rotar_horario == true){
            nave_rotar(nave, - NAVE_ROTACION_PASO);
        }
        if(rotar_antihorario == true){
            nave_rotar(nave, + NAVE_ROTACION_PASO);
        }
        if(mover_adelante){
            nave_mover_adelante(nave, NAVE_ACELERACION, DT);
        } else{
            nave_mover_adelante(nave, 0, DT);
        }

        //DEBUGGING.    Impresion de la posicion de la nave.
        //printf("posicion_x: %lf \tposicion_y: %lf \t factor:%lf \n", nave_get_coord_x(nave), nave_get_coord_y(nave), factor);


        /**********************************************************/
        /**********************************************************/
        /**********************************************************/        

        //Seteo del escudo por cada nivel
        if(nivel != 0)
            nave_set_propulsor_escudo_escudonivel(nave, chorro_prendido, false, escudo_prendido);
        else
            nave_set_propulsor_escudo_escudonivel(nave, chorro_prendido, escudo_prendido, false);


        //Dibujo los disparos que tenga
        if(!auxiliar_disparo_dibujar(lista_disparos, renderer, figuras_a_usar[DISPARO_FIG], factor, nivel_infinito, (-centro + VENTANA_ANCHO / 2 / factor)*factor,0)){
            fprintf(stderr, "ERROR EN IMPRIMIR DISPARO\n");
            return 1;
        }


        if(disparo_activado && disparo_habilitado){
            double angulo, px, py, vx, vy;
            nave_get_all_data(nave, &px, &py, &vx, &vy, &angulo);
            if(!lista_insertar_ultimo(lista_disparos, disparo_crear(angulo,px,py,vx + VELOCIDAD_DISPARO * cos(angulo) ,vy + VELOCIDAD_DISPARO * sin(angulo), true))){
                fprintf(stderr, "ERRROR INSERTAR AL FINAL \n");
                return 1;
            }

            disparo_habilitado = false;
        }          
        
        disparo_cooldown += DT;

        if(disparo_cooldown >= COOLDOWN_NAVE){
            disparo_cooldown = 0;
            disparo_habilitado = true;
        }
        
        if(nivel_get_bonus(niveles[NIVEL_1]) == 0 && nivel_get_bonus(niveles[NIVEL_2]) == 0 && nivel_get_bonus(niveles[NIVEL_3]) == 0 && nivel_get_bonus(niveles[NIVEL_4]) == 0 && nivel_get_bonus(niveles[NIVEL_5]) == 0){
            break;
        }

        printf("P N_1 = %.1lf  ", nivel_get_bonus(niveles[NIVEL_1]));
        printf("P N_2 = %.1lf  ", nivel_get_bonus(niveles[NIVEL_2]));
        printf("P N_3 = %.1lf  ", nivel_get_bonus(niveles[NIVEL_3]));
        printf("P N_4 = %.1lf  ", nivel_get_bonus(niveles[NIVEL_4]));
        printf("P N_5 = %.1lf  ", nivel_get_bonus(niveles[NIVEL_5]));
        putchar('\n');

        // END código del alumno
        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        ticks = SDL_GetTicks();
    
        if(vidas <= 0){
            break;
        }
    }

    //Impresion del mensaje de GAME OVER
    while(true){
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
        
        //Impresion del putnaje final
        char p[10];
        sprintf(p,"%.0f", (puntaje + combustible + (vidas * 5000)));
        caracter_multiple_cadena_imprimir(p, 6,true,true,false, (VENTANA_ANCHO/2) - 50, VENTANA_ALTO/2 - 100, renderer);
        caracter_multiple_cadena_imprimir("FINAL SCORE", 6,false,true,true, (VENTANA_ANCHO/2)- 150, VENTANA_ALTO/2 -50, renderer);
        
        if(vidas != 0)
            caracter_multiple_cadena_imprimir("VICTORY ROYALE", 10,true,false, false, (VENTANA_ANCHO/2) -350 ,VENTANA_ALTO/2 + 40, renderer);
        else
            caracter_multiple_cadena_imprimir("GAME OVER", 10,true,false, false, (VENTANA_ANCHO/2) -210 ,VENTANA_ALTO/2 + 40, renderer);


        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        ticks = SDL_GetTicks();
        
        tiempo_game_over += DT;
        if(tiempo_game_over >= 3)
            break;
    }

    /*************************************************/        
    /*************************************************/        
    /*************************************************/
    //BEGIN DESTRUCCION DE LAS ENTIDADES
    auxiliar_vector_figuras_destruir(vect_figuras, cant_figuras);
    
    nave_destruir(nave);

    auxiliar_vector_de_planetas_destruir(planetas, MAX_PLANETAS);

    auxiliar_vector_de_niveles_destruir(niveles, MAX_NIVELES);    
    
    lista_destruir(lista_disparos, (void (*)(void *)) disparo_destruir);

    // No tengo nada que destruir.

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    
    return 0;
}