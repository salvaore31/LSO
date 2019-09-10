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
#include <string.h>
#include <signal.h>
#include "log.c"
#include "../costanti.h"


int registerUser(char* newuser, char* newpassw);//Prende un nome utente, una password e,se non presente, lo aggiunge al file degli utenti; Ritorna 0 in caso di successo

int checkUsername(char* username); // Ritorna la posizione delll'offset dello username all'interno del file se presente, -1 altrimenti

int logInUser(char* user, char* passw);//Effettua il login del utente passato

int createNewGame(GameGrid grid);//Crea un nuovo processo che inzializza una partita

int addPlayer(char *user, char** CurrentPlayer);//aggiunge un giocatore alla partita e aggiorna l'array di stringhe dei giocatori correnti

void endgame();

void readPlayerMovement();

int countdown();

void handleSignal(int Sig);//Handler dei segnali da gestire

int logInUserMenu(int);

int signInUserMenu(int);
