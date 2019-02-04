#include "dosglob.h"

	WORD
dos_create(pname, attr)	/* create file */
	LONG		pname;
	WORD		attr;
{
	DOS_CX = attr;
	dos_func(0x3c00, LLOWD(pname), LHIWD(pname));

	return((WORD) DOS_AX);
}

