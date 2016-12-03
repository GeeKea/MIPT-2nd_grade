#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <malloc.h>
int zip(char *path) {
  pid_t pid;
  pid = fork();
  if (pid == -1) {
    printf("Fork error.\n");
    exit(-1);
  } else if (pid == 0) {
  execlp("gzip", "gzip", "-Nr", path, NULL);
  }
  while (wait(NULL) != -1) {}
  return(0);
}
int unzip(char *path) {
  DIR *dir;
  dir = opendir(path);
  if (dir == NULL) {
    return(-1);
  }
  pid_t pid;
  pid = fork();
  if (pid == -1) {
      printf("Fork error.\n");
      exit(-1);
  } else if (pid == 0) {
  execlp("gzip", "gzip", "-dNr", path, NULL);
  }
  while (wait(NULL) != -1) {}
  return(0);
}
int backup(char *inpath, char *outpath) {
  pid_t pid;
  DIR *sourcedir;
  DIR *destdir;
  struct dirent *r;
  char *str[4096];
  char buf[4096];
  int i = 2, k;
  str[0] = "cp";
  str[1] = "-upr";
  sourcedir = opendir(inpath);
  if (sourcedir == NULL) {
    printf("Error: no source directory.\n");
    exit(-1);
  }
  destdir = opendir(outpath);
  if (destdir == NULL) {
    mkdir(outpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  if (sourcedir == destdir) {
    printf("Error: backuping directory to itself.\n");
    exit(-1);
  }
  if ((r = readdir(sourcedir)) == NULL) {
    printf("Source directory is empty.\n");
    exit(-1);
  }
  rewinddir(sourcedir);
  while (r = readdir(sourcedir)) {
    if(!strcmp(r->d_name, ".") || !strcmp(r->d_name, ".."))
        continue;
    memset(buf, 0, sizeof(buf));
    strcpy(buf, inpath);
    strcat(buf, "/");
    strcat(buf, r->d_name);
    str[i] = malloc(strlen(buf)+1);
    strcpy(str[i], buf);
    i++;
  }
  str[i] = outpath;
  closedir(sourcedir);
  closedir(destdir);
  pid = fork();
  if (pid == -1) {
    printf("Fork error.\n");
    exit(-1);
  } else if (pid == 0) {
    execvp(str[0], str);
  }
  while (wait(NULL) != -1) {}
  for (k = 2; k < i; k++) {
    free(str[k]);
  }
  return 0;
}
int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Wrong number of command line arguments.\n");
        exit(-1);
    }
  unzip(argv[2]);
  backup(argv[1], argv[2]);
  zip(argv[2]);
  return 0;
}
