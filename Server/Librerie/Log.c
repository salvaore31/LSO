#include "Log.h"

void substituteChar(char st[],char a, char b){
  int i=0;
  while(st[i]!='\0'){
    if(st[i]==a)
      st[i]=b;
    i++;
  }
  return;
}

void oraEsatta(char s[]){
  char buffer[26];
  time_t ora;
  time(&ora);
  strcpy(s,ctime_r(&ora, buffer));
  substituteChar(s,'\n','\0');
}

void LogServerStart(int *fdLog){

  char ora[80];
  char fileName[45]="File/Log/LOG_";
  int n_b_w;

  oraEsatta(ora);
  strcat(fileName,ora);
  substituteChar(fileName,' ' , '_');
  substituteChar(fileName,':','_');
  strcat(fileName,".txt");

  if((*fdLog=open(fileName,O_CREAT | O_APPEND | O_WRONLY,S_IRWXU))<0){
    /*Gestire cosa succede in caso di errore*/
  } else {
    if((n_b_w = write(*fdLog,ora,strlen(ora))< strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
    } else {
      if((n_b_w = write(*fdLog,LOG_START_SERVER,sizeof(LOG_START_SERVER)-1)) < sizeof(LOG_START_SERVER)-1){
        /*Gesire mancata scrittura su LOG*/
      }
    }
  }
  return ;
}

void LogServerClose(int*fdLog){

  char ora[80];
  int n_b_w;

  oraEsatta(ora);

  if((n_b_w = write(*fdLog,ora,strlen(ora))< strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w = write(*fdLog,LOG_CLOSE_SERVER,sizeof(LOG_CLOSE_SERVER)-1)) < sizeof(LOG_CLOSE_SERVER)-1){
      /*Gesire mancata scrittura su LOG*/
    }
  }
  return ;
}

void LogNewGame(int* fdLog, int gameId){

  char ora[80];
  int n_b_w;
  char buf[100];

  oraEsatta(ora);
  sprintf(buf, LOG_NEW_GAME, gameId);

  if((n_b_w = write(*fdLog,ora,strlen(ora))< strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w = write(*fdLog,buf,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }
  return;
}

void LogEndGame(int* fdLog, int gameId){
  char ora[80];
  int n_b_w;
  char buf[100];

  oraEsatta(ora);
  sprintf(buf, LOG_END_GAME, gameId);

  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w = write(*fdLog,buf,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }
  return;
}

void LogPlayerJoin(int* fdLog, int gameId, char* player){
  char ora[80];
  int n_b_w;
  char buf[100];

  oraEsatta(ora);
  sprintf(buf, LOG_NEW_PLAYER_JOIN, player,gameId);

  if( (n_b_w = write(*fdLog,ora,strlen(ora) ) < strlen(ora) ) ) {
      /*Gesire mancata scrittura su LOG*/
  }else{
    if((n_b_w = write(*fdLog,buf,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }
  return;
}

void LogPlayerMoves(int* fdLog, int gameId, char* player, char* src, char *dst ){
  char ora[80];
  int n_b_w;
  char buf[100];

  oraEsatta(ora);
  sprintf(buf, LOG_PLAYER_MOVES, gameId, player, src, dst);

  if( (n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora)) ) {
      //Gesire mancata scrittura su LOG
  } else {
    if( (n_b_w= write(*fdLog, buf ,strlen(buf)) ) < strlen(buf) ){
      //Già sai
    }
  }
  return;
}

void LogPlayerTakePackage(int* fdLog,int gameId,char player[], int pacco, char loc[]){

  char ora[80];
  int n_b_w;
  char buf[100];

  oraEsatta(ora);
  sprintf(buf, LOG_PLAYER_TAKE, gameId,player,pacco,loc);

  if( (n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora)) ) {
      //Gesire mancata scrittura su LOG
  }else{
    if((n_b_w = write(*fdLog, buf ,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }
  return;
}

void LogPlayerLeavePackage(int *fdLog, int gameId, char player[], int pacco, char loc[]){

  char ora[80];
  int n_b_w;
  char buf[100];

  oraEsatta(ora);
  sprintf(buf, LOG_PLAYER_DELIVER, gameId,player,pacco,loc);
  if( (n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora)) ) {
      //Gesire mancata scrittura su LOG
  }else{
    if((n_b_w = write(*fdLog, buf ,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }
  return;
}

void LogPlayerMakeAPoint(int *fdLog, int gameId, char player[]){

  char ora[80];
  int n_b_w;
  char buf[100];

  oraEsatta(ora);
  sprintf(buf, LOG_PLAYER_MAKE_POINT, gameId,player);
  if( (n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora)) ) {
      //Gesire mancata scrittura su LOG
  }else{
    if((n_b_w = write(*fdLog, buf ,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }
  return;

}

void LogPlayerWin(int* fdLog, int gameId, char * player){
  char ora[80];
  int n_b_w;
  char buf[100];

  oraEsatta(ora);
  sprintf(buf, LOG_PLAYER_WINS, player,gameId);

  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,buf,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }

  return;
}

void LogNewUser(int* fdLog, char* player) {
  char ora[80];
  int n_b_w;
  char buf[64];

  oraEsatta(ora);
  sprintf(buf,LOG_NEW_USER,player);

  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,buf ,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }
  return;
}

void LogUserSignIn(int* fdLog, char* player){
  char ora[80];
  int n_b_w;
  char buf[64];

  oraEsatta(ora);
  sprintf(buf,LOG_SIGN_IN,player);

  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,buf ,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }
  return;
}

void LogUserSignOut(int* fdLog, char* player){
  char ora[80];
  int n_b_w;
  char buf[64];

  oraEsatta(ora);
  sprintf(buf,LOG_SIGN_OUT,player);

  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,buf ,strlen(buf))) < strlen(buf)){
      /*Già sai*/
    }
  }
  return;
}

void LogErrorMessage(int* fdLog, char* err){
  char ora[80];
  int n_b_w;
  char buf[1000];

  oraEsatta(ora);
  sprintf(buf,LOG_ERROR,err);
  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  }else{
    if((n_b_w= write(*fdLog, buf ,strlen(buf))) < strlen(buf)){
        /*che fare che fare*/
    }
  }
  return;
}

void LogUnkownClientDisconnection (int*fdLog){

  char ora[80];
  int n_b_w;

  oraEsatta(ora);

  if((n_b_w = write(*fdLog,ora,strlen(ora))< strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w = write(*fdLog,LOG_UNKNOWN_CLIENT_DISCONNECTION,sizeof(LOG_UNKNOWN_CLIENT_DISCONNECTION)-1)) < sizeof(LOG_UNKNOWN_CLIENT_DISCONNECTION)-1){
      /*Gesire mancata scrittura su LOG*/
    }
  }
  return ;
}
