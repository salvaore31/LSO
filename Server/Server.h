#include "costanti.h"
typedef short int[][]

int registerUser(char* newuser, char* newpassw, int fdUserFile);//Prende un nome utente, una password e file descriptor del file degli utenti e se non presente lo aggiunge

int checkUsername(char* username, int fdUserFile); // Controlla che all' interno del file sia presente l'username passato

int logInUser(char* user, char* passw, int fdUserFile);//Effettua il login del utente passato

int createNewGame(GameGrid grid);//Crea un nuovo processo che inzializza una partita

int addPlayer(char *user, char** CurrentPlayer);//aggiunge un giocatore alla partita e aggiorna l'array di stringhe dei giocatori correnti

void endgame();

void readPlayerMovement();
