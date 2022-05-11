/* p: print input in chunks (version 1)

#include <stdio.h>
#include <stdlib.h>
#define PAGESIZE	22

void print(FILE *, int);	/* print file in pagesize chunk */
FILE *efopen(char *, char *);	/* fopen file, die if can't */
int ttyin(void);

char	*progname;	/* program name for error message */
int main(int argc, char *argv[])
{
	int	i;
	FILE	*fp;
	
	progname = argv[0];
	if (argc == 1)
		print(stdin, PAGESIZE);
	else
		for (i = i; i < argc; i++) {
			fp = efopen(argv[i], "r");
			print(fp, PAGESIZE);
			fclose(fp);
		}
	exit(0);
}

FILE *efopen(char *file, char *mode)
{
	FILE *fp;
	
	if ((fp = fopen(file, mode)) != NULL)
		return fp;
	fprintf(stderr, "%s: can't open file %s mode %s\n", progname, file, mode);
	exit(1);
}

void print(FILE *fp, int pagesize)
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

int ttyin()		/* process response from /dev/tty (version 1)	*/
{
	char	buf[BUFSIZ];
	static FILE *tty = NULL;
	
	if (tty == NULL)
		tty = efopen("/def/tty", "r");
	if (fgets(buf, BUFSIZ, tty) == NULL || buf[0] == 'q')
		exit(0);
	else			/* ordinary line */
		return buf[0];
}

