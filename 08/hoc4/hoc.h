#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

typedef struct Symbol { 		/* symbol table entry */
	char	*name;
	short	type;			/* VAR, BLTIN, UNDEF */
	union {
		double	val;		/* if VAR */
		double	(*ptr)();	/* if BLTIN */
	} u;
	struct Symbol	*next; 		/* to link to another */
} Symbol;

typedef union Datum {			/* interpreter stack type */
	double	val;
	Symbol	*sym;
} Datum;

int yylex(void);
void yyerror(char *);
void execerror(char *, char*);
void init(void);
void *emalloc(unsigned int);

Symbol *install(char *, int, double);
Symbol *lookup(char *);

typedef int (*Inst)();			/* machine instruction */
#define STOP (Inst) 0
extern Inst prog[];

Datum pop(void);
asign();
bltin();
varpush();
constpush();
print();
eval();
add();
sub();
mul();
div();
negate();
power();

double Log(double), Log10(double), Exp(double), Sqrt(double), integer(double);
double Pow(double, double);
double errcheck(double, char *);
