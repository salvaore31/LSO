#include "Users.h"

int signInUserMenu(int sockfd, char usrn[], LogFile *log){
  int n_b_r;
  char pssw[50],msg[100];
  int err;
  //da aggiungere controllo su effettiva lettura
    n_b_r=sendMsg(sockfd,INSERT_USERNAME_SIM,usrn);
    n_b_r=sendMsg(sockfd,INSERT_PASSWORD_SIM,pssw);
    while((err=registerUser(usrn, pssw)) != 0){
      //l'utente non è stato trovato tra quelli registrati
      switch(err){
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
    return ERR_INVALID_USERNAME;
  }else{
    if(checkUsername(newuser) < 0){// Se checkUsername ritorna un valore minore di 0 vuol dire che quel username non è presente nel file ed è quindi disponibile
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
      return ERR_INVALID_USERNAME;
    }
  }
}
//Non avendo ancora deciso di preciso cosa deve fare intanto ritorna 0 se l'utente e la password inseriti sono validi
int logInUser(char* user, char* passw){

  int fdUserFile, pos, n_b_r,i=0;
  char c,str[MAX_SIZE_PASSW];
  str[0]='\0';
  if((pos=checkUsername(user))<0){
    //La funzione ritorna -2 nel caso in cui non trova l'utente tra quelli registrati;
    return ERR_USERNAME_NOT_FOUND;
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
          return 0;
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
int logInUserMenu(int sockfd, char usrn[],LogFile *log){

  int n_b_r;
  char pssw[50],msg[100];
  int err;

  //da aggiungere controllo su effettiva lettura
    n_b_r=sendMsg(sockfd,INSERT_USERNAME_LIM,usrn);
    while(checkUsername(usrn)<0){
      n_b_r=sendMsg(sockfd,WRONG_USERNAME_LIM,usrn);
    }
    n_b_r=sendMsg(sockfd,INSERT_PASSWORD_LIM,pssw);
    while((err=logInUser(usrn, pssw)) != 0){
      //l'utente non è stato trovato tra quelli registrati
      switch(err){
        case ERR_WRONG_PASSWORD:
          n_b_r=sendMsg(sockfd,WRONG_PASSWORD_LIM,pssw);
          break;
        case ERR_USERNAME_NOT_FOUND:
          n_b_r=sendMsg(sockfd,WRONG_USERNAME_LIM,usrn);
          n_b_r=sendMsg(sockfd,INSERT_PASSWORD_LIM,pssw);
          break;
        case ERR_INPUT_OUTPUT:
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
