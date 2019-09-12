#include "Server.h"
#include <malloc.h>
int main(int argc, char const *argv[]) {

  srand(time(NULL));

  clear();
  unsigned int giocatore=1;
  int i,j;
  GameGrid **p;
  char msg[1024*5];
  msg[0]='\0';
  p=(GameGrid**)malloc(MAX_GRID_SIZE_H * sizeof(GameGrid*));

  for(i=0;i<MAX_GRID_SIZE_H;i++)
    p[i]=(GameGrid*)calloc(MAX_GRID_SIZE_L, sizeof(GameGrid));

  for(i=0;i<MAX_GRID_SIZE_H;i++){
    for(j=0;j<MAX_GRID_SIZE_L;j++){
      p[i][j].infocasella=rand()%4;
      p[i][j].playerI=rand()%8;
      p[i][j].playerJ=rand()%8;
      p[i][j].permessi=rand()%3;//%256;
    }
  }
  for(i=0;i<MAX_GRID_SIZE_H;i++){
    for(j=0;j<MAX_GRID_SIZE_L;j++){
      if(p[i][j].permessi-giocatore!=(2-giocatore))
        strcat(msg,"? ");
      else{
        switch(p[i][j].infocasella){
          case 0:
            //printf(GREEN);
            strcat(msg,"\033[92m\033[92m0 \033[0m");
            //printf(RESET);
            break;
          case 1:
            //printf(RED);
            strcat(msg,"\033[91m\033[91mX \033[0m");
            //printf(RESET);
            break;
          case 2:
            //printf(CYAN);
            strcat(msg,"\033[96m\033[96mI \033[0m");
            //printf(RESET);
            break;
          default:
            //printf(YELLOW);
            strcat(msg,"\033[93m\033[93m! \033[0m");
            //printf(RESET);
            break;
        }
      }
    }
    strcat(msg,"\n");
  }
  printf("%s\n\n msg ha numero %d di caratteri.\n",msg, strlen(msg));
  /*GameGrid d;
  unsigned int one=1, temp;
  d.permessi=0;
  printf("%d\n", d.permessi);
  d.permessi= d.permessi | (one<<(1-1));
  printf("%d\n", d.permessi);
  d.permessi = d.permessi | (one<<(2-1));
  printf("%d\n", d.permessi);
  d.permessi = d.permessi | (one<<(3-1));
  printf("%d\n", d.permessi);
  d.permessi = d.permessi | (one<<(4-1));
  printf("%d\n", d.permessi);
*/

  return 0;
}
