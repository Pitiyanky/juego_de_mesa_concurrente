#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>  // Para rand() y srand()
#include <time.h>    // Para time()

// Definimos las cartas
typedef enum {
    ESPERAR,
    JUGAR
} TipoCarta;

typedef struct {
    TipoCarta tipo;
} Carta;

// Definiciones de las funciones
void* jugador(void* arg);
void* jefe_de_mesa(void* arg);

Carta mazo[10];
int MAX_JUGADORES = 10;
int num_cartas = 0;
int tablero_libre = 1;
int reordenando = 0;
// Mutex para el acceso al mazo de cartas y al tablero
pthread_mutex_t mutex_mazo = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_tablero = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_tablero = PTHREAD_COND_INITIALIZER;

void llenar_mazo() {
    srand(time(NULL));  // Inicializar la semilla del generador de números aleatorios
    for (int i = 0; i < 10; i++) {
        Carta carta;
        if (rand() % 2 == 0) {  // Generar un número aleatorio y comprobar si es par
            carta.tipo = JUGAR;
        } else {
            carta.tipo = ESPERAR;
        }
        mazo[i] = carta;
        num_cartas++;
    }
}


int main() {
  // Crear hilos para jugadores y jefe de mesa
  int i, num_jugador[MAX_JUGADORES];
  llenar_mazo();
  pthread_t tid_jugador[MAX_JUGADORES], tid_jefe_de_mesa;
  for (i = 0; i < MAX_JUGADORES; i++) {
    num_jugador[i] = i + 1;  // Asignar el número del jugador
    pthread_create(&tid_jugador[i], NULL, jugador, &num_jugador[i]);
  }
  pthread_create(&tid_jefe_de_mesa, NULL, jefe_de_mesa, NULL);

  // Esperar a que los hilos terminen
  for (i = 0; i < MAX_JUGADORES; i++) {
     pthread_join(tid_jugador[i], NULL);
  }
  pthread_join(tid_jefe_de_mesa, NULL);

  return 0;
}

void pensar_jugada() {
    sleep(rand() % 3 + 1);  // Pensar durante 1-3 segundos
}

Carta sacar_carta_de_mazo() {
    pthread_mutex_lock(&mutex_mazo);
    Carta carta = mazo[--num_cartas];
    pthread_mutex_unlock(&mutex_mazo);
    return carta;
}

void jugar() {
    pthread_mutex_lock(&mutex_tablero);
    sleep(rand() % 3 + 1);  // Jugar durante 1-3 segundos
    pthread_mutex_unlock(&mutex_tablero);
}

void* jugador(void* arg) {
  int num = *(int*)arg;
  while (1) {
    pensar_jugada();
    if(num_cartas>0){ // Nunca se debe sacar uan carta si no hay cartas en el mazo
    Carta carta = sacar_carta_de_mazo();
    if (carta.tipo == JUGAR){
      printf("El jugador %d realiza su jugada.\n", num);
      jugar();
    } else {
      while (reordenando) {
          pthread_cond_wait(&cond_tablero, &mutex_tablero);
      }
      pthread_mutex_unlock(&mutex_tablero);
      printf("El jugador %d espera a que el jefe de mesa reordene el tablero.\n", num);
      }
    }
  }
  return NULL;
}

void pensar_reordenamiento() {
    sleep(rand() % 3 + 1);  // Pensar durante 1-3 segundos
}

void reordenar_tablero() {
    pthread_mutex_lock(&mutex_tablero);
    printf("El jefe de mesa está reordenando el tablero.\n");
    sleep(rand() % 3 + 1);  // Reordenar durante 1-3 segundos
    tablero_libre = 1;
    pthread_mutex_unlock(&mutex_tablero);
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
  printf("El jefe de mesa está colocando la carta en el mazo.\n");
  if (num_cartas < 10) {
      mazo[num_cartas++] = carta;
  }
  pthread_mutex_unlock(&mutex_mazo);
}

void* jefe_de_mesa(void* arg) {
    while (1) {
      pensar_reordenamiento();
      reordenando = 1;
      reordenar_tablero();
      reordenando = 0;
      pthread_cond_broadcast(&cond_tablero);
      if(num_cartas == 0){
        pthread_mutex_lock(&mutex_mazo);
        llenar_mazo();
        pthread_mutex_unlock(&mutex_mazo);
      }else{
        Carta carta = elegir_proxima_carta();
        colocar_carta_en_mazo(carta);
      }
      printf("%d",num_cartas);
    }
    return NULL;
}