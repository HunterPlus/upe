/*
 * Safer version of system for interactive programs
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

typedef void (*sighandler_t)(int);
int sys(char *s)     /* run command line s */
{
        int     status, pid, w, tty;        
        sighandler_t istat, qstat;
        extern char *progname;

        fflush(stdout);
        tty = open("/dev/tty", 2);      /* O_RDWR */
        if (tty == -1) {
                fprintf(stderr, "%s: can't open /dev/tty\n", progname);
                return -1;
        }
        if ((pid = fork()) == 0) {      /* child process */
                close(0); dup(tty);     /* tty become fd 0 */
                close(1); dup(tty);     /* also, tty become fd 1 */
                close(2); dup(tty);     /* tty become fd 2 */
                close(tty);
                execlp("sh", "sh", "-c", s, (char *) 0);
                exit(127);              /*if error, goes here   */
        }
        close(tty);
        istat = signal(SIGINT, SIG_IGN);
        qstat = signal(SIGQUIT, SIG_IGN);
        while ((w = wait(&status)) != pid && w != -1)   /* w = waitpid(pid, &status, 0); */
                ;
        if (w == -1)
                status = -1;
        signal(SIGINT, istat);
        signal(SIGQUIT, qstat);
        return status;
}