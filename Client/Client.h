#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "termios.h"
#include "../costanti.h"
#include "../messaggi.h"

/*
  La funzione comunication si occupa di leggere i messaggi inviati dal Server durante il login e la registrazione.
  Inoltre scrive le risposte del client sulla socket. Nel caso l'utente decida di uscire, premendo il tasto 'e' lancia un segnale di SIGINT.
  Il controllo sull'effettivo login è svolto dalla funzione goOn.
  Parametri:
    int sockfd, il fd della socket;
  Valori di Ritorno:
    1, se l'utente si è loggato con successo;
    -1, altrimenti.
 */
int comunication(int sockfd);

/*
  La funzione communicationGame si occupa di gestire la comunicazione con il Server durante la partita. Imposta il terminale in modalita no ECHO e
  disabilita ICANON, così che i caratteri inseriti vengano letti senza dover premere il carattere di newline. Al termine della partita, cioè quando
  l'utente decide d'uscire O la partita è terminata reimposta il terminale alle impostazioni standard.
  Parametri:
    int sockfd, il fd della socket;
 */
int comunicationGame(int sockfd);

/*
  La funzione di handleSignal gestisce l'evento di chiusura forzata del terminale. Nel caso l'utente sia loggato specifica al Server l'utente che si
  è appena disconesso, altrimenti, se l'utente è connesso ma non autenticato invia un messaggio al Server per far partire la gestione dell'evento
  'disconessione Client non autenticato'. In entrambi i casi chiude la socket di comunicazione e termina il processo.
  Parametri:
    int sig, il segnale da gestire.
 */
void handleSignal(int sig);

/*
  La funzione goOn riceve un messaggio come parametro e controlla se corrisponde ad un login o ad una registrazione avvenuta con successo.
  Parametri:
    char[], il messsaggio da controllare;
  Valori di Ritorno:
    1, se il login o la registrazione sono andati a buon fine;
    0, altrimenti;
 */
int goOn(char []);
