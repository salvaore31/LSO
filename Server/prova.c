#include "Server.h"
#include <malloc.h>
int main(int argc, char const *argv[]) {

  player a,b;
  strcpy(a.nome,"Giggino");
  b=a;
  printf("%s\n",b.nome );
  return 0;
}
