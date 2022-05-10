/* p: print input in chunks (version 1)

#include <stdio.h>
#include <stdlib.h>
#define PAGESIZE	22

void print(FILE *, int);
FILE *efopen(char *, char *);	/* fopen file, die if can't */

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
