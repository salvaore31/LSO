#include "Server.h"
#include <time.h>

int fdLog;
int main(int argc, char* argv[]){
  srand(time(NULL));
  signal(SIGINT, handleSignal);
  clear();
  int sock, sockfd, n_b_w=0, n_b_r=0, fdPlayer, fdGame;
  char msg[1024*5],c;
  struct sockaddr_un mio_indirizzo;
  LogServerStart(&fdLog);
  if((sock=socket(PF_LOCAL,SOCK_STREAM,0))<0){
    printf("Errore apertura socket\n");
    //Gestire cosa fare in caso di errore in apertura Socket
  }else{
    mio_indirizzo.sun_family=AF_LOCAL;
    strcpy(mio_indirizzo.sun_path,MIO_SOCK);
    if(bind(sock,(struct sockaddr *)&mio_indirizzo,sizeof(mio_indirizzo))<0){
      printf("Errore bind\n");
    }else{
      if(listen(sock,3)<0){
        printf("Errore listen\n");
      }else{
        while((sockfd=accept(sock,NULL,NULL))>-1){
          write(sockfd,WELCOME_MESSAGE,sizeof(WELCOME_MESSAGE));
          n_b_r=read(sockfd,msg,50);
          msg[n_b_r]='\0';
          char user[50];
          int baba=-1;
          while(baba==-1){
            if(n_b_r==1){
              switch (msg[0]) {
                case 'l': case 'L':
                  logInUserMenu(sockfd,user);
                  baba=1;
                  break;
                case 'r': case 'R':
                  signInUserMenu(sockfd,user);
                  baba=1;
                  break;
                case 'e': case 'E':
                  write(sockfd, "-1", sizeof("-1"));
                  break;
                default:
                  clear();
                  write(sockfd,WELCOME_MESSAGE,sizeof(WELCOME_MESSAGE));
                  n_b_r=read(sockfd,msg,50);
                  msg[n_b_r]='\0';
                  break;
              }
            }else{
                  clear();
                  write(sockfd,WELCOME_MESSAGE,sizeof(WELCOME_MESSAGE));
                  n_b_r=read(sockfd,msg,50);
                  msg[n_b_r]='\0';
            }
          }
          read(sockfd,msg,1);
          write(sockfd,GAME_SELECTION_MENU,sizeof(GAME_SELECTION_MENU));
          n_b_r=read(sockfd,msg,50);
          msg[n_b_r]='\0';
          baba=-1;
          while(baba==-1){
            switch (msg[0]) {
              case 'n': case 'N':
                newGame(sockfd, user);
                baba=1;
                break;
              case 'j': case 'J':
                joinGame(sockfd,user);
                baba=1;
                break;
              case 'e': case 'E':
                write(sockfd, "-1", sizeof("-1"));
                break;
              default:
                write(sockfd,GAME_SELECTION_MENU,sizeof(GAME_SELECTION_MENU));
                n_b_r=read(sockfd,msg,50);
                msg[n_b_r]='\0';
                break;
            }
          }
          /*GameGrid **game=NULL;
          game=createGameGrid(game);
          GameGridToText(game,msg,1);
          write(sockfd,msg,strlen(msg));
          printf("%s",msg );
          msg[n_b_r]='\0';
          if(strcmp(msg,USER_LOG_OUT)==0){
            LogUserSignOut(&fdLog,user);
          }
          /*Qui va la fork che crea nuove partite
          si crea nuovo processo; si dichiara nuova GameGrid, si mette mutex,
          se client ha accesso e non è finito il tempo:
            il client invia messaggio e il server controlla casella di destinazione, poi:
              Player si muove, server sposta giocatore e invia nuova posizione a client, cliente aggiorna
                              propria vista su matrice e libera mutex;
              Player su ostacolo, server non sposta il giocatore e aggiorna matrice così da far vedere ostacolo
                              a quel player, client riceve nuova vista, stampa e libera mutex;
              Player  spawna, server assegna al nuovo player posizione iniziale, posizione di vittoria, aggiorna tutte
                              le viste delle matrici aggiungendo posizione nuovo player e sua win condition, il client
                              riceve la nuova matrice e la stampa, libera mutex;
              Player vince, server manda messaggio di vittoria a vincitore e di sconfitta a perdenti, client libera mutex,
                            server chiude mutex, fa ritornare al menu principale i client
          se è finito il tempo:
            messaggio di sconfitta e chiusura partita.
          */
        }
      }
    }
  }
  close(sock);
  close(sockfd);
  unlink(MIO_SOCK);
  LogServerClose(&fdLog);
  return 1;
}

void joinGame(int sockfd, char user[]){
  write(sockfd,"Hai scelto joinGame",20);
  clear();
  return ;
}

