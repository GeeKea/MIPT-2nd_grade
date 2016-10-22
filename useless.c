#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Wrong number of incoming arguments.\n");
		exit(-1);
	}
	FILE *file;
  float time;
	int k;
	char sep[5] = " ";
	char *istr[100];
	int i = 0;
	int t = 0;
	char *filename = argv[1];
	char prog[4096];
	pid_t pid;
	file = fopen(filename, "r");
	if (file == NULL) {
		printf("Opening file failure: check if it exists.\n");
		exit(-1);
	}
	if (getc(file) == EOF) {
		printf("File is empty.\n");
		exit(-1);
	}
	rewind(file);
	while (!feof(file)) {
		i++;
		while	((k = fscanf(file, "%f %[^\n]", &time, prog)) == 2) {
		i++;
		if (time < 0) {
			printf("Wrong time format with %s.\n", prog);
			continue;
		}
			printf("%s will be executed in %.3f sec.\n", prog, time);
			pid = fork();
			if (pid == -1) {
				printf("Fork failure.\n");
			} else if (pid == 0) {
				sleep(time);
				t = 0;
				istr[0] = strtok(prog, sep);
				while (istr[t] != 0) {
					istr[t+1] = strtok(NULL,sep);
					t++;
				}
				execvp(istr[0], istr);
				printf("Can't execute %s, check if it exists.\n", prog);
				exit(-1);
				}
		}
		if (ferror(file)) {
			printf("Input/output error.\n");
			i++;
		} else if (k != EOF) {
			printf("Wrong data in line №%d\n", i);
			fscanf(file, "%*[^\n]]");
		}
	}
	while (wait(NULL) != -1) {};
	fclose(file);
	return 0;
}
