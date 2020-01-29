#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"
#include "Communication.h"
#include "Users.h"
/*
  La funzione modifica la griglia di gioco in accordo con le mosse del giocatore.
    Il parametro di tipo Game game è un puntatore alla partita specifica;
    Il parametro di tipo int giocatore contiente l'identificativo del giocatore specifico;
    Il parametro di tipo char action contiene l'identificativo testuale della mossa;
    Il parametro di tipo int gameId contiene l'id della partita specifica.
    Il parametro di tipo puntatore a intero è il file descriptor del file di log
      specifico della sessione server in corso.
*/
int azioneGiocatore(Game *game, int giocatore, char action, int gameId, int * fdLog);

/*
  Setta i permessi per la visibilità di una casella in accordo con le mosse di un giocatore.
*/
void setPermessi(int x, int y, int giocatore, GameGrid ** grid);

/*
  Setta i permessi per la visibilità di una casella per ogni giocatore.
*/
void setPermessiToAll(int x, int y, GameGrid ** grid);

int gameHasToEnd(Game *,int);

void * timer(void *arg);

/*
  Crea la matrice di gioco associata ad una partita, passata in input.
*/
int createGameGrid(Game *g);

int isGameEmpty(Game*);

int didIWin(Game*, int);

Game * createGame();

int playGame(Game * game, int idGiocatore, int gameId,int sockfd, LogFile* serverLog);

void spawnNewPlayer(Game ** , char* username, int , LogFile* ,loggedUser*);

void deleteGrid(GameGrid **g);

void initializaNewGame(Game**,int sockfd, char user[],LogFile*,loggedUser*);
