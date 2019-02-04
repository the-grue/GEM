#include "dosglob.h"


	WORD
dos_dele(pname)	/* delete file */
	LONG		pname;
{
	dos_func(0x4100, LLOWD(pname), LHIWD(pname));
	return((WORD) DOS_AX);
}
