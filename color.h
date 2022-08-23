#ifndef COLOR_H
#define COLOR_H

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t color_t;

//recibe 3 bits y devuelve el color que representan con una color_t. De la forma xxxx xrgb
color_t color_crear(bool r, bool g, bool b);

/*
    Como color crear, pero recibe uint8_t en vez de booleanos
*/
color_t color_crear_recibe_valor(uint8_t r, uint8_t g, uint8_t b);

/*
    Recibe un color_t que es un uint8_t debe ser de la siguiente forma:
    0000 0xxx
    Luego va a modificar las variables r, g y b por interfaz.
    Les asigna el valor 255 o 0 en el caso de su respectivo bit está encendidos o no.
*/
void color_a_rgb(color_t c, uint8_t *r, uint8_t *g, uint8_t *b);



#endif