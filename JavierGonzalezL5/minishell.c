/*
Name: Javier Gonzalez
ID: 5754068
I affirm that I wrote this program myself without any help from any
other people or sources from the internet
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

char *cmd1[] = { "/bin/ls", "-al", 0 };
char *cmd2[] = { "/bin/grep", "minishell.c", 0 };

void runsource(int pfd[]);
void rundest(int pfd[]);
int i=1;

int
main(int argc, char **argv)
{
	int pid, status;
	int fd[2];

	pipe(fd);

	runsource(fd);
	rundest(fd);
	close(fd[0]); close(fd[1]);
	
	while ((pid = wait(&status)) != -1)
		printf("Child %d PID = %d\n", i++, pid);
	exit(0);
}


void runsource(int pfd[])
{
	int pid;

	switch (pid = fork()) {

	case 0:
		dup2(pfd[1], 1);
		close(pfd[0]);
		execvp(cmd1[0], cmd1);
		perror(cmd1[0]);

	default:
		break;

	case -1:
		perror("fork");
		exit(1);
	}
}

void rundest(int pfd[])
{
	int pid;

	switch (pid = fork()) {

	case 0:
		dup2(pfd[0], 0);
		close(pfd[1]);
		execvp(cmd2[0], cmd2);
		perror(cmd2[0]);

	default:
		break;

	case -1:
		perror("fork");
		exit(1);
	}
}