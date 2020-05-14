#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

int main(int argc, char *argv[]) {
  pid_t w, child = fork();
  int status;

  if (child < 0) {
    printf("fork() failed\n");
    return -1;
  }
  if (child == 0){
    printf("Child process\n");
    execlp("ps", "-e", NULL);
    return 0;
  }
  //w = waitpid(child, &status, WUNTRACED | WCONTINUED);
  printf("Parent process\n");
  printf("Zombie pid=%d\n", child);
  sleep(5);
  execlp("ps", "-e", NULL);
}