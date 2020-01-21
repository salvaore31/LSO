#include "Communication.h"

/*
  La funzione creaSocket si occupa di creare un socket per la comunicazione
  Client Server.
*/
int creaSocket(int porta){
  struct sockaddr_in server_address;
  int sock;
  if((sock=socket(AF_INET, SOCK_STREAM, 0)) < 0){
    return ERR_SOCKET_CREATION ;
  }else{
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(porta);
    memset(&(server_address.sin_zero),'\0',8);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if((bind(sock,(struct sockaddr *)&server_address,sizeof(server_address)))<0){
      return ERR_SOCKET_BINDING;
    }
  }
  return sock;
}

int GameGridToText(GameGrid **p, char msg[], int giocatore){

  int i,j;
  char mom[150];
  msg[0]='\0';
  for(i=0;i<MAX_GRID_SIZE_L+1;i++){
    if(i!=0){
      if(i/10==0)
        sprintf(mom,"\033[30;47m%d   \033[0m",i);
      else
        sprintf(mom,"\033[30;47m%d  \033[0m",i);
      strcat(msg,mom);
    }else{
      sprintf(mom,"    ");
      strcat(msg, mom);
    }
  }
  sprintf(mom, "\n");
  strcat(msg, mom);
  for(i=0;i<MAX_GRID_SIZE_H;i++){
    if(i+1!=10)
      sprintf(mom,"\033[30;47m%d   \033[0m",i+1);
    else
      sprintf(mom,"\033[30;47m%d  \033[0m",i+1);
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

int sendMsgNoReply(int sockfd,char toSend[]){

  int n_b_r, n_b_w;
  char msg[150];

  clear();
  sprintf(msg,"%ld",strlen(toSend));
  n_b_w=write(sockfd,msg,strlen(msg));
  if(n_b_w<strlen(msg)){
      return ERR_SENDING_MESSAGE;
  }
  n_b_r=read(sockfd,msg,5);
  if (n_b_r<0) {
    return ERR_RECEIVING_MESSAGE;
  }
  n_b_w=write(sockfd,toSend,strlen(toSend));
  if (n_b_w<strlen(toSend)) {
    return ERR_SENDING_MESSAGE;
  }
  return n_b_w;
}

int sendMsg(int sockfd,char toSend[],char received[]){

  int n_b_r, n_b_w;
  char msg[150];

  clear();
  sprintf(msg,"%ld",strlen(toSend));
  n_b_w=write(sockfd,msg,strlen(msg));
  if(n_b_w<strlen(msg)){
      return ERR_SENDING_MESSAGE;
  }
  n_b_r=read(sockfd,msg,5);
  if (n_b_r<0) {
    return ERR_RECEIVING_MESSAGE;
  }
  n_b_w=write(sockfd,toSend,strlen(toSend));
  if (n_b_w<strlen(toSend)) {
    return ERR_SENDING_MESSAGE;
  }
  n_b_r=read(sockfd,received,50);
  if (n_b_r<0) {
    return ERR_RECEIVING_MESSAGE;
  }
  received[n_b_r]='\0';
  return n_b_r;

}
