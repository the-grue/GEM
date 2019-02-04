#include "dosglob.h"

	WORD
dos_mkdir(ppath)	/* create a sub-directory */
	LONG		ppath;
{
	dos_func(0x3900, LLOWD(ppath), LHIWD(ppath));
	return(!DOS_ERR);
}
