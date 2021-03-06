/* DOS.C for compicon.exe */

#include <portab.h>
#include <machine.h>


EXTERN WORD	__DOS();


GLOBAL UWORD	DOS_AX;
GLOBAL UWORD	DOS_BX;
GLOBAL UWORD	DOS_CX;
GLOBAL UWORD	DOS_DX;
GLOBAL UWORD	DOS_DS;
GLOBAL UWORD	DOS_ES;
GLOBAL UWORD	DOS_SI;
GLOBAL UWORD	DOS_DI;
GLOBAL UWORD	DOS_ERR;

	VOID
dos_func(ax, lodsdx, hidsdx)
	UWORD		ax;
	UWORD		lodsdx;
	UWORD		hidsdx;
{
	DOS_AX = ax;
	DOS_DX = lodsdx;
	DOS_DS = hidsdx;

	__DOS();
}




	WORD
dos_close(handle)
	WORD		handle;
{
	DOS_AX = 0x3e00;
	DOS_BX = handle;

	__DOS();

	return(!DOS_ERR);
}


	WORD
dos_create(pname, attr)
	LONG		pname;
	WORD		attr;
{
	DOS_CX = attr;
	dos_func(0x3c00, pname);

	return(DOS_AX);
}


	WORD
dos_write(handle, cnt, pbuffer)
	WORD		handle;
	WORD		cnt;
	LONG		pbuffer;
{
	DOS_CX = cnt;
	DOS_BX = handle;
	dos_func(0x4000, pbuffer);
	return(DOS_AX);
}

