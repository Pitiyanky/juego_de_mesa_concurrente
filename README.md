## Proyecto: Sistemas Operativos – Sem 1 - 2023
#Descripción del Juego
Este proyecto modela un juego de mesa que incluye N jugadores y un jefe de mesa. Los jugadores pueden mover sus fichas todos al mismo tiempo, siempre y cuando el jefe de mesa no esté reordenando el tablero. El jefe de mesa reordena el tablero cada vez que termina de pensar sus cambios. Además, el jefe de mesa deja cartas con instrucciones para los jugadores en caso de que esté esperando para reordenar el tablero. Hay dos tipos de cartas: una que indica que se debe esperar a que el jefe de mesa reordene el tablero para poder jugar, y otra que permite jugar antes del reordenamiento de fichas.

#Funciones
Los jugadores ejecutan las siguientes funciones:

pensar_jugada()
sacar_carta_de_mazo():Carta
jugar()
El Jefe de Mesa ejecuta las siguientes funciones:

pensar_reordenamiento()
reordenar_tablero()
elegir_proxima_carta():Carta
colocar_carta_en_mazo(Carta)
#Contabilización
Se contabiliza lo siguiente:

Número de veces que el jefe de mesa reordenó el tablero durante una partida
Número de jugadas por jugador durante la partida
Número total de cartas de jugar durante la partida, por jugador
Número total de cartas de esperar durante la partida, por jugador
Número total de cartas de jugar durante la partida
Número total de cartas de esperar durante la partida
#Simulación
La simulación debe evitar las situaciones de interbloqueo, inanición y espera activa. Se debe procurar un elevado grado de concurrencia entre los procesos. Jugadores y el jefe de mesa deben emitir mensajes informativos por pantalla cada vez que su situación se modifique. De esta forma, la salida permitirá tener acceso a toda la información, para que se pueda verificar el comportamiento.


