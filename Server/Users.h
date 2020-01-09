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
#include "log.h"
#include "../costanti.h"
#include "../codicierrori.h"
#include "../messaggi.h"

int registerUser(char* newuser, char* newpassw);//Prende un nome utente, una password e,se non presente, lo aggiunge al file degli utenti; Ritorna 0 in caso di successo

int checkUsername(char* username); // Ritorna la posizione dell'offset dello username all'interno del file se presente, -1 altrimenti

int logInUser(char* user, char* passw);//Effettua il login dell'utente passato come paremetro usando la password passata come parametro

int logInUserMenu(int, char[]); //Contiene la gestione del menu di Login

int signInUserMenu(int, char[]);//Contiene la gestione del menu di Registrazione
