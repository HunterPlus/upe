/* pick: offer choice on each argument	*/

#include <stdio.h>
#include <stdlib.h>

void pick(char *);		/* offer choice of string s	*/
FILE *efopen(char *, char *);	
int ttyin(void);

int main(int argc, char *argv[])
{
	
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
