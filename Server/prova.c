#include "Server.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  loggedUser a;
  initializaLoggedUser(&a);
  insertLoggedUser("capo",&a);
  printf("%s\n","inserimento");
  printf("%s\n",a.user[0]);
  insertLoggedUser("giangiorgio",&a);
  printf("%s\n",a.user[1]);
  deleteLoggedUser("capo",&a);
  deleteLoggedUser("giangiorgio",&a);
  printf("%s\n","cancellazione");
  printf("%s\n",a.user[0] );
  return 0;
}
