#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"
#include "Log.h"
#include "Communication.h"

/*
  La libereria contiene le funzioni utili a gestire registrazione/login degli utenti. Per tal motivo tutte le funzioni ricevono un puntatore ad
  una struttura di tipo loggedUser, che contiene le informazione sugli utenti correntemente collegati al Server.
*/

/*
  La funzione registerUser aggiunge un nuovo utente al file RegisteredUser.txt.
  Al termine della registrazione l'utente viene considerato loggato e aggiunto, tramite la funzione insertLoggedUser tra
  gli utenti correntemente connessi al Server,
  Parametri:
    char* newuser, il nome utente da aggiungere;
    char* newpassw, la password del nuovo utente;
  Valori di Ritorno:
    ERR_INVALID_USERNAME, nel caso il nome utente non sia valido (lunghezza> MAX_SIZE_USERNAME);
    ERR_NO_USER_FILE, nel caso sia fallita l'apertura del file RegisteredUser.txt;
    ERR_INPUT_OUTPUT, nel caso sia fallita la scrittura su RegisteredUser.txt;
    ERR_INSERT_LOGGED_USER, nel caso sia fallita la funzione insertLoggedUser.
*/
int registerUser(char* newuser, char* newpassw, loggedUser*);

/*
  La funzione checkUsername controlla se lo username passato come parametro sia presente all'interno del file. Al suo interno richiama
  la funzione isLogged per controllare che l'utente non sia già loggato sul Server. Se la ricerca è andata a buon fine ritorna l'offset
  del nome utente cercato.
  Parametri:
    char* username, il nome utente da cercare;.
  Valori di Ritorno:
    int res, l'offset del nome utente cercato all'interno del file;
    ERR_NO_USER_FILE, nel caso sia fallita l'apertura del file RegisteredUser.txt;
    ERR_USER_ALREADY_LOGGED, nel caso l'utente specificato sia già loggato;
    ERR_USERNAME_NOT_FOUND, nel caso l'utente specificato non sia stato trovato.
*/
int checkUsername(char* username, loggedUser*);

/*
  La funzione logInUser autentica un utente passato come parametro tramite la password passata come parametro.Se l'autenticazione è andata a
  a buon fine inserisce l'utente all'interno della struttura loggedUser. Richiama al suo interno la funzione checkUsername per controllare se il nome utente è effettivamente presente all'interno
  del file RegisteredUser.txt. L'effettivo inserimento dell'utente tra quelli loggati è affidato alla funzione insertLoggedUser.
  Parametri:
    char* user, il nome utente da autenticare;
    char* passw, la password da usare per l'autenticazione;
  Valori di Ritorno:
    ERR_NO_USER_FILE, nel caso sia fallita l'apertura del file RegisteredUser.txt;
    ERR_INPUT_OUTPUT, nel caso sia fallita la funzione di lseek;
    ERR_INSERT_LOGGED_USER, nel caso sia fallito l'inserimento nella struttura di tipo loggedUser;
    ERR_WRONG_PASSWORD, nel caso la password registrata non corrisponda a quella passata come parametro;
    0, nel caso il login sia andato a buon fine;
    -1, negli altri casi d'errore.
 */
int logInUser(char* user, char* passw, loggedUser*);

/*
  La funzione gestisce il flusso d'interazione con l'utente durante il login. Invia un prompt per l'inserimento dell'username e, dopo aver effettuato controlli sullo stesso
  chiede all'utente la propria password. Se la coppia username, password è valida invia un messaggio di successo di login al client per portare avanti l'interazione.
  Parametri:
    int sockfd, la socket su cui scrivere;
    char usrn[], il buffer per il nome utente;
    LogFile* log, la struttura di log;
    char addrClient[], l'indirizzo ip del Client specifico.
  Valori di Ritono:
    ERR_NO_USER_FILE, nel caso sia fallita l'apertura del file RegisteredUser.txt;
    ERR_USER_ALREADY_LOGGED, nel caso l'utente specificato sia già loggato;
    ERR_INPUT_OUTPUT, nel caso sia fallita la funzione di lseek;
    ERR_WRONG_PASSWORD, nel caso la password registrata non corrisponda a quella passata come parametro;
    1, se l'utente è stato correttamente autenticato.
 */
int logInUserMenu(int sockfd, char usrn[], LogFile* log, loggedUser* loggati, char addrClient[]);

/*
  La funzione controlla il flusso dell'interazione con l'utente durante la registrazione. Invia un prompt per l'inserimento dello username e, se questo è valido, chiede
  di inserire una password. Dopo di che chiama la funzione registerUser sui dati inseriti.
  Parametri:
    int sockfd, la socket su cui scrivere;
    char usrn[], il buffer per il nome utente;
    LogFile* log, la struttura di log;
    char addrClient[], l'indirizzo ip del Client specifico.
  Valori di Ritorno:
    1, se la funzione registerUser è andata a buon fine;
    ERR_NO_USER_FILE, nel caso sia fallita l'apertura del file RegisteredUser.txt;
    ERR_INPUT_OUTPUT, nel caso sia fallita la funzione di lseek;
    ERR_INVALID_USERNAME, nel caso il nome utente non sia valido.
 */
int signInUserMenu(int sockfd, char usrn[], LogFile*log, loggedUser*loggati,char addrClient[]);

/*
  La funzione controlla se l'utente passato come parametro è tra quelli loggati al Server correntemente.
  Parametri:
    char user[], il nome utente da controllare;
  Valori di Ritono;
    1, se l'utente è loggato;
    0, altrimenti.
 */
int isLogged(char user[], loggedUser*);

/*
  La funzione inserisce l'utente passato come parametro tra quelli correntemente loggati al Server.
  Parametri:
    char user[], il nome utente da inserire;
  Valori di Ritorno:
    1, se l'inserimento è andato a buon fine;
    0, altrimenti.
 */
int insertLoggedUser(char user[], loggedUser*);

/*
  La funzione cancella l'utente passato come parametro tra quelli correntemente loggati al Server.
  Parametri:
    char user[], il nome utente da cancellare;
  Valori di Ritorno:
    1, se la cancellazione è andata a buon fine;
    0, altrimenti.
 */
int deleteLoggedUser(char [], loggedUser*);

/*
  La funzione alloca la memoria necessaria alla struttura globale loggati di tipo loggedUser*.
  Parametri:
    loggedUser* loggati, il puntatore alla struttura globale.
 */
void initializaLoggedUser(loggedUser* loggati);
