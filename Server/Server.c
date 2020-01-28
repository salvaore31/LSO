#include "Server.h"
#include <time.h>

LogFile serverLog;
Game *g;
int gameId=-1;
loggedUser loggati;

int main(int argc, char* argv[]){

  clear();
  //seed per la generazione di numeri casuali;
  srand(time(NULL));
  //signal handler set;
  signal(SIGINT, handleSignal);
  signal(SIGHUP, handleSignal);
  signal(SIGQUIT, handleSignal);
  signal(SIGTERM, handleSignal);
  signal(SIGALRM, handleSignal);
  signal(SIGSEGV, handleSignal);

  if (argc!=2) {
    printf("Passa il numero della porta.\n");
    return -1;
  }

  int *thread_sd, sock, sockfd,porta =atoi(argv[1]), pid;
  pthread_t tid;

  struct sockaddr_in client_addr;
  socklen_t client_len;

  pthread_mutex_lock(&serverLog.sem);
  LogServerStart(&serverLog.fd);
  pthread_mutex_unlock(&serverLog.sem);
  initializaLoggedUser(&loggati);
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

/*
  La funzione che esegue ogni nuovo thread;
*/
void * run(void *arg){
  char msg[1000];
  int n_b_r;
  int sockfd=*((int *)arg);
  char user[50];
  int done=0, result;

  n_b_r=sendMsg(sockfd,WELCOME_MESSAGE,msg);
  if(n_b_r<0){
    pthread_exit((int*)-1);
  }
  while(!done){
    if(n_b_r==1){
      switch (msg[0]) {
        case 'l': case 'L':
          result = logInUserMenu(sockfd,user,&serverLog,&loggati);
          if(result == ERR_NO_USER_FILE || result == ERR_INPUT_OUTPUT){
            clear();
            write(sockfd,"-1", strlen("-1"));
            n_b_r = read(sockfd, msg, 50);
            if(strcmp(msg, USER_LOG_OUT)){
              pthread_mutex_lock(&serverLog.sem);
              LogUnkownClientDisconnection(&serverLog.fd);
              pthread_mutex_unlock(&serverLog.sem);
            }
            pthread_exit((int*)1);
            break;
          }
          done = 1;
          break;
        case 'r': case 'R':
          result = signInUserMenu(sockfd,user,&serverLog,&loggati);
          if(result == ERR_NO_USER_FILE || result == ERR_INPUT_OUTPUT){
            clear();
            write(sockfd,"-1", strlen("-1"));
            n_b_r = read(sockfd, msg, 50);
            if(strcmp(msg, USER_LOG_OUT)){
              pthread_mutex_lock(&serverLog.sem);
              LogUnkownClientDisconnection(&serverLog.fd);
              pthread_mutex_unlock(&serverLog.sem);
            }
            pthread_exit((int*)1);
            break;
          }
          done = 1;
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
          pthread_exit((int*)1);
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
    initializaNewGame(&g,sockfd,user,&serverLog,&loggati);
  }else{
    spawnNewPlayer(&g,user, sockfd, &serverLog,&loggati);
  }
}

void handleSignal(int Sig){
  switch (Sig) {
    case SIGSEGV:
      free(g);
      g=NULL;
      initializaLoggedUser(&loggati);
      printf("%d\n",errno);
      printf("%s\n","dovrebbe essere sigsegv ma invece no" );
    break;
    case SIGINT:
      pthread_mutex_lock(&serverLog.sem);
      LogServerClose(&serverLog.fd);
      pthread_mutex_unlock(&serverLog.sem);
      exit(1);
    break;
    case SIGALRM:
      pthread_mutex_lock(&g->sem);
      g->timeOver = 1;
      pthread_mutex_unlock(&g->sem);
    break;
  }
}
