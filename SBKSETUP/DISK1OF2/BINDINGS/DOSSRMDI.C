#include "dosglob.h"


	WORD
dos_rmdir(ppath)	/* remove directory entry */
	LONG		ppath;
{
	dos_func(0x3a00, LLOWD(ppath), LHIWD(ppath));
	return(!DOS_ERR);
}

