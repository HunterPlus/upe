%{
#include "hoc.h"

int yylex(void);
void yyerror(char *);
void execerror(char *, char*);

extern double Pow();
%}
%union {
	double	val;		/* actual value */
	Symbol	*sym;		/* symbol table pointer */
}
%token	<val>	NUMBER
%token	<sym>	VAR BLTIN UNDEF
%type	<val>	expr asgn
%right	'='
%left	'+' '-'
%left 	'*' '/'
%left	UNARYMINUS
%right	'^'		/* exponentiation */
%%
list:	  /* nothing */
	| list '\n'
	| list asgn '\n'
	| list expr '\n'	{ printf("\t%.8g\n", $2); }
	| list error '\n'	{ yyerror; }
	;
asgn:	  VAR '=' expr { $$=$1->u.val=$3; $1->type = VAR; }
	;
expr:	  NUMBER
	| VAR { if ($1->type == UNDEF)
			execerror("undefined variable", $1->name);
		$$ = $1->u.val; }
	| asgn
	| BLTIN '(' expr ')'	{ $$ = (*($1->u.ptr))($3); }
	| expr '+' expr { $$ = $1 + $3; }
	| expr '-' expr { $$ = $1 - $3; }
	| expr '*' expr { $$ = $1 * $3; }
	| expr '/' expr {
		if ($3 == 0.0)
			execerror("division by zero", "");
		$$ = $1 / $3; }
	| expr '^' expr { $$ = Pow($1, $3); }
	| '(' expr ')' 	{ $$ = $2; }
	| '-' expr %prec UNARYMINUS { $$ = -$2; }
	;
%%
	/* end of grammar */

#include <ctype.h>
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
	signal(SIGFPE, fpecatch);
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

void yyerror(char *s)			/* called for yacc syntax error */
{
	warning(s, (char *) 0);
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
