#ifndef JUGADOR_H
#define JUGADOR_H
#include "variables.h"

void pensar_jugada(int num_jugador);
Carta sacar_carta_de_mazo(int num_jugador);
void jugar(int num_jugador);
void esperar(int num_jugador);
void* jugador(void* arg);

#endif