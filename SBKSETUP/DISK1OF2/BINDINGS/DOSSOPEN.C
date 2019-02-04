#include "dosglob.h"


	WORD
dos_open(pname, access)	/* open file */
	LONG		pname; /* filename */
	WORD		access;/* 0 = read, 1 = write, 2 = both */
{
	dos_func((UWORD) 0x3d00 + access, LLOWD(pname), LHIWD(pname));
	return((WORD) DOS_AX);	/* DOS_AX contains file handle */
}
