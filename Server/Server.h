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
#include <signal.h>
#include <sys/mman.h>
#include "../costanti.h"
#include "../messaggi.h"
#include "../codici_errori.h"
#include "Librerie/Log.c"
#include "Librerie/Gameplay.c"
#include "Librerie/Users.c"
#include "Librerie/Communication.c"
#include "Librerie/GameArrayManagement.c"

Game* joinGame(int sockfd, char user[], int fdLog);

void deleteGrid(GameGrid **g);

void handleSignal(int Sig);

void * gestisci(void *arg);

void initializeNewGameProcess(int sockfd, char user[]);
