#include "Server.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  char ci[3];
  int n_b_r;
  n_b_r=read(STDIN_FILENO,ci,10);
  ci[n_b_r]='\0';
  printf("%s\n%d\n",ci,strlen(ci) );
  fflush(STDIN_FILENO);
  return 0;
}
