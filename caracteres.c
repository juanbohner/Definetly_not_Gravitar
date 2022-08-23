#include <stdio.h>
#include <string.h>


#include "caracteres.h"
#include "polilinea.h"
#include "color.h"
#include "config.h"



struct caracter{
  char letra;
  const float (*coord)[2];
  size_t n_puntos;
};

const caracter_t caracteres[] = {
	{'A', caracter_a, sizeof(caracter_a)/sizeof(caracter_a[0])},
	{'B', caracter_b, sizeof(caracter_b)/sizeof(caracter_a[0])},
	{'C', caracter_c, sizeof(caracter_c)/sizeof(caracter_a[0])},
	{'D', caracter_d, sizeof(caracter_d)/sizeof(caracter_a[0])},
	{'E', caracter_e, sizeof(caracter_e)/sizeof(caracter_a[0])},
	{'F', caracter_f, sizeof(caracter_f)/sizeof(caracter_a[0])},
	{'G', caracter_g, sizeof(caracter_g)/sizeof(caracter_a[0])},
	{'H', caracter_h, sizeof(caracter_h)/sizeof(caracter_a[0])},
	{'I', caracter_i, sizeof(caracter_i)/sizeof(caracter_a[0])},
	{'J', caracter_j, sizeof(caracter_j)/sizeof(caracter_a[0])},
	{'K', caracter_k, sizeof(caracter_k)/sizeof(caracter_a[0])},
	{'L', caracter_l, sizeof(caracter_l)/sizeof(caracter_a[0])},
	{'M', caracter_m, sizeof(caracter_m)/sizeof(caracter_a[0])},
	{'N', caracter_n, sizeof(caracter_n)/sizeof(caracter_a[0])},
	{'O', caracter_o, sizeof(caracter_o)/sizeof(caracter_a[0])},
	{'P', caracter_p, sizeof(caracter_p)/sizeof(caracter_a[0])},
	{'Q', caracter_q, sizeof(caracter_q)/sizeof(caracter_a[0])},
	{'R', caracter_r, sizeof(caracter_r)/sizeof(caracter_a[0])},
	{'S', caracter_s, sizeof(caracter_s)/sizeof(caracter_a[0])},
	{'T', caracter_T, sizeof(caracter_T)/sizeof(caracter_a[0])},
	{'U', caracter_u, sizeof(caracter_u)/sizeof(caracter_a[0])},
	{'V', caracter_v, sizeof(caracter_v)/sizeof(caracter_a[0])},
	{'X', caracter_x, sizeof(caracter_x)/sizeof(caracter_a[0])},
	{'Y', caracter_y, sizeof(caracter_y)/sizeof(caracter_a[0])},
	{'Z', caracter_z, sizeof(caracter_z)/sizeof(caracter_a[0])},
	{'0', caracter_0, sizeof(caracter_0)/sizeof(caracter_a[0])},
	{'1', caracter_1, sizeof(caracter_1)/sizeof(caracter_a[0])},
	{'2', caracter_2, sizeof(caracter_2)/sizeof(caracter_a[0])},
	{'3', caracter_3, sizeof(caracter_3)/sizeof(caracter_a[0])},
	{'4', caracter_4, sizeof(caracter_4)/sizeof(caracter_a[0])},
	{'5', caracter_5, sizeof(caracter_5)/sizeof(caracter_a[0])},
	{'6', caracter_6, sizeof(caracter_6)/sizeof(caracter_a[0])},
	{'7', caracter_7, sizeof(caracter_7)/sizeof(caracter_a[0])},
	{'8', caracter_8, sizeof(caracter_8)/sizeof(caracter_a[0])},
	{'9', caracter_9, sizeof(caracter_9)/sizeof(caracter_a[0])},
	{'>', caracter_derecha, sizeof(caracter_derecha)/sizeof(caracter_a[0])},
	{'<', caracter_izquierda, sizeof(caracter_izquierda)/sizeof(caracter_a[0])},
	{'^', caracter_arriba, sizeof(caracter_arriba)/sizeof(caracter_a[0])},
	{'v', caracter_abajo, sizeof(caracter_abajo)/sizeof(caracter_a[0])},
	{' ', caracter_espacio, sizeof(caracter_espacio)/sizeof(caracter_a[0])},
};

