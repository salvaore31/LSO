#ifndef CONSTANT_H_
#define CONSTANT_H_
typedef struct {
  unsigned int ostacolo:1;
  unsigned int giocatore:1;
  unsigned int pacco:1;
  unsigned int locazione:1;
  unsigned int codiceGiocatore:3;
  unsigned int codicePacco:4;
  unsigned int codiceLocazione:4;
  unsigned int p1:1;
  unsigned int p2:1;
  unsigned int p3:1;
  unsigned int p4:1;
  unsigned int p5:1;
  unsigned int p6:1;
  unsigned int p7:1;
  unsigned int p8:1;
} GameGrid;

#define USER_LOG_OUT  "UTENTE ANDATO"
#define MAX_SIZE_USERNAME 15
#define MAX_SIZE_PASSW 12
#define USERS_FILE "File/RegisteredUser.txt"
#define MIO_SOCK  "/tmp/mio_socket"
#define clear() sleep(1); printf("\033[H\033[J");
#define leggi() n_b_r=read(sockfd,msg,5000);msg[n_b_r]='\0';printf("%s",msg);

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
#define MAX_GRID_SIZE_L 12
#define MAX_GRID_SIZE_H 8
/*Le costanti seguenti contengono i valori degli errori
 *
 */
#define NO_CONNECTION -1
#define INVALID_USERNAME -2
#endif
