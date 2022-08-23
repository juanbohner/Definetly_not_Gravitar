#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

//Arreglo de errores
typedef enum{
    EXIT_SUCCESS = 0,
    ERROR_MEMORIA = 1,
    ERROR_APERTURA = 2,
    ERROR_LECTURA = 3,
} errors_t; 

const char *errores[] = {
    [EXIT_SUCCESS] = "OK",
    [ERROR_APERTURA] = "Error de apertura", 
    [ERROR_MEMORIA] = "Error de memoria",
    [ERROR_LECTURA] = "Error de lectura",
};

#endif