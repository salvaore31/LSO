#ifndef CODICIERRORI_H_

/*Le costanti seguenti contengono i valori degli errori
  I codici di errori seguono la seguente codifica:
    da -100 a -200 per i codici di errori relativi a problemi tecnici
    da -200 a -300 per i codici di errori relativi a Registrazione e Log-In
    da -300 a -400 per i codici di errori relativi a errori su operazioni di Log

 */

/*
    Codici di errore relativi a problemi tecnici
 */
#define ERR_NO_CONNECTION -100
#define ERR_WRITE_ERROR -101
#define ERR_INPUT_OUTPUT -102

/*
    Codici di errore relativi a problemi su Registrazione e Log-In
 */
#define ERR_INVALID_USERNAME -200
#define ERR_NO_USER_FILE -201
#define ERR_USERNAME_NOT_FOUND -202
#define ERR_WRONG_PASSWORD -203

/*
  Codici di errore relativi a problemi su operazioni di log
*/
#define ERR_LOG_FILE_CREATION -300
#define ERR_LOG_WRITE -301


#endif
