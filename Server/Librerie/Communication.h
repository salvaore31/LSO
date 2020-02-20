/*
  La libreria conterrà tutte le funzioni utili a gestire la comunicazione tra i Client e il Server.
*/

#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"

/*
  La funzione crea una socket tcp sulla porta passata come parametro.
  Parametri:
    int porta, un intero rappresentante la porta su cui verrà creata la socket tcp;
  Valori di Ritorno:
    int sock, un intero corrispondente al fd della socket appena creata;
    ERR_SOCKET_CREATION, un codice d'errore per il fallimento della funzione di libreria socket;
    ERR_SOCKET_BINDING, un codice d'errore per il fallimento della funzione di libreria bind;
*/
int creaSocket(int porta);

/*
  La funzione GameGridToText trasforma la struttura GameGrid associata alla partita in una stringa
  che potrà poi essere comunicata al Client. Sfrutta la funzione printCellForPlayer per stampare
  la cella per il giocatore specificato dall'id.
  Parametri:
    GameGrid ** p, la matrice da trasformare;
    char msg[], il buffer su cui sarà scritta la matrice trasformata;
    int giocatore, l'id del giocatore specifico,
    player* gamer, il puntatore alla struttura player corrispondente al giocatore.
*/
int GameGridToText(GameGrid **p, char msg[], int giocatore, player* gamer);

/*
  La funzione che si occupa di comunicare con il Client nel caso il Server necessiti di una
  risposta per continuare l'esecuzione.
  Parametri:
    int sockfd, il fd della socket di comunicazione;
    char toSend[], la stringa da scrivere sulla porta;
    char received[], la risposta del Client.
  Valori di ritorna:
    int n_b_r, corrispondente al numero di bit letti dal server;
    ERR_SENDING_MESSAGE, codice di errore corrispondente alla mancata scrittura sulla socket;
    -1, se il messaggio inviato dal Client corrispondeva ad uno dei messaggi di log out, cioé
        USER_LOG_OUT e CLIENT_GONE.
*/
int sendMsg(int , char [], char []);

/*
  La funzione che si occupa di comunicare con il Client nel caso il Server NON necessiti di
  una risposta per continuare l'esecuzione.
  Parametri:
    int sockfd, il fd della socket di comunicazione;
    char toSend[], la stringa da scrivere sulla porta;
  Valori di ritorna:
    int n_b_w, corrispondente al numero di bit scritti dal server;
    ERR_SENDING_MESSAGE, codice di errore corrispondente alla mancata scrittura sulla socket;
    ERR_RECEIVING_MESSAGE, codice di errore corrispondente alla mancata lettura sulla socket.
*/
int sendMsgNoReply(int, char []);

/*
  La funzione printCellForPlayer inserisce all'interno del messaggio da inviare al Client una traduzione
  simbolica del contenuto della cella, in accordo con i permessi del giocatore specifico.
  Parametri:
    GameGrid ** p, la matrice da trasformare;
    char msg[], il buffer su cui sarà scritta la matrice trasformata;
    int giocatore, l'id del giocatore specifico,
    player* gamer, il puntatore alla struttura player corrispondente al giocatore.
    int i, j, specificano la cella della matrice.
*/
void printCellForPlayer(GameGrid **p, char msg[], int giocatore, player * gamer, int i, int j);
