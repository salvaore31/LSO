#include "Server.h"
#include <time.h>

int fdLog;
Game *g;
int main(int argc, char* argv[]){

    clear();
    //seed per la generazione di numeri casuali;
    srand(time(NULL));

    //signal handler set;
    signal(SIGINT, handleSignal);
    signal(SIGHUP, handleSignal);
    signal(SIGQUIT, handleSignal);
    signal(SIGTERM, handleSignal);
    signal(SIGKILL, handleSignal);
    int *thread_sd, sock, sockfd;
    pthread_t tid;

    struct sockaddr_in client_addr;
    socklen_t client_len;
    LogServerStart(&fdLog);/*
    sessionGamesSem =(pthread_mutex_t *) mmap(NULL, (sizeof(pthread_mutex_t)), PROT_READ|PROT_WRITE,
                        MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    pthread_mutex_lock(&sessionGamesSem);
    printf("Fino a qui tutto bene\n" );
    sessionGames = (Game * ) initializeGamesArray(sessionGames);
    pthread_mutex_unlock(&sessionGamesSem);

    if(sessionGames == MAP_FAILED){
      printf("%s.\n", MAP_FAILED_ERR_MESSAGE);
      LogErrorMessage(&fdLog, MAP_FAILED_ERR_MESSAGE);
      LogServerClose(&fdLog);
      return 1;
    }*/
    if((sock = creaSocket())<0){
      if(sock == ERR_SOCKET_CREATION){
        printf("%s", SOCKET_CREATION_ERR_MESSAGE);
        LogErrorMessage(&fdLog, SOCKET_CREATION_ERR_MESSAGE);
      }else if(sock = ERR_SOCKET_BINDING){
        printf("%s", SOCKET_BINDING_ERR_MESSAGE);
        LogErrorMessage(&fdLog, SOCKET_BINDING_ERR_MESSAGE);
      }else{
        printf("%s", NOT_SURE_ERR_MESSAGE);
        LogErrorMessage(&fdLog, NOT_SURE_ERR_MESSAGE);
      }
    }else{
      if((listen(sock,MAXIMUM_SOCKET_BACKLOG))<0){
        printf("%s", SOCKET_LISTEN_ERR_MESSAGE);
        LogErrorMessage(&fdLog,SOCKET_LISTEN_ERR_MESSAGE);
      }else{
        while(1){
          client_len = sizeof(client_addr);
          if((sockfd = accept(sock, NULL, NULL))<0){
            printf("%s", ACCEPT_SOCKET_ERR_MESSAGE);
            LogErrorMessage(&fdLog, ACCEPT_SOCKET_ERR_MESSAGE);
            exit(-1);
          }

          int* thread_sd;
          thread_sd = (int*) malloc(sizeof(int));
          thread_sd = &sockfd;
          if((pthread_create(&tid, NULL, gestisci, (void *) thread_sd))<0){
              printf("%s", THREAD_CREATION_ERR_MESSAGE);
              LogErrorMessage(&fdLog, THREAD_CREATION_ERR_MESSAGE);
          }
        }
      }
    }
    close(sock);
    close(sockfd);
    LogServerClose(&fdLog);
    return 1;
}


/*
  La funzione initializeNewGameProcess si occupa di creare la fork per una nuova partita.
*/
void initializeNewGameProcess(int sockfd, char user[]){

  int gameId;
  if((gameId=fork())<0){
    /*Gestire errore nuova partita*/
  }else if(gameId==0){
    char matrix[2000];
    char msg[50];
    int n_b_r;
    g=createGame();
    strcpy(g->giocatori[0].nome,user);
    g->giocatori[0].nome[strlen(user)]='\0';
    if(createGameGrid(g) == 0){
      pthread_mutex_lock(&g->sem);
      g->gameId = getpid();
      LogPlayerJoin(&fdLog, g->gameId, user);
      GameGridToText(g->grid,matrix,1);
      pthread_mutex_unlock(&g->sem);
      sprintf(msg,"%ld",strlen(matrix));
      write(sockfd,msg,strlen(msg));
      n_b_r=read(sockfd,msg,5);
      write(sockfd,matrix,strlen(matrix));
      playGame(g,0,g->gameId);
    }else{
      sprintf(msg,"%ld",strlen(NO_CONNECTION_ERR_MESSAGE));
      write(sockfd,msg,strlen(msg));
      n_b_r=read(sockfd,msg,5);
      write(sockfd,NO_CONNECTION_ERR_MESSAGE,strlen(NO_CONNECTION_ERR_MESSAGE));

    }
  }else{
    LogNewGame(&fdLog,gameId);
  }
  return;
}


/*
  La funzione che esegue ogni nuovo thread;
*/
void * gestisci(void *arg){
  Game* current;
  char msg[1000];
  int n_b_r;
  int sockfd=*((int *)arg);
  char user[50];
  int baba=-1;

  n_b_r=sendMsg(sockfd,WELCOME_MESSAGE,msg);
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
          n_b_r=sendMsg(sockfd,WELCOME_MESSAGE,msg);
          break;
      }
    }else{
          n_b_r=sendMsg(sockfd,WELCOME_MESSAGE,msg);
    }
  }
  read(sockfd,msg,1);
  if(g==NULL){
    initializeNewGameProcess(sockfd,user);
  }
}


void handleSignal(int Sig){
  if(Sig == SIGINT){
    LogServerClose(&fdLog);
    //unlink(SOCKET);
    exit(1);
  }
}

void deleteGrid(GameGrid **g){
  int i;
  for(i=0;i<MAX_GRID_SIZE_H;i++)
    free(g[i]);
  free(g);
  return;
}

Game* joinGame(int sockfd, char user[], int fdLog){
  printf("prima di printList.\n");

  return NULL;
}
