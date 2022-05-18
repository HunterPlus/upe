%{
#include "hoc.h"
#define code2(c1,c2)	code(c1); code(c2)
#define code3(c1,c2,c3)	code(c1); code(c2); code(c3)
%}
%union {
	Symbol	*sym;		/* symbole table pointer */
	Inst	*inst;		/* machine instruction */
}
