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
