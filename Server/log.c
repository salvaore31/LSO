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

void LogPlayerJoin(int* fdLog, int gameId, char* player){
  char ora[26];
  int n_b_w;
  char buf[10];

  oraEsatta(ora);
  sprintf(buf, "%d", gameId);
  strcat(buf,".\n");
  if( (n_b_w = write(*fdLog,ora,strlen(ora) ) < strlen(ora) ) ) {
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,LOG_NEW_PLAYER_JOIN1,sizeof(LOG_NEW_PLAYER_JOIN1)-1)) < sizeof(LOG_NEW_PLAYER_JOIN1)-1){
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

void LogPlayerMoves(int* fdLog, int gameId, char* player, char* src, char *dst ){
  char ora[26];
  int n_b_w;
  char buf[100];

  oraEsatta(ora);
  sprintf(buf, "%d", gameId);
  strcat(buf, " ");


  if( (n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora)) ) {
      //Gesire mancata scrittura su LOG
  } else {
    if((n_b_w= write(*fdLog,LOG_PLAYER_MOVES1 ,sizeof(LOG_PLAYER_MOVES1)-1)) < sizeof(LOG_PLAYER_MOVES1)-1){
      //Già sai
    } else {
      if((n_b_w = write(*fdLog, buf ,strlen(buf))) < strlen(buf)){
        /*Già sai*/
      } else {
        if((n_b_w = write(*fdLog,player,strlen(player))) < strlen(player)){
          /*Gesire mancata scrittura su LOG*/
        } else{
            if((n_b_w = write(*fdLog,LOG_PLAYER_MOVES2,sizeof(LOG_PLAYER_MOVES2)-1)) < sizeof(LOG_PLAYER_MOVES2)-1){
          /*Già sai*/
        } else{
              buf[0] = '\0';
              strcat(buf, " da ");
              strcat(buf, src);
              strcat(buf, " a ");
              strcat(buf, dst);
              strcat(buf,".\n");
              if((n_b_w = write(*fdLog, buf, strlen(buf))) < strlen(buf)){
                /*Sai bene*/
              }
            }
          }
        }
      }
    }
  return;
}

void LogPlayerWin(int* fdLog, int gameId, char * player){
  char ora[26];
  int n_b_w;
  char buf[10];

  oraEsatta(ora);
  sprintf(buf, "%d", gameId);
  strcat(buf,".\n");
  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,LOG_PLAYER_WINS1,sizeof(LOG_PLAYER_WINS1)-1)) < sizeof(LOG_PLAYER_WINS1)-1){
      /*Già sai*/
    } else {
      if((n_b_w = write(*fdLog,player,strlen(player))) < strlen(player)){
        /*Già sai*/
      } else {
        if((n_b_w = write(*fdLog,LOG_PLAYER_WINS2,sizeof(LOG_PLAYER_WINS2)-1)) < sizeof(LOG_PLAYER_WINS2)-1){
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

void LogNewUser(int* fdLog, char* player) {
  char ora[26];
  int n_b_w;
  char buf[64];

  oraEsatta(ora);
  strcat(ora, ": ");

  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {

    if((n_b_w= write(*fdLog,player ,strlen(player))) < strlen(player)){
      /*Già sai*/
    } else {
      if((n_b_w = write(*fdLog, LOG_NEW_USER ,sizeof(LOG_NEW_USER)-1 )) < sizeof(LOG_NEW_USER) -1){
        /*Già sai*/
      }
    }
  }
  return;
}

void LogUserSignIn(int* fdLog, char* player){
  char ora[26];
  int n_b_w;
  char buf[64];

  oraEsatta(ora);
  strcat(ora, ": ");

  if( (n_b_w = write(*fdLog,ora,strlen(ora) ) ) < strlen(ora) ){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,player ,strlen(player))) < strlen(player)){
      /*Già sai*/
    } else {
      if((n_b_w = write(*fdLog, LOG_SIGN_IN ,sizeof(LOG_SIGN_IN)-1 )) < sizeof(LOG_SIGN_IN) -1){
        /*Già sai*/
      }
    }
  }
  return;
}

void LogUserSignOut(int* fdLog, char* player){
  char ora[26];
  int n_b_w;
  char buf[64];

  oraEsatta(ora);
  strcat(ora, ": ");

  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,player ,strlen(player))) < strlen(player)){
      /*Già sai*/
    } else {
      if((n_b_w = write(*fdLog, LOG_SIGN_OUT ,sizeof(LOG_SIGN_OUT)-1 )) < sizeof(LOG_SIGN_OUT) -1){
        /*Già sai*/
      }
  return;
    }
  }

}


void LogErrorMessage(int* fdLog, char* err){
  char ora[26];
  int n_b_w;
  char buf[1000];
  strcpy(buf, err);
  strcat(buf, ")\n");

  oraEsatta(ora);

  if((n_b_w = write(*fdLog,ora,strlen(ora)) < strlen(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w= write(*fdLog,LOG_ERROR ,sizeof(LOG_ERROR)-1)) < sizeof(LOG_ERROR)-1){
      /*Già sai*/
    }else{
      if((n_b_w= write(*fdLog, buf ,strlen(buf))) < strlen(buf)){
        /*che fare che fare*/
      }
    }
  }
  return;
}
