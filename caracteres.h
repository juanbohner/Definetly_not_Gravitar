#ifndef _LETRAS_H_
#define _LETRAS_H_

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct caracter caracter_t;

#define CARACTER_ANCHO 5
#define CARACTER_ALTO 6

//Devuelve el caracter_t que coincide con la letra que recibe
caracter_t caracter_desde_letra(char letra);

size_t caracter_cant_puntos(caracter_t car);

//Dibuja un solo caracter
void caracter_dibujar(char letra, double factor, bool r, bool g, bool b, float px, float py, SDL_Renderer *renderer);

//Dibuja una cadena de caracteres
void caracter_multiple_cadena_imprimir(char *texto,double factor, bool r, bool g, bool b, float px, float py, SDL_Renderer *renderer);

//void caracter_multiple_flotante_imprimir(float num,double factor, bool r, bool g, bool b, float px, float py, SDL_Renderer *renderer);

extern const float caracter_a[7][2];
extern const float caracter_b[12][2];
extern const float caracter_c[4][2];
extern const float caracter_d[7][2];
extern const float caracter_e[7][2];
extern const float caracter_f[6][2];
extern const float caracter_g[7][2];
extern const float caracter_h[6][2];
extern const float caracter_i[6][2];
extern const float caracter_j[4][2];
extern const float caracter_k[6][2];
extern const float caracter_l[3][2];
extern const float caracter_m[5][2];
extern const float caracter_n[4][2];
extern const float caracter_o[5][2];
extern const float caracter_p[5][2];
extern const float caracter_q[9][2];
extern const float caracter_r[7][2];
extern const float caracter_s[6][2];
extern const float caracter_T[4][2];
extern const float caracter_u[4][2];
extern const float caracter_v[3][2];
extern const float caracter_w[5][2];
extern const float caracter_x[5][2];
extern const float caracter_y[5][2];
extern const float caracter_z[4][2];

extern const float caracter_0[5][2];
extern const float caracter_1[2][2];
extern const float caracter_2[6][2];
extern const float caracter_3[7][2];
extern const float caracter_4[5][2];
extern const float caracter_5[6][2];
extern const float caracter_6[5][2];
extern const float caracter_7[3][2];
extern const float caracter_8[7][2];
extern const float caracter_9[5][2];

extern const float caracter_derecha[5][2];
extern const float caracter_izquierda[5][2];
extern const float caracter_arriba[5][2];
extern const float caracter_abajo[5][2];

extern const float caracter_espacio[1][2];

#endif // _LETRAS_H_
