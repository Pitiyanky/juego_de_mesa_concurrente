#ifndef JEFE_DE_MESA_H
#define JEFE_DE_MESA_H
#include "variables.h"

void pensar_reordenamiento();
void reordenar_tablero();
Carta elegir_proxima_carta();
void colocar_carta_en_mazo(Carta carta);
void* jefe_de_mesa(void* arg);

#endif