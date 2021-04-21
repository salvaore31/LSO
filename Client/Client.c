#include "Client.h"

int sockfd,ingame=0,logged=0, connected=0;
int main(int argc, char* argv[]){
  signal(SIGINT, handleSignal);
  clear();
  int n_b_r;
  char msg[500],input[100],c;
  if(argc!=3){
    printf("Passa indirizzo e porta\n");
    return(0);
  }
  struct sockaddr_in server_addr;
  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(atoi(argv[2]));
  inet_aton(argv[1],&server_addr.sin_addr);
  if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0) {
    printf("Errore apertura socket");
  }else{
    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
      printf("Errore connessione socket %d\n", errno);
    }else{
      connected=1;
      logged=comunication(sockfd);
      if (logged<0) {
        clear();
        if(logged!=-5)
          printf("Al momento risulta impossibile eseguire operazione di Log-In e Sign-In.\nRiprova tra poco.\n");
      }else{
        clear();
        ingame=comunicationGame(sockfd);
      }
    }
  }
  printf("Arrivederci.\n");
  close(sockfd);
  return 0;
}

void handleSignal(int sig){
  if(sig==SIGINT || sig==SIGHUP || sig==SIGQUIT || sig==SIGTERM){
    clear();
    fflush(STDIN_FILENO);
    if(logged==1)
      write(sockfd,USER_LOG_OUT,strlen(USER_LOG_OUT));
    else if(connected==1)
      write(sockfd,CLIENT_GONE,strlen(CLIENT_GONE));
    close(sockfd);
    system("reset");
    printf("Arrivederci.\n");
    exit(-1);
  }
}

int comunication(int sockfd){
  int n_b_r,c;
  char msg[250],input[1000];
  while(1){
    leggi();
    if(goOn(msg)){
      return 1;
    }else{
      if( (strcmp(msg, "-1")==0) ){
        return -1;
      }else{
        if (strcmp(msg,WELCOME_MESSAGE)==0) {
          scanf("%1s",input);
          if ( (strcmp(input,"e")==0) || (strcmp(input,"E")==0) ) {
            raise(SIGINT);
          }
        }else{
          scanf("%20s",input);
        }
        while((c = getchar()) != '\n' && c != EOF);
        write(sockfd,input,strlen(input));
        clear();
      }
    }
  }
}

int comunicationGame(int sockfd){
  int n_b_r;
  struct termios orig;
  clear();
  char msg[4000],input[100];
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  orig = raw;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  while(1){
    leggi();
    if(strcmp(msg,"GETOUT")==0){
      break;
    }else if((strcmp(msg, VICTORY_MESSAGE)==0) || (strcmp(msg, LOSS_MESSAGE)==0)){
      clear();
      printf("%s", msg);
      sleep(4);
      break;
    }
    read(STDIN_FILENO, input, 1);
    write(sockfd,input,1);
    fflush(STDIN_FILENO);
    clear();
  }
  clear();
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
  return 0;
}

int goOn(char msg[]){
  return ((strcmp(msg,SUCCESS_MESSAGE_LIM)==0) || (strcmp(msg,SUCCESS_MESSAGE_SIM)==0));
}
