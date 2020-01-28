//#include "Server.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  char ciao[]="bimba";
  strcpy(ciao,"");
  if(strcmp(ciao,"")==0){
    printf("%s\n","eccolo" );
  }else{
    printf("%s\n","non è cosa" );
  }
  return 0;
}
