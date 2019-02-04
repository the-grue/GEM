#include "dosglob.h"


	WORD
dos_rename(poname, pnname)	/* rename file */
	LONG		poname;
	LONG		pnname;
{
	DOS_DI = LLOWD(pnname);
	DOS_ES = LHIWD(pnname);
	dos_func(0x5600, LLOWD(poname), LHIWD(poname));
	return((WORD) DOS_AX);
}
