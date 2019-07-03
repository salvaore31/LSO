#ifndef STRINGHE_H_
#define STRINGHE_H_

/*
 * Stringhe per la generazione dei menu.
 */
#define LOG_IN_MENU         "Cosa vuoi fare?\nPremi 'l' per eseguire il login.\nPremi 'r' per registrarti.\nPremi 'e' per uscire."
#define GAME_SELECTION_MENU "Cosa vuoi fare?\nPremi 'n' per iniziare una nuova partita.\nPremi 'j' per unirti ad una partita in corso.\n"
#define GAMES_PLAYED        "Partita %i.\nGiocatori: [...]\n"
#define SELECT_FROM_CURRENT "Inserisci il numero della partita a cui vuoi aggiungerti.\n"
#define SELECT_DIRECTION    "Inserisci:\n\t'w' per muoverti verso NORD;\n\t'a' per muoverti verso OVEST;\n\t's' per muoverti verso SUD\n\t'd' per muoverti verso EST"

/*
 * Messaggi di errore per gestire problemi sul login/signin
 */
#define ERR_NO_SUCH_USER    "L'username inserito non corrisponde a nessun utente.\n"
#define ERR_ALREADY_PRESENT "L'username inserito è già associato ad un utente.\n"
#define ERR_WRONG_PASSWORD  "La password inserita non è corretta.\n"

/*
 * Messaggi di errore relativi al server.
 */
#define ERR_NO_CONNECTION   "Impossibile connettersi al server.\n"

/*
 * Messaggi di errore relativi al gioco stesso
 */
#define ERR_FILLED_GAME     "Non puoi unirti a questa partita...\n"

#endif
