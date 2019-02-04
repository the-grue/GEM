#include "dosglob.h"


	WORD
dos_getdt(handle, time, date)	/* get a file's date and time */
	WORD		handle ;
	UWORD		*time, *date;
{
	DOS_AX = 0x5700;
	DOS_BX = handle;
	__DOS();
	*time = DOS_CX ;
	*date = DOS_DX ;

}
