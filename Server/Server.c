#include "Server.h"
#include <time.h>

void oraEsatta(char s[]){
  char buffer[26];
  time_t ora;
  time(&ora);
  strcpy(s,ctime_r(&ora, buffer));
}

int main(){

  int sock,sockfd,n_b_w=0;
  struct sockaddr_un mio_indirizzo;
  if((sock=socket(PF_LOCAL,SOCK_STREAM,0))<0){
    printf("Errore apertura socket\n");
  }else{
    mio_indirizzo.sun_family=AF_LOCAL;
    strcpy(mio_indirizzo.sun_path,"/tmp/mio_socket5");
    if(bind(sock,(struct sockaddr *)&mio_indirizzo,sizeof(mio_indirizzo))<0){
      printf("Errore bind\n");
    }else{
      if(listen(sock,3)<0){
        printf("Errore listen\n");
      }else{
        if((sockfd=accept(sock,NULL,NULL))<0){
          printf("Errore accept\n");
        }else{
          char str[25];
          oraEsatta(str);
          write(sockfd,str,sizeof(str));
        }
      }
    }
  }
  close(sock);
  close(sockfd);
  unlink("/tmp/mio_socket5");
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
  if(lenght_user>MAX_SIZE_USERNAME||lenght_passw>MAX_SIZE_PASSW){
    //Gestire il comportamento nel caso in cui l'input non è valido
  }else{
    if(checkUsername(newuser)<0){
      if((fdUserFile=open(USERS_FILE,O_WRONLY|O_APPEND,S_IRWXU))<0){
        /*Gestire cosa succede se non si riesce ad aprire il file*/
      }else{
        if((n_b_w=write(fdUserFile,newuser,lenght_user))<lenght_user){
          /*Gestire cosa succede se non si riesce a scrivere su file*/
          close(fdUserFile);
        }else{
          write(fdUserFile,"%",1);
          if((n_b_w=write(fdUserFile,newpassw,lenght_passw))<lenght_passw){
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
    /*Gestire cosa succede se il nome utente inserito è già presente*/
    }
  }
}
//Non avendo ancora deciso di preciso cosa deve fare intanto ritorna 0 se l'utente e la password inseriti sono validi
int logInUser(char* user, char* passw){

  int fdUserFile, pos, n_b_r,i=0;
  char c,str[MAX_SIZE_PASSW];

  if((pos=checkUsername(user))<0){
    //Gestire il comportamento in caso di problemi checkUsername
  }else{
    if((fdUserFile=open(USERS_FILE,O_RDONLY))<0){
      //Gestire il comportamento in caso di errore apertura file
    }else{
      if(lseek(fdUserFile,pos,SEEK_SET)!=pos){
        //GESTIRE il comportamentoin caso di errore lseek
      }else{
        while((n_b_r=read(fdUserFile,&c,1))>0 && c!='\n' )
          str[i++]=c;
        str[--i]='\0';
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
