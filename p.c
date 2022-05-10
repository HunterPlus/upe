/* p: print input in chunks (version 1)

#include <stdio.h>
#include <stdlib.h>
#define PAGESIZE	22

void print(FILE *, int);

char	*progname;	/* program name for error message */
int main(int argc, char *argv[])
{
	int	i;
	FILE	*fp, *efopen();
	
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
