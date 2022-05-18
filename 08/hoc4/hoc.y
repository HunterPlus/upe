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
