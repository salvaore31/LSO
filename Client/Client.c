#include <stdio.h>
#include <stdlib.h>
#include "Client.h"

int main(int argc, char* argv[]){

  /*int inputInt;
  char[MAX_SIZE_USERNAME] inputUser;

  printf("Benvenuto in questo gioco di merda.\nCosa vuoi fare:\n1)Registrati.\n2)Accedi.\n3)Esci.\n");
  scanf("%d",&inputInt);
  while(inputInt<4&&inputInt>0)
  switch (inputInt) {
    case 1 :signInRequest();
    break;
    case 2: logInRequest();
    break;
    case 3: return 0;
    default: printf("Da 1 a 3 Bucchin\n");
    scanf("%d",&inputInt);
  }
  //Da questo punto in poi il Client è Loggato
  printf("Benvenuto %s.\nCosa vuoi fare:\n1)Nuova partita.\n2)Entra in una partita.\n3)Esci.", inputUser);
  scanf("%d",&inputInt);
  while(inputInt<4&&inputInt>0)
  switch (inputInt) {
    case 1 :newGame();
    break;
    case 2: joinGame();
    break;
    case 3: return 0;
    default: printf("Da 1 a 3 Bucchin\n");
    scanf("%d",&inputInt);
  }
  //Da questo punto in poi il Client si trova in gioco
  while(/*Partita ancora in corso){
    printGameGrid();
    movementRequest();
  }*/
  int sockfd,n_b_r;
  char msg[250],input;
  struct sockaddr_un server_addr;
  if ((sockfd=socket(PF_LOCAL,SOCK_STREAM,0))<0) {
    printf("Errore apertura socket");
  }else{

    server_addr.sun_family=AF_LOCAL;
    strcpy(server_addr.sun_path,MIO_SOCK);

    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
      printf("Errore connessione socket\n");
    }else{
      beforeLogin(sockfd);
      clear();
    }
  }
  
  close(sockfd);
  return 0;
}

int beforeLogin(int sockfd){

  int n_b_r, logged=0;
  char msg[250],input[50];
  while(1){
    n_b_r=read(sockfd,msg,250);
    msg[n_b_r]='\0';
    printf("%s",msg);
    if(strcmp(msg,SUCCESS_MESSAGE_LIM)==0)
      return 1;
    if(strcmp(msg,ERR_NO_CONNECTION)==0 || strcmp(msg,"-1")==0 )
      return -1;
    scanf("%s",input);
    write(sockfd,input,strlen(input));
  }
}
