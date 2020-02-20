#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"

/*
  La libreria si occupa di gestire il logging degli eventi. Come tale ogni funzione in essa contenuta
  riceve come parametro il fd del file di Log relativo alla specifica sessione Server e le informazioni
  necesssarie al logging dell'evento specifico. I file di Log si trovano nella directory File/Log/<nome_file_log>
*/

/*
  La funzione LogServerStart logga l'evento dell'inizio della sessione Server, si occupa anche di creare il file
  di Log stesso.
*/
void LogServerStart(int* fdLog);

/*
  La funzione LogServerClose logga l'evento di fine sessione Server.
*/
void LogServerClose(int* fdLog);

/*
  La funzione LogNewGame logga l'evento di inzio nuova partita.
  Parametri:
    int gameId, l'id della partita specifica.
*/
void LogNewGame(int* fdLog, int gameId);

/*
  La funzione LogEndGame logga l'evento di fine partita.
  Parametri:
    int gameId, l'id della partita.
*/
void LogEndGame(int* fdLog, int gameId);

/*
  La funzione LogPlayerJoin logga l'evento giocatore si unisce a partita.
  Parametri:
    int gameId, l'id della partita;
    char * player, il nickname del giocatore;
*/
void LogPlayerJoin(int* fdLog, int gameId, char* player);

/*
  La funzione LogPlayerMoves logga l'evento giocatore va da casella (x,y) a casella (x',y').
  Parametri:
    int gameId, l'id della partita;
    char * player, il nickname del giocatore;
    char * src, rappresentazione testuale della casella di origine;
    char * dst, rappresentazione testutale della casella di destinazione.
*/
void LogPlayerMoves(int* fdLog, int gameId, char* player, char* src , char* dst);

/*
  La funzione LogPlayerWin logga l'evento giocatore x vince partita.
  Parametri:
    int gameId, l'id della partita;
    char * player, il nickname del giocatore;
*/
void LogPlayerWin(int* fdLog, int gameId, char* player);

/*
  La funzione LogNewUSer logga l'evento giocatore x si è registrato sul server:
  Parametri:
    int gameId, l'id della partita;
    char * player, il nickname del giocatore;
*/
void LogNewUser(int* fdLog, char* player);

/*
  La funzione LogPlayerTakePackage logga l'evento giocatore raccoglie pacco.
  Parametri:
    int gameId, l'id della partita;
    char * player, il nickname del giocatore;
    int pacco, l'id dell pacco specifico;
    cher * loc, la casella da cui si è raccolto il pacco.
*/
void LogPlayerTakePackage(int* fdLog,int gameId, char* player, int pacco, char* loc);

/*
  La funzione LogPlayerLeavePackage logga l'evento giocatore lascia pacco.
  Parametri:
    int gameId, l'id della partita;
    char * player, il nickname del giocatore;
    int pacco, l'id dell pacco specifico;
    cher * loc, la casella su cui si è lasciato il pacco.
*/
void LogPlayerLeavePackage(int *fdLog, int gameId, char* player, int pacco, char* loc);

/*
  La funzione LogPlayerMakeAPoint logga l'evento giocatore fa un punto:
  Parametri:
    int gameId, l'id della partita;
    char * player, il nickname del giocatore.
*/
void LogPlayerMakeAPoint(int *fdLog, int gameId, char* player);

/*
  La funzione LogUserSignIn logga l'evento utente x si è connesso al Server:
  Parametri:
    int gameId, l'id della partita;
    char * player, il nickname del giocatore.
*/
void LogUserSignIn(int* fdLog, char* player);

/*
  La funzione LogUserSignIn logga l'evento utente x si è disconesso Server:
  Parametri:
    int gameId, l'id della partita;
    char * player, il nickname del giocatore.
*/
void LogUserSignOut(int* fdLog, char* player);

/*
  La funzione LogErrorMessage logga un eventuale messaggio d'errore:
  Parametri:
    int gameId, l'id della partita;
    char * err, il messaggio d'errore da loggare.
*/
void LogErrorMessage(int* fdLog, char* err);

/*
  La funzione LogUnknownClientDisconnection logga l'evento un Client non identificato da un nickname
  si è disconesso dal Server.
  Parametri:
    char addr*, l'indirizzo IP del client che si è appena disconesso.
*/
void LogUnknownClientDisconnection (int*fdLog, char* addr);
