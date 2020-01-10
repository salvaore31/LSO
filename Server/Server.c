#include "Server.h"
#include <time.h>
int fdLog;
int main(int argc, char* argv[]){

  clear();
  srand(time(NULL));
  signal(SIGINT, handleSignal);
	signal(SIGHUP, handleSignal);
	signal(SIGQUIT, handleSignal);
	signal(SIGTERM, handleSignal);
	signal(SIGKILL, handleSignal);
  int *thread_sd, sock, sockfd;
  pthread_t tid;
  struct sockaddr_in client_addr;
  socklen_t client_len;
  LogServerStart(&fdLog);
  if((sock=creaSocket())<0){
  }else{
    if(listen(sock,3)<0){
      printf("Errore listen\n");
    }else{
      while(1){
        client_len = sizeof(client_addr);
        if((sockfd=accept(sock,NULL,NULL))<0){
          printf("Accept\n");
          exit(-1);
        }
        thread_sd = (int *) malloc(sizeof(int));
        thread_sd = &sockfd;
        pthread_create(&tid, NULL, gestisci, (void *) thread_sd);
      }
    }
  }
  close(sock);
  close(sockfd);
  LogServerClose(&fdLog);
  return 1;
}

void joinGame(int sockfd, char user[]){
  write(sockfd,"Hai scelto joinGame",20);
  clear();
  return ;
}

void newGame(int sockfd, char user[]){

  int gameId;
  if((gameId=fork())<0){
    /*Gestire errore nuova partita*/
  }else if(gameId==0){
    char matrix[2000];
    char msg[50];
    int n_b_r;
    Game *g=NULL;
    g=createGame();
    strcpy(g->giocatori[0].nome,user);
    g->giocatori[0].nome[strlen(user)]='\0';
    if(createGameGrid(g) == 0){
      pthread_mutex_lock(&g->sem);
      LogPlayerJoin(&fdLog, getpid(), user);
      GameGridToText(g->grid,matrix,1);
      pthread_mutex_unlock(&g->sem);
      sprintf(msg,"%d",strlen(matrix));
      write(sockfd,msg,strlen(msg));
      n_b_r=read(sockfd,msg,5);
      write(sockfd,matrix,strlen(matrix));
      playGame(g,0,getpid());
    }else{
      sprintf(msg,"%d",strlen(NO_CONNECTION_ERR_MESSAGE));
      write(sockfd,msg,strlen(msg));
      n_b_r=read(sockfd,msg,5);
      write(sockfd,NO_CONNECTION_ERR_MESSAGE,strlen(NO_CONNECTION_ERR_MESSAGE));

    }
  }else{
    LogNewGame(&fdLog,gameId);
  }
  return;
}

Game *createGame(){
  Game* temp;
  temp=(Game*)malloc(sizeof(Game));
  temp->grid=NULL;
  pthread_mutex_init(&(temp->sem),NULL);
  return temp;
}

int playGame(Game * game, int idGiocatori, int gameId){

  int n_b_r;
  char msg[100], matrix[2000];
  while(0/*Partita ancora in corso*/){

  }
  return 0;
}

