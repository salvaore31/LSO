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


int createNewGame(GameGrid grid);//Crea un nuovo processo che inzializza una partita

int addPlayer(char *user, char** CurrentPlayer);//aggiunge un giocatore alla partita e aggiorna l'array di stringhe dei giocatori correnti

void endgame();

void readPlayerMovement();

int countdown();

void handleSignal(int Sig);//Handler dei segnali da gestire

GameGrid **createGameGrid(GameGrid **);

int GameGridToText(GameGrid **, char [], int);

void joinGame(int, char[]);

void newGame(int, char[]);

void deleteGrid(GameGrid **);
