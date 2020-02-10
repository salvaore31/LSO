#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include "../costanti.h"
#include "../messaggi.h"
#include "../codici_errori.h"
#include "Librerie/Log.c"
#include "Librerie/Gameplay.c"
#include "Librerie/Users.c"
#include "Librerie/Communication.c"

/*
  VARIABILI GLOBALI:
  LogFile serverLog, una struttura dati contenente il file di Log della specifica sesssione server e il mutex che ne regola l'accesso;
  Game *g, un puntatore alla partita in corso, inizialmente non allocata (definizione di Game in costanti.h);
  int gameId=-1, l'id della partita che verrà assegnato al momento dell'allocazione della stessa;
  loggedUser loggati, una struttura dati cotenente i giocatori correntemente loggati e il mutex che ne regola l'accesso;
  pthread_t clientsId[MAX_PLAYER_N], un array contentene il tid dei thread giocanti utilizzato per gestire il fine partita;
  int gameEnded = 0, un valore utilizzato per indicare al Server quando si trova all'interno di una partita;
  */


/*
  La funzione handleSignal viene utilizzata per la gestione dei segnali. In particolare il Server gestisce in maniera particolare
  i segnali di SIGINT e SIGALRM.
  Il segnale di SIGINT viene utilizzato solo per loggare l'evento di chiusura sessione server, mentre il segnale di SIGALRM viene usato
  per gestire il fine partita. Si imposta SIGALRM per essere lanciato quando si è raggiunto il tempo massimo di gioco oppure il segnale viene lanciato,
  in vari punti del codice, quando si raggiunge un altra condizione di terminazione. Raccogliere SIGALRM si traduce poi nel cambio del bit timeOver della struttura
  Game da 0 ad 1 e, di conseguenza, nell'invio di un segnale di terminazione partita ai thread giocanti.
  Parametri:
    Sig, un intero che corrisponde al segnale da gestire;
  */
void handleSignal(int Sig);

/*
  La funzione lanciata da ogni nuovo thread che si occupa di gestire la comunicazione con il Client. Ad ogni Client viene associato un thread.
  Parametri:
    arg[0], un intero che rappresenta il file descriptor della socket TCP adibita alla comunicazione;
  */
void * run(void *arg);

/*
  La funzione che si occupa di gestire il fine partita, viene lanciata da handleSignal ogniqualvolta viene raccolto il segnale di SIGALRM. Aspetta la terminazione
  dei vari thread giocanti e dealloca la partita. Al termine di endGameManagement il valore intero gameEnded viene reimpostato ad 1.
  */
void * endGameManagement(void *arg);
