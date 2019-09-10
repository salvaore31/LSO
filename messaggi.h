#ifndef STRINGHE_H_
#define STRINGHE_H_

/*
 * Stringhe per la generazione dei menu.
 */
#define WELCOME_MESSAGE       "Benvenuto in Progetto Laboratorio di Sistemi Operativi\nCosa vuoi fare?\nPremi 'l' per eseguire il login.\nPremi 'r' per registrarti.\nPremi 'e' per uscire."
#define GAME_SELECTION_MENU   "Cosa vuoi fare?\nPremi 'n' per iniziare una nuova partita.\nPremi 'j' per unirti ad una partita in corso.\n"
#define GAMES_PLAYED          "Partita %i.\nGiocatori: [...]\n"
#define SELECT_FROM_CURRENT   "Inserisci il numero della partita a cui vuoi aggiungerti.\n"
#define SELECT_DIRECTION      "Inserisci:\n\t'w' per muoverti verso NORD;\n\t'a' per muoverti verso OVEST;\n\t's' per muoverti verso SUD\n\t'd' per muoverti verso EST"
#define LOG_IN_MENU           "Inserisci il tuo nome utente:\n"
/*
 * Messaggi relativi al gioco.
 */
#define VICTORY_MESSAGE       "To the victor the spoils!\n"
#define OBSTACLE_MESSAGE      "You choose poorly.\n"
#define COLLISION_MESSAGE     "Vedi che è occupata.\n"
#define TIME_OVER_MESSAGE     "Il gioco finisce e nessuno vince.\n"
#define LOSS_MESSAGE          "... Sarà per un'altra volta.\n"

/*
 * Messaggi di errore per gestire problemi sul login/signin
 */
#define ERR_NO_SUCH_USER      "L'username inserito non corrisponde a nessun utente.\n"
#define ERR_ALREADY_PRESENT   "L'username inserito è già associato ad un utente.\n"
#define ERR_WRONG_PASSWORD    "La password inserita non è corretta.\n"

/*
 * Messaggi di errore relativi al server.
 */
#define ERR_NO_CONNECTION     "Impossibile connettersi al server.\n"
#define ERR_SOCKET_CREATION   "Impossibile creare la socket.\n"
#define ERR_SOCKET_BINDING    "Impossibile collegarsi alla socket.\n"
#define ERR_SOCKET_LISTEN     "Impossibile ricevere dati dalla socket.\n"

/*
 * Messaggi di errore relativi al gioco stesso
 */
#define ERR_FILLED_GAME       "Non puoi unirti a questa partita...\n"
#define ERR_MATRIX_ALLOCATION "Errore nella creazione della matrice di gioco.\n"
#define ERR_ON_MOVEMENT       "Errore nel movimento del giocatore.\n"

/*
 * Messaggi di errore lato client.
 */
#define ERR_INVALID_CHOICE    "La scelta inserita non è valida.\n"

/*
 * Messaggi di errore relativi ai thread.
 */
#define ERR_THREAD_CREATION   "Impossibile creare un nuovo thread.\n"
/*
 *Messaggi di LOG
 */
#define LOG_START_SERVER      ": iniziata nuova sessione Server.\n"
#define LOG_CLOSE_SERVER      ": finita sessione Server.\n"
#define LOG_NEW_GAME          ": iniziata nuova partita con ID "
#define LOG_END_GAME          ": finita partita con ID "
#define LOG_NEW_PLAYER_JOIN1  ": l'utente "
#define LOG_NEW_PLAYER_JOIN2  " si è unito alla partita con ID "
#define LOG_PLAYER_MOVES1     ": in partita con ID "
#define LOG_PLAYER_MOVES2     " è andato da "
#define LOG_PLAYER_WINS1      ": l'utente "
#define LOG_PLAYER_WINS2      " ha vinto la partita con ID "
#define LOG_NEW_USER          " si è unito al Server.\n"
#define LOG_SIGN_IN           " si è connesso al Server.\n"
#define LOG_SIGN_OUT          " si è disconesso dal Server.\n"
#define LOG_ERROR             ": ERRORE( "

#endif
