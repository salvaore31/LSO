#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"

void LogServerStart(int*);
void LogServerClose(int*);
void LogNewGame(int*, int);
void LogEndGame(int*, int);
void LogPlayerJoin(int*, int, char[]);
void LogPlayerMoves(int*, int, char[], char[], char[]);
void LogPlayerWin(int*, int, char[]);
void LogNewUser(int*, char[]);
void LogPlayerTakePackage(int*,int,char[],int,char[]);
void LogPlayerLeavePackage(int*,int,char[],int,char[]);
void LogPlayerMakeAPoint(int*,int,char[]);
void LogUserSignIn(int*, char[]);
void LogUserSignOut(int*, char[]);
void LogErrorMessage(int*, char[]);
void LogUnkownClientDisconnection (int*fdLog);
