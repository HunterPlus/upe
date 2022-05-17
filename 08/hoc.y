%{
#define	YYSTYPE double		/* data type of yacc stack */
int yylex(void);
void yyerrors(char *);
%}

%token	NUMBER
%left	'+' '-'			/* left associative, same precedence */
%left 	'*' '/'			/* left assoc., higher precedence */
%left	UNARYMINUS		/* new */

%%
list:	  /* nothing */
	| list '\n'
	| list expr '\n'	{ printf("\t%.8g\n", $2); }
	;
expr:	  NUMBER		{ $$ = $1; }
	| '-' expr	%prec UNARYMINUS { $$ = -$2; }		/* new */
	| expr '+' expr		{ $$ = $1 + $3; }
	| expr '-' expr		{ $$ = $1 - $3; }
	| expr '*' expr		{ $$ = $1 * $3; }
	| expr '/' expr		{ $$ = $1 / $3; }
	| '(' expr ')'		{ $$ = $2; }
	;
%%	
	  /* end of grammar */
	  
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

void warning(char *s, char *t)		/* print warning message */
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
}

void yyerrors(char *s)			/* called for yacc syntax error */
{
	warning(s, (char *) 0);
}
