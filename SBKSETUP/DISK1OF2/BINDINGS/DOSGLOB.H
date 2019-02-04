
#include "portab.h"
#include "machine.h"
#include "dos.h"
						/* in DOSIF.A86		*/
EXTERN WORD	__DOS();
EXTERN WORD	__EXEC();

EXTERN UWORD	DOS_AX;
EXTERN UWORD	DOS_BX;
EXTERN UWORD	DOS_CX;
EXTERN UWORD	DOS_DX;
EXTERN UWORD	DOS_DS;
EXTERN UWORD	DOS_ES;
EXTERN UWORD	DOS_SI;
EXTERN UWORD	DOS_DI;
EXTERN UWORD	DOS_ERR;


EXTERN BYTE	gl_dta[];
