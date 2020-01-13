#ifndef CODICIERRORI_H_

/*Le costanti seguenti contengono i valori degli errori
  I codici di errori seguono la seguente codifica:
    da -1000 a -2000 per i codici di errori relativi a problemi tecnici
    da -2000 a -3000 per i codici di errori relativi a Registrazione e Log-In
    da -3000 a -4000 per i codici di errori relativi a errori su operazioni di Log

 */

/*
    Codici di errore relativi a problemi tecnici
 */
#define ERR_NO_CONNECTION -1000
#define ERR_WRITE_ERROR -1001
#define ERR_INPUT_OUTPUT -1002
#define ERR_SOCKET_CREATION -1003
#define ERR_SOCKET_BINDING -1004

/*
    Codici di errore relativi a problemi su Registrazione e Log-In
 */
#define ERR_INVALID_USERNAME -2000
#define ERR_NO_USER_FILE -2001
#define ERR_USERNAME_NOT_FOUND -2002
#define ERR_WRONG_PASSWORD -2003

/*
  Codici di errore relativi a problemi su operazioni di log
*/
#define ERR_LOG_FILE_CREATION -3000
#define ERR_LOG_WRITE -3001


#endif
