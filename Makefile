PROGRAM=Gravitar
CC=gcc
CFLAGS= -Wall -Werror -std=c99 -pedantic -g
LDFLAGS= -lm
SDL2= -lSDL2

all: $(PROGRAM)

$(PROGRAM): main.o color.o figura.o polilinea.o lista_enlazada.o lectura.o caracteres.o planeta.o nave.o fisicas.o torreta.o tanque.o nivel.o disparo.o auxiliar.o
	$(CC) $(CFLAGS) main.o color.o figura.o polilinea.o lista_enlazada.o lectura.o caracteres.o planeta.o nave.o fisicas.o torreta.o tanque.o nivel.o disparo.o auxiliar.o -o $(PROGRAM) $(SDL2) $(LDFLAGS)

caracteres.o: caracteres.c polilinea.h color.h config.h
	$(CC) $(CFLAGS) -c caracteres.c

color.o: color.c 
	$(CC) $(CFLAGS) -c color.c

lectura.o: lectura.c  figura.h polilinea.h 
	$(CC) $(CFLAGS) -c lectura.c

figura.o: figura.c color.h lectura.h polilinea.h
	$(CC) $(CFLAGS) -c figura.c

lista_enlazada.o: lista_enlazada.c  
	$(CC) $(CFLAGS) -c lista_enlazada.c

fisicas.o: fisicas.c polilinea.h
	$(CC) $(CFLAGS) -c fisicas.c

nave.o: nave.c config.h figura.h fisicas.h
	$(CC) $(CFLAGS) -c nave.c

planeta.o: planeta.c figura.h nave.h fisicas.h
	$(CC) $(CFLAGS) -c planeta.c

polilinea.o: polilinea.c color.h config.h fisicas.h
	$(CC) $(CFLAGS) -c polilinea.c 

nivel.o: nivel.c figura.h lista_enlazada.h
	$(CC) $(CFLAGS) -c nivel.c

torreta.o: torreta.c figura.h
	$(CC) $(CFLAGS) -c torreta.c 

tanque.o: tanque.c figura.h
	$(CC) $(CFLAGS) -c tanque.c

disparo.o: disparo.c figura.h fisicas.h
	$(CC) $(CFLAGS) -c disparo.c

auxiliar.o: auxiliar.c figura.h disparo.h lista_enlazada.h nivel.h nave.h torreta.h tanque.h planeta.h
	$(CC) $(CFLAGS) -c auxiliar.c


main.o: main.c config.h lista_enlazada.h polilinea.h lectura.h planeta.h figura.h caracteres.h nave.h nivel.h torreta.h tanque.h disparo.h auxiliar.h
	$(CC) $(CFLAGS) -c main.c 

clean:
	rm -vf *.o main_v