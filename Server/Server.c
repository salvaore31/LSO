#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "../costanti.h"
#include "Server.h"


int main(){

  return 1;
}
/*checkUsername FUNZIONA BISOGNA GESTIRE IL COMPORTAMENTO IN CASO DI ERRORE NELL' APERTURA*/
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
        lseek(fdUserFile,34,SEEK_CUR);
        read(fdUserFile,&c,1);
        close(fdUserFile);
        return res+1;
      }
      while(((r=read(fdUserFile,&c,1))>0)&&c!='\n');
      i=0;
    }
    close(fdUserFile);
  }
  return -1;
}
