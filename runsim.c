#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>
#include <wait.h>
#include <signal.h>
pid_t execution(char *input) {
  char *prog;
  char *istr[100];
  int i = 0;
  pid_t pid;
  prog = malloc(sizeof(input)+1);
  strcpy(prog, input);
  istr[0] = strtok(prog, " ");
  if (istr[0] == NULL) {
    istr[0] = malloc(strlen(prog));
    strcpy(istr[0], prog);
  } else {
    while (istr[i] != NULL) {
      istr[i+1] = strtok(NULL, " ");
      i++;
    }
  }
  i = 0;
  if ((pid = fork()) == -1) {
    fprintf(stdout, "Fork failure.\n");
    return -1;
  } else if (pid == 0) {
    execvp(istr[0], istr);
    fprintf(stdout, "Command running failure.\n");
    return -1;
  }
  i = 0;
  free(prog);
  return pid;
}
int t = 0;
void my_handler(int nsig) {
  int status;
  pid_t pid;
  if ((pid = waitpid(-1, &status, 0)) < 0) {
    printf("Waitpid error\n");
  } else {
    if ((status & 0xff) == 0) {
      printf("Process %d has exited.\n", pid);
      --t;
    } else if ((status & 0xff00) == 0) {
      printf("Process %d has been killed.\n", pid);
      --t;
    }
  }
  }
int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Wrong number of command line arguments.\n");
    exit(-1);
  }
  int n = 0;
  if ((n = atoi(argv[1])) == 0) {
    printf("Incorrect command line argument.\n");
    exit(-1);
  }
  int i,k;
  char cmd[200];
  k = 0;
  i = 0;
  while (1) {
    (void)signal(SIGCHLD, my_handler);
    fgets(cmd, sizeof(cmd), stdin);
    cmd[strlen(cmd) - 1] = '\0';
    if (t >= n) {
     printf("Too much commands running, please wait until some exit.\n");
     continue;
    } else {
    execution(cmd);
    ++t;
  }
  }
  return 0;
}
