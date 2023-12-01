#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>  // Para rand() y srand()
#include <time.h>    // Para time()

// Definicion de las cartas
typedef enum {
    ESPERAR,
    JUGAR
} TipoCarta;

typedef struct {
    TipoCarta tipo;
} Carta;

// Definiciones de las entidades jugador y jefe
void* jugador(void* arg);
void* jefe_de_mesa(void* arg);

//Variables Compartidas
Carta mazo[10];
const int MAX_JUGADORES = 10; //Numero de jugadores en la partida
int num_cartas = 0; // Indica el numero de cartas en el mazo, se asume que el mazo inicia vacio
int reordenando = 0; //Indica si el jefe quiere reordenar el tablero
int maximo_reordenamientos = 10; // Indica el numero de reordenamientos que durara una partida

//Contadores
int num_reordenamientos = 0;  // Número de veces que el jefe de mesa reordenó el tablero
int num_jugadas[MAX_JUGADORES] = {0};  // Número de jugadas por jugador
int num_cartas_jugar[MAX_JUGADORES] = {0};  // Número total de cartas de jugar por jugador
int num_cartas_esperar[MAX_JUGADORES] = {0};  // Número total de cartas de esperar por jugador
int total_cartas_jugar = 0;  // Número total de cartas de jugar
int total_cartas_esperar = 0; //Numero total de cartas de esperar

// Mutex para el acceso al mazo de cartas y al tablero
pthread_mutex_t mutex_mazo = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_tablero = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_tablero = PTHREAD_COND_INITIALIZER;

int main() {
  // Crear hilos para jugadores y jefe de mesa
  int i, num_jugador[MAX_JUGADORES];
  pthread_t tid_jugador[MAX_JUGADORES], tid_jefe_de_mesa;
  for (i = 0; i < MAX_JUGADORES; i++) {
    num_jugador[i] = i + 1;  // Asignar el número del jugador
    pthread_create(&tid_jugador[i], NULL, jugador, &num_jugador[i]);
  }
  pthread_create(&tid_jefe_de_mesa, NULL, jefe_de_mesa, NULL);

  // Esperar a que los hilos terminen
  pthread_join(tid_jefe_de_mesa, NULL);
  for (i = 0; i < MAX_JUGADORES; i++) {
     pthread_join(tid_jugador[i], NULL);
  }
    
  //Imprimir todos los conteos
  printf("Total reordenamientos: %d\n", num_reordenamientos);
  printf("Total de cartas Jugar: %d\n", total_cartas_jugar);
  printf("Total de cartas Rsperar: %d\n", total_cartas_esperar);
  for(int i = 0; i < MAX_JUGADORES; i++) {
      printf("Numero Jugadas del jugador %d: %d\n", i, num_jugadas[i]);
      printf("Numero de cartas Jugar del jugador %d: %d\n", i, num_cartas_jugar[i]);
      printf("Numero de cartas Esperar del jugador %d: %d\n", i, num_cartas_esperar[i]);
  }
  return 0;
}

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
