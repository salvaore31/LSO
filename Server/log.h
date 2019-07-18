#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "../costanti.h"
#include "../messaggi.h"

void LogServerStart(int*);
void LogServerClose(int*);
void LogNewGame(int*, int);
void LogEndGame(int*, int);
void LogPlayerJoin(int*, int, char[]);
void LogPlayerMoves(int*, int, char[], char[], char[]);//sino
void LogPlayerWin(int*, int, char[]);
void LogNewUser(int*, char[]);//sino
void LogUserSignIn(int*, char[]);
void LogUserSignOut(int*, char[]);//sino
void LogErrorMessage(int*);
