/* pick: offer choice on each argument	*/

#include <stdio.h>

void pick(char *);		/* offer choice of string s	*/

int main(int argc, char *argv[])
{
	
}

void pick(char *s)
{
	fprintf(stderr, "%s? ", s);
	if (ttyin() == 'y')
		printf("%s\n", s);
}
