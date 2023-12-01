#include "jefe_de_mesa.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void pensar_reordenamiento() {
    sleep(rand() % 3 + 1);  // Pensar durante 1-3 segundos
}

void reordenar_tablero() {
  reordenando = 1; //Indico que quiero reordenar el tablero
  printf("El jefe de mesa quiere reordenar el tablero.\n");
  pthread_mutex_lock(&mutex_tablero);
  printf("El jefe de mesa está reordenando el tablero.\n");
  sleep(rand() % 3 + 1);  // Reordenar durante 1-3 segundos
  num_reordenamientos++;
  pthread_cond_broadcast(&cond_tablero); //Libero a todos los que estaban esperando que se reordene el tablero
  pthread_mutex_unlock(&mutex_tablero); //Libero el tablero
  printf("El jefe de mesa termino de reordenar el tablero.\n");
  reordenando = 0; //Indico que ya no quiero reordenar el tablero
}

Carta elegir_proxima_carta() {
  printf("El jefe de mesa está eligiendo la próxima carta.\n");
  srand(time(NULL));  // Inicializar la semilla del generador de números aleatorios
  Carta carta;
  if (rand() % 2 == 0) {  // Generar un número aleatorio y comprobar si es par
      carta.tipo = JUGAR;
  } else {
      carta.tipo = ESPERAR;
  }
  return carta;
}

void colocar_carta_en_mazo(Carta carta) {
  pthread_mutex_lock(&mutex_mazo);
  if (num_cartas < 10) { //No se puede colocar una carta en un mazo lleno
    printf("El jefe de mesa está colocando la carta en el mazo.\n");
    mazo[num_cartas++] = carta;
  }
  pthread_mutex_unlock(&mutex_mazo);
}

void* jefe_de_mesa(void* arg) {
    while (num_reordenamientos<maximo_reordenamientos+1) { //El jefe de mesa solo realizara un numero maximo de reordenamientos
      pensar_reordenamiento();
      reordenar_tablero();
      //si no hay cartas se reordena el tablero de nuevo
      if(num_cartas == 0){
        reordenar_tablero();
      }
      Carta carta = elegir_proxima_carta();
      colocar_carta_en_mazo(carta);

    }
    return NULL;
}