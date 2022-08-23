#ifndef LECTURA_H
#define LECTURA_H

#include <stdio.h>
#include <stdbool.h>

#include "polilinea.h"
#include "figura.h"


/*
    Lee del archivo binario el encabezado. Devulve false si no pudo leer.
*/
bool leer_encabezado(FILE *f, char nombre[], figura_tipo_t *tipo, bool *infinito, size_t *cantidad_polilineas);

/*
    Lee del archivo binario la informacion de la polilinea.
*/
polilinea_t *leer_polilinea(FILE *f);

#endif