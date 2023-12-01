#include "variables.h"

// Inicialización de las variables globales
int num_reordenamientos = 0;
int num_jugadas[MAX_JUGADORES] = {0};
int num_cartas_jugar[MAX_JUGADORES] = {0};
int num_cartas_esperar[MAX_JUGADORES] = {0};
int total_cartas_jugar = 0;
int total_cartas_esperar = 0;
int maximo_reordenamientos = 10;
int num_cartas = 0;
int reordenando = 0;
Carta mazo[10];
pthread_mutex_t mutex_tablero;
pthread_mutex_t mutex_mazo;
pthread_cond_t cond_tablero;