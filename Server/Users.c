#include "Users.h"

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
          write(sockfd, NO_CONNECTION_ERR_MESSAGE, sizeof(NO_CONNECTION_ERR_MESSAGE));
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
    return ERR_INVALID_USERNAME;
  }else{
    if(checkUsername(newuser) < 0){// Se checkUsername ritorna un valore minore di 0 vuol dire che quel username non è presente nel file ed è quindi disponibile
      if((fdUserFile = open(USERS_FILE,O_WRONLY|O_APPEND,S_IRWXU))<0){
        return ERR_NO_CONNECTION;
        /*Gestire cosa succede se non si riesce ad aprire il file*/
      }else{
        if((n_b_w = write(fdUserFile,newuser,lenght_user))<lenght_user){
          return ERR_NO_CONNECTION;
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
      return ERR_INVALID_USERNAME;
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
          write(sockfd, NO_CONNECTION_ERR_MESSAGE, sizeof(NO_CONNECTION_ERR_MESSAGE));
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
