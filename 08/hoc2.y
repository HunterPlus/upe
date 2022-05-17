%{
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
			execerror("divison by zero", "");
		$$ = $1 / $3; }
	| '(' expr ')'	{ $$ = $2; }
	| '-' expr %prec UNARYMINUS	{ $$ = -$2; }
%%
	/* end of grammar */
