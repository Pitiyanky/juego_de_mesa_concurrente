# Definir el compilador
CC = gcc

# Definir las banderas del compilador
CFLAGS = -Wall -lpthread

# Definir los archivos objeto
OBJ = main.o jugadores.o jefe_de_mesa.o variables.o

# Regla para el objetivo final (juego)
juego: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Reglas para los archivos objeto
main.o: main.c jugadores.h jefe_de_mesa.h variables.h
	$(CC) -c -o $@ $< $(CFLAGS)

jugadores.o: jugadores.c jugadores.h variables.h
	$(CC) -c -o $@ $< $(CFLAGS)

jefe_de_mesa.o: jefe_de_mesa.c jefe_de_mesa.h variables.h
	$(CC) -c -o $@ $< $(CFLAGS)

variables.o: variables.c variables.h
	$(CC) -c -o $@ $< $(CFLAGS)

# Regla para limpiar los archivos objeto y el ejecutable
.PHONY: clean
clean:
	rm -f $(OBJ) juego
