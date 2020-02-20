#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"
#include "Communication.h"
#include "Users.h"

/*La libreria contiene tutte le funzioni utili a gestire una partita.*/

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

/*
  La funzion controlla se il giocatore specificato abbia vinto la partita, controllando se il
  suo id corrisponde al valore massimo presente all'interno dell'array Punteggio contenuto in game.
  Parametri:
    Game * game, il puntatore alla partita corrente;
    int idGiocatore, l'id del giocatore corrente.
  Valori di ritorno:
    1, se il giocatore corrente ha vinto;
    0, altrimenti.
*/
int didIWin(Game* g, int idGiocatore);

/*
  La funzione alloca una partita e inizializza il semaforo contenuto in Game.
  Valori di Ritorno:
    un punatore ad una struttura game se l'allocazione è andata a buon fine;
    NULL altriment.
*/
Game * createGame();

/*
  La funzione playGame consente al giocatore di giocare alla partita corrente. Preso un giocatore ed una partita
  si mette in ascolto dei messaggi del Client e richiama la funzione azioneGiocatore su quel giocatore per calcolare
  il risultato della mossa indicata dal client. Dopo di ché invia la matrice aggiornata al Client. Esegue inoltre i controlli
  necessari a verificare la fine della partita. Se avviene il fine partita PlayGame si occupa anche di inviare ai vari client
  giocanti i messaggi di vittoria e sconfitta.
  Parametri:
    Game* game, il puntatore alla partita corrente;
    int idGiocatore, l'id del giocatore corrente;
    int gameId, l'id del giocatore corrente;
    int sockfd, il fd della socket di comunicazione;
    LogFile* serverLog, il puntatore alla struttura che contiene il fd del Log e il relativo mutex.
  Valori di Ritono:
    PLAYER_EXITS, un valore costante associato all'evente 'giocatore esce dalla partita';
    GAME_END_FOR_TIME, un valore costante rappresentante l'evento 'fine tempo massimo di gioco'.
*/
int playGame(Game * game, int idGiocatore, int gameId,int sockfd, LogFile* serverLog);

/*
  La funzione spawnNewPlayer si occupa di gestire l'evento giocatore si unisce a partita in corso.
  Inserisce un giocatore all'interno della matrice di gioco e gli assegna un id. Dopodichè popola
  la corrispettiva cella dell'array di player in game. Dopo aver fatto ciò richiama la funzione PlayGame per
  quella partita e quel giocatore.
  Se la funzione playGame ritorna PLAYER_EXITS il server logga l'evento d'uscita
  giocatore ed elimina il giocatore dalla struttura che gestisce i giocatori logGame * game, il puntatore alla partita corrente;
    int idGiocatore, l'id del giocatore corrente.gati. Dopo aver chiuso la socket
  controlla se la partita contiene ancora almeno un giocatore, se ciò non avviene lancia un segnale di SIGALRM.
  In entrambi casi a quel punto l'esecuzione del thread viene terminata.
  Se playGame ritorna GAME_END_FOR_TIME allora si logga l'evento d'uscita, si cancella l'utente dall'elenco di giocatori loggati,
  si chiude la socket e si termina l'esecuzione del thread.
  Parametri:
    Game ** game, un puntatore a puntatore alla partita corrente;
    char* username, una stringa contenente il nickname dell'utente;
    int sockfd, il fd della socket di comunicazione;
    LogFile* serverLog, il puntatore alla struttura che contiene il fd del Log e il relativo mutex,
    loggedUSer* loggati, il puntatore alla struttura che contiene i giocatori correntemente loggati.
*/
void spawnNewPlayer(Game ** game , char* username, int sockfd, LogFile* serverLog, loggedUser* loggati);

/*
  La funzione dealloca la matrice di gioco associata alla partita.
  Parametri:
    GameGrid ** g, un puntatore a puntatore alla matrice associata alla partita.
*/
void deleteGrid(GameGrid **g);

/*
  La funzione crea una nuova partita. Sfrutta la funzione createGame.
  Dopo aver creato la partita e il giocatore che, essendo il primo, riceve id 0 chiama la funzione playGame.
  Il suo compartamento di gestione dei valori di ritorno di playGame (PLAYER_EXITS e GAME_END_FOR_TIME) è perfettamente
  uguale a quello della funzione spawnNewPlayer descritto sopra.
  Parametri:
    Game ** game, un puntatore a puntatore alla partita corrente;
    char user[], una stringa contenente il nickname dell'utente;
    int sockfd, il fd della socket di comunicazione;
    LogFile* toLog, il puntatore alla struttura che contiene il fd del Log e il relativo mutex,
    loggedUSer* loggati, il puntatore alla struttura che contiene i giocatori correntemente loggati.
*/
void initializaNewGame(Game** game,int sockfd, char user[],LogFile* toLog,loggedUser* loggati);
