#include "Users.h"

int signInUserMenu(int sockfd, char usrn[], LogFile *log, loggedUser *loggati){
  int n_b_r;
  char pssw[50],msg[100];
  int err;
  //da aggiungere controllo su effettiva lettura
    n_b_r=sendMsg(sockfd,INSERT_USERNAME_SIM,usrn);
    n_b_r=sendMsg(sockfd,INSERT_PASSWORD_SIM,pssw);
    while((err=registerUser(usrn, pssw, loggati)) != 0){
      //l'utente non è stato trovato tra quelli registrati
      switch(err){
        case ERR_NO_USER_FILE:
          pthread_mutex_lock(&log->sem);
          LogErrorMessage(&log->fd, USER_FILE_OPEN_ERR_MESSAGE);
          pthread_mutex_unlock(&log->sem);
          return ERR_NO_USER_FILE;
          break;
        case ERR_INPUT_OUTPUT:
          pthread_mutex_lock(&log->sem);
          LogErrorMessage(&log->fd, I_O_ERR_MESSAGE);
          pthread_mutex_unlock(&log->sem);
          return ERR_INPUT_OUTPUT;
          break;
        case ERR_INVALID_USERNAME:
          n_b_r=sendMsg(sockfd,USER_ALREADY_PRESENT_SIM,usrn);
          n_b_r=sendMsg(sockfd,INSERT_PASSWORD_SIM,pssw);
          break;
        default:
          break;
      }
    }
    pthread_mutex_lock(&log->sem);
    LogNewUser(&log->fd, usrn);
    pthread_mutex_unlock(&log->sem);
    n_b_r=sendMsgNoReply(sockfd,SUCCESS_MESSAGE_SIM);
    return 1;
  }
