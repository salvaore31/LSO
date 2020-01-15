
void* initializeGamesArray(){
  int i = 0;
  Game** sessionGames;
  sessionGames= mmap(NULL, (sizeof(Game*)*MAX_GAMES_PLAYABLE), PROT_READ|PROT_WRITE,
                      MAP_SHARED|MAP_ANONYMOUS, -1, 0);
  if(sessionGames!=MAP_FAILED){
    for(i = 0; i< MAX_GAMES_PLAYABLE; i++){
      sessionGames[i]=0;
    }
  }
  return sessionGames;
}

void putGameInArray(Game** sessionGames,Game* g){
  int i=0;
  for(i = 0; i<MAX_GAMES_PLAYABLE; i++){
    if(sessionGames[i]==0){
      sessionGames[i]=g;
      break;
    }
  }
  return;
}


void printArray(Game** sessionGames){
  int i=0;
  clear();
  for(i=0; i<MAX_GAMES_PLAYABLE; i++){
    if(sessionGames[i] == 0){
      printf("%d° cella array: [NULL, NULL].\n",i+1, sessionGames[i]);
    }else{
      Game * tmp = sessionGames[i];

      printf("%d° cella array: [partita in locazione %d, id partita  %d].\n",i+1, sessionGames[i], sessionGames[i]->gameId);
    }
  }
  return;
}
