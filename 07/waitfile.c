/* waitfile: wait until file stops changing */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>             /* exec(), sleep() */
#include <fcntl.h>              /* open(), close() */
#include <sys/stat.h>

void error(char *s1, char *s2);

char *progname;

int main(int argc, char *argv[])
{
        int     fd;
        struct stat stbuf;
        time_t  old_time = 0;

        progname = argv[0];
        if (argc < 2)
                error("Usage: %s filename [cmd]", progname);
        if ((fd = open(argv[1], 0)) == -1)
                error("can't open %s", argv[1]);
        fstat(fd, &stbuf);
        while (stbuf.st_mtime != old_time) {
                old_time = stbuf.st_mtime;
                sleep(10);
                fstat(fd, &stbuf);
        }
        if (argc == 2) {        /* copy file */
                execlp("cat", "cat", argv[1], (char *) 0);
                error("can't execute cat %s", argv[1]);
        } else {                /* run process */
        /*      for (int i = 0; i < argc; i++)
                        fprintf(stderr, "%s\n", argv[i]);       */
                execvp(argv[2], &argv[2]);
                error("can't execute %s", argv[2]);
        }
        exit(0);
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