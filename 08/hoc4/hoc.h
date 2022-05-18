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

typedef int (*Inst)();			/* machine instruction */
#define STOP (Inst) 0

int yylex(void);
void yyerror(char *);
void execerror(char *, char*);
void init(void);
void *emalloc(unsigned int);

Symbol *install(char *, int, double);
Symbol *lookup(char *);

Datum pop(void);

extern Inst prog[];

asign();
bltin();
varpush();
constpush();
print();

double Log(double);
double Log10(double);
double Exp(double);
double Sqrt(double);
double integer(double);
double Pow(double, double);
double errcheck(double, char *);