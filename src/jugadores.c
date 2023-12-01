#include "jugadores.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void pensar_jugada(int num_jugador) {
  printf("El jugador %d esta pensando\n", num_jugador);
    sleep(rand() % 3 + 1);  // Pensar durante 1-3 segundos
}

Carta sacar_carta_de_mazo(int num_jugador) {
  printf("El jugador %d saca una carta\n", num_jugador);
  Carta carta = mazo[--num_cartas];
  return carta;
}

void jugar(int num_jugador) {
  printf("El jugador %d quiere realizar su jugada.\n", num_jugador);
  pthread_mutex_lock(&mutex_tablero);
  printf("El jugador %d realiza su jugada.\n", num_jugador);
  num_jugadas[num_jugador]++;
  sleep(rand() % 3 + 1);  // Jugar durante 1-3 segundos
  pthread_mutex_unlock(&mutex_tablero);
  printf("El jugador %d termino su jugada.\n", num_jugador);
}

void esperar(int num_jugador) {
  printf("El jugador %d espera a que el jefe de mesa reordene el tablero.\n", num_jugador);
  pthread_mutex_lock(&mutex_tablero); //Es obligatorio bloquear el tablero antes del conditional wait para evitar interbloqueos
  pthread_cond_wait(&cond_tablero, &mutex_tablero); //Se espera hasta el jefe avise que ya reordeno el tablero
  pthread_mutex_unlock(&mutex_tablero);
}

void* jugador(void* arg) {
  int num_jugador = *(int*)arg;
  while (num_reordenamientos<maximo_reordenamientos) { //El jugador solo podra jugar mientras el jefe no haga un numero de reordenamientos
    pensar_jugada(num_jugador);
    if(reordenando){ //El jugador comprueba que el jefe no quiera reordenar el tablero
      pthread_mutex_lock(&mutex_mazo);
      if(num_cartas>0){ // Nunca se debe sacar una carta si no hay cartas en el mazo
        Carta carta = sacar_carta_de_mazo(num_jugador);
        pthread_mutex_unlock(&mutex_mazo);
        if (carta.tipo == ESPERAR){
          num_cartas_esperar[num_jugador]++;
          total_cartas_esperar++;
          esperar(num_jugador);
        } else {
          num_cartas_jugar[num_jugador]++;
          total_cartas_jugar++;
          jugar(num_jugador);
        }

      }else{
        pthread_mutex_unlock(&mutex_mazo);
        esperar(num_jugador); // Si no hay cartas se debe esperar a que el Jefe reordene el tablero
      }
    } else {
      jugar(num_jugador); //Encaso de que el jefe no quiera reordenar el tablero el jugador podra hacer su jugada
    }
  }
  return NULL;
}