#include "dosglob.h"


	VOID
dos_spnt(handle)  /* reset the file pointer to the beginning */
	WORD		handle;
{
	DOS_AX = 0x4200;
	DOS_CX = 0x0000;
	DOS_DX = 0x0000;

	__DOS();

}

