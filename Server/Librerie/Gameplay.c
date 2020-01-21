#include "Gameplay.h"

Game *createGame(){
  Game* temp;
  temp=(Game*)calloc(1,sizeof(Game));
  temp->grid=NULL;
  int i;
  for(i=0;i<MAX_PLAYER_N;i++)
    temp->giocatori[i].posx=-1;
  pthread_mutex_init(&(temp->sem),NULL);
  return temp;
}


int playGame(Game * game, int idGiocatore, int gameId,int sockfd,LogFile *serverLog){

  int n_b_r;
  char msg[100], matrix[4000];
  while(1){
    pthread_mutex_lock(&game->sem);
    GameGridToText(game->grid,matrix,idGiocatore);
    pthread_mutex_unlock(&game->sem);
    sendMsg(sockfd,matrix,msg);
    pthread_mutex_lock(&serverLog->sem);
    azioneGiocatore(game,idGiocatore,msg[0],game->gameId,&serverLog->fd);
    pthread_mutex_unlock(&serverLog->sem);

  }
  return 0;
}

/*
  La funzione si occupa di determinare l'azione del giocatore all'interno di una partita.
   - game, di tipo puntatore a Game(struct definita in costanti.h) determina la partita;
   - giocatore, è un intero rappresentante l'id del giocatore specifico;
   - action, è un carattere rappresentante l'identificativo dell'azione;
   - gameId, è un intero rappresentante l'id della partita;
   - fdLog, rappresenta il file descriptore del file di Log.
*/
int azioneGiocatore(Game *game, int giocatore, char action, int gameId, int * fdLog){

  pthread_mutex_lock(&game->sem);
  char src[9], dest[9];
  player *player = &game->giocatori[giocatore];
  GameGrid ** grid = game->grid;
  int x=player->posx,y=player->posy;
  int destx;
  int desty;
  switch (action) {
    case 'w': case 'W':
    if(y>0){
      setPermessi(x,y-1,giocatore,grid);
      if(grid[y-1][x].ostacolo || grid[y-1][x].giocatore){
        printf("ostacolo o giocatore\n" );
        /*Gestire mossa non valida*/
      }else{
        sprintf(src,"[%d,%d]",player->posx,player->posy);
        grid[y][x].giocatore=0;
        grid[y-1][x].giocatore=1;
        grid[y-1][x].codiceGiocatore=giocatore;
        player->posy=y-1;
        sprintf(dest,"[%d,%d]",player->posx,player->posy);
        LogPlayerMoves(fdLog,gameId,player->nome,src,dest);
      }
    }else{
      printf("stai fuori\n" );
      /*Gesire mossa non valida*/
    }
    break;
    case 'a': case 'A':
      if(x>0){
        setPermessi(x-1,y,giocatore,grid);
        if(grid[y][x-1].ostacolo || grid[y][x-1].giocatore){
          printf("ostacolo o giocatore\n" );
          /*Gestire mossa non valida*/
        }else{
          sprintf(src,"[%d,%d]",player->posx,player->posy);
          grid[y][x].giocatore=0;
          grid[y][x-1].giocatore=1;
          grid[y][x-1].codiceGiocatore=giocatore;
          player->posx=x-1;
          sprintf(dest,"[%d,%d]",player->posx,player->posy);
          LogPlayerMoves(fdLog,gameId,player->nome,src,dest);
        }
      }else{
        printf("stai fuori\n" );
        /*Gesire mossa non valida*/
      }
    break;
    case 's': case 'S':
    if(y<MAX_GRID_SIZE_H-1){
      setPermessi(x,y+1,giocatore,grid);
      if(grid[y+1][x].ostacolo || grid[y+1][x].giocatore){
        printf("ostacolo o giocatore\n" );
        /*Gestire mossa non valida*/
      }else{
        sprintf(src,"[%d,%d]",player->posx,player->posy);
        grid[y][x].giocatore=0;
        grid[y+1][x].giocatore=1;
        grid[y+1][x].codiceGiocatore=giocatore;
        player->posy=y+1;
        sprintf(dest,"[%d,%d]",player->posx,player->posy);
        LogPlayerMoves(fdLog,gameId,player->nome,src,dest);
      }
    }else{
      printf("stai fuori\n");
      /*Gesire mossa non valida*/
    }
    break;
    case 'd': case 'D':
      if(x<MAX_GRID_SIZE_L-1){
        setPermessi(x+1,y,giocatore,grid);
        if(grid[y][x+1].ostacolo || grid[y][x+1].giocatore){
          printf("ostacolo o giocatore\n" );
          /*Gestire mossa non valida*/
        }else{
          sprintf(src,"[%d,%d]",player->posx,player->posy);
          grid[y][x].giocatore=0;
          grid[y][x+1].giocatore=1;
          grid[y][x+1].codiceGiocatore=giocatore;
          player->posx=x+1;
          sprintf(dest,"[%d,%d]",player->posx,player->posy);
          LogPlayerMoves(fdLog,gameId,player->nome,src,dest);
        }
      }else{
        printf("stai fuori\n" );
        /*Gesire mossa non valida*/
      }
    break;
    case 'q': case 'Q':
      if(grid[y][x].pacco){
        player->pacco = 1;
        player->codicePacco = grid[y][x].codicePacco;
        grid[y][x].codicePacco = 0;
        grid[y][x].pacco = 0;
      }else{
        /*mossa non valida perché no pacco*/
      }
    break;
    case 'e': case 'E':
      if(player->pacco){
        if(grid[y][x].locazione){
          //controllo se la locazione corrisponde al pacco
          if(grid[y][x].codiceLocazione == player->codicePacco){
            game->punteggio[giocatore]++;
            grid[y][x].locazione = 0;
            grid[y][x].codiceLocazione = 0;
            player->pacco = 0;
            player->codicePacco = 0;
          }else{
            //mossa non valida, non puoi occupare locazione con pacco non valido
          }
        }else{
          grid[y][x].pacco=1;
          grid[y][x].codicePacco = player->codicePacco;
          player->pacco = 0;
          player->codicePacco = 0;
        }
      }else{
        /*sciocchino non hai un pacco*/
      }
    break;
    case 'r': case 'R'://refresh
    break;
    case 'l': case 'L'://leggi locazione pacco
      if(player->pacco){
        destx=game->locazioneXPacchi[player->codicePacco];
        desty=game->locazioneYPacchi[player->codicePacco];
      }
    break;
    case '0'://esci
    break;
    default:
    break;
  }
  pthread_mutex_unlock(&game->sem);
}

