#include "dosglob.h"


	WORD
dos_len(handle)	/* return length of file specified by handle */
	WORD		handle;
{
	DOS_AX = 0x4202;
	DOS_CX = 0x0000;
	DOS_DX = 0x0000;

	__DOS();

	return((WORD) DOS_AX);
}