int azioneGiocatore(Game *game, int giocatore, char action, int gameId){

  pthread_mutex_lock(&game->sem);
  char src[9], dest[9];
  player *player = &game->giocatori[giocatore];
  GameGrid ** grid = game->grid;
  int x=player->posx,y=player->posy;
  int destx;
  int desty;
  switch (action) {
    case 'w': case 'W':
    if(y>0){
      setPermessi(x,y-1,giocatore,grid);
      if(grid[y-1][x].ostacolo || grid[y-1][x].giocatore){
        printf("ostacolo o giocatore\n" );
        /*Gestire mossa non valida*/
      }else{
        sprintf(src,"[%d,%d]",player->posx,player->posy);
        grid[y][x].giocatore=0;
        grid[y-1][x].giocatore=1;
        grid[y-1][x].codiceGiocatore=giocatore;
        player->posy=y-1;
        sprintf(dest,"[%d,%d]",player->posx,player->posy);
        LogPlayerMoves(&fdLog,gameId,player->nome,src,dest);
      }
    }else{
      printf("stai fuori\n" );
      /*Gesire mossa non valida*/
    }
    break;
    case 'a': case 'A':
      if(x>0){
        setPermessi(x-1,y,giocatore,grid);
        if(grid[y][x-1].ostacolo || grid[y][x-1].giocatore){
          printf("ostacolo o giocatore\n" );
          /*Gestire mossa non valida*/
        }else{
          sprintf(src,"[%d,%d]",player->posx,player->posy);
          grid[y][x].giocatore=0;
          grid[y][x-1].giocatore=1;
          grid[y][x-1].codiceGiocatore=giocatore;
          player->posx=x-1;
          sprintf(dest,"[%d,%d]",player->posx,player->posy);
          LogPlayerMoves(&fdLog,gameId,player->nome,src,dest);
        }
      }else{
        printf("stai fuori\n" );
        /*Gesire mossa non valida*/
      }
    break;
    case 's': case 'S':
    if(y<MAX_GRID_SIZE_H){
      setPermessi(x,y+1,giocatore,grid);
      if(grid[y+1][x].ostacolo || grid[y+1][x].giocatore){
        printf("ostacolo o giocatore\n" );
        /*Gestire mossa non valida*/
      }else{
        sprintf(src,"[%d,%d]",player->posx,player->posy);
        grid[y][x].giocatore=0;
        grid[y+1][x].giocatore=1;
        grid[y+1][x].codiceGiocatore=giocatore;
        player->posy=y+1;
        sprintf(dest,"[%d,%d]",player->posx,player->posy);
        LogPlayerMoves(&fdLog,gameId,player->nome,src,dest);
      }
    }else{
      printf("stai fuori\n" );
      /*Gesire mossa non valida*/
    }
    break;
    case 'd': case 'D':
      if(x<MAX_GRID_SIZE_L){
        setPermessi(x+1,y,giocatore,grid);
        if(grid[y][x+1].ostacolo || grid[y][x+1].giocatore){
          printf("ostacolo o giocatore\n" );
          /*Gestire mossa non valida*/
        }else{
          sprintf(src,"[%d,%d]",player->posx,player->posy);
          grid[y][x].giocatore=0;
          grid[y][x+1].giocatore=1;
          grid[y][x+1].codiceGiocatore=giocatore;
          player->posx=x+1;
          sprintf(dest,"[%d,%d]",player->posx,player->posy);
          LogPlayerMoves(&fdLog,gameId,player->nome,src,dest);
        }
      }else{
        printf("stai fuori\n" );
        /*Gesire mossa non valida*/
      }
    break;
    case 'q': case 'Q':

    break;
    case 'e': case 'E':
    break;
    case 'r': case 'R':
    break;
    case 'l': case 'L':
    break;
    case '0':
    break;
    default:
    break;
  }
  pthread_mutex_unlock(&game->sem);
}

