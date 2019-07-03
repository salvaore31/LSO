#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "../costanti.h"
#include "Server.h"


int main(){

  return 0;
}

int checkUsername(char* username){
  int fdUserFile, i=0, res,r;
  char c, str[MAX_SIZE_USERNAME];
  if(fdUserFile=open(USERS_FILE,O_RDONLY)<0||fdUserFile==1){
    printf("errore\n" );
    //GESTIRE ERRORE APERTURA FILE
  }else{
    while(read(fdUserFile,&c,1)>0){
      printf("%d%c",fdUserFile,c);
      if(c!='%')
        str[i++]=c;
      str[i++]='\0';
      printf("ohhh%s\n",str);
      if(strcmp(str,username)==0){
        res=lseek(fdUserFile,0,SEEK_CUR);
        close(fdUserFile);
        return res;
      }
      while(((r=read(fdUserFile,&c,1))>0)&&c=='\n');
      i=0;
    }
    close(fdUserFile);
  }
  return -1;
}
