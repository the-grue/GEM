#include "dosglob.h"

	WORD
dos_chmod(pname, func, attr)	/* change file mode */
	LONG		pname;
	WORD		func;
	WORD		attr;
{
	DOS_CX = attr;
	dos_func((UWORD) 0x4300 + func, LLOWD(pname), LHIWD(pname));
	return((WORD) DOS_CX);
}