caracter_t caracter_desde_letra(char letra){
  size_t j = 0;
  caracter_t car;
  while(j < sizeof(caracteres)/sizeof(caracteres[0])){
    if(caracteres[j].letra == letra){
      car = caracteres[j];
      return car;
    }
	j++;
  }
  caracter_t nul = {'\0', NULL,0};
  return nul;
}

size_t caracter_cant_puntos(caracter_t car){
  return car.n_puntos;
}

void caracter_dibujar(char letra, double factor, bool r, bool g, bool b, float px, float py, SDL_Renderer *renderer){
  	caracter_t car = caracter_desde_letra(letra);
  	color_t color = color_crear(r, g, b);
  	polilinea_t *pol = polilinea_crear(car.coord, car.n_puntos, color);
  	polilinea_trasladar(pol, px, py);
  	polilinea_imprimir(pol, renderer, factor,px,py);
  	polilinea_destruir(pol);
}

void caracter_multiple_cadena_imprimir(char *texto,double factor, bool r, bool g, bool b, float px, float py, SDL_Renderer *renderer){
  size_t l = strlen(texto);
  for(size_t i = 0; i < l;i++){
    caracter_dibujar(texto[i], factor, r, g, b, px + i * CARACTER_ANCHO * factor, py, renderer);
    }
}


const float caracter_a[7][2] = {
	{0, 0},
	{0, 4},
	{2, 6},
	{4, 4},
	{4, 0},
	{4, 2},
	{0, 2}
};

const float caracter_b[12][2] = {
	{0, 3},
	{0, 6},
	{3, 6},
	{4, 5},
	{4, 4},
	{3, 3},
	{0, 3},
	{0, 0},
	{3, 0},
	{4, 1},
	{4, 2},
	{3, 3},
};

const float caracter_c[4][2] = {
	{4, 0},
	{0, 0},
	{0, 6},
	{4, 6},
};

const float caracter_d[7][2] = {
	{0, 0},
	{0, 6},
	{2, 6},
	{4, 4},
	{4, 2},
	{2, 0},
	{0, 0}
};

const float caracter_e[7][2] = {
	{4, 0},
	{0, 0},
	{0, 3},
	{3, 3},
	{0, 3},
	{0, 6},
	{4, 6},
};

const float caracter_f[6][2] = {
	{0, 0},
	{0, 3},
	{3, 3},
	{0, 3},
	{0, 6},
	{4, 6},
};

const float caracter_g[7][2] = {
	{2, 2},
	{4, 2},
	{4, 0},
	{0, 0},
	{0, 6},
	{4, 6},
	{4, 4},
};

const float caracter_h[6][2] = {
	{0, 0},
	{0, 6},
	{0, 3},
	{4, 3},
	{4, 6},
	{4, 0},
};

const float caracter_i[6][2] = {
	{0, 0},
	{4, 0},
	{2, 0},
	{2, 6},
	{4, 6},
	{0, 6},
};

const float caracter_j[4][2] = {
	{0, 2},
	{2, 0},
	{4, 0},
	{4, 6},
};

const float caracter_k[6][2] = {
	{0, 0},
	{0, 6},
	{0, 3},
	{3, 6},
	{0, 3},
	{3, 0},
};

const float caracter_l[3][2] = {
	{4, 0},
	{0, 0},
	{0, 6},
};

const float caracter_m[5][2] = {
	{0, 0},
	{0, 6},
	{2, 4},
	{4, 6},
	{4, 0},
};

const float caracter_n[4][2] = {
	{0, 0},
	{0, 6},
	{4, 0},
	{4, 6},
};

