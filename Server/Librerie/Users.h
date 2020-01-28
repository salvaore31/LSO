#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"
#include "Log.h"
#include "Communication.h"
//Prende un nome utente, una password e,se non presente, lo aggiunge al file degli utenti; Ritorna 0 in caso di successo
int registerUser(char* newuser, char* newpassw, loggedUser*);

// Ritorna la posizione dell'offset dello username all'interno del file se presente, -1 altrimenti
int checkUsername(char* username, loggedUser*);

//Effettua il login dell'utente passato come paremetro usando la password passata come parametro
int logInUser(char* user, char* passw, loggedUser*);

//Contiene la gestione del menu di Login
int logInUserMenu(int, char[], LogFile*, loggedUser*);

//Contiene la gestione del menu di Registrazione
int signInUserMenu(int, char[], LogFile*, loggedUser*);

int isLogged(char [], loggedUser*);

int insertLoggedUser(char [], loggedUser*);

int deleteLoggedUser(char [], loggedUser*);
