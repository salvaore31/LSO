#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"
#include "Log.h"

//Prende un nome utente, una password e,se non presente, lo aggiunge al file degli utenti; Ritorna 0 in caso di successo
int registerUser(char* newuser, char* newpassw);

// Ritorna la posizione dell'offset dello username all'interno del file se presente, -1 altrimenti
int checkUsername(char* username);

//Effettua il login dell'utente passato come paremetro usando la password passata come parametro
int logInUser(char* user, char* passw);

//Contiene la gestione del menu di Login
int logInUserMenu(int, char[], int*);

//Contiene la gestione del menu di Registrazione
int signInUserMenu(int, char[], int*);
