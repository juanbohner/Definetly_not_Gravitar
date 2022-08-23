#include <stdbool.h>
#include <stdint.h>

#include "color.h"

#define MASK_1      0x01 //0000 0001
#define R_SHIFT     2
#define G_SHIFT     1
#define B_SHIFT     0   //Se lee mejor con esto por mas que sea inecesario 

//Considero que color_t es de la forma xxxx xrgb.
color_t color_crear(bool r, bool g, bool b){
    return ((r << R_SHIFT) | (g << G_SHIFT) | (b << B_SHIFT));
}

color_t color_crear_recibe_valor(uint8_t r, uint8_t g, uint8_t b){
    bool rojo, verde, azul;
    
    (r != 0) ? (rojo = true) : (rojo = false);
    (g != 0) ? (verde = true) : (verde = false);
    (b != 0) ? (azul = true)  : (azul = false);

    return color_crear(rojo,verde,azul);
}


void color_a_rgb(color_t c, uint8_t *r, uint8_t *g, uint8_t *b){

    ((c & (MASK_1 << R_SHIFT)) != 0 ) ? (*r = 255) : (*r = 0); //Veo si el r esta encendido
    
    ((c & (MASK_1 << G_SHIFT)) != 0 ) ? (*g = 255) : (*g = 0); //Veo si el g esta encendido
    
    ((c & (MASK_1 << B_SHIFT)) != 0 ) ? (*b = 255) : (*b = 0); //Veo si el b esta encendido 

}