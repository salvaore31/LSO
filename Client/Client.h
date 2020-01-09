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
#include "../costanti.h"
#include "../messaggi.h"
void printGameGrid(GameGrid grid);

int logInRequest(char* username, char * password);

int signInRequest(char* username, char* password);

int comunication(int);

int newGame();

int joinGame();

int movementRequest();

void handleSignal(int);

int selectGame(int);

int goOn(char []);

int goOut(char []);
