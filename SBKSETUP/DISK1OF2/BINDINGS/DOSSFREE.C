#include "dosglob.h"


	WORD
dos_free(maddr)	/* free memory that was allocated via dos_alloc() */
	LONG		maddr;
{
	DOS_AX = 0x4900;
	DOS_ES = LHIWD(maddr);

	__DOS();

	return((WORD) DOS_AX);
}

