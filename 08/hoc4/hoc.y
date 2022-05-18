%{
#include "hoc.h"

#define code2(c1,c2)	code(c1); code(c2)
#define code3(c1,c2,c3)	code(c1); code(c2); code(c3)
%}
%union {
	Symbol 	*sym;		/* Symbol table pointer */
	Inst	*inst;		/* Machine instruction */
}
%token	<sym>	NUMBER VAR BLTIN UNDEF
%right	'='
%left	'+' '-'
%left	'*' '/'
%left	UNARYMINUS
%right	'^'	/* exponentiation */
%%
list:	  /* nothing */
	| list '\n'
	| list asgn '\n'	{ code2(pop, STOP); return 1; }
	| list expr '\n'	{ code2(print, STOP); return 1; }
	| list error '\n'	{ yyerror; }
	;
asgn:	  VAR '=' expr		{ code3(varpush, (Inst)$1, assign); }
	;
expr:	  NUMBER		{ code2(constpush, (Inst)$1); }
	| VAR			{ code3(varpush, (Inst)$1, eval); }
	| asgn		
	| BLTIN '(' expr ')'	{ code2(bltin, (Inst)$1->u.ptr); }
	| expr '+' expr		{ code(add); }
	| expr '-' expr		{ code(sub); }
	| expr '*' expr		{ code(mul); }
	| expr '/' expr		{ code(div); }
	| expr '^' expr		{ code(power); }
	| '-' expr %prec UNARYMINUS { code(negate); }
	;
%%
	/* end of grammar */


int main(int argc, char *argv[])	/* hoc4 */
{
	void	fpecatch(int);
	
	progname = argv[0];
	init();
	setjump(begin);
	signal(SIGFPE, fpecatch);
	for (initcode(); yyparse(); initcode())
		execute(prog);
	return 0;
}