void setPermessi(int x, int y, int giocatore, GameGrid ** grid){
  switch (giocatore) {
    case 0:
      grid[y][x].p0 = 1;
      break;
    case 1:
      grid[y][x].p1 = 1;
      break;
    case 2:
      grid[y][x].p2 = 1;
      break;
    case 3:
      grid[y][x].p3 = 1;
      break;
    case 4:
      grid[y][x].p4 = 1;
      break;
    case 5:
      grid[y][x].p5 = 1;
      break;
    case 6:
      grid[y][x].p6 = 1;
      break;
    case 7:
      grid[y][x].p7 = 1;
      break;
    default:
      printf("this should not be reachable.\n");
      break;
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

int creaSocket(){

  struct sockaddr_in mio_indirizzo;
  int sock;
  if((sock=socket(AF_INET,SOCK_STREAM,0))<0){
    printf("Errore apertura socket\n");
    return -1;
    //Gestire cosa fare in caso di errore in apertura Socket
  }else{
    mio_indirizzo.sin_family=AF_INET;
    mio_indirizzo.sin_port=htons(5200);
    memset(&(mio_indirizzo.sin_zero),'\0',8);
    mio_indirizzo.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(sock,(struct sockaddr *)&mio_indirizzo,sizeof(mio_indirizzo))<0){
      printf("Errore bind\n");
      return -1;
    }
  }
  return sock;
}

int createGameGrid(Game *g){

  int i, j, x, y, numOstacoli=0;
  GameGrid **p=(GameGrid**)malloc(MAX_GRID_SIZE_H * sizeof(GameGrid*));

  if(p!=NULL){
    for(i=0;i<MAX_GRID_SIZE_H;i++)
      p[i]=(GameGrid*)calloc(MAX_GRID_SIZE_L, sizeof(GameGrid));
    y=rand()%MAX_GRID_SIZE_H;
    x=rand()%MAX_GRID_SIZE_L;
    p[y][x].giocatore=1;
    p[y][x].p0=1;
    p[y][x].codiceGiocatore=0;
    g->giocatori[0].posx=x;
    g->giocatori[0].posy=y;
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_H;
        x=rand()%MAX_GRID_SIZE_L;
      } while(p[y][x].giocatore);
      p[y][x].pacco=1;
      p[y][x].p0=1;
      p[y][x].p1=1;
      p[y][x].p2=1;
      p[y][x].p3=1;
      p[y][x].p4=1;
      p[y][x].p5=1;
      p[y][x].p6=1;
      p[y][x].p7=1;
      p[y][x].codicePacco=i;
    }
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_H;
        x=rand()%MAX_GRID_SIZE_L;
      } while(p[y][x].giocatore || p[y][x].pacco);
      p[y][x].locazione=1;
      p[y][x].p0=1;
      p[y][x].p1=1;
      p[y][x].p2=1;
      p[y][x].p3=1;
      p[y][x].p4=1;
      p[y][x].p5=1;
      p[y][x].p6=1;
      p[y][x].p7=1;
      p[y][x].codiceLocazione=i;
      g->locazioneXPacchi[i]=x;
      g->locazioneYPacchi[i]=y;
    }
    for(i=0;i<MAX_OBSTACLES_N;i++){
      y=rand()%MAX_GRID_SIZE_H;
      x=rand()%MAX_GRID_SIZE_L;
      if(!(p[y][x].giocatore || p[y][x].pacco || p[y][x].locazione)){
        p[y][x].ostacolo=1;
        numOstacoli++;
      }
    }
    while(numOstacoli<MIN_OBSTACLES){
      y=rand()%MAX_GRID_SIZE_H;
      x=rand()%MAX_GRID_SIZE_L;
      if(!(p[y][x].giocatore || p[y][x].pacco || p[y][x].locazione)){
        p[y][x].ostacolo=1;
        numOstacoli++;
      }
    }
  }else{
    return -1;
  }
  g->grid=p;
  return 0;

}