void setPermessi(int x, int y, int giocatore, GameGrid ** grid){
  switch (giocatore) {
    case 0:
      grid[y][x].p0 = 1;
      break;
    case 1:
      grid[y][x].p1 = 1;
      break;
    case 2:
      grid[y][x].p2 = 1;
      break;
    case 3:
      grid[y][x].p3 = 1;
      break;
    case 4:
      grid[y][x].p4 = 1;
      break;
    case 5:
      grid[y][x].p5 = 1;
      break;
    case 6:
      grid[y][x].p6 = 1;
      break;
    case 7:
      grid[y][x].p7 = 1;
      break;
    default:
      printf("this should not be reachable.\n");
      break;
  }
  return;
}

int createGameGrid(Game *g){
  srand(time(NULL));
  int i, j, x, y, numOstacoli=0;
  GameGrid **p=(GameGrid**)malloc(MAX_GRID_SIZE_H * sizeof(GameGrid*));

  if(p!=NULL){
    for(i=0;i<MAX_GRID_SIZE_H;i++)
      p[i]=(GameGrid*)calloc(MAX_GRID_SIZE_L, sizeof(GameGrid));
    y=rand()%MAX_GRID_SIZE_H;
    x=rand()%MAX_GRID_SIZE_L;
    p[y][x].giocatore=1;
    p[y][x].p0=1;
    p[y][x].codiceGiocatore=0;
    g->giocatori[0].posx=x;
    g->giocatori[0].posy=y;
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_H;
        x=rand()%MAX_GRID_SIZE_L;
      } while(p[y][x].giocatore);
      p[y][x].pacco=1;
      p[y][x].p0=1;
      p[y][x].p1=1;
      p[y][x].p2=1;
      p[y][x].p3=1;
      p[y][x].p4=1;
      p[y][x].p5=1;
      p[y][x].p6=1;
      p[y][x].p7=1;
      p[y][x].codicePacco=i;
    }
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_H;
        x=rand()%MAX_GRID_SIZE_L;
      } while(p[y][x].giocatore || p[y][x].pacco);
      p[y][x].locazione=1;
      p[y][x].p0=1;
      p[y][x].p1=1;
      p[y][x].p2=1;
      p[y][x].p3=1;
      p[y][x].p4=1;
      p[y][x].p5=1;
      p[y][x].p6=1;
      p[y][x].p7=1;
      p[y][x].codiceLocazione=i;
      g->locazioneXPacchi[i]=x;
      g->locazioneYPacchi[i]=y;
    }
    for(i=0;i<MAX_OBSTACLES_N;i++){
      y=rand()%MAX_GRID_SIZE_H;
      x=rand()%MAX_GRID_SIZE_L;
      if(!(p[y][x].giocatore || p[y][x].pacco || p[y][x].locazione)){
        p[y][x].ostacolo=1;
        numOstacoli++;
      }
    }
    while(numOstacoli<MIN_OBSTACLES){
      y=rand()%MAX_GRID_SIZE_H;
      x=rand()%MAX_GRID_SIZE_L;
      if(!(p[y][x].giocatore || p[y][x].pacco || p[y][x].locazione)){
        p[y][x].ostacolo=1;
        numOstacoli++;
      }
    }
  }else{
    return -1;
  }
  g->grid=p;
  return 0;

}

void spawnNewPlayer(Game* g, char* username){
  srand(time(NULL));
  printf("Sono in spawn new Player.\n");
  int x, y;
  int i = 0;
  int done = 0;
  player playerToAdd;
  strcpy(playerToAdd.nome, username);
  playerToAdd.codicePacco = 0;
  playerToAdd.pacco = 0;
  pthread_mutex_lock(&g->sem);
  for(i = 0; i < MAX_PLAYER_N; i++){
    if(g->giocatori[i].posx==-1){
      g->giocatori[i] = playerToAdd;
      break;
    }
  }
  GameGrid** p = g->grid;
  while(!done){
    y=rand()%MAX_GRID_SIZE_H;
    x=rand()%MAX_GRID_SIZE_L;
    if(!(p[y][x].giocatore && p[y][x].pacco && p[y][x].locazione && p[y][x].ostacolo)){
      p[y][x].giocatore = 1;
      p[y][x].codiceGiocatore = i;
      if(i == MAX_PLAYER_N-1){
        g->piena = 1;
      }
      setPermessi(x, y, i, p);
      done = 1;
    }
  }
  pthread_mutex_unlock(&g->sem);
  return;
}
