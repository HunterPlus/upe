%{
#include "hoc.h"
#define code2(c1,c2)	code(c1); code(c2)
#define code3(c1,c2,c3)	code(c1); code(c2); code(c3)
%}
%union {
	Symbol	*sym;		/* symbole table pointer */
	Inst	*inst;		/* machine instruction */
}
%token	<sym>	NUMBER PRINT VAR BLTIN UNDEF WHILE IF ELSE
%type	<inst>	stmt asgn expr stmtlist cond while if end
%right	'='
%left	OR
%left	AND
%left	GT GE LT LE EQ NE
%left	'+' '-'
%left	'*' '/'
%left	UNARYMINUS NOT
%right	'^'
%%
list:	  /* nothing */
	| list '\n'
	| list asgn '\n'	{ code2((Inst)pop, STOP); return 1; }
	| list stmt '\n'	{ code(STOP); return 1; }
	| list expr '\n'	{ code2(print, STOP); return 1; }
	| list error '\n'	{ yyerror; }
	;
asgn:	  VAR '=' expr		{ code3(varpush, (Inst)$1, assign); }
	;
stmt:	  expr			{ code((Inst)pop); }
	| PRINT expr		{ code(prexpr); $$ = $2; }
	| while cond stmt end {
		($1)[1] = (Inst)$3;	/* body of loop */
		($1)[2] = (Inst)$4; }	/* end, if cond fails */
	| if cond stmt end {		/* else-less if */
		($1)[1] = (Inst)$3;	/* thenpart */
		($1)[3] = (Inst)$4; }	/* end, if cond fails */
	| if cond stmt end ELSE stmt end {	/* if with else */
		($1)[1] = (Inst)$3;	/* thenpart */
		($1)[2] = (Inst)$6;	/* elsepart */
		($1)[3] = (Inst)$7; }	/* end, if cond fails */
	| '{' stmtlist '}'	{ $$ = $2; }
	;
cond:	  '(' expr ')'		{ code(STOP); $$ = $2; }
	;
while:	  WHILE	{ $$ = code3(whilecode, STOP, STOP); }
	;
if:	  IF	{ $$ = code(ifcode); code3(STOP, STOP, STOP); }
	;
end:	  /* nother */	{ code(STOP); $$ = progp; }
	;
stmtlist: /* nothing */	{ $$ = progp; }
	| stmtlist '\n'
	| stmtlist stmt
	;
expr:	  NUMBER	{ $$ = code2(constpush, (Inst)$1); }
	| VAR		{ $$ = code3(varpush, (Inst)$1, eval); }
	| asgn
	| BLTIN '(' expr ')' { $$ = $3; code2(bltin, (Inst)$1->u.ptr); }
	| '(' expr ')'	{ $$ = $2; }
	| expr '+' expr	{ code(add); }
	| expr '-' expr	{ code(sub); }
	| expr '*' expr	{ code(mul); }
	| expr '/' expr { code(xdiv); }
	| expr '^' expr { code(power); }
	| '-' expr %prec UNARYMINUS { $$ = $2; code(negate); }
	| expr GT expr	{ code(gt); }
	| expr GE expr	{ code(ge); }
	| expr LT expr	{ code(lt); }
	| expr LE expr	{ code(le); }
	| expr EQ expr	{ code(eq); }
	| expr NE expr	{ code(ne); }
	| expr AND expr	{ code(and); }
	| expr OR expr	{ code(or); }
	| NOT expr	{ $$ = $2; code(not); }
	;
%%
	/* end of grammar */

#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf	begin;
char	*progname;
int	lineno = 1;

int main(int argc, char *argv[])	/* hoc5 */
{
	void	fpecatch(int);
	
	progname = argv[0];
	init();
	setjmp(begin);
	signal(SIGFPE, fpecatch);
	for (initcode(); yyparse(); initcode())
		execute(prog);
	return 0;
}

int follow(int expect, int ifyes, int ifno)	/* look ahead for >= etc. */
{
		int	c = getchar();
		
		if (c == expect)
			return ifyes;
		ungetc(c, stdin);
		return ifno;
}

int yylex()		/* hoc5 */
{
	int	c;
	
	while ((c = getchar()) == ' ' || c == '\t')
		;
	if (c == EOF)
		return 0;
	if (c == '.' || isdigit(c)) {		/* number */
		double d;
		ungetc(c, stdin);
		scanf("%lf", &d);
		yylval.sym = install("", NUMBER, d);
		return NUMBER;
	}
	if (isalpha(c)) {
		Symbol *s;
		char sbuf[100], *p = sbuf;
		
		do {
			*p++ = c;
		} while ((c = getchar()) != EOF && isalnum(c));
		ungetc(c, stdin);
		*p = '\0';
		if ((s = lookup(sbuf)) == 0)
			s = install(sbuf, UNDEF, 0.0);
		yylval.sym = s;
		return s->type == UNDEF ? VAR : s->type;
	}
	switch(c) {
	case '>':	return follow('=', GE, GT);
	case '<':	return follow('=', LE, LT);
	case '=':	return follow('=', EQ, '=');
	case '!':	return follow('=', NE, NOT);
	case '|':	return follow('|', OR, '|');
	case '&':	return follow('&', AND, '&');
	case '\n':	lineno++; return '\n';
	default:	return c;
	}
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
