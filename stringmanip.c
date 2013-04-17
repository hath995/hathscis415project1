#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  char *newargv[] = { "/bin/ls", NULL};  
  char *newenviron[] = {NULL};
  /*char *cmd;
  char **stuff;
parsecommand(newargv[0],cmd,stuff);*/
  execve("/bin/ls", newargv, newenviron);
  perror("execve"); 
  exit(EXIT_FAILURE);

} 


