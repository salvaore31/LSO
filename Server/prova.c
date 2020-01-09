#include "Server.h"
#include <malloc.h>
int main(int argc, char const *argv[]) {

  srand(time(NULL));

  clear();
  unsigned int giocatore=1;
  int i,j,x,y,numOstacoli;
  GameGrid **p;
  char msg[1024*5];
  msg[0]='\0';
  p=(GameGrid**)malloc(MAX_GRID_SIZE_H * sizeof(GameGrid*));

  for(i=0;i<MAX_GRID_SIZE_H;i++)
    p[i]=(GameGrid*)calloc(MAX_GRID_SIZE_L, sizeof(GameGrid));

    for(i=0;i<MAX_GRID_SIZE_H;i++){
      for(j=0;j<MAX_GRID_SIZE_L;j++){
        p[i][j].ostacolo=0;
        p[i][j].giocatore=0;
        p[i][j].pacco=0;
        p[i][j].locazione=0;
        p[i][j].codiceGiocatore=0;
        p[i][j].codicePacco=0;
        p[i][j].codiceLocazione=0;
        p[i][j].p0=1;
        p[i][j].p1=0;
        p[i][j].p2=0;
        p[i][j].p3=0;
        p[i][j].p4=0;
        p[i][j].p5=0;
        p[i][j].p6=0;
        p[i][j].p7=0;
      }
    }
    y=rand()%MAX_GRID_SIZE_L;
    x=rand()%MAX_GRID_SIZE_H;
    p[x][y].giocatore=1;
    p[x][y].codiceGiocatore=0;
    p[x][y].p1=1;
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_L;
        x=rand()%MAX_GRID_SIZE_H;
      } while(p[x][y].giocatore);
      p[x][y].pacco=1;
      p[x][y].codicePacco=i;
    }
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_L;
        x=rand()%MAX_GRID_SIZE_H;
      } while(p[x][y].giocatore || p[x][y].pacco);
      p[x][y].locazione=1;
      p[x][y].codiceLocazione=i;
    }
    for(i=0;i<MAX_OBSTACLES_N;i++){
      y=rand()%MAX_GRID_SIZE_L;
      x=rand()%MAX_GRID_SIZE_H;
      if(!(p[x][y].giocatore || p[x][y].pacco || p[x][y].locazione)){
        p[x][y].ostacolo=1;
        numOstacoli++;
      }
    }
    while(numOstacoli<MIN_OBSTACLES){
      y=rand()%MAX_GRID_SIZE_L;
      x=rand()%MAX_GRID_SIZE_H;
      if(!(p[x][y].giocatore || p[x][y].pacco || p[x][y].locazione)){
        p[x][y].ostacolo=1;
        numOstacoli++;
      }
    }

  for(i=0;i<MAX_GRID_SIZE_H;i++){
    for(j=0;j<MAX_GRID_SIZE_L;j++){
      if(!p[i][j].p0)
        strcat(msg,"? ");
      else{
        if (p[i][j].ostacolo) {
          strcat(msg,"\033[91m\033[91mX   \033[0m");
        }
        else{
          if (p[i][j].giocatore) {
            if (p[i][j].codiceGiocatore==0) {
              strcat(msg,"\033[92m\033[92m0   \033[0m");
            }else{
              /*ALTRO GIOCATORE*/
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
    strcat(msg,"\n\n");
  }

  printf("%s\n\nmsg ha numero %d di caratteri.\nnumero ostacoli %d\n",msg, strlen(msg),numOstacoli);

  return 0;
}
