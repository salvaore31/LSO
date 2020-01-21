&log.fd#include "Server.h"
#include <time.h>

Log log;
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
    if (argc!=2) {
      printf("Passa il numero porta\n");
      return -1;
    }
    int porta =atoi(argv[1]);
    struct sockaddr_in client_addr;
    socklen_t client_len;
<<<<<<< HEAD
    pthread_mutex_lock(&log.sem);
    LogServerStart(&log.fd);
    pthread_mutex_unlock(&log.sem);
    if((sock = creaSocket(porta))<0){
=======
    LogServerStart(&fdLog);

    if((sock = creaSocket())<0){
>>>>>>> bf9d1e53425aae729a06762491fbbd172bbe78da
      if(sock == ERR_SOCKET_CREATION){
        printf("%s", SOCKET_CREATION_ERR_MESSAGE);
        pthread_mutex_lock(&log.sem);
        LogErrorMessage(&log.fd, SOCKET_CREATION_ERR_MESSAGE);
        pthread_mutex_unlock(&log.sem);
      }else if(sock = ERR_SOCKET_BINDING){
        printf("%s", SOCKET_BINDING_ERR_MESSAGE);
        pthread_mutex_lock(&log.sem);
        LogErrorMessage(&log.fd, SOCKET_BINDING_ERR_MESSAGE);
        pthread_mutex_unlock(&log.sem);
      }else{
        printf("%s", NOT_SURE_ERR_MESSAGE);
        pthread_mutex_lock(&log.sem);
        LogErrorMessage(&log.fd, NOT_SURE_ERR_MESSAGE);
        pthread_mutex_unlock(&log.sem);
      }
    }else{
      if((listen(sock,MAXIMUM_SOCKET_BACKLOG))<0){
        printf("%s", SOCKET_LISTEN_ERR_MESSAGE);
        pthread_mutex_lock(&log.sem);
        LogErrorMessage(&log.fd,SOCKET_LISTEN_ERR_MESSAGE);
        pthread_mutex_unlock(&log.sem);
      }else{
        while(1){
          client_len = sizeof(client_addr);
          if((sockfd = accept(sock, NULL, NULL))<0){
            printf("%s", ACCEPT_SOCKET_ERR_MESSAGE);
            pthread_mutex_lock(&log.sem);
            LogErrorMessage(&log.fd, ACCEPT_SOCKET_ERR_MESSAGE);
            pthread_mutex_unlock(&log.sem);
            exit(-1);
          }

          int* thread_sd;
          thread_sd = (int*) malloc(sizeof(int));
          thread_sd = &sockfd;
          if((pthread_create(&tid, NULL, run, (void *) thread_sd))<0){
              printf("%s", THREAD_CREATION_ERR_MESSAGE);
              pthread_mutex_lock(&log.sem);
              LogErrorMessage(&log.fd, THREAD_CREATION_ERR_MESSAGE);
              pthread_mutex_unlock(&log.sem);
          }
        }
      }
    }
    close(sock);
    close(sockfd);
    pthread_mutex_lock(&log.sem);
    LogServerClose(&log.fd);
    pthread_mutex_unlock(&log.sem);
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
      pthread_mutex_lock(&log.sem);
      LogPlayerJoin(&log.fd, g->gameId, user);
      pthread_mutex_unlock(&log.sem);
      GameGridToText(g->grid,matrix,1);
      pthread_mutex_unlock(&g->sem);
<<<<<<< HEAD
      sendMsgNoReply(sockfd,matrix);
      pthread_t tid;
      int* thread_sd;
      thread_sd = (int*) malloc(sizeof(int));
      thread_sd = &sockfd;
      if((pthread_create(&tid, NULL, timer, (void *) thread_sd))<0){
          printf("%s", THREAD_CREATION_ERR_MESSAGE);
          pthread_mutex_lock(&log.sem);
          LogErrorMessage(&log.fd, THREAD_CREATION_ERR_MESSAGE);
          pthread_mutex_unlock(&log.sem);
      }
=======
      n_b_r= sendMsg(sockfd, matrix,msg);
>>>>>>> bf9d1e53425aae729a06762491fbbd172bbe78da
      playGame(g,0,g->gameId);
    }else{
      /*Gestione errore*/
    }
  }else{
    pthread_mutex_lock(&log.sem);
    LogNewGame(&log.fd,gameId);
    pthread_mutex_unlock(&log.sem);
  }
  return;
}


/*
  La funzione che esegue ogni nuovo thread;
*/
void * run(void *arg){
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
          logInUserMenu(sockfd,user,&log);
          baba=1;
          break;
        case 'r': case 'R':
          signInUserMenu(sockfd,user,&log);
          baba=1;
          break;
        case 'e': case 'E':
          clear();
          write(sockfd,"-1", strlen("-1"));
          n_b_r = read(sockfd, msg, 50);
          if(strcmp(msg, USER_LOG_OUT)){
            pthread_mutex_lock(&log.sem);
            LogUnkownClientDisconnection(&log.fd);
            pthread_mutex_unlock(&log.sem);
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

void * timer(void *arg){

  int sockfd=*((int *)arg);
  char mom[]="\033[91mTEMPO SCADUTO\033[0m\n";
  sleep(MAX_TIME);
  sendMsgNoReply(sockfd,mom);
  pthread_mutex_lock(&log.sem);
  LogEndGame(&log.fd,getpid());
  pthread_mutex_unlock(&log.sem);
}


void handleSignal(int Sig){
  if(Sig == SIGINT){
    pthread_mutex_lock(&log.sem);
    LogServerClose(&log.fd);
    pthread_mutex_unlock(&log.sem);
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

<<<<<<< HEAD
Game* joinGame(int sockfd, char user[]){
  printf("prima di printList.\n");
=======
Game* joinGame(int sockfd, char user[], int fdLog){
>>>>>>> bf9d1e53425aae729a06762491fbbd172bbe78da

  return NULL;
}