//checkUsername FUNZIONA BISOGNA GESTIRE IL COMPORTAMENTO IN CASO DI ERRORE NELL' APERTURA
int checkUsername(char* username, loggedUser *loggati){
  int fdUserFile, i=0, res,r=1;
  char c, str[MAX_SIZE_USERNAME];
  if((fdUserFile=open(USERS_FILE,O_RDONLY))<0){
    return ERR_NO_USER_FILE;
  }else{
    while(r>0){
      while(((r=read(fdUserFile,&c,1))>0)&&c!='%')
        str[i++]=c;
      str[i++]='\0';
      if(strcmp(str,username)==0){
        if(isLogged(username,loggati)){
          close(fdUserFile);
          return ERR_USER_ALREADY_LOGGED;
        }
        res=lseek(fdUserFile,0,SEEK_CUR);
        close(fdUserFile);
        return res;
      }
      while(((r=read(fdUserFile,&c,1))>0)&&c!='\n');
      i=0;
    }
    close(fdUserFile);
  }
  return ERR_USERNAME_NOT_FOUND;
}
//registerUser FUNZIONA BISOGNA GESTIRE IL COMPORTAMENTO NEI VARI CASI DI ERRORE
int registerUser(char* newuser, char* newpassw,loggedUser *loggati){

  int res,fdUserFile,n_b_w,lenght_user,lenght_passw;
  lenght_user=strlen(newuser);
  lenght_passw=strlen(newpassw);

  if( lenght_user > MAX_SIZE_USERNAME || lenght_passw > MAX_SIZE_PASSW ){
    return ERR_INVALID_USERNAME;
  }else{
    if((res=checkUsername(newuser,loggati)) == ERR_USERNAME_NOT_FOUND){// Se checkUsername ritorna un valore minore di 0 vuol dire che quel username non è presente nel file ed è quindi disponibile
      if((fdUserFile = open(USERS_FILE,O_WRONLY|O_APPEND,S_IRWXU))<0){
        return ERR_NO_USER_FILE;
        /*Gestire cosa succede se non si riesce ad aprire il file*/
      }else{
        if((n_b_w = write(fdUserFile,newuser,lenght_user))<lenght_user){
          /*Gestire cosa succede se non si riesce a scrivere su file*/
          close(fdUserFile);
          return ERR_INPUT_OUTPUT;
        }else{
          write(fdUserFile,"%",1);
          if((n_b_w = write(fdUserFile,newpassw,lenght_passw))<lenght_passw){
            /*Gestire cosa succede se non si riesce a scrivere su file*/
            close(fdUserFile);
            return ERR_INPUT_OUTPUT;
          }else{
            write(fdUserFile,"\n",1);
            close(fdUserFile);
            return 0;
          }
        }
      }
    }else{
      return res;
    }
  }
}
//Non avendo ancora deciso di preciso cosa deve fare intanto ritorna 0 se l'utente e la password inseriti sono validi
int logInUser(char* user, char* passw, loggedUser *loggati){

  int fdUserFile, pos, err,n_b_r,i=0;
  char c,str[MAX_SIZE_PASSW];
  str[0]='\0';
  if((pos=checkUsername(user,loggati))<0){
    return pos;
  }else{
    if((fdUserFile=open(USERS_FILE,O_RDONLY))<0){
      //Gestire il comportamento in caso di errore apertura file
      return ERR_NO_USER_FILE;
    }else{
      if(lseek(fdUserFile,pos,SEEK_SET)!=pos){
        //GESTIRE il comportamentoin caso di errore lseek
        return ERR_INPUT_OUTPUT;
      }else{
        while((n_b_r=read(fdUserFile,&c,1))>0 && (c!=13 && c!='\n'))
          str[i++]=c;
        str[i]='\0';
        if(strcmp(passw,str)==0){
          if(err=insertLoggedUser(user,loggati)<0){
            return ERR_INSERT_LOGGED_USER;
          }else{
            return 0;
          }
        }else{
          return ERR_WRONG_PASSWORD;
          //Gestire il comportamento in caso in cui la password inserita non è corretta
        }
      }
    }
  }
  return -1;
}
//logInUserMenu gesitsce la comunicazione con il client per quanto riguarda il logIn
int logInUserMenu(int sockfd, char usrn[],LogFile *log, loggedUser *loggati){

  int n_b_r;
  char pssw[50],msg[100];
  int err;

  //da aggiungere controllo su effettiva lettura
    n_b_r=sendMsg(sockfd,INSERT_USERNAME_LIM,usrn);
    while((err = checkUsername(usrn, loggati))<0){
      if(err == ERR_NO_USER_FILE){
        pthread_mutex_lock(&log->sem);
        LogErrorMessage(&log->fd, USER_FILE_OPEN_ERR_MESSAGE);
        pthread_mutex_unlock(&log->sem);
        return ERR_NO_USER_FILE;
      }
      if(err == ERR_USER_ALREADY_LOGGED){
        pthread_mutex_lock(&log->sem);
        LogErrorMessage(&log->fd, USER_FILE_OPEN_ERR_MESSAGE);
        pthread_mutex_unlock(&log->sem);
        n_b_r=sendMsg(sockfd,USER_ALREADY_LOGGED,usrn);
      }else{
        n_b_r=sendMsg(sockfd,WRONG_USERNAME_LIM,usrn);
      }
    }
    n_b_r=sendMsg(sockfd,INSERT_PASSWORD_LIM,pssw);
    while((err=logInUser(usrn, pssw, loggati)) != 0){
      //l'utente non è stato trovato tra quelli registrati
      switch(err){
        case ERR_NO_USER_FILE:
        pthread_mutex_lock(&log->sem);
        LogErrorMessage(&log->fd, USER_FILE_OPEN_ERR_MESSAGE);
        pthread_mutex_unlock(&log->sem);
        return ERR_NO_USER_FILE;
        case ERR_INPUT_OUTPUT:
        pthread_mutex_lock(&log->sem);
        LogErrorMessage(&log->fd, I_O_ERR_MESSAGE);
        pthread_mutex_unlock(&log->sem);
        return ERR_INPUT_OUTPUT;
        case ERR_WRONG_PASSWORD:
          n_b_r=sendMsg(sockfd,WRONG_PASSWORD_LIM,pssw);
          break;
        case ERR_USERNAME_NOT_FOUND:
          n_b_r=sendMsg(sockfd,WRONG_USERNAME_LIM,usrn);
          n_b_r=sendMsg(sockfd,INSERT_PASSWORD_LIM,pssw);
          break;
        default:
          break;
      }
    }
    pthread_mutex_lock(&log->sem);
    LogUserSignIn(&log->fd, usrn);
    pthread_mutex_unlock(&log->sem);
    n_b_r=sendMsgNoReply(sockfd,SUCCESS_MESSAGE_LIM);
    return 1;
}

int deleteLoggedUser(char user[], loggedUser* loggati){

  int i, res=0;

  pthread_mutex_lock(&loggati->sem);
  for(i=0;i<MAX_PLAYER_N;i++){
    if(strcmp(user,loggati->user[i])==0){
      strcpy(loggati->user[i],"");
      res=1;
      break;
    }
  }
  pthread_mutex_unlock(&loggati->sem);
  return res;
}

int insertLoggedUser(char user[],loggedUser *loggati){

  int i, res=0;
  pthread_mutex_lock(&loggati->sem);
  for(i=0;i<MAX_PLAYER_N;i++){
    if(strcmp(loggati->user[i],"")==0){
      strcpy(loggati->user[i],user);
      res=1;
      break;
    }
  }
  pthread_mutex_unlock(&loggati->sem);
  return res;
}

int isLogged(char user[], loggedUser *loggati){

  int i,res=0;
  pthread_mutex_lock(&loggati->sem);
  for(i=0;i<MAX_PLAYER_N; i++){
    if(strcmp(user,loggati->user[i])==0){
      res=1;
      break;
    }
  }
  pthread_mutex_unlock(&loggati->sem);
  return res;
}
