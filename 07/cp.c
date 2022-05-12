/* cp: minimal version */
#include <stdio.h>
#include <stdlib.h>
#define PERMS   0644    /* RW for owner, R for group, others */
char    *progname;

void error(char *, char *);

int main(int argc, char* argv[])        /* cp: copy f1 to f2 */
{
        int     f1, f2, n;
        char    buf[BUFSIZ];

        progname = argv[0];
        if (argc != 3)
                error("Usage: %s from to", progname);
        if ((f1 = open(argv[1], 0)) == -1)
                error("can't open %s", argv[1]);
        if ((f2 = creat(argv[2], PERMS)) == -1)
                error("can't create %s", argv[2]);

        while ((n = read(f1, buf, BUFSIZ)) > 0)
                if (write(f2, buf, n) != n)
                        error("write error", (char *) 0);
        exit(0);
}

void error(char *s1, char *s2)  /* print error message and die */
{
        extern int errno, sys_nerr;
        extern char *sys_errlist[];
        
        if (progname)
                fprintf(stderr, "%s: ", progname);
        fprintf(stderr, s1, s2);
        if (errno > 0 && errno < sys_nerr)
                fprintf (stderr, " (%s)", sys_errlist[errno]);
        fprintf(stderr, "\n");
        exit(1);
}
