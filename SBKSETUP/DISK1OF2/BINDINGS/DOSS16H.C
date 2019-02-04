#include "dosglob.h"


	BOOLEAN
dos_16h(xfcb)	/* set the Disk Transfer Address ( DTA ) */
	LONG		xfcb;
{
	dos_func(0x1600, LLOWD(xfcb), LHIWD(xfcb));
	return(DOS_AX & 0x00FF);
}

