#include "Server.h"
#include <time.h>

/*
  VARIABILI GLOBALI:
  LogFile serverLog, una struttura dati contenente il file di Log della specifica sesssione server e il mutex che ne regola l'accesso;
  Game *g, un puntatore alla partita in corso, inizialmente non allocata (definizione di Game in costanti.h);
  int gameId=-1, l'id della partita che verrà assegnato al momento dell'allocazione della stessa;
  loggedUser loggati, una struttura dati cotenente i giocatori correntemente loggati e il mutex che ne regola l'accesso;
  pthread_t clientsId[MAX_PLAYER_N], un array contentene il tid dei thread giocanti utilizzato per gestire il fine partita;
  int gameEnded = 0, un valore utilizzato per indicare al Server quando si trova all'interno di una partita;
  */

LogFile serverLog;
Game *g;
int gameId=-1;
loggedUser loggati;
pthread_t clientsId[MAX_PLAYER_N];
int gameEnded = 0;
struct sockaddr_in client_addr;

int main(int argc, char* argv[]){

  int i = 0;
  for(i = 0; i<MAX_PLAYER_N; i++){
    clientsId[i] = 0;
  }
  i = 0;

  clear();
  system("/sbin/ifconfig");
  //seed per la generazione di numeri casuali;
  srand(time(NULL));
  //signal handler set;
  signal(SIGINT, handleSignal);
  signal(SIGHUP, handleSignal);
  signal(SIGQUIT, handleSignal);
  signal(SIGTERM, handleSignal);
  signal(SIGALRM, handleSignal);
  signal(SIGSEGV, handleSignal);
  signal(SIGPIPE, handleSignal);

  if (argc!=2) {
    printf("Passa il numero della porta.\n");
    return -1;
  }

  int *thread_sd, sock, sockfd,porta =atoi(argv[1]), pid;
  pthread_t tid;

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
        while(1){
          while(!gameEnded){
            client_len = sizeof(client_addr);
            if((sockfd = accept(sock,(struct sockaddr *) &client_addr, &client_len))<0){
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
            clientsId[i]=tid;
            i++;
          }
          pthread_create(&tid, NULL, endGameManagement, NULL);
          pthread_join(tid, NULL);
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
  La funzione che esegue ogni nuovo thread;}

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
          result = logInUserMenu(sockfd,user,&serverLog,&loggati,inet_ntoa(client_addr.sin_addr));
          if(result == ERR_NO_USER_FILE || result == ERR_INPUT_OUTPUT){
            clear();
            write(sockfd,"-1", strlen("-1"));
            n_b_r = read(sockfd, msg, 50);
            if(strcmp(msg, USER_LOG_OUT)){
              pthread_mutex_lock(&serverLog.sem);
              LogUnknownClientDisconnection(&serverLog.fd,inet_ntoa(client_addr.sin_addr));
              pthread_mutex_unlock(&serverLog.sem);
            }
            pthread_exit((int*)1);
            break;
          }
          done = 1;
          break;
        case 'r': case 'R':
          result = signInUserMenu(sockfd,user,&serverLog,&loggati,inet_ntoa(client_addr.sin_addr));
          if(result == ERR_NO_USER_FILE || result == ERR_INPUT_OUTPUT){
            clear();
            write(sockfd,"-1", strlen("-1"));
            n_b_r = read(sockfd, msg, 50);
            if(strcmp(msg, USER_LOG_OUT)){
              pthread_mutex_lock(&serverLog.sem);
              LogUnknownClientDisconnection(&serverLog.fd,inet_ntoa(client_addr.sin_addr));
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
            LogUnknownClientDisconnection(&serverLog.fd,inet_ntoa(client_addr.sin_addr));
            pthread_mutex_unlock(&serverLog.sem);
          }
          pthread_exit((int*)1);
          break;
        default:
          n_b_r=sendMsg(sockfd,WELCOME_MESSAGE,msg);
          if(n_b_r<0){
            pthread_exit((int*)-1);
          }
          break;
      }
    }else{
          n_b_r=sendMsg(sockfd,WELCOME_MESSAGE,msg);
          if(n_b_r<0){
            pthread_exit((int*)-1);
          }
    }
  }
  if(g==NULL){
    initializaNewGame(&g,sockfd,user,&serverLog,&loggati);
  }else{
    spawnNewPlayer(&g,user, sockfd, &serverLog,&loggati);
  }
}

void handleSignal(int Sig){
  switch (Sig) {
    case SIGINT:
      pthread_mutex_lock(&serverLog.sem);
      LogServerClose(&serverLog.fd);
      pthread_mutex_unlock(&serverLog.sem);
      exit(1);
    break;
    case SIGALRM:
      alarm(0);
      pthread_mutex_lock(&g->sem);
      g->timeOver = 1;
      gameEnded = 1;
      pthread_mutex_unlock(&g->sem);
    break;
    case SIGPIPE:
    
    break;
  }
}

void * endGameManagement(void * arg){

  for(int i = 0; i<0; i++){
    if(clientsId[i]!=0){
      pthread_join(clientsId[i], NULL);
    }
  }
  if(g!= NULL){
    pthread_mutex_lock(&g->sem);
    deleteGrid(g->grid);
    Game * tmp = g;
    pthread_mutex_lock(&serverLog.sem);
    LogEndGame(&serverLog.fd,g->gameId);
    pthread_mutex_unlock(&serverLog.sem);
    pthread_mutex_unlock(&g->sem);
    g = NULL;
    free(tmp);
  }
  gameEnded = 0;
  pthread_exit((int *) 1);
}
