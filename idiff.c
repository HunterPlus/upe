/* idiff: interactive diff */

#include <stdio.h>
#include <stdlib.h>             /* exit(); char *mktemp(char *template); */
#include <ctype.h>
#include <unistd.h>             /* int unlink(const char *pathname)     */

FILE *efopen(char *, char *);
void idiff(FILE *, FILE *, FILE *, FILE *);
void parse(char *, int *, int *, int *, int *, int *);

char    *progname;
#define HUGE    10000   /* large number of lines */
int main(int argc, char *argv[])
{
        FILE    *fin, *fout, *f1, *f2;
        char    buf[BUFSIZ];
        char    *diffout = "idiff.xxxxxx";

        progname = argv[0];
        if (argc != 3) {
                fprintf(stderr, "Usage: idiff file1 file2\n");
                exit(1);
        }
        f1 = efopen(argv[1], "r");
        f2 = efopen(argv[2], "r");
        fout = efopen("idiff.out", "w");
        mktemp(diffout);
        sprintf(buf, "diff %s %s >%s", argv[1], argv[2], diffout);
        system(buf);
        fin = efopen(diffout, "r");
        idiff(f1, f2, fin, fout);
        unlink(diffout);
        printf("%s output in file idiff.out\n", progname);
        exit(0);
}

vid idff(FILE *f1, FILE *f2, FILE *fin, FILE *fout)	/* process diffs */
{
	char	*tempfile = "idiff.xxxxxx";
	char	buf[BUFSIZ], buf2[BUFSIZ];
	FILE	*ft;
	int	cmd, n, from1, to1, from2, to2, nf1, nf2;
	
	mktemp(tempfile);
	nf1 = nf2 = 0;
	while (fgets(buf, sizeof buf, fin) != NULL) {
		parse(buf, &from1, &to1, &cmd, &from2, &to2);
	}
}

void parse(char *s, int *pfrom1, int *pto1, int *pcmd, int *from2, int *pto2)
{
	
}

FILE *efopen(char *file, char *mode)		/* fopen file, die if can't */
{
	FILE *fp;
	
	if ((fp = fopen(file, mode)) != NULL)
		return fp;
	fprintf(stderr, "%s: can't open file %s mode %s\n", progname, file, mode);
	exit(1);
}
