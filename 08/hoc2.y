%{
int yylex(void);
void execerror(char *, char *);

double	mem[26];		/* memory for variable 'a' ... 'z' */
%}
%union {			/* stack type */
	double 	val;		/* actual value */
	int	index;		/* index into mem[] */
}
%token 	<val>	NUMBER
%token 	<index>	VAR
%type	<val>	expr
%right	'='
%left	'+' '-'
%left	'*' '/'
%left	UNARYMINUS
%%
list:	  /* nothing */
	| list '\n'
	| list expr '\n'	{ printf("\t%.8g\n", $2); }
	| list error '\n'	{ yyerrok; }
	;
expr:	  NUMBER
	| VAR		{ $$ = mem[$1]; }
	| VAR '=' expr	{ $$ = mem[$1] = $3; }
	| expr '+' expr	{ $$ = $1 + $3; }
	| expr '-' expr	{ $$ = $1 - $3; }
	| expr '*' expr	{ $$ = $1 * $3; }
	| expr '/' expr	{
		if ($3 == 0.0)
			execerror("division by zero", "");
		$$ = $1 / $3; }
	| '(' expr ')'	{ $$ = $2; }
	| '-' expr %prec UNARYMINUS	{ $$ = -$2; }
%%
	/* end of grammar */

#include <signal.h>
#include <setjmp.h>
jmp_buf	begin;
char	*progname;
int	lineno = 1;

int main(int argc, char *argv[])		/* hoc2 */
{
	void	fpecatch(int);
	
	progname = argv[0];
	setjmp(begin);
	siganal(SIGFPE, fpecatch);
	yyparse();
}

int yylex()					/* hoc2 */
{
	int	c;
	
	while ((c = getchar()) == ' ' || c == '\t')
		;
	if (c == EOF)
		return 0;
	if (c == '.' || isdigit(c)) {
		ungetc(c, stdin);
		scanf("%lf", &yylval.val);
		return NUMBER;
	}
	if (islower(c)) {
		yylval.index = c - 'a';		/* ASCII only */
		return VAR;
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

void execerror(char *s, char *t)	/* recover from run-time error */
{
	warning(s, t);
	longjmp(begin, 0);
}

void fpecatch(int signo)	/* catch floating point exceptions */
{
	execerror("floating point exception", (char *) 0);
}
