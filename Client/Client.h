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

int comunication(int);

int comunicationGame(int);

void handleSignal(int);

int goOn(char []);
