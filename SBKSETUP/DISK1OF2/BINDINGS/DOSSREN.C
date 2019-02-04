#include "dosglob.h"


	BOOLEAN
dos_ren(dta)
	LONG		dta;
{
	dos_func(0x1700, LLOWD(dta), LHIWD(dta));
	return(DOS_AX & 0x00FF);
}