int GameGridToText(GameGrid **p, char msg[], int giocatore){

  int i,j;
  char mom[150];
  msg[0]='\0';
  for(i=0;i<MAX_GRID_SIZE_L+1;i++){
    if(i!=0){
      if(i/10==0)
        sprintf(mom,"%d   ",i);
      else
        sprintf(mom,"%d  ",i);
      strcat(msg,mom);
    }else{
      sprintf(mom,"    ");
      strcat(msg, mom);
    }
  }
  sprintf(mom, "\n\n\n");
  strcat(msg, mom);
  for(i=0;i<MAX_GRID_SIZE_H;i++){
    if(i+1!=10)
      sprintf(mom,"%d   ",i+1);
    else
      sprintf(mom,"%d  ",i+1);
    strcat(msg,mom);
    for(j=0;j<MAX_GRID_SIZE_L;j++){
      switch(giocatore){
        case 1:
        if(p[i][j].p0==0)
          strcat(msg,"?   ");
          else{
            if (p[i][j].ostacolo) {
              strcat(msg,"\033[91m\033[91mX   \033[0m");
            }
            else{
              if (p[i][j].giocatore) {
                if (p[i][j].codiceGiocatore==giocatore-1) {
                  strcat(msg,"\033[92m\033[92mME  \033[0m");
                }else{
                  strcat(msg,"\033[92m\x1b[35mEN  \033[0m");
                }
              }else{
                if (p[i][j].pacco) {
                  strcat(msg,"\033[93m\033[93mP   \033[0m");
                } else {
                  if(p[i][j].locazione){
                    strcat(msg,"\033[96m\033[96mL   \033[0m");
                  }else{
                    strcat(msg,"[]  ");
                  }
                }
              }
            }
          }
        break;
        case 2:
        if(p[i][j].p1==0)
        strcat(msg,"?   ");
        else{
          if (p[i][j].ostacolo) {
            strcat(msg,"\033[91m\033[91mX   \033[0m");
          }
          else{
            if (p[i][j].giocatore) {
              if (p[i][j].codiceGiocatore==giocatore-1) {
                strcat(msg,"\033[92m\033[92mME  \033[0m");
              }else{
                strcat(msg,"\033[92m\x1b[35mEN  \033[0m");
              }
            }else{
              if (p[i][j].pacco) {
                strcat(msg,"\033[93m\033[93mP   \033[0m");
              } else {
                if(p[i][j].locazione){
                  strcat(msg,"\033[96m\033[96mL   \033[0m");
                }else{
                  strcat(msg,"[]  ");
                }
              }
            }
          }
        }
        break;
        case 3:
        if(p[i][j].p2==0)
          strcat(msg,"?   ");
        else{
          if (p[i][j].ostacolo) {
            strcat(msg,"\033[91m\033[91mX   \033[0m");
          }
          else{
            if (p[i][j].giocatore) {
              if (p[i][j].codiceGiocatore==giocatore-1) {
                strcat(msg,"\033[92m\033[92mME  \033[0m");
              }else{
                strcat(msg,"\033[92m\x1b[35mEN  \033[0m");
              }
            }else{
              if (p[i][j].pacco) {
                strcat(msg,"\033[93m\033[93mP   \033[0m");
              } else {
                if(p[i][j].locazione){
                  strcat(msg,"\033[96m\033[96mL   \033[0m");
                }else{
                  strcat(msg,"[]  ");
                }
              }
            }
          }
        }
        break;
        case 4:
        if(p[i][j].p3==0)
          strcat(msg,"?   ");
        else{
          if (p[i][j].ostacolo) {
            strcat(msg,"\033[91m\033[91mX   \033[0m");
          }
          else{
            if (p[i][j].giocatore) {
              if (p[i][j].codiceGiocatore==giocatore-1) {
                strcat(msg,"\033[92m\033[92mME  \033[0m");
              }else{
                strcat(msg,"\033[92m\x1b[35mEN  \033[0m");
              }
            }else{
              if (p[i][j].pacco) {
                strcat(msg,"\033[93m\033[93mP   \033[0m");
              } else {
                if(p[i][j].locazione){
                  strcat(msg,"\033[96m\033[96mL   \033[0m");
                }else{
                  strcat(msg,"[]  ");
                }
              }
            }
          }
        }
        break;
        case 5:
        if(p[i][j].p4==0)
          strcat(msg,"?   ");
        else{
          if (p[i][j].ostacolo) {
            strcat(msg,"\033[91m\033[91mX   \033[0m");
          }
          else{
            if (p[i][j].giocatore) {
              if (p[i][j].codiceGiocatore==giocatore-1) {
                strcat(msg,"\033[92m\033[92mME  \033[0m");
              }else{
                strcat(msg,"\033[92m\x1b[35mEN  \033[0m");
              }
            }else{
              if (p[i][j].pacco) {
                strcat(msg,"\033[93m\033[93mP   \033[0m");
              } else {
                if(p[i][j].locazione){
                  strcat(msg,"\033[96m\033[96mL   \033[0m");
                }else{
                  strcat(msg,"[]  ");
                }
              }
            }
          }
        }
        break;
        case 6:
        if(p[i][j].p5==0)
          strcat(msg,"?   ");
        else{
          if (p[i][j].ostacolo) {
            strcat(msg,"\033[91m\033[91mX   \033[0m");
          }
          else{
            if (p[i][j].giocatore) {
              if (p[i][j].codiceGiocatore==giocatore-1) {
                strcat(msg,"\033[92m\033[92mME  \033[0m");
              }else{
                strcat(msg,"\033[92m\x1b[35mEN  \033[0m");
              }
            }else{
              if (p[i][j].pacco) {
                strcat(msg,"\033[93m\033[93mP   \033[0m");
              } else {
                if(p[i][j].locazione){
                  strcat(msg,"\033[96m\033[96mL   \033[0m");
                }else{
                  strcat(msg,"[]  ");
                }
              }
            }
          }
        }
        break;
        case 7:
        if(p[i][j].p6==0)
          strcat(msg,"?   ");
        else{
          if (p[i][j].ostacolo) {
            strcat(msg,"\033[91m\033[91mX   \033[0m");
          }
          else{
            if (p[i][j].giocatore) {
              if (p[i][j].codiceGiocatore==giocatore-1) {
                strcat(msg,"\033[92m\033[92mME  \033[0m");
              }else{
                strcat(msg,"\033[92m\x1b[35mEN  \033[0m");
              }
            }else{
              if (p[i][j].pacco) {
                strcat(msg,"\033[93m\033[93mP   \033[0m");
              } else {
                if(p[i][j].locazione){
                  strcat(msg,"\033[96m\033[96mL   \033[0m");
                }else{
                  strcat(msg,"[]  ");
                }
              }
            }
          }
        }
        break;
        case 8:
        if(p[i][j].p7==0)
          strcat(msg,"?   ");
        else{
          if (p[i][j].ostacolo) {
            strcat(msg,"\033[91m\033[91mX   \033[0m");
          }
          else{
            if (p[i][j].giocatore) {
              if (p[i][j].codiceGiocatore==giocatore-1) {
                strcat(msg,"\033[92m\033[92mME  \033[0m");
              }else{
                strcat(msg,"\033[92m\x1b[35mEN  \033[0m");
              }
            }else{
              if (p[i][j].pacco) {
                strcat(msg,"\033[93m\033[93mP   \033[0m");
              } else {
                if(p[i][j].locazione){
                  strcat(msg,"\033[96m\033[96mL   \033[0m");
                }else{
                  strcat(msg,"[]  ");
                }
              }
            }
          }
        }
      }
    }
    strcat(msg,"\n\n");
  }
  return 1;
}

void deleteGrid(GameGrid **g){

  int i;
  for(i=0;i<MAX_GRID_SIZE_H;i++)
    free(g[i]);
  free(g);
  return;
}


void * gestisci(void *arg){
  char msg[1000];
  int n_b_r;
  int sockfd=*((int *) arg);
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
          clear();
          write(sockfd,"-1", strlen("-1"));
          n_b_r = read(sockfd, msg, 50);
          if(strcmp(msg, USER_LOG_OUT)){
            LogUnkownClientDisconnection(&fdLog);
          }
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
        clear();
        write(sockfd, "-1", strlen("-1"));
        n_b_r = read(sockfd, msg, 50);
        if(strcmp(msg, USER_LOG_OUT)){
          LogUserSignOut(&fdLog, user);
        }
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
