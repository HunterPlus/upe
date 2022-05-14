/* sv: save new files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void sv(char *file, char *dir);
void error(char *, char *);

char *progname;

int main(int argc, char *argv[])
{
        int     i;
        struct stat stbuf;
        char    *dir = argv[argc -1];

        progname = argv[0];
        if (argc <= 2)
                error("Usage: %s files... dir", progname);
        if (stat(dir, &stbuf) == -1)
                error("can't access directory %s", dir);
        if ((stbuf.st_mode & S_IFMT) != S_IFDIR)
                error("%s is no a directory", dir);
        for (i = 1; i < argc-1; i++)
                sv(argv[i], dir);
        exit(0);
}

void sv(char *file, char *dir)  /* save file in dir */
{
        struct stat sti, sto;
        int     fin, fout, n;
        char    target[BUFSIZ], buf[BUFSIZ];

        sprintf(target, "%s/%s", dir, file);
        if (strchr(file, '/') != NULL)
                error("won't handle /'s in %s", file);
        if (stat(file, &sti) == -1)
                error("can't stat %s", file);
        if (stat(target, &sto) == -1)   /* target not present */
                sto.st_mtim.tv_sec = 0;        /* st_mtime = 0; so make it look old */
        if (sti.st_mtim.tv_sec < sto.st_mtim.tv_sec)    /* target is newer */
                fprintf(stderr, "%s: %s not copied\n", progname, file);
        else if ((fin = open(file, 0)) == -1)   /* 0 - O_RDONLY, 1 - O_WRONLY, 2 - O_RDWR */
                error("can't open file %s", file);
        else if ((fout = creat(target, sti.st_mode)) == -1)
                error("can't create %s", target);
        else
                while ((n = read(fin, buf, sizeof buf)) > 0)
                        if (write(fout, buf, n) != n)
                                error("error writing %s", target);
        close(fin);
        close(fout);
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