void newGame(int sockfd, char user[]){

  char msg[5000];
  GameGrid **g=NULL;
  if((g=createGameGrid(g))!=NULL){
    GameGridToText(g,msg,1);
    write(sockfd,msg,strlen(msg));
    printf("%s\n",msg );
    clear();
  }else{
    write(sockfd,ERR_NO_CONNECTION,sizeof(ERR_NO_CONNECTION));

  }
  return;
}

void handleSignal(int Sig){

  if(Sig == SIGINT){
    LogServerClose(&fdLog);
    unlink(MIO_SOCK);
    exit(1);
  }
}

int signInUserMenu(int sockfd, char usrn[]){
  int n_b_r;
  char pssw[50];
  int err;

  //da aggiungere controllo su effettiva lettura
    write(sockfd, INSERT_USERNAME_SIM, sizeof(INSERT_USERNAME_SIM));
    n_b_r = read(sockfd, usrn, 50);
    usrn[n_b_r] ='\0';
    write(sockfd, INSERT_PASSWORD_SIM, sizeof(INSERT_PASSWORD_SIM));
    n_b_r = read(sockfd, pssw, 50);
    pssw[n_b_r] = '\0';

    while((err=registerUser(usrn, pssw)) != 0){
      //l'utente non è stato trovato tra quelli registrati
      switch(err){
        case NO_CONNECTION:
          write(sockfd, ERR_NO_CONNECTION, sizeof(ERR_NO_CONNECTION));
          n_b_r = read(sockfd, pssw, 50);
          pssw[n_b_r] = '\0';
          break;
        case INVALID_USERNAME:
          write(sockfd, USER_ALREADY_PRESENT_SIM, sizeof(USER_ALREADY_PRESENT_SIM));
          n_b_r = read(sockfd, usrn, 50);
          usrn[n_b_r] ='\0';
          write(sockfd, INSERT_PASSWORD_SIM, sizeof(INSERT_PASSWORD_SIM));
          n_b_r = read(sockfd, pssw, 50);
          pssw[n_b_r] = '\0';
          break;
        default:
          break;
      }
    }
    LogNewUser(&fdLog, usrn);
    write(sockfd,SUCCESS_MESSAGE_SIM,sizeof(SUCCESS_MESSAGE_SIM));
    return 1;
  }

