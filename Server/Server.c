#include "Server.h"
#include <time.h>

int fdLog;
int main(int argc, char* argv[]){
  srand(time(NULL));
  signal(SIGINT, handleSignal);
  clear();
  int sock, sockfd, n_b_w=0, n_b_r=0, fdPlayer, fdGame;
  char msg[1024*5],c;
  struct sockaddr_un mio_indirizzo;
  LogServerStart(&fdLog);
  if((sock=socket(PF_LOCAL,SOCK_STREAM,0))<0){
    printf("Errore apertura socket\n");
    //Gestire cosa fare in caso di errore in apertura Socket
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
          sprintf(msg,"%d",strlen(WELCOME_MESSAGE));
          write(sockfd,msg,strlen(msg));
          n_b_r=read(sockfd,msg,5);
          write(sockfd,WELCOME_MESSAGE,strlen(WELCOME_MESSAGE));
          n_b_r=read(sockfd,msg,50);
          msg[n_b_r]='\0';
          char user[50];
          int baba=-1;
          while(baba==-1){
            if(n_b_r==1){
              switch (msg[0]) {
                case 'l': case 'L':
                  logInUserMenu(sockfd,user,&fdLog);
                  baba=1;
                  break;
                case 'r': case 'R':
                  signInUserMenu(sockfd,user,&fdLog);
                  baba=1;
                  break;
                case 'e': case 'E':
                  write(sockfd, "-1", strlen("-1"));
                  break;
                default:
                  clear();
                  sprintf(msg,"%d",strlen(WELCOME_MESSAGE));
                  write(sockfd,msg,strlen(msg));
                  n_b_r=read(sockfd,msg,5);
                  write(sockfd,WELCOME_MESSAGE,strlen(WELCOME_MESSAGE));
                  n_b_r=read(sockfd,msg,50);
                  msg[n_b_r]='\0';
                  break;
              }
            }else{
                  clear();
                  sprintf(msg,"%d",strlen(WELCOME_MESSAGE));
                  write(sockfd,msg,strlen(msg));
                  n_b_r=read(sockfd,msg,5);
                  write(sockfd,WELCOME_MESSAGE,strlen(WELCOME_MESSAGE));
                  n_b_r=read(sockfd,msg,50);
                  msg[n_b_r]='\0';
            }
          }
          read(sockfd,msg,1);
          sprintf(msg,"%d",strlen(GAME_SELECTION_MENU));
          write(sockfd,msg,strlen(msg));
          n_b_r=read(sockfd,msg,5);
          write(sockfd,GAME_SELECTION_MENU,strlen(GAME_SELECTION_MENU));
          n_b_r=read(sockfd,msg,5);
          msg[n_b_r]='\0';
          baba=-1;
          while(baba==-1){
            switch (msg[0]) {
              case 'n': case 'N':
                newGame(sockfd, user);
                baba=1;
                break;
              case 'j': case 'J':
                joinGame(sockfd,user);
                baba=1;
                break;
              case 'e': case 'E':
                write(sockfd, "-1", strlen("-1"));
                break;
              default:
                sprintf(msg,"%d",strlen(GAME_SELECTION_MENU));
                write(sockfd,msg,strlen(msg));
                n_b_r=read(sockfd,msg,5);
                write(sockfd,GAME_SELECTION_MENU,strlen(GAME_SELECTION_MENU));
                n_b_r=read(sockfd,msg,50);
                msg[n_b_r]='\0';
                break;
            }
          }
          /*GameGrid **game=NULL;
          game=createGameGrid(game);
          GameGridToText(game,msg,1);
          write(sockfd,msg,strlen(msg));
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

void joinGame(int sockfd, char user[]){
  write(sockfd,"Hai scelto joinGame",20);
  clear();
  return ;
}

void newGame(int sockfd, char user[]){

  char msg[5000];
  int n_b_r;
  GameGrid **g=NULL;
  if((g=createGameGrid(g))!=NULL){
    GameGridToText(g,msg,1);
    write(sockfd,msg,strlen(msg));
    printf("%s\n",msg );
    clear();
  }else{
    sprintf(msg,"%d",strlen(NO_CONNECTION_ERR_MESSAGE));
    write(sockfd,msg,strlen(msg));
    n_b_r=read(sockfd,msg,5);
    write(sockfd,NO_CONNECTION_ERR_MESSAGE,strlen(NO_CONNECTION_ERR_MESSAGE));

  }
  return;
}

void handleSignal(int Sig){

  if(Sig == SIGINT){
    LogServerClose(&fdLog);
    unlink(MIO_SOCK);
    exit(1);
  }
}

GameGrid **createGameGrid(GameGrid **p){

  int i, j, x, y, numOstacoli=0;
  p=(GameGrid**)malloc(MAX_GRID_SIZE_H * sizeof(GameGrid*));

  if(p!=NULL){
    for(i=0;i<MAX_GRID_SIZE_H;i++)
      p[i]=(GameGrid*)calloc(MAX_GRID_SIZE_L, sizeof(GameGrid));
    for(i=0;i<MAX_GRID_SIZE_H;i++){
      for(j=0;j<MAX_GRID_SIZE_L;j++){
        p[i][j].ostacolo=0;
        p[i][j].giocatore=0;
        p[i][j].pacco=0;
        p[i][j].locazione=0;
        p[i][j].codiceGiocatore=0;
        p[i][j].codicePacco=0;
        p[i][j].codiceLocazione=0;
        p[i][j].p0=0;
        p[i][j].p1=0;
        p[i][j].p2=0;
        p[i][j].p3=0;
        p[i][j].p4=0;
        p[i][j].p5=0;
        p[i][j].p6=0;
        p[i][j].p7=0;
      }
    }
    y=rand()%MAX_GRID_SIZE_L;
    x=rand()%MAX_GRID_SIZE_H;
    p[x][y].giocatore=1;
    p[x][y].codiceGiocatore=0;
    p[x][y].p1=1;
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_L;
        x=rand()%MAX_GRID_SIZE_H;
      } while(p[x][y].giocatore);
      p[x][y].pacco=1;
      p[x][y].codicePacco=i;
    }
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_L;
        x=rand()%MAX_GRID_SIZE_H;
      } while(p[x][y].giocatore || p[x][y].pacco);
      p[x][y].locazione=1;
      p[x][y].codiceLocazione=i;
    }
    for(i=0;i<MAX_OBSTACLES_N;i++){
      y=rand()%MAX_GRID_SIZE_L;
      x=rand()%MAX_GRID_SIZE_H;
      if(!(p[x][y].giocatore || p[x][y].pacco || p[x][y].locazione)){
        p[x][y].ostacolo=1;
        numOstacoli++;
      }
    }
    while(numOstacoli<MIN_OBSTACLES){
      y=rand()%MAX_GRID_SIZE_L;
      x=rand()%MAX_GRID_SIZE_H;
      if(!(p[x][y].giocatore || p[x][y].pacco || p[x][y].locazione)){
        p[x][y].ostacolo=1;
        numOstacoli++;
      }
    }
  }
  return p;

}

int GameGridToText(GameGrid **p, char msg[], int giocatore){

  /*int i,j;
  msg[0]='\0';
  for(i=0;i<MAX_GRID_SIZE_H;i++){
    for(j=0;j<MAX_GRID_SIZE_L;j++){
      switch(giocatore){
        case 1:
        if(p[i][j].p1==0)
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
        break;
        case 3:
        if(p[i][j].p3==0)
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
        break;
        case 4:
        if(p[i][j].p4==0)
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
        break;
        case 5:
        if(p[i][j].p5==0)
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
        break;
        case 6:
        if(p[i][j].p6==0)
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
        break;
        case 7:
        if(p[i][j].p7==0)
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
        break;
        case 8:
        if(p[i][j].p8==0)
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
        break;
        case 2:
        if(p[i][j].p2==0)
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
        break;
      }
    }
  strcat(msg,"\n");
}*/
  return 1;
}

void deleteGrid(GameGrid **g){

  int i;
  for(i=0;i<MAX_GRID_SIZE_H;i++)
    free(g[i]);
  free(g);
  return;
}
