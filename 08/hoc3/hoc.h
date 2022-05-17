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

int yylex(void);
void yyerror(char *);
void execerror(char *, char*);
void init(void);

Symbol *install(char *, int, double);
Symbol *lookup(char *);

void *emalloc(int);

double Log(double);
double Log10(double);
double Exp(double);
double Sqrt(double);
double integer(double);
double Pow(double, double);
double errcheck(double, char *);