//checkUsername FUNZIONA BISOGNA GESTIRE IL COMPORTAMENTO IN CASO DI ERRORE NELL' APERTURA
int checkUsername(char* username){
  int fdUserFile, i=0, res,r=1;
  char c, str[MAX_SIZE_USERNAME];
  if((fdUserFile=open(USERS_FILE,O_RDONLY))<0){
    printf("errore\n" );
    //GESTIRE ERRORE APERTURA FILE
  }else{
    while(r>0){
      while(((r=read(fdUserFile,&c,1))>0)&&c!='%')
        str[i++]=c;
      str[i++]='\0';
      if(strcmp(str,username)==0){
        res=lseek(fdUserFile,0,SEEK_CUR);
        close(fdUserFile);
        return res;
      }
      while(((r=read(fdUserFile,&c,1))>0)&&c!='\n');
      i=0;
    }
    close(fdUserFile);
  }
  return -1;
}
//registerUser FUNZIONA BISOGNA GESTIRE IL COMPORTAMENTO NEI VARI CASI DI ERRORE
int registerUser(char* newuser, char* newpassw){

  int fdUserFile,n_b_w,lenght_user,lenght_passw;
  lenght_user=strlen(newuser);
  lenght_passw=strlen(newpassw);

  if( lenght_user > MAX_SIZE_USERNAME || lenght_passw > MAX_SIZE_PASSW ){
    return INVALID_USERNAME;
  }else{
    if(checkUsername(newuser) < 0){// Se checkUsername ritorna un valore minore di 0 vuol dire che quel username non è presente nel file ed è quindi disponibile
      if((fdUserFile = open(USERS_FILE,O_WRONLY|O_APPEND,S_IRWXU))<0){
        return NO_CONNECTION;
        /*Gestire cosa succede se non si riesce ad aprire il file*/
      }else{
        if((n_b_w = write(fdUserFile,newuser,lenght_user))<lenght_user){
          return NO_CONNECTION;
          /*Gestire cosa succede se non si riesce a scrivere su file*/
          close(fdUserFile);
        }else{
          write(fdUserFile,"%",1);
          if((n_b_w = write(fdUserFile,newpassw,lenght_passw))<lenght_passw){
            /*Gestire cosa succede se non si riesce a scrivere su file*/
            close(fdUserFile);
          }else{
            write(fdUserFile,"\n",1);
            close(fdUserFile);
            return 0;
          }
        }
      }
    }else{
      return INVALID_USERNAME;
    }
  }
}
//Non avendo ancora deciso di preciso cosa deve fare intanto ritorna 0 se l'utente e la password inseriti sono validi
int logInUser(char* user, char* passw){

  int fdUserFile, pos, n_b_r,i=0;
  char c,str[MAX_SIZE_PASSW];

  if((pos=checkUsername(user))<0){
    //La funzione ritorna -2 nel caso in cui non trova l'utente tra quelli registrati;
    return -2;
  }else{
    if((fdUserFile=open(USERS_FILE,O_RDONLY))<0){
      //Gestire il comportamento in caso di errore apertura file
      return -3;
    }else{
      if(lseek(fdUserFile,pos,SEEK_SET)!=pos){
        //GESTIRE il comportamentoin caso di errore lseek
        return -4;
      }else{
        while((n_b_r=read(fdUserFile,&c,1))>0 && c!='\n')
          str[i++]=c;
        str[i]='\0';
        if(strcmp(passw,str)==0){
          return 0;
        }else{
          return -1;
          //Gestire il comportamento in caso in cui la password inserita non è corretta
        }
      }
    }
  }
  return -1;
}
//logInUserMenu gesitsce la comunicazione con il client per quanto riguarda il logIn
int logInUserMenu(int sockfd, char usrn[]){

  int n_b_r;
  char pssw[50];
  int err;

  //da aggiungere controllo su effettiva lettura
    write(sockfd, INSERT_USERNAME_LIM, sizeof(INSERT_USERNAME_LIM));
    n_b_r = read(sockfd, usrn, 50);
    usrn[n_b_r] ='\0';
    while(checkUsername(usrn)<0){
      write(sockfd, WRONG_USERNAME_LIM, sizeof(WRONG_USERNAME_LIM));
      n_b_r = read(sockfd, usrn, 50);
      usrn[n_b_r] ='\0';
    }
    write(sockfd, INSERT_PASSWORD_LIM, sizeof(INSERT_PASSWORD_LIM));
    n_b_r = read(sockfd, pssw, 50);
    pssw[n_b_r] = '\0';

    while((err=logInUser(usrn, pssw)) != 0){
      //l'utente non è stato trovato tra quelli registrati
      switch(err){
        case -1:
          write(sockfd, WRONG_PASSWORD_LIM, sizeof(WRONG_PASSWORD_LIM));
          n_b_r = read(sockfd, pssw, 50);
          pssw[n_b_r] = '\0';
          break;
        case -2:
          write(sockfd, WRONG_USERNAME_LIM, sizeof(WRONG_USERNAME_LIM));
          n_b_r = read(sockfd, usrn, 50);
          usrn[n_b_r] ='\0';
          write(sockfd, INSERT_PASSWORD_LIM, sizeof(INSERT_PASSWORD_LIM));
          n_b_r = read(sockfd, pssw, 50);
          pssw[n_b_r] = '\0';
          break;
        case -3:
          write(sockfd, ERR_NO_CONNECTION, sizeof(ERR_NO_CONNECTION));
          break;
        case -4:
          break;
        default:
          break;
      }
    }
    LogUserSignIn(&fdLog, usrn);
    write(sockfd,SUCCESS_MESSAGE_LIM,sizeof(SUCCESS_MESSAGE_LIM));
    return 1;
}

GameGrid **createGameGrid(GameGrid **p){

  int i,j;
  p=(GameGrid**)malloc(MAX_GRID_SIZE_H * sizeof(GameGrid*));

  if(p!=NULL){
    for(i=0;i<MAX_GRID_SIZE_H;i++)
      p[i]=(GameGrid*)calloc(MAX_GRID_SIZE_L, sizeof(GameGrid));
    for(i=0;i<MAX_GRID_SIZE_H;i++){
      for(j=0;j<MAX_GRID_SIZE_L;j++){
        p[i][j].infocasella=0;
        p[i][j].playerI=0;
        p[i][j].playerJ=0;
        p[i][j].p1=0;
        p[i][j].p2=0;
        p[i][j].p3=0;
        p[i][j].p4=0;
        p[i][j].p5=0;
        p[i][j].p6=0;
        p[i][j].p7=0;
        p[i][j].p8=0;
      }
    }
    int x,y=rand()%MAX_GRID_SIZE_L;
    x=rand()%MAX_GRID_SIZE_H;
    p[x][y].playerI=1;
    p[x][y].infocasella=2;
    p[x][y].p1+=1;
    y=rand()%MAX_GRID_SIZE_L;
    x=rand()%MAX_GRID_SIZE_H;
    p[x][y].playerJ=1;
    p[x][y].infocasella=3;
    p[x][y].p1+=1;
    for(i=0;i<MAX_OBSTACLES_N;i++){
      y=rand()%MAX_GRID_SIZE_L;
      x=rand()%MAX_GRID_SIZE_H;
      if(p[x][y].infocasella==0)
        p[x][y].infocasella=1;
    }
  }
  return p;
}

