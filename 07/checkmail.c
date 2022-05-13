/* checkmail: watch user's mailbox */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>              /* errno */
#include <string.h>             /* strerror */
#include <sys/stat.h>
#include <unistd.h>             /* getlogin(), chdir()   */

void error(char *, char *);

char    *progname;
char    *maildir = "/usr/spool/mail";       /* system dependent     */

int main(int argc, char *argv[])
{
        struct stat buf;
        char    *name;
        int     lastsize = 0;

        progname = argv[0];
        if ((name = getlogin()) == NULL)
                error("can't get login name", (char *) 0);
        if (chdir(maildir) == -1)
                error("can't cd to %s", maildir);
        for (;;) {
                if (stat(name, &buf) == -1)     /* no mailbox */
                        buf.st_size = 0;
                if (buf.st_size > lastsize)
                        fprintf(stderr, "\nYou have mail\007\n");
                lastsize = buf.st_size;
                sleep(10);
        }
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