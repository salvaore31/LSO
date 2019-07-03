#ifndef CONSTANT_H_
#define CONSTANT_H_

//TODO CAPIRE COME SI DEFINISCE UN VALORE INTERO POSITIVO A 16 bit e metterli dove ha senso

/* Le costanti seguenti corrispondono ai valori da usare come secondo operando
 * di un AND bit a bit con il valore corrente di una casella della matrice
 * di gioco per ottenere informazioni sullo stato della casella.
 */
#define READ_CELL
#define READ_WIN_CONDITION
#define READ_CURRENT_PLAYER

/* Le costanti seguenti corrispondo ai valori da usare come confronto per valutare
 * i valori ottenuti letti dalla matrice di gioco.
 */
#define FREE_CELL
#define OBSTACLE
#define PLAYER_POSITION
#define PLAYER_WIN_CONDITION
#define IF_VISIBLE

/* Le costanti seguenti contengono valori utili alla gestione della matrice di
 * gioco.
 */
#define RESET  "\033[0m"
#define GREEN  "\033[92m\033[92m"
#define RED    "\033[91m\033[91m"
#define CYAN   "\033[96m\033[96m"
#define YELLOW "\033[93m\033[93m"

/* Le costanti seguenti contengono valori utili per gestire una partita.
 *
 */
#define MAX_PLAYER_N 8
#define MAX_OBSTACLES_N 50 //po se vere
#define MAX_GRID_SIZE_L 64
#define MAX_GRID_SIZE_H 48
