#include "hoc.h"
#include "y.tab.h"

#define	NSTACK	256
static	Datum	stack[NSTACK];		/* the stack */
static	Datum	*stackp;		/* next free spot on stack */

#define	NPROG	2000
Inst	prog[NPROG];			/* the machine */
Inst	*progp;				/* next free spot for code generation */
Inst	*pc;				/* program counter during execution */

initcode()				/* initialize for code generation */
{
	stackp = stack;
	progp = prog;
}

void push(Datum d)			/* push d onto stack */
{
	if (stackp >= &stack[NSTACK]) 
		execerror("stack overflow", (char *) 0);
	*stackp++ = d;
}

Datum pop()				/* pop and return top elem from stack */
{
	if (stackp <= stack)
		execerror("stack underflow", (char *) 0);
	return *--stackp;
}
