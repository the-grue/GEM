#include "dosglob.h"

EXTERN WORD	__DOS();  			/* in DOSASM.ASM 	*/

GLOBAL UWORD	DOS_AX;
GLOBAL UWORD	DOS_BX;
GLOBAL UWORD	DOS_CX;
GLOBAL UWORD	DOS_DX;
GLOBAL UWORD	DOS_DS;
GLOBAL UWORD	DOS_ES;
GLOBAL UWORD	DOS_SI;
GLOBAL UWORD	DOS_DI;
GLOBAL UWORD	DOS_ERR;

GLOBAL BYTE	gl_dta[128];

	VOID
dos_func(ax, lodsdx, hidsdx)  
	UWORD		ax;
	UWORD		lodsdx;
	UWORD		hidsdx;
{
	DOS_AX = ax;
	DOS_DX = lodsdx;	/* low  DS:DX   */
	DOS_DS = hidsdx;	/* high DS:DX   */

	__DOS();
}

