



#include <stdio.h>
#include <ctype.h>
char	*progname;		/* for error message */
int	lineno = 1;

int main(int argc, char *argv[])		/* hoc1 */
{
	progname = argv[0];
	yyparse();
}
