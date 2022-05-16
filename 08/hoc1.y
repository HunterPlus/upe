



#include <stdio.h>
#include <ctype.h>
char	*progname;		/* for error message */
int	lineno = 1;

int main(int argc, char *argv[])		/* hoc1 */
{
	progname = argv[0];
	yyparse();
}

int yylex()					/* hoc1 */
{
	int	c;
	
	while ((c = getchar()) == ' ' || c == '\t')
		;
	if (c == EOF)
		return 0;
	if (c == '.' || isdigit(c)) {
		ungetc(c, stdin);
		scanf("%lf", &yylval);
		return NUMBER;
	}
	if (c == '\n')
		lineno++;
	return c;
}
