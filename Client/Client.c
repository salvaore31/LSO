#include "Client.h"

int sockfd,ingame=0,logged=0;

int main(int argc, char* argv[]){
  signal(SIGINT, handleSignal);
  clear();
  int n_b_r;
  char msg[500],input[10];
  if(argc!=3){
    perror("Passa indirizzo e porta\n");
    exit(-1);
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
      logged=comunication(sockfd);
      if (logged<0) {
        exit(-1);
      }
      scanf("%s",input);
      write(sockfd,input,strlen(input));
      clear();

      ingame=comunicationGame(sockfd);
      if(ingame<0){
        write(sockfd,USER_LOG_OUT,strlen(USER_LOG_OUT));
      //  clear();
        close(sockfd);
        exit(-1);
      }
      scanf("%s",input);
      write(sockfd,input,strlen(input));
      clear();

    }
  }
  write(sockfd,USER_LOG_OUT,strlen(USER_LOG_OUT));
  close(sockfd);
  return 0;
}
void handleSignal(int sig){
  if(sig==SIGINT){
    if(logged)
      write(sockfd,USER_LOG_OUT,strlen(USER_LOG_OUT));
    clear();
    exit(-1);
  }
}

int comunication(int sockfd){
  int n_b_r;
  char msg[250],input[50];
  while(1){
    leggi();
    if(goOn(msg)){
      return 1;
    }else{
      if(goOut(msg))
        return -1;
      else{
        scanf("%s",input);
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
  char msg[4000],input[50];
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  orig = raw;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  while(1){
    leggi();
    read(STDIN_FILENO, input, 1);
    write(sockfd,input,strlen(input));
    clear();
  }
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
  return 0;
}




int selectGame(int sockfd){
  int n_b_r;
  char msg[250],input[50];
  while(1){
    leggi();
    if(goOn(msg)){
      return 1;
    }else{
      if(goOut(msg))
        return -1;
      else{
        scanf("%s",input);
        write(sockfd,input,strlen(input));
        clear();
      }
    }
  }
}

int goOn(char msg[]){
  return ((strcmp(msg,SUCCESS_MESSAGE_LIM)==0) || (strcmp(msg,SUCCESS_MESSAGE_SIM)==0));
}

int goOut(char msg[]){
  return (strcmp(msg,NO_CONNECTION_ERR_MESSAGE)==0 || strcmp(msg,"-1")==0);
}