const float caracter_o[5][2] = {
	{4, 0},
	{0, 0},
	{0, 6},
	{4, 6},
	{4, 0},
};

const float caracter_p[5][2] = {
	{0, 0},
	{0, 6},
	{4, 6},
	{4, 3},
	{0, 3},
};

const float caracter_q[9][2] = {
	{2, 2},
	{3, 1},
	{2, 0},
	{0, 0},
	{0, 6},
	{4, 6},
	{4, 2},
	{3, 1},
	{4, 0},
};

const float caracter_r[7][2] = {
	{0, 0},
	{0, 6},
	{4, 6},
	{4, 3},
	{0, 3},
	{1, 3},
	{4, 0},
};

const float caracter_s[6][2] = {
	{0, 0},
	{4, 0},
	{4, 3},
	{0, 3},
	{0, 6},
	{4, 6},
};

const float caracter_T[4][2] = {
	{2, 0},
	{2, 6},
	{4, 6},
	{0, 6},
};

const float caracter_u[4][2] = {
	{0, 6},
	{0, 0},
	{4, 0},
	{4, 6},
};

const float caracter_v[3][2] = {
	{0, 6},
	{2, 0},
	{4, 6},
};

const float caracter_w[5][2] = {
	{0, 6},
	{0, 0},
	{2, 2},
	{4, 0},
	{4, 6},
};

const float caracter_x[5][2] = {
	{0, 0},
	{4, 6},
	{2, 3},
	{0, 6},
	{4, 0},
};

const float caracter_y[5][2] = {
	{0, 6},
	{2, 4},
	{4, 6},
	{2, 4},
	{2, 0},
};

const float caracter_z[4][2] = {
	{0, 6},
	{4, 6},
	{0, 0},
	{4, 0},
};

const float caracter_0[5][2] = {
	{4, 0},
	{0, 0},
	{0, 6},
	{4, 6},
	{4, 0},
};

const float caracter_1[2][2] = {
	{2, 0},
	{2, 6},
};

const float caracter_2[6][2] = {
	{0, 6},
	{4, 6},
	{4, 3},
	{0, 3},
	{0, 0},
	{4, 0},
};

const float caracter_3[7][2] = {
	{0, 0},
	{4, 0},
	{4, 3},
	{0, 3},
	{4, 3},
	{4, 6},
	{0, 6},
};

const float caracter_4[5][2] = {
	{0, 6},
	{0, 3},
	{4, 3},
	{4, 6},
	{4, 0},
};

const float caracter_5[6][2] = {
	{0, 0},
	{4, 0},
	{4, 3},
	{0, 3},
	{0, 6},
	{4, 6},
};

const float caracter_6[5][2] = {
	{0, 6},
	{0, 0},
	{4, 0},
	{4, 3},
	{0, 3},
};

const float caracter_7[3][2] = {
	{0, 6},
	{4, 6},
	{4, 0},
};

const float caracter_8[7][2] = {
	{0, 3},
	{0, 6},
	{4, 6},
	{4, 0},
	{0, 0},
	{0, 3},
	{4, 3},
};

const float caracter_9[5][2] = {
	{4, 0},
	{4, 6},
	{0, 6},
	{0, 3},
	{4, 3},
};

const float caracter_derecha[5][2] = {
	{0, 3},
	{12, 3},
	{9, 5},
	{9, 1},
	{12, 3},
};

const float caracter_izquierda[5][2] = {
	{12, 3},
	{0, 3},
	{3, 5},
	{3, 1},
	{0, 3},
};

const float caracter_arriba[5][2] = {
	{5, -2},
	{5, 10},
	{3, 7},
	{7, 7},
	{5, 10},
};

const float caracter_abajo[5][2] = {
	{5, 10},
	{5, -2},
	{3, 1},
	{7, 1},
	{5, -2},
};

const float caracter_espacio[1][2] = {
	{0, 0},
};

