#ifndef VARIABLES_H
#define VARIABLES_H

#include <pthread.h>

#define MAX_JUGADORES 10  // Número máximo de jugadores

// Definición de los tipos de cartas
typedef enum {JUGAR, ESPERAR} Tipo;
typedef struct {
    Tipo tipo;
} Carta;

// Variables globales
extern int num_reordenamientos;  // Número de veces que el jefe de mesa reordenó el tablero
extern int num_jugadas[MAX_JUGADORES];  // Número de jugadas por jugador
extern int num_cartas_jugar[MAX_JUGADORES];  // Número total de cartas de jugar por jugador
extern int num_cartas_esperar[MAX_JUGADORES];  // Número total de cartas de esperar por jugador
extern int total_cartas_jugar;  // Número total de cartas de jugar
extern int total_cartas_esperar;  // Número total de cartas de esperar
extern int maximo_reordenamientos;  // Número máximo de reordenamientos permitidos
extern int num_cartas;  // Número de cartas en el mazo
extern int reordenando;  // Indica si el jefe de mesa quiere reordenar el tablero
extern Carta mazo[10];  // El mazo de cartas
extern pthread_mutex_t mutex_tablero;  // Mutex para el tablero
extern pthread_mutex_t mutex_mazo;  // Mutex para el mazo
extern pthread_cond_t cond_tablero;  // Condición para el tablero

#endif