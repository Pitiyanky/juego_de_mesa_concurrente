#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>  // Para rand() y srand()
#include <time.h>    // Para time()
#include "jugadores.h"
#include "jefe_de_mesa.h"
#include "variables.h"

int main() {
  // Crear hilos para jugadores y jefe de mesa
  int i, num_jugador[MAX_JUGADORES];
  int choice = 0;
  //Caso de prueba, Y si el Jefe no quiere poner cartas?
  do
  {
      printf("=================================================\n");
      printf(" MENÚ PRINCIPAL - SIMULACIÓN DE jUEGO DE MESA\n");
      printf("=================================================\n");
      printf("Seleccione una opción:\n");
      printf("1 - Se permiten cartas (El jefe de mesa pondra cartas cada cierto tiempo)\n");
      printf("0 - No se permiten cartas (El jefe de mesa nunca pondra cartas)\n");
      printf("Introduzca su elección: ");
      scanf("%d", &choice);
      printf("\n");
  }while (choice < 0 && choice > 1);
  
  pthread_t tid_jugador[MAX_JUGADORES];
  for (i = 0; i < MAX_JUGADORES; i++) {
    num_jugador[i] = i;  // Asignar el número del jugador
    pthread_create(&tid_jugador[i], NULL, jugador, &num_jugador[i]);
  }
  pthread_t hilo_jefe_reordenar;
  pthread_create(&hilo_jefe_reordenar, NULL, jefe_de_mesa_reordenar, NULL);
  if(choice){
    pthread_t hilo_jefe_cartas;
    pthread_create(&hilo_jefe_cartas, NULL, jefe_de_mesa_cartas, NULL);
    pthread_join(hilo_jefe_cartas, NULL);
  }
  // Los hilos para que terminen
  pthread_join(hilo_jefe_reordenar, NULL);
  for (i = 0; i < MAX_JUGADORES; i++) {
     pthread_join(tid_jugador[i], NULL);
  }
  
  //Imprimir todos los conteos
  printf("Total reordenamientos: %d\n", num_reordenamientos);
  printf("Total de cartas Jugar: %d\n", total_cartas_jugar);
  printf("Total de cartas Esperar: %d\n", total_cartas_esperar);
  for(int i = 0; i < MAX_JUGADORES; i++) {
      printf("Numero Jugadas del jugador %d: %d\n", i, num_jugadas[i]);
      printf("Numero de cartas Jugar del jugador %d: %d\n", i, num_cartas_jugar[i]);
      printf("Numero de cartas Esperar del jugador %d: %d\n", i, num_cartas_esperar[i]);
  }

  return 0;
}

