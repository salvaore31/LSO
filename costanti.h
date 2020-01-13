#ifndef CONSTANT_H_
#define CONSTANT_H_

#define MAX_PACCHI 16
#define MIN_OBSTACLES 5
#define MAX_SIZE_USERNAME 15
#define MAX_SIZE_PASSW 12
#define USERS_FILE "File/RegisteredUser.txt"
#define SOCKET  8080
#define MAXIMUM_SOCKET_BACKLOG 3
#define clear() sleep(1); printf("\033[H\033[J");
#define leggi() n_b_r=read(sockfd,msg,6);msg[n_b_r]='\0';n_b_r=atoi(msg);write(sockfd,"1",strlen("1")) ;n_b_r=read(sockfd,msg,n_b_r);msg[n_b_r]='\0';printf("%s",msg);


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
#define MAX_OBSTACLES_N 20
#define MAX_GRID_SIZE_L 15
#define MAX_GRID_SIZE_H 10

/*
  La struttura seguente contiene i dati utili per la gestione di un giocatore all'interno di una Partita:
    -(posx, posy) rappresentano la sua posizione all'interno della griglia di gioco;
    -pacco è un valore booleano che rappresenta se il giocatore ha o meno un pacco in carico;
    -codicePacco è un intero a quattro bit, se pacco è impostato ad 1 rappresenta l'identificativo del pacco stesso;
    -nome è una stringa contente l'username del giocatore;
*/
typedef struct {
  int posx;
  int posy;
  unsigned int pacco:1;
  unsigned int codicePacco:4;
  char nome[MAX_SIZE_USERNAME];
} player;

/*
  La struttura GameGrid rappresenta la singola cella della matrice di gioco, contiene 23 bit suddivisi in vari campi:
    - ostacolo, 1 bit, è un flag booleano. Vale 1 se la cella contiene un ostacolo, 0 altrimenti;
    - giocatore, 1 bit, è un flag booleano. Vale 1 se la cella contiene un giocatore, 0 altrimenti;
    - pacco, 1 bit, è un flag booleano. Vale 1 se la cella contiene un pacco, 0 altrimetni;
    - locazione, 1 bit, è un flag booleano. Vale 1 se la cella contiene una locazione di consegna, 0 altrimenti;
    - codiceGiocatore, 3 bit, è un intero. Contiene un numero da 0 a 7, identificativo di uno specifico giocatore. Se il flag giocatore
        è settato ad 1 viene utilizzato per rappresentare uno specifico giocatore sulla matrice.
    - codicePacco, 4 bit, è un intero. Viene utilizzato per rappresentare uno dei 16 pacchi disponibili all'interno della partita;
    - codiceLocazione, 4 bit, è un intero. Viene utilizzato per rappresentare una delle 16 locazioni disponibili. Contiene lo stesso valore
        del pacco associato a quella locazione;
    - I restanti 8 bit vengono usati come codifica one hot della visibilità. Ogni qual volta un giocatore scopre una casella il bit corrispondente
        al suo codiceGiocatore(quindi p0 per il giocatore con codiceGiocatore 0 etc etc) viene settato ad 1.
*/
typedef struct {
  unsigned int ostacolo:1;
  unsigned int giocatore:1;
  unsigned int pacco:1;
  unsigned int locazione:1;
  unsigned int codiceGiocatore:3;
  unsigned int codicePacco:4;
  unsigned int codiceLocazione:4;
  unsigned int p0:1;
  unsigned int p1:1;
  unsigned int p2:1;
  unsigned int p3:1;
  unsigned int p4:1;
  unsigned int p5:1;
  unsigned int p6:1;
  unsigned int p7:1;
} GameGrid;

/*
  La struttura Game contiene le informazioni necessarie a gestire una partita:
    -gameId, un intero contiene l'id della partita corrispondente al pid del processo che l'ha in gestione;
    -grid, contiene la matrice di gioco;
    -giocatori, un array di struct player contente le informazioni sui giocatori della partita. L'id del giocatore corrisponde alla
      sua indicizzazione nell'array;
    -piena, un flag booleano. Vale 1 se alla partita non si possono aggiungere altri giocatori, 0 altrimenti;
    -sem, un semaforo associato alla partita, serve a far sì che solo un client per volta possa accedere alla matrice di gioco;
    -punteggio, un array di interi. Ad ogni casella e associato un giocatore tramite il suo id e contiente il numero di pacchi consegnati
      per ogni giocatore;
    -i due array di interi locazioneXPacchi e locazioneYPacchi contengono informazioni su dove ogni singolo pacco vada consegnato.
*/
typedef struct {
  int gameId;
  GameGrid **grid;
  player giocatori[MAX_PLAYER_N];
  unsigned int piena:1;
  pthread_mutex_t sem;
  int punteggio[MAX_PLAYER_N];
  int locazioneXPacchi[MAX_PACCHI];
  int locazioneYPacchi[MAX_PACCHI];
} Game;


#endif
