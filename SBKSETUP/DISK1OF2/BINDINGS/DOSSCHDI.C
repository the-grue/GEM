#include "dosglob.h"

	WORD
dos_chdir(pdrvpath) 	/*	change the current directory 	*/
	LONG		pdrvpath;
{
	dos_func(0x3b00, LLOWD(pdrvpath), LHIWD(pdrvpath));	
}

