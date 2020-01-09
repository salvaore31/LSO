#include <stdio.h>
#include <stdlib.h>
#include "Client.h"

int sockfd,ingame=0,logged=0;

int main(int argc, char* argv[]){
  signal(SIGINT, handleSignal);
  clear();
  int n_b_r;
  char msg[500],input[10];
  struct sockaddr_un server_addr;
  if ((sockfd=socket(PF_LOCAL,SOCK_STREAM,0))<0) {
    printf("Errore apertura socket");
  }else{

    server_addr.sun_family=AF_LOCAL;
    strcpy(server_addr.sun_path,MIO_SOCK);

    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
      printf("Errore connessione socket\n");
    }else{
      sleep(5);
      logged=comunication(sockfd);
      if (logged<0) {
        printf("LET ME OUT\n" );
        write(sockfd,USER_LOG_OUT,sizeof(USER_LOG_OUT));
        //clear();
        close(sockfd);
        exit(-1);
      }
      scanf("%s",input);
      write(sockfd,input,strlen(input));
      clear();
      ingame=comunication(sockfd);
      if(ingame<0){
        printf("NON SONO INGAME");
        write(sockfd,USER_LOG_OUT,sizeof(USER_LOG_OUT));
      //  clear();
        close(sockfd);
        exit(-1);
      }
      scanf("%s",input);
      write(sockfd,input,strlen(input));
      clear();


    }
  }
  write(sockfd,USER_LOG_OUT,sizeof(USER_LOG_OUT));
  close(sockfd);
  return 0;
}
void handleSignal(int sig){
  if(sig==SIGINT){
    if(logged)
      write(sockfd,USER_LOG_OUT,sizeof(USER_LOG_OUT));
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
