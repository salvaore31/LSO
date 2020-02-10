#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"
#include "Communication.h"
#include "Users.h"

/*
  La funzione modifica la griglia di gioco in accordo con le mosse del giocatore.
  Parametri:
    Game game, è un puntatore alla partita specifica;
    int giocatore, contiente l'identificativo del giocatore specifico;
    char action, contiene l'identificativo testuale della mossa:
      action può assumere i seguenti valori: {'W','A','S','D', 'Q', 'E', '0', 'R'};
    int gameId, contiene l'id della partita specifica.
    int* fdLog, è il file descriptor del file di log specifico della sessione server in corso.
  Valori di Ritorno:
    -1, se la mossa descritta da action non era possibile (es. spostamento al di fuori della matrice di gioco);
    PLAYER_EXITS, un valore costante definito in costanti.h se la mossa specificata corrispondeva ad uscita giocatore;
*/
int azioneGiocatore(Game *game, int giocatore, char action, int gameId, int * fdLog);

/*
  Setta i permessi per la visibilità di una casella in accordo con le mosse di un giocatore.
  Parametri:
    int x, posizione x della cella all'interno della matrice;
    int y, posizione y della cella all'interno della matrice;
    int giocatore, identificativo del giocatore per cui settare i permessi;
    GameGrid ** grid, la matrice di gioco;
*/
void setPermessi(int x, int y, int giocatore, GameGrid ** grid);

/*
  Setta i permessi per la visibilità di una casella per ogni giocatore.
  Parametri:
    int x, posizione x della cella all'interno della matrice;
    int y, posizione y della cella all'interno della matrice;
    GameGrid ** grid, la matrice di gioco;
*/
void setPermessiToAll(int x, int y, GameGrid ** grid);

/*
  Controlla se la partita corrente deve terminare. Ritorna 0 se la partita deve continuare, 1 altrimenti.
  Parametri:
    Game * game, il puntatore alla partita corrente;
    int idGiocatore, l'id del giocatore corrente.
  Valori di Ritorno:
    1, se il giocatore specificato da idGiocatore ha consegnato la metà dei pacchi più uno O
       se il numero totali di pacchi consegnati corrisponde al numero totale di pacchi disponibili;
    0, se le condizioni sopracitate non avvengono;
*/
int gameHasToEnd(Game *game, int idGiocatore);


/*
  Crea la matrice di gioco associata ad una partita.
  Parametri:
    Game * g, il puntatore alla partita corrente;
  Valori di Ritorno:
    1, se la creazione ha avuto successo;
    0, altrimenti;
*/
int createGameGrid(Game *g);

/*
  Al momento della disconessione di un Client controlla che la partita corrente contiene almeno
  un giocatore.
  Parametri:
    Game* g, il puntatore alla partita corrente;
  Valori di Ritorno:
   1, se la partita corrente è vuota;
   0, altrimenti;
*/
int isGameEmpty(Game* game);

int didIWin(Game*, int);

Game * createGame();

int playGame(Game * game, int idGiocatore, int gameId,int sockfd, LogFile* serverLog);

void spawnNewPlayer(Game ** , char* username, int , LogFile* ,loggedUser*);

void deleteGrid(GameGrid **g);

void initializaNewGame(Game**,int sockfd, char user[],LogFile*,loggedUser*);
