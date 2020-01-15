#ifndef MESSAGGI_H_
#define MESSAGGI_H_

/*
 * Stringhe per la generazione dei menu.
 */
#define WELCOME_MESSAGE       "Benvenuto in Progetto Laboratorio di Sistemi Operativi\nCosa vuoi fare?\nPremi 'l' per eseguire il login.\nPremi 'r' per registrarti.\nPremi 'e' per uscire.\n"
      /*
       * Stringhe per la generazione di schermate di Login.
       */
       #define INSERT_USERNAME_LIM           "Inserisci il tuo nome utente:\n"
       #define INSERT_PASSWORD_LIM           "Inserisci la tua password:\n"
       #define SUCCESS_MESSAGE_LIM           "Bentornato!\nPremi qualcosa per andare avanti...\n"
       #define WRONG_USERNAME_LIM            "Il nome utente inserito non è stato trovato.\nInserisci di nuovo il tuo nome utente:\n"
       #define WRONG_PASSWORD_LIM            "Password errata.\nInserisci di nuovo la tua password:\n"

      /*
       * Stringhe per la generazione della schermata di Sign In.
       */
       #define INSERT_USERNAME_SIM            "Come vuoi farti chiamare? (lunghezza massima 15)\n"
       #define INSERT_PASSWORD_SIM            "Inserisci la tua password (lunghezza massima 12).\n"
       #define INSERT_PASSWORD_CONFIRM_SIM    "Re-inserisci la password.\n"
       #define USER_ALREADY_PRESENT_SIM       "Il nome utente non è valido.\nScegli un altro nome, di lunghezza massima 15, con cui farti chiamare:\n"
       #define SUCCESS_MESSAGE_SIM            "Benvenuto!\nPremi qualcosa per andare avanti...\n"

#define GAME_SELECTION_MENU   "Cosa vuoi fare?\nPremi 'n' per iniziare una nuova partita.\nPremi 'j' per unirti ad una partita in corso.\nPremi 'e' per uscire.\n"
#define NO_SUCH_GAMES_MESSAGE        "Non ci sono partite disponibili in corso.\nPremi un tasto per continuare.\n"

//E' follia, follia dico però il messaggio non lo cancello perché l'ho scritto e mi scoccio troppo assai di cancellarlo
#define CURRENT_GAMES_PLAYED  "Partita %i.\nGiocatori: [...]\n"
#define SELECT_FROM_CURRENT   "Inserisci il numero della partita a cui vuoi aggiungerti.\n"
#define SELECT_DIRECTION      "Inserisci:\n\t'w' per muoverti verso NORD;\n\t'a' per muoverti verso OVEST;\n\t's' per muoverti verso SUD\n\t'd' per muoverti verso EST"

/*
 * Messaggi relativi al gioco.
 */
#define VICTORY_MESSAGE       "To the victor the spoils!\n"
#define OBSTACLE_MESSAGE      "You choose poorly.\n"
#define COLLISION_MESSAGE     "Vedi che è occupata.\n"
#define TIME_OVER_MESSAGE     "Il gioco finisce e nessuno vince.\n"
#define LOSS_MESSAGE          "... Sarà per un'altra volta.\n"
#define USER_LOG_OUT "No Utente\n"

/*
 * Messaggi di errore relativi al server.
 */
#define NO_CONNECTION_ERR_MESSAGE     "Impossibile connettersi al server.\nL'applicazione verrà chiusa"
#define SOCKET_CREATION_ERR_MESSAGE   "Impossibile creare la socket."
#define SOCKET_LISTEN_ERR_MESSAGE     "Impossibile settare la socket in ascolto."
#define SOCKET_BINDING_ERR_MESSAGE    "Impossibile collegarsi alla socket."
#define SOCKET_RECEIVE_DATA_ERR_MESSAGE     "Impossibile ricevere dati dalla socket."
#define NOT_SURE_ERR_MESSAGE "Qualcosa è andato storto."
#define ACCEPT_SOCKET_ERR_MESSAGE "Errore nella funzione accept."
#define MAP_FAILED_ERR_MESSAGE "Impossibile allocare memoria.\nL'applicazione verrà chiusa."
/*
 * Messaggi di errore relativi al gioco stesso
 */
#define FILLED_GAME_ERR_MESSAGE       "Non puoi unirti a questa partita..."
#define MATRIX_ALLOCATION_ERR_MESSAGE "Errore nella creazione della matrice di gioco"
#define ON_MOVEMENT_ERR_MESSAGE       "Errore nel movimento del giocatore"

/*
 * Messaggi di errore lato client.
 */
#define INVALID_CHOICE_ERR_MESSAGE    "La scelta inserita non è valida.\n"

/*
 * Messaggi di errore relativi ai thread.
 */
#define THREAD_CREATION_ERR_MESSAGE   "Impossibile creare un nuovo thread.\n"
/*
 *Messaggi di LOG
 */
#define LOG_START_SERVER                    ": iniziata nuova sessione Server.\n"
#define LOG_CLOSE_SERVER                    ": finita sessione Server.\n"
#define LOG_NEW_GAME                        ": iniziata nuova partita con ID "
#define LOG_END_GAME                        ": finita partita con ID "
#define LOG_NEW_PLAYER_JOIN1                ": l'utente "
#define LOG_NEW_PLAYER_JOIN2                " si è unito alla partita con ID "
#define LOG_PLAYER_MOVES1                   ": in partita con ID "
#define LOG_PLAYER_MOVES2                   " è andato da "
#define LOG_PLAYER_WINS1                    ": l'utente "
#define LOG_PLAYER_WINS2                    " ha vinto la partita con ID "
#define LOG_NEW_USER                        " si è registrato al Server.\n"
#define LOG_SIGN_IN                         " si è connesso al Server.\n"
#define LOG_SIGN_OUT                        " si è disconesso dal Server.\n"
#define LOG_ERROR                           ": ERRORE( "
#define LOG_UNKNOWN_CLIENT_DISCONNECTION    ": un Client non identificato si è disconesso dal Server.\n"


#endif
