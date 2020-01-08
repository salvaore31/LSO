#ifndef CODICIERRORI_H_

/*Le costanti seguenti contengono i valori degli errori
  I codici di errori seguono la seguente codifica:
    da -100 a -200 per i codici di errori relativi a problemi tecnici
    da -200 a -300 per i codici di errori relativi a Registrazione e Log-In

 */

/*
    Codici di errore relativi a problemi tecnici
 */
#define ERR_NO_CONNECTION -100
#define ERR_WRITE_ERROR -101


/*
    Codici di errore relativi a problemi su Registrazione e Log-In
 */
#define ERR_INVALID_USERNAME -200
#define ERR_NO_USER_FILE -201
#define ERR_INVALID_USERNAME -202


#endif CODICIERRORI_H_
