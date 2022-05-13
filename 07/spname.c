#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#define PAGESIZE        22

int spdist(char *s, char *t);
int mindist(char *dir, char *guess, char *best);
int spname(char *oldname, char *newname);
FILE *efopen(char *, char *);
int ttyin();
void print(FILE *fp, int pagesize);

char    *progname;      /* program name for error message */
int main(int argc, char *argv[])
{
        FILE    *fp;
        int     i, pagesize = PAGESIZE;
        char    *p, buf[BUFSIZ];

        progname = argv[0];
        if ((p = getenv("PAGESIZE")) != NULL)
                pagesize = atoi(p);
        if (argc > 1 && argv[1][0] == '-') {
                pagesize = atoi(&argv[1][1]);
                argc--;
                argv++;
        }
        if (argc == 1)
                print(stdin, pagesize);
        else
                for (i = 1; i < argc; i++)
                        switch (spname(argv[i], buf)) {
                        case -1:        /* no match possible */
                                fp = efopen(argv[i], "r");
                                break;
                        case 1:         /* corrected */
                                fprintf(stderr, "\"%s\"? ", buf);
                                if (ttyin() == 'n')
                                        break;
                                argv[i] = buf;
                                /* fall through... */
                        case 0:         /* exact match */
                                fp = efopen(argv[i], "r");
                                print(fp, pagesize);
                                fclose(fp);
                        }
        exit(0);
}

/* spname: return correctly spelled filename */
/*
 * spname(char *oldname, char *newname);
 *  returns     -1 if no reasonable match to oldname,
 *               0 if exact match,
 *               1 if  corrected.
 *  stores corrected name in newname.
 */ 

int spname(char *oldname, char *newname)
{
        char    *p, guess[NAME_MAX + 1], best[NAME_MAX + 1];
        char    *new = newname, *old = oldname;

        for (;;) {
                while (*old == '/')     /* skip slashes */
                        *new++ = *old++;
                *new = '\0';
                if (*old == '\0')       /* exact or corrected */
                        return strcmp(oldname, newname) != 0;
                p = guess;      /* copy next component into guess */
                for (; *old != '/' && *old != '\0'; old++)
                        if (p < guess + NAME_MAX)
                                *p++ = *old;
                *p = '\0';
                if (mindist(newname, guess, best) >= 3)
                        return -1;      /* hopeless */
                for (p = best; *new = *p++; )   /* add to end */
                        new++;                  /* of newname */
        }
}

int mindist(char *dir, char *guess, char *best) /* search dir for guess */
{
        /* set best, return distance 0..3 */
        int     d, nd;
        DIR     *dp;
        struct dirent *ent;
        char    name[NAME_MAX + 1];

        if (dir[0] == '\0')     /* current directory */
                dir = ".";
        d = 3;                  /* minimum distance */
        if ((dp = opendir(dir)) == NULL)
                return d;
        while ((ent = readdir(dp)) != NULL) {
                strcpy(name, ent->d_name);
                nd = spdist(name, guess);
                if (nd <= d && nd !=3) {
                        strcpy(best, name);
                        d = nd;
                        if (d == 0)     /* exact match */
                                break;
                }
        }
        closedir(dp);
        return d;
}

/* spdist: return distance between two names */
/*
 *  very rough spelling metric:
 * 0 if the strings are identical
 * 1 if two chars are transposed
 * 2 if one char wrong, added or deleted
 * 3 otherwise
 */

#define EQ(s, t) (strcmp(s, t) == 0)

int spdist(char *s, char *t)
{
        while (*s++ == *t)
                if (*t++ == '\0')
                        return 0;       /* exact match */
        if (*--s) {
                if (*t) {
                        if (s[1] && t[1] && *s == t[1] && *t == s[1] && EQ(s+2, t+2))
                                return 1;       /* transpositon */
                        if (EQ(s+1, t+1))
                                return 2;       /* 1 char mismatch */
                }
                if (EQ(s+1, t))
                        return 2;               /* extra character */
        }
        if (*t && EQ(s, t+1))
                return 2;                       /* missing character */
        return 3;
}

FILE *efopen(char *file, char *mode)		/* fopen file, die if can't */
{
	FILE *fp;
	
	if ((fp = fopen(file, mode)) != NULL)
		return fp;
	fprintf(stderr, "%s: can't open file %s mode %s\n", progname, file, mode);
	exit(1);
}

int ttyin()		/* process response from /dev/tty (version 1)	*/
{
	char	buf[BUFSIZ];
	static FILE *tty = NULL;
	
	if (tty == NULL)
		tty = efopen("/dev/tty", "r");
	if (fgets(buf, BUFSIZ, tty) == NULL || buf[0] == 'q')
		exit(0);
	else			/* ordinary line */
		return buf[0];
}

void print(FILE *fp, int pagesize)	/* print file in pagesize chunk */
{
	static int lines = 0;		/* number of lines so far */
	char	buf[BUFSIZ];
	
	while (fgets(buf, sizeof buf, fp) != NULL)
		if (++lines < pagesize)
			fputs(buf, stdout);
		else {
			buf[strlen(buf) - 1] = '\0';
			fputs(buf, stdout);
			fflush(stdout);
			ttyin();
			lines = 0;
		}
}