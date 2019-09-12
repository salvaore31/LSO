#include "Server.h"
#include <time.h>

int fdLog;
int main(int argc, char* argv[]){

  signal(SIGINT, handleSignal);
  clear();
  int sock, sockfd, n_b_w=0, n_b_r=0, fdPlayer, fdGame;
  char msg[1024*5],c;
  struct sockaddr_un mio_indirizzo;
  LogServerStart(&fdLog);
  if((sock=socket(PF_LOCAL,SOCK_STREAM,0))<0){
    printf("Errore apertura socket\n");
  }else{
    mio_indirizzo.sun_family=AF_LOCAL;
    strcpy(mio_indirizzo.sun_path,MIO_SOCK);
    if(bind(sock,(struct sockaddr *)&mio_indirizzo,sizeof(mio_indirizzo))<0){
      printf("Errore bind\n");
    }else{
      if(listen(sock,3)<0){
        printf("Errore listen\n");
      }else{
        while((sockfd=accept(sock,NULL,NULL))>-1){
          write(sockfd,WELCOME_MESSAGE,sizeof(WELCOME_MESSAGE));
          n_b_r=read(sockfd,msg,50);
          msg[n_b_r]='\0';
          char user[50];
          switch (msg[0]) {
            case 'l': case 'L':
              logInUserMenu(sockfd,user);
              break;
            case 'r': case 'R':
              signInUserMenu(sockfd,user);
              break;
            case 'e': case 'E':
              write(sockfd, "-1", sizeof("-1"));
              break;
          }
          GameGrid **game=NULL;
          game=createGameGrid(game);
          GameGridToText(game,msg,1);
          write(sockfd,msg,strlen(msg));
          n_b_r=read(sockfd,msg,250);
          printf("%s",msg );
          msg[n_b_r]='\0';
          if(strcmp(msg,USER_LOG_OUT)==0){
            LogUserSignOut(&fdLog,user);
          }
          /*Qui va la fork che crea nuove partite
          si crea nuovo processo; si dichiara nuova GameGrid, si mette mutex,
          se client ha accesso e non è finito il tempo:
            il client invia messaggio e il server controlla casella di destinazione, poi:
              Player si muove, server sposta giocatore e invia nuova posizione a client, cliente aggiorna
                              propria vista su matrice e libera mutex;
              Player su ostacolo, server non sposta il giocatore e aggiorna matrice così da far vedere ostacolo
                              a quel player, client riceve nuova vista, stampa e libera mutex;
              Player  spawna, server assegna al nuovo player posizione iniziale, posizione di vittoria, aggiorna tutte
                              le viste delle matrici aggiungendo posizione nuovo player e sua win condition, il client
                              riceve la nuova matrice e la stampa, libera mutex;
              Player vince, server manda messaggio di vittoria a vincitore e di sconfitta a perdenti, client libera mutex,
                            server chiude mutex, fa ritornare al menu principale i client
          se è finito il tempo:
            messaggio di sconfitta e chiusura partita.
          */
        }
      }
    }
  }
  close(sock);
  close(sockfd);
  unlink(MIO_SOCK);
  LogServerClose(&fdLog);
  return 1;
}

void handleSignal(int Sig){

  if(Sig == SIGINT){
    LogServerClose(&fdLog);
    unlink(MIO_SOCK);
    exit(1);
  }
}

int signInUserMenu(int sockfd, char usrn[]){
  int n_b_r;
  char pssw[50];
  int err;

  //da aggiungere controllo su effettiva lettura
    write(sockfd, INSERT_USERNAME_SIM, sizeof(INSERT_USERNAME_SIM));
    n_b_r = read(sockfd, usrn, 50);
    usrn[n_b_r] ='\0';
    write(sockfd, INSERT_PASSWORD_SIM, sizeof(INSERT_PASSWORD_SIM));
    n_b_r = read(sockfd, pssw, 50);
    pssw[n_b_r] = '\0';

    while((err=registerUser(usrn, pssw)) != 0){
      //l'utente non è stato trovato tra quelli registrati
      switch(err){
        case NO_CONNECTION:
          write(sockfd, ERR_NO_CONNECTION, sizeof(ERR_NO_CONNECTION));
          n_b_r = read(sockfd, pssw, 50);
          pssw[n_b_r] = '\0';
          break;
        case INVALID_USERNAME:
          write(sockfd, USER_ALREADY_PRESENT_SIM, sizeof(USER_ALREADY_PRESENT_SIM));
          n_b_r = read(sockfd, usrn, 50);
          usrn[n_b_r] ='\0';
          write(sockfd, INSERT_PASSWORD_SIM, sizeof(INSERT_PASSWORD_SIM));
          n_b_r = read(sockfd, pssw, 50);
          pssw[n_b_r] = '\0';
          break;
        default:
          break;
      }
    }
    LogNewUser(&fdLog, usrn);
    write(sockfd,SUCCESS_MESSAGE_SIM,sizeof(SUCCESS_MESSAGE_SIM));
    return 1;
  }

//checkUsername FUNZIONA BISOGNA GESTIRE IL COMPORTAMENTO IN CASO DI ERRORE NELL' APERTURA
int checkUsername(char* username){
  int fdUserFile, i=0, res,r=1;
  char c, str[MAX_SIZE_USERNAME];
  if((fdUserFile=open(USERS_FILE,O_RDONLY))<0){
    printf("errore\n" );
    //GESTIRE ERRORE APERTURA FILE
  }else{
    while(r>0){
      while(((r=read(fdUserFile,&c,1))>0)&&c!='%')
        str[i++]=c;
      str[i++]='\0';
      if(strcmp(str,username)==0){
        res=lseek(fdUserFile,0,SEEK_CUR);
        close(fdUserFile);
        return res;
      }
      while(((r=read(fdUserFile,&c,1))>0)&&c!='\n');
      i=0;
    }
    close(fdUserFile);
  }
  return -1;
}
//registerUser FUNZIONA BISOGNA GESTIRE IL COMPORTAMENTO NEI VARI CASI DI ERRORE
int registerUser(char* newuser, char* newpassw){

  int fdUserFile,n_b_w,lenght_user,lenght_passw;
  lenght_user=strlen(newuser);
  lenght_passw=strlen(newpassw);

  if( lenght_user > MAX_SIZE_USERNAME || lenght_passw > MAX_SIZE_PASSW ){
    return INVALID_USERNAME;
  }else{
    if(checkUsername(newuser) < 0){
      if((fdUserFile = open(USERS_FILE,O_WRONLY|O_APPEND,S_IRWXU))<0){
        return NO_CONNECTION;
        /*Gestire cosa succede se non si riesce ad aprire il file*/
      }else{
        if((n_b_w = write(fdUserFile,newuser,lenght_user))<lenght_user){
          return NO_CONNECTION;
          /*Gestire cosa succede se non si riesce a scrivere su file*/
          close(fdUserFile);
        }else{
          write(fdUserFile,"%",1);
          if((n_b_w = write(fdUserFile,newpassw,lenght_passw))<lenght_passw){
            /*Gestire cosa succede se non si riesce a scrivere su file*/
            close(fdUserFile);
          }else{
            write(fdUserFile,"\n",1);
            close(fdUserFile);
            return 0;
          }
        }
      }
    }else{
      return INVALID_USERNAME;
    }
  }
}
//Non avendo ancora deciso di preciso cosa deve fare intanto ritorna 0 se l'utente e la password inseriti sono validi
int logInUser(char* user, char* passw){

  int fdUserFile, pos, n_b_r,i=0;
  char c,str[MAX_SIZE_PASSW];

  if((pos=checkUsername(user))<0){
    //La funzione ritorna -2 nel caso in cui non trova l'utente tra quelli registrati;
    return -2;
  }else{
    if((fdUserFile=open(USERS_FILE,O_RDONLY))<0){
      //Gestire il comportamento in caso di errore apertura file
      return -3;
    }else{
      if(lseek(fdUserFile,pos,SEEK_SET)!=pos){
        //GESTIRE il comportamentoin caso di errore lseek
        return -4;
      }else{
        while((n_b_r=read(fdUserFile,&c,1))>0 && c!='\n')
          str[i++]=c;
        str[i]='\0';
        if(strcmp(passw,str)==0){
          return 0;
        }else{
          return -1;
          //Gestire il comportamento in caso in cui la password inserita non è corretta
        }
      }
    }
  }
  return -1;
}
//logInUserMenu gesitsce la comunicazione con il client per quanto riguarda il logIn
int logInUserMenu(int sockfd, char usrn[]){

  int n_b_r;
  char pssw[50];
  int err;

  //da aggiungere controllo su effettiva lettura
    write(sockfd, INSERT_USERNAME_LIM, sizeof(INSERT_USERNAME_LIM));
    n_b_r = read(sockfd, usrn, 50);
    usrn[n_b_r] ='\0';
    while(checkUsername(usrn)<0){
      write(sockfd, WRONG_USERNAME_LIM, sizeof(WRONG_USERNAME_LIM));
      n_b_r = read(sockfd, usrn, 50);
      usrn[n_b_r] ='\0';
    }
    write(sockfd, INSERT_PASSWORD_LIM, sizeof(INSERT_PASSWORD_LIM));
    n_b_r = read(sockfd, pssw, 50);
    pssw[n_b_r] = '\0';

    while((err=logInUser(usrn, pssw)) != 0){
      //l'utente non è stato trovato tra quelli registrati
      switch(err){
        case -1:
          write(sockfd, WRONG_PASSWORD_LIM, sizeof(WRONG_PASSWORD_LIM));
          n_b_r = read(sockfd, pssw, 50);
          pssw[n_b_r] = '\0';
          break;
        case -2:
          write(sockfd, WRONG_USERNAME_LIM, sizeof(WRONG_USERNAME_LIM));
          n_b_r = read(sockfd, usrn, 50);
          usrn[n_b_r] ='\0';
          write(sockfd, INSERT_PASSWORD_LIM, sizeof(INSERT_PASSWORD_LIM));
          n_b_r = read(sockfd, pssw, 50);
          pssw[n_b_r] = '\0';
          break;
        case -3:
          write(sockfd, ERR_NO_CONNECTION, sizeof(ERR_NO_CONNECTION));
          break;
        case -4:
          break;
        default:
          break;
      }
    }
    LogUserSignIn(&fdLog, usrn);
    write(sockfd,SUCCESS_MESSAGE_LIM,sizeof(SUCCESS_MESSAGE_LIM));
    return 1;
}

GameGrid **createGameGrid(GameGrid **p){

  int i,j;
  p=(GameGrid**)malloc(MAX_GRID_SIZE_H * sizeof(GameGrid*));

  for(i=0;i<MAX_GRID_SIZE_H;i++)
    p[i]=(GameGrid*)calloc(MAX_GRID_SIZE_L, sizeof(GameGrid));

  for(i=0;i<MAX_GRID_SIZE_H;i++){
    for(j=0;j<MAX_GRID_SIZE_L;j++){
      p[i][j].infocasella=rand()%4;
      p[i][j].playerI=rand()%8;
      p[i][j].playerJ=rand()%8;
      p[i][j].permessi=rand()%256;
    }
  }
  return p;
}

int GameGridToText(GameGrid **p, char msg[], int giocatore){

  int i,j;
  msg[0]='\0';
  for(i=0;i<MAX_GRID_SIZE_H;i++){
    for(j=0;j<MAX_GRID_SIZE_L;j++){
      if(p[i][j].permessi-giocatore!=(255-giocatore))
        strcat(msg,"? ");
      else{
        switch(p[i][j].infocasella){
          case 0:
            strcat(msg,"\033[92m\033[92m0 \033[0m");
            break;
          case 1:
            strcat(msg,"\033[91m\033[91mX \033[0m");
            break;
          case 2:
            strcat(msg,"\033[96m\033[96mI \033[0m");
            break;
          default:
            strcat(msg,"\033[93m\033[93m! \033[0m");
            break;
        }
      }
    }
    strcat(msg,"\n");
  }
  return 1;
}
