/* timeout: set time limit on a process */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

void onalarm(int);
void error(char *, char *);

int     pid;            /* child process id */
char    *progname;

int main(int argc, char *argv[])
{
        int     sec = 10, status;

        progname = argv[0];
        if (argc > 1 && argv[1][0] == '-') {
                sec = atoi(&argv[1][1]);
                argc--;
                argv++;
        }
        if (argc < 2)
                error("Usage: %s [-10] command", progname);
        if ((pid = fork()) == 0) {
                execvp(argv[1], &argv[1]);
                error("couldn't start %s", argv[1]);
        }
        signal(SIGALRM, onalarm);
        alarm(sec);
        if (wait(&status) == -1 || (status & 0177) != 0)
                error("%s killed", argv[1]);
        exit((status >> 8) & 0377);
}

void onalarm(int signo)         /* kill child when alarm arrives */
{
        kill(pid, SIGKILL);
}

void error(char *s1, char *s2)  /* print error message and die */
{      
        if (progname)
                fprintf(stderr, "%s: ", progname);
        fprintf(stderr, s1, s2);
        fprintf (stderr, " (%s)", strerror(errno));
        fprintf(stderr, "\n");
        exit(1);
}