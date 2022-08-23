#include <stddef.h>


#define MAX_LEN_NOMBRE_FIGURA   20  //Es el caso de TORRETA+DISPARO

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

#define MAX_PLANETAS   7



typedef enum{
    NIVEL_BASE= 0,
    NIVEL_1   = 2,
    NIVEL_2   = 3,
    NIVEL_3   = 4,
    NIVEL_4   = 5,
    NIVEL_5   = 6,
}niveles_usados_t;

struct niveles_usados tabla_niveles[] = {
    [NIVEL_BASE]= {388,             218,                false, 0,    ""},
    [NIVEL_1]   = {VENTANA_ANCHO/2, 400,                true,  2000, "NIVEL1NE"},
    [NIVEL_2]   = {VENTANA_ANCHO/2, 400,                true,  4000, "NIVEL1SE"},
    [NIVEL_3]   = {VENTANA_ANCHO/2, 400,                true,  6000, "NIVEL1SW"},
    [NIVEL_4]   = {VENTANA_ANCHO/2, VENTANA_ALTO - 100, false, 8000, "NIVEL1NW"},
    [NIVEL_5]   = {VENTANA_ANCHO/2, VENTANA_ALTO - 100, false, 9000, "NIVEL1R"},
};


typedef enum{
    PLANETA_BASE        = 0,
    PLANETA_ESTRELLA    = 1,
    PLANETA_1   = 2,
    PLANETA_2   = 3,
    PLANETA_3   = 4,
    PLANETA_4   = 5,
    PLANETA_5   = 6,
}planetas_usados_t;