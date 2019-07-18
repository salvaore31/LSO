#include "log.h"
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

  char ora[26];
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

  char ora[26];
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

  char ora[26];
  int n_b_w;
  char buf[10];

  oraEsatta(ora);
  sprintf(buf, "%d", gameId);
  strcat(buf,".\n");
  if((n_b_w = write(*fdLog,ora,strlen(ora))< strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w = write(*fdLog,LOG_NEW_GAME,sizeof(LOG_NEW_GAME)-1)) < sizeof(LOG_NEW_GAME)-1){
      /*Gesire mancata scrittura su LOG*/
    } else{
      if((n_b_w = write(*fdLog,buf,strlen(buf))) < strlen(buf)){
        /*Già sai*/
      }
    }
  }
  return;
}

void LogEndGame(int* fdLog, int gameId){
  char ora[26];
  int n_b_w;
  char buf[10];

  oraEsatta(ora);
  sprintf(buf, "%d", gameId);
  strcat(buf,".\n");
  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w = write(*fdLog,LOG_END_GAME,sizeof(LOG_END_GAME)-1)) < sizeof(LOG_END_GAME)-1){
      /*Gesire mancata scrittura su LOG*/
    } else{
      if((n_b_w = write(*fdLog,buf,strlen(buf))) < strlen(buf)){
        /*Già sai*/
      }
    }
  }
  return;
}

void LogPlayerJoin(int* fdLog, int gameId, char[] player){
  char ora[26];
  int n_b_w;
  char buf[10];

  oraEsatta(ora);
  sprintf(buf, "%d", gameId);
  strcat(buf,".\n");
  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,LOG_NEW_PLAYER_JOIN1,sizeof(LOG_NEW_PLAYER_JOIN1)-1)) < sizeof(LOG_NEW_PLAYER_JOIN1)){
      /*Già sai*/
    } else {
      if((n_b_w = write(*fdLog,player,strlen(player))) < strlen(player)){
        /*Già sai*/
      } else {
        if((n_b_w = write(*fdLog,LOG_NEW_PLAYER_JOIN2,sizeof(LOG_NEW_PLAYER_JOIN2)-1)) < sizeof(LOG_NEW_PLAYER_JOIN2)-1){
          /*Gesire mancata scrittura su LOG*/
        } else{
          if((n_b_w = write(*fdLog,buf,strlen(buf))) < strlen(buf)){
            /*Già sai*/
          }
        }
      }
    }
  }
  return;
}

void LogPlayerMoves(int* fdLog, int gameId, char[] player, char[] )
