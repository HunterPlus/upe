/* pick: offer choice on each argument	*/

#include <stdio.h>
#include <stdlib.h>

void pick(char *);		/* offer choice of string s	*/
FILE *efopen(char *, char *);	
int ttyin(void);

char *progname;		/* program name for error message	*/
int main(int argc, char *argv[])
{
	int	i;
	char	buf[BUFSIZ];
	
	progname = argv[0];
	if (argc == 2 && strcmp(argv[1], "-") == 0)	/* pick -	*/
		while (fgets(buf, BUFSIZ, stdin) != NULL) {
			buf[strlen(buf) - 1] = '\0';	/* drop newline	*/
			pick(buf);
		}
	else
		for (i = 1; i < argc; i++)
			pick(argv[i]);
	exit(0);
}

void pick(char *s)
{
	fprintf(stderr, "%s? ", s);
	if (ttyin() == 'y')
		printf("%s\n", s);
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
