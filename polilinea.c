#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "polilinea.h"
#include "fisicas.h"


struct polilinea {
  float (*puntos)[2];
  size_t n;
  uint8_t r, g, b;
};

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
// Manejo de memoria

polilinea_t *polilinea_crear_vacia(size_t n) {
  polilinea_t *p = malloc(sizeof(polilinea_t)); // Memoria para la structura
  if (p == NULL)
    return NULL;

  p->puntos = malloc(n * sizeof(float[2])); // Memoria para los n puntos. Cada punto tiene 2 flotantes
  if (p->puntos == NULL) {
    free(p);
    return NULL;
  }
  p->n = n;

  return p;
}

void polilinea_setear_color(polilinea_t *p, color_t c) {
  uint8_t r, g, b;
  color_a_rgb(c, &r, &g, &b);

  p->r = r;
  p->g = g;
  p->b = b;
}

polilinea_t *polilinea_crear(const float puntos_recibidos[][2], size_t num, color_t color) {
  polilinea_t *p = polilinea_crear_vacia(num);
  if (p == NULL) {
    return NULL;
  }

  polilinea_setear_color(p, color);
  memcpy(*p->puntos, puntos_recibidos, num * sizeof(float) * 2);

  return p;
}

// Libera memoria de la structura
void polilinea_destruir(polilinea_t *polilinea) {
  free(polilinea->puntos);
  free(polilinea);
}

polilinea_t *polilinea_clonar(const polilinea_t *polilinea) {
  color_t c = color_crear(polilinea->r, polilinea->g, polilinea->b);
  return polilinea_crear((const float(*)[2])(polilinea->puntos), polilinea->n, c);
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
// Lectura del archivo binario

#define MASK_NUM_PUNTOS 0x03ff // 0000 0011 1111 1111
#define MASK_COLOR_POLI 0xe000 // 1110 0000 0000 0000

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
// seters y geters

/*
    Seters
*/
bool polilinea_setear_punto(polilinea_t *polilinea, size_t pos, float x, float y) {
  if (polilinea == NULL)
    return false;

  if (pos >= polilinea->n)
    return false;

  // Seteado del punto
  polilinea->puntos[pos][0] = x;
  polilinea->puntos[pos][1] = y;

  return true;
}

/*
    Geters
*/
size_t polilinea_cantidad_puntos(const polilinea_t *polilinea) {
  return polilinea->n;
}

bool polilinea_get_punto(const polilinea_t *polilinea, size_t pos, float *x,
                             float *y) {
  if (pos >= (polilinea->n)) // Punto invalido
    return false;

  *x = polilinea->puntos[pos][0];
  *y = polilinea->puntos[pos][1];
  return true;
}

uint8_t polilinea_get_color_r(const polilinea_t *p) { return p->r; }

uint8_t polilinea_get_color_g(const polilinea_t *p) { return p->g; }

uint8_t polilinea_get_color_b(const polilinea_t *p) { return p->b; }

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
// Transformaciones
#define LARGO_FILA 2
#define X 0
#define Y 1

static void trasladar(float polilinea[][2], size_t n, float dx, float dy){
  for(size_t i = 0; i < n; i ++){
    polilinea[i][X] += dx;
    polilinea[i][Y] += dy;
  }
}

static void rotar(float polilinea[][2], size_t n, double rad){
  static double coseno;
  coseno = cos(rad);
  static double seno;
  seno = sin(rad);

  //static double aux;

  double p_x, p_y;

  for (size_t i = 0; i < n; i++) {

    p_x = polilinea[i][X];
    p_y = polilinea[i][Y];

    polilinea[i][X] = p_x * coseno - p_y * seno;
    polilinea[i][Y] = p_x * seno + p_y * coseno;
  }
}

void polilinea_trasladar(polilinea_t *p, float dx, float dy) {
  trasladar(p->puntos, p->n, dx, dy);  
}

// rota RESPECTO DEL ORIGEN, recibe angulo en radianes.
void polilinea_rotar(polilinea_t *p, double rad) {
  rotar(p->puntos, p->n, rad);
}

void polilinea_cambiar_factor(polilinea_t *p, float factor){
    for(int i = 0; i < p->n; i+=1){
        (p->puntos)[i][X] *= factor;
        (p->puntos)[i][Y] *= factor;
    }
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
// Interacciones poliliena

double grados_a_radianes(double angulo_grados) {
  return angulo_grados * PI / 180.0;
}

/*
    Algunas funciones privadas
*/
static double producto_interno(double x1, double y1, double x2, double y2) {
  return x1 * x2 + y1 * y2;
}

static double norma(double x, double y){
  return sqrt(x * x + y * y); 
}


// Recibe incio del segmento (A), final del segmento (B) y el punto con el que
// se quiere encontrar la distancia.
static double distancia_punto_segmento(const float p1_x, const float p1_y, const float p2_x, const float p2_y, float px, float py) {
  static double modulo_cuadrado;
  modulo_cuadrado = norma(p1_x - p2_x, p1_y - p2_y); // |(B - A)|

  modulo_cuadrado *= modulo_cuadrado;

  static double alfa;
  alfa = producto_interno(px - p1_x, py - p1_y, p2_x - p1_x, p2_y - p1_y) / modulo_cuadrado;

  if (alfa <= 0) // Punto mas cercano es p = A
    return distancia_entre_puntos(p1_x, p1_y, px, py);

  if (alfa >= 1) // Punto mas cercano es p = B
    return distancia_entre_puntos(p2_x, p2_y, px, py);

  // Punto mas cercano es p = A + alfa * (B-A)
  return distancia_entre_puntos(p1_x + alfa * (p2_x - p1_x), p1_y + alfa * (p2_y - p1_y), px, py);
}


/*Esta funcion va a ir checkeando para cada segmento, de la polilinea cual es su
punto mas cercano (p) al punto P. Obtiene p y calcula la distancia entre p y
P.*/
double polilinea_distancia(polilinea_t *p, size_t n, float px, float py) {
  // arranca con la distancia del punto P al primer segmento de la polilinea
  static double distancia_minima;
  distancia_minima =
      distancia_punto_segmento((p->puntos)[0][X], (p->puntos)[0][Y], (p->puntos)[1][X], (p->puntos)[1][Y], px, py);

  static double temp_distancia_minima_segmento; // distancia minima de cada
                                                // segmento AB a P=(px, py)

  static size_t iteraciones;
  iteraciones = n - 1;
  for (size_t i = 1; i < iteraciones; i++) {
    temp_distancia_minima_segmento = distancia_punto_segmento(
        (p->puntos)[i][X], (p->puntos)[i][Y], (p->puntos)[i + 1][X],
        (p->puntos)[i + 1][Y], px, py);

    if (temp_distancia_minima_segmento == 0)
      return temp_distancia_minima_segmento;

    if (temp_distancia_minima_segmento < distancia_minima)
      distancia_minima = temp_distancia_minima_segmento;
  }

  return distancia_minima;
}

double polilinea_interseccion(polilinea_t *p, double px, double py){
  return polilinea_distancia(p, p->n, px, py);
}

double polilinea_min_x(polilinea_t *polilinea){
  double min = polilinea->puntos[0][X];
  for(size_t i = 1; i < polilinea->n; i++){
    if(polilinea->puntos[i][X] < min)
      min = polilinea->puntos[i][X]; 
  }
  return min;
}

double polilinea_min_y(polilinea_t *polilinea){
  double min = polilinea->puntos[0][Y];
  for(size_t i = 1; i < polilinea->n; i++){
    if(polilinea->puntos[i][Y] < min)
      min = polilinea->puntos[i][Y]; 
  }
  return min;
}

double polilinea_max_y(polilinea_t *polilinea){
  double max = polilinea->puntos[0][Y];
  for(size_t i = 1; i < polilinea->n; i++){
    if(polilinea->puntos[i][Y] > max)
      max = polilinea->puntos[i][Y]; 
  }
  return max;
}

double polilinea_max_x(polilinea_t *polilinea){
  double max = polilinea->puntos[0][X];
  for(size_t i = 1; i < polilinea->n; i++){
    if(polilinea->puntos[i][X] > max)
      max = polilinea->puntos[i][X]; 
  }
  return max;
}

double polilinea_ancho(polilinea_t *polilinea){
  return polilinea_max_x(polilinea) - polilinea_min_x(polilinea);
}

double polilinea_alto(polilinea_t *polilinea){
  return polilinea_max_y(polilinea) - polilinea_min_y(polilinea);
}


void polilinea_imprimir(polilinea_t *p, SDL_Renderer *renderer, float factor, double dx, double dy) {
    // Asigno el color a dibujar
    SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 0xFF);

    for (size_t j = 0; j < p->n - 1; j++) {      
      SDL_RenderDrawLine(
          renderer, 
          (p->puntos[j][X]-dx)*factor + dx,
          -(p->puntos[j][Y]-dy)*factor + VENTANA_ALTO - dy,
          (p->puntos[j+1][X]-dx)*factor + dx,
          -(p->puntos[j+1][Y]-dy)*factor +VENTANA_ALTO - dy 
          );
    }
}


void polilinea_imprimir_movida(polilinea_t *p, SDL_Renderer *renderer, float factor, double dx, double dy) {
    // Asigno el color a dibujar
    SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 0xFF);

    for (size_t j = 0; j < p->n - 1; j++) {      
      SDL_RenderDrawLine(
          renderer, 
          (p->puntos[j][X])*factor + dx,
          -(p->puntos[j][Y])*factor + VENTANA_ALTO - dy,
          (p->puntos[j+1][X])*factor + dx,
          -(p->puntos[j+1][Y])*factor +VENTANA_ALTO - dy 
          );
    }
}
