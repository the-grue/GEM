#include "dosglob.h"

	WORD
dos_sfirst(pspec, attr)	/* search for first matching file */
	LONG		pspec;
	WORD		attr;
{
	DOS_CX = attr; /* file attributes */

	dos_func(0x4e00, LLOWD(pspec), LHIWD(pspec));
	return(!DOS_ERR);
}
