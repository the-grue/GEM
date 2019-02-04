#include "dosglob.h"

	WORD
dos_gdir(drive, pdrvpath)	/*	get current directory	*/
	WORD		drive;
	LONG		pdrvpath;
{
	DOS_AX = 0x4700;
	DOS_DX = (UWORD) drive;	/* 0 = default drive, 1 = A:,etc */
	DOS_SI = LLOWD(pdrvpath);
	DOS_DS = LHIWD(pdrvpath);

	__DOS();

	return(TRUE);
}

