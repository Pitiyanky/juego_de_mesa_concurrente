# Proyecto de Simulación de Juego de Cartas

## Descripción
Este proyecto simula un juego de cartas con múltiples jugadores y un jefe de mesa. Los jugadores pueden jugar o esperar dependiendo de las cartas que sacan del mazo, y el jefe de mesa puede reordenar el tablero y colocar nuevas cartas en el mazo. El código utiliza hilos y mecanismos de sincronización para manejar la concurrencia.

## Características
- Implementa hilos con la biblioteca pthreads de C.
- Usa mutexes y variables de condición para controlar el acceso concurrente.
- Simula diferentes escenarios de juego concurrente.

## Requisitos
- Compilador GCC
- Soporte para pthreads

## Compilación y Ejecución
Utilice el archivo makefile incluido para compilar:

```bash
$ make
```

Para ejecutar el programa:

```bash
$ ./juego
```

## Estructura del Proyecto
- `main.c`: Punto de entrada del programa, contiene la lógica principal.
- `jugador.h` y `jugador.c`: Definen las funciones y comportamientos del jugador.
- `jefe_de_mesa.h` y `jefe_de_mesa.c`: Definen las funciones y comportamientos del jefe de mesa.
- `variables.h` y `variables.c`: Definen las variables globales utilizadas en el proyecto.