int GameGridToText(GameGrid **p, char msg[], int giocatore){

  int i,j;
  msg[0]='\0';
  for(i=0;i<MAX_GRID_SIZE_H;i++){
    for(j=0;j<MAX_GRID_SIZE_L;j++){
      switch(giocatore){
        case 1:
        if(p[i][j].p1==0)
          strcat(msg,"? ");
        else{
          switch(p[i][j].infocasella){
            case 0:
              strcat(msg,"\033[92m\033[92m0 \033[0m");
              break;
            case 1:
              strcat(msg,"\033[91m\033[91mX \033[0m");
              break;
            case 2:
              strcat(msg,"\033[96m\033[96mI \033[0m");
              break;
            default:
              strcat(msg,"\033[93m\033[93m! \033[0m");
              break;
          }
        }
        break;
        case 3:
        if(p[i][j].p3==0)
          strcat(msg,"? ");
        else{
          switch(p[i][j].infocasella){
            case 0:
              strcat(msg,"\033[92m\033[92m0 \033[0m");
              break;
            case 1:
              strcat(msg,"\033[91m\033[91mX \033[0m");
              break;
            case 2:
              strcat(msg,"\033[96m\033[96mI \033[0m");
              break;
            default:
              strcat(msg,"\033[93m\033[93m! \033[0m");
              break;
          }
        }
        break;
        case 4:
        if(p[i][j].p4==0)
          strcat(msg,"? ");
        else{
          switch(p[i][j].infocasella){
            case 0:
              strcat(msg,"\033[92m\033[92m0 \033[0m");
              break;
            case 1:
              strcat(msg,"\033[91m\033[91mX \033[0m");
              break;
            case 2:
              strcat(msg,"\033[96m\033[96mI \033[0m");
              break;
            default:
              strcat(msg,"\033[93m\033[93m! \033[0m");
              break;
          }
        }
        break;
        case 5:
        if(p[i][j].p5==0)
          strcat(msg,"? ");
        else{
          switch(p[i][j].infocasella){
            case 0:
              strcat(msg,"\033[92m\033[92m0 \033[0m");
              break;
            case 1:
              strcat(msg,"\033[91m\033[91mX \033[0m");
              break;
            case 2:
              strcat(msg,"\033[96m\033[96mI \033[0m");
              break;
            default:
              strcat(msg,"\033[93m\033[93m! \033[0m");
              break;
          }
        }
        break;
        case 6:
        if(p[i][j].p6==0)
          strcat(msg,"? ");
        else{
          switch(p[i][j].infocasella){
            case 0:
              strcat(msg,"\033[92m\033[92m0 \033[0m");
              break;
            case 1:
              strcat(msg,"\033[91m\033[91mX \033[0m");
              break;
            case 2:
              strcat(msg,"\033[96m\033[96mI \033[0m");
              break;
            default:
              strcat(msg,"\033[93m\033[93m! \033[0m");
              break;
          }
        }
        break;
        case 7:
        if(p[i][j].p7==0)
          strcat(msg,"? ");
        else{
          switch(p[i][j].infocasella){
            case 0:
              strcat(msg,"\033[92m\033[92m0 \033[0m");
              break;
            case 1:
              strcat(msg,"\033[91m\033[91mX \033[0m");
              break;
            case 2:
              strcat(msg,"\033[96m\033[96mI \033[0m");
              break;
            default:
              strcat(msg,"\033[93m\033[93m! \033[0m");
              break;
          }
        }
        break;
        case 8:
        if(p[i][j].p8==0)
          strcat(msg,"? ");
        else{
          switch(p[i][j].infocasella){
            case 0:
              strcat(msg,"\033[92m\033[92m0 \033[0m");
              break;
            case 1:
              strcat(msg,"\033[91m\033[91mX \033[0m");
              break;
            case 2:
              strcat(msg,"\033[96m\033[96mI \033[0m");
              break;
            default:
              strcat(msg,"\033[93m\033[93m! \033[0m");
              break;
          }
        }
        break;
        case 2:
        if(p[i][j].p2==0)
          strcat(msg,"? ");
        else{
          switch(p[i][j].infocasella){
            case 0:
              strcat(msg,"\033[92m\033[92m0 \033[0m");
              break;
            case 1:
              strcat(msg,"\033[91m\033[91mX \033[0m");
              break;
            case 2:
              strcat(msg,"\033[96m\033[96mI \033[0m");
              break;
            default:
              strcat(msg,"\033[93m\033[93m! \033[0m");
              break;
          }
        }
        break;
      }
    }
  strcat(msg,"\n");
  }
  return 1;
}

void deleteGrid(GameGrid **g){

  int i;
  for(i=0;i<MAX_GRID_SIZE_H;i++)
    free(g[i]);
  free(g);
  return;
}
