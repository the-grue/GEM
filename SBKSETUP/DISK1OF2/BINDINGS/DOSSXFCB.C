#include "dosglob.h"


	WORD
dos_xfcb(xfcb,dta)
	
	LONG		xfcb;
	LONG		dta;
{
	DOS_BX = LLOWD(dta);
	dos_func(0x1100, LLOWD(xfcb), LHIWD(xfcb));
	return(DOS_AX & 0x00FF);
}

