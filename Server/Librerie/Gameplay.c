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
  int result=0;
  char msg[100], matrix[4000];
  sendMsg(sockfd,matrix,msg);
  while(1){
    pthread_mutex_lock(&serverLog->sem);
    result=azioneGiocatore(game,idGiocatore,msg[0],game->gameId,&serverLog->fd);
    pthread_mutex_unlock(&serverLog->sem);
    pthread_mutex_lock(&game->sem);
    GameGridToText(game->grid,matrix,idGiocatore,&game->giocatori[idGiocatore]);
    if(result<0){
      strcat(matrix,"\a");
    }
    pthread_mutex_unlock(&game->sem);
    if(result == PLAYER_EXITS){
      /*
        Qua mandi segnale a cliente per chiudere e dopodiché ammazzi thread;
        */
    }
    sendMsg(sockfd,matrix,msg);

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

  srand(time(NULL));
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
        pthread_mutex_unlock(&game->sem);
        return -1;
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
      pthread_mutex_unlock(&game->sem);
      return -1;
    }
    break;
    case 'a': case 'A':
      if(x>0){
        setPermessi(x-1,y,giocatore,grid);
        if(grid[y][x-1].ostacolo || grid[y][x-1].giocatore){
          pthread_mutex_unlock(&game->sem);
          return -1;
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
        pthread_mutex_unlock(&game->sem);
        return -1;
      }
    break;
    case 's': case 'S':
    if(y<MAX_GRID_SIZE_H-1){
      setPermessi(x,y+1,giocatore,grid);
      if(grid[y+1][x].ostacolo || grid[y+1][x].giocatore){
        pthread_mutex_unlock(&game->sem);
        return -1;
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
      pthread_mutex_unlock(&game->sem);
      return -1;
    }
    break;
    case 'd': case 'D':
      if(x<MAX_GRID_SIZE_L-1){
        setPermessi(x+1,y,giocatore,grid);
        if(grid[y][x+1].ostacolo || grid[y][x+1].giocatore){
          pthread_mutex_unlock(&game->sem);
          return -1;
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
        pthread_mutex_unlock(&game->sem);
        return -1;
      }
    break;
    case 'q': case 'Q':
      if(grid[y][x].pacco){
        player->pacco = 1;
        player->codicePacco = grid[y][x].codicePacco;
        grid[y][x].codicePacco = 0;
        grid[y][x].pacco = 0;
        sprintf(dest,"[%d,%d]",player->posx,player->posy);
        LogPlayerTakePackage(fdLog,gameId,player->nome,player->codicePacco,dest);
      }else{
        pthread_mutex_unlock(&game->sem);
        return -1;
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
            LogPlayerMakeAPoint(fdLog,gameId,player->nome);
          }else{
            pthread_mutex_unlock(&game->sem);
            return -1;
          }
        }else{
          grid[y][x].pacco=1;
          grid[y][x].codicePacco = player->codicePacco;
          player->pacco = 0;
          player->codicePacco = 0;
          for(int i = 0; i<MAX_PLAYER_N; i++){
            setPermessi(x, y, i, grid);
          }
          sprintf(dest,"[%d,%d]",player->posx,player->posy);
          LogPlayerLeavePackage(fdLog,gameId,player->nome,grid[y][x].codicePacco,dest);
        }
      }else{
        pthread_mutex_unlock(&game->sem);
        return -1;

      }
    break;
    case 'r': case 'R'://refresh
    break;
    case '0'://esci
      grid[y][x].giocatore = 0;
      int empty = 1;
      if(player->pacco){
        destx = x;
        desty = y;
        while(grid[desty][destx].pacco || grid[desty][destx].locazione || grid[desty][destx].giocatore || grid[desty][destx].ostacolo ){
          destx = rand()%MAX_GRID_SIZE_L;
          desty = rand()%MAX_GRID_SIZE_L;
        }
        grid[desty][destx].pacco=1;
        grid[desty][destx].codicePacco = player->codicePacco;
        for(int i = 0; i<MAX_PLAYER_N; i++){
          setPermessi(destx, desty, i, grid);
        }
        player->pacco = 0;
        player->codicePacco = 0;
      }
      game->giocatori[giocatore].posx = -1;
      game->punteggio[giocatore] = 0;
      if(game->piena){
        game->piena = 0;
      }
      for(int i = 0; i<MAX_PLAYER_N; i++ ){
        if(game->giocatori[i].posx>=0){
          empty = 0;
        }
      }
      if(empty){

      }
      pthread_mutex_unlock(&game->sem);
      return PLAYER_EXITS;
    break;
    default:
    break;
  }
  pthread_mutex_unlock(&game->sem);
  return 0;
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
      } while(p[y][x].giocatore || p[y][x].pacco);
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
      } while(p[y][x].giocatore || p[y][x].pacco || p[y][x].locazione);
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

void spawnNewPlayer(Game* g, char* username,int sockfd,LogFile* serverLog){

  srand(time(NULL));
  printf("Sono in spawn new Player.\n");
  int x, y, i;
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
  if (i<MAX_PLAYER_N) {
    GameGrid** p = g->grid;
    do{
      y=rand()%MAX_GRID_SIZE_H;
      x=rand()%MAX_GRID_SIZE_L;
    }while(p[y][x].giocatore || p[y][x].pacco || p[y][x].locazione || p[y][x].ostacolo);
    p[y][x].giocatore = 1;
    p[y][x].codiceGiocatore = i;
    g->giocatori[i].posx = x;
    g->giocatori[i].posy = y;
    if(i == MAX_PLAYER_N-1){
      g->piena = 1;
    }

    setPermessi(x, y, i, p);
    pthread_mutex_lock(&serverLog->sem);
    LogPlayerJoin(&serverLog->fd,g->gameId,username);
    pthread_mutex_unlock(&serverLog->sem);
    pthread_mutex_unlock(&g->sem);
    playGame(g,i,g->gameId,sockfd,serverLog);
  }
  return;
}
