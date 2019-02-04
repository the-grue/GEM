#include "dosglob.h"


	WORD
dos_sdta(ldta)	/* set the Disk Transfer Address ( DTA ) */
	LONG		ldta;
{
	dos_func(0x1a00, LLOWD(ldta), LHIWD(ldta));
}

