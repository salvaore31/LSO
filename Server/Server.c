#include "Server.h"
#include <time.h>

LogFile serverLog;
Game *g;
int gameId=-1;

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
    int *thread_sd, sock, sockfd,porta =atoi(argv[1]), pid;
    pthread_t tid;
    if (argc!=2) {
      printf("Passa il numero porta\n");
      return -1;
    }
    struct sockaddr_in client_addr;
    socklen_t client_len;
    pthread_mutex_lock(&serverLog.sem);
    LogServerStart(&serverLog.fd);
    pthread_mutex_unlock(&serverLog.sem);
    if((sock = creaSocket(porta))<0){
      if(sock == ERR_SOCKET_CREATION){
        printf("%s", SOCKET_CREATION_ERR_MESSAGE);
        pthread_mutex_lock(&serverLog.sem);
        LogErrorMessage(&serverLog.fd, SOCKET_CREATION_ERR_MESSAGE);
        pthread_mutex_unlock(&serverLog.sem);
      }else if(sock = ERR_SOCKET_BINDING){
        printf("%s", SOCKET_BINDING_ERR_MESSAGE);
        pthread_mutex_lock(&serverLog.sem);
        LogErrorMessage(&serverLog.fd, SOCKET_BINDING_ERR_MESSAGE);
        pthread_mutex_unlock(&serverLog.sem);
      }else{
        printf("%s", NOT_SURE_ERR_MESSAGE);
        pthread_mutex_lock(&serverLog.sem);
        LogErrorMessage(&serverLog.fd, NOT_SURE_ERR_MESSAGE);
        pthread_mutex_unlock(&serverLog.sem);
      }
    }else{
      if((listen(sock,MAXIMUM_SOCKET_BACKLOG))<0){
        printf("%s", SOCKET_LISTEN_ERR_MESSAGE);
        pthread_mutex_lock(&serverLog.sem);
        LogErrorMessage(&serverLog.fd,SOCKET_LISTEN_ERR_MESSAGE);
        pthread_mutex_unlock(&serverLog.sem);
      }else{
        pid=fork();
        if (pid<0) {
          /* code */
        } else if(pid>0){
          /* code */
        }else{
          gameId=getpid();
          while(1){
            client_len = sizeof(client_addr);
            if((sockfd = accept(sock, NULL, NULL))<0){
              printf("%s", ACCEPT_SOCKET_ERR_MESSAGE);
              pthread_mutex_lock(&serverLog.sem);
              LogErrorMessage(&serverLog.fd, ACCEPT_SOCKET_ERR_MESSAGE);
              pthread_mutex_unlock(&serverLog.sem);
              exit(-1);
            }

            int* thread_sd;
            thread_sd = (int*) malloc(sizeof(int));
            thread_sd = &sockfd;
            if((pthread_create(&tid, NULL, run, (void *) thread_sd))<0){
                printf("%s", THREAD_CREATION_ERR_MESSAGE);
                pthread_mutex_lock(&serverLog.sem);
                LogErrorMessage(&serverLog.fd, THREAD_CREATION_ERR_MESSAGE);
                pthread_mutex_unlock(&serverLog.sem);
            }
          }
        }
      }
    close(sock);
    close(sockfd);
    pthread_mutex_lock(&serverLog.sem);
    LogServerClose(&serverLog.fd);
    pthread_mutex_unlock(&serverLog.sem);
    return 1;
  }
}
/*
  La funzione initializaNewGame si occupa di creare la fork per una nuova partita.
*/
void initializaNewGame(int sockfd, char user[]){


  char matrix[2000];
  char msg[50];
  int n_b_r;

  g=createGame();
  strcpy(g->giocatori[0].nome,user);
  g->giocatori[0].nome[strlen(user)]='\0';

  if(createGameGrid(g) == 0){
    pthread_mutex_lock(&g->sem);
    g->gameId = getpid();

    pthread_mutex_lock(&serverLog.sem);
    LogNewGame(&serverLog.fd,gameId);
    pthread_mutex_unlock(&serverLog.sem);
    pthread_mutex_lock(&serverLog.sem);
    LogPlayerJoin(&serverLog.fd, g->gameId, user);
    pthread_mutex_unlock(&serverLog.sem);

    pthread_mutex_unlock(&g->sem);

    pthread_t tid;
    int* thread_sd;
    thread_sd = (int*) malloc(sizeof(int));
    thread_sd = &sockfd;

    if((pthread_create(&tid, NULL, timer, (void *) thread_sd))<0){
        printf("%s", THREAD_CREATION_ERR_MESSAGE);
        pthread_mutex_lock(&serverLog.sem);
        LogErrorMessage(&serverLog.fd, THREAD_CREATION_ERR_MESSAGE);
        pthread_mutex_unlock(&serverLog.sem);
    }


    playGame(g,0,g->gameId,sockfd,&serverLog);
  }else{
    /*Gestione errore*/
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
          logInUserMenu(sockfd,user,&serverLog);
          baba=1;
          break;
        case 'r': case 'R':
          signInUserMenu(sockfd,user,&serverLog);
          baba=1;
          break;
        case 'e': case 'E':
          clear();
          write(sockfd,"-1", strlen("-1"));
          n_b_r = read(sockfd, msg, 50);
          if(strcmp(msg, USER_LOG_OUT)){
            pthread_mutex_lock(&serverLog.sem);
            LogUnkownClientDisconnection(&serverLog.fd);
            pthread_mutex_unlock(&serverLog.sem);
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
    initializaNewGame(sockfd,user);
  }else{
    spawnNewPlayer(g,user, sockfd, &serverLog);
  }
}

void * timer(void *arg){

  int sockfd=*((int *)arg);
  char mom[]="\033[91mTEMPO SCADUTO\033[0m\n";
  sleep(MAX_TIME);
  sendMsgNoReply(sockfd,mom);
  pthread_mutex_lock(&serverLog.sem);
  LogEndGame(&serverLog.fd,getpid());
  pthread_mutex_unlock(&serverLog.sem);
}


void handleSignal(int Sig){
  if(Sig == SIGINT){
    pthread_mutex_lock(&serverLog.sem);
    LogServerClose(&serverLog.fd);
    pthread_mutex_unlock(&serverLog.sem);
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
  return NULL;
}
