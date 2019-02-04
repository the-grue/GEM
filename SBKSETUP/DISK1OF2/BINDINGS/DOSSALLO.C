#include "dosglob.h"

	LONG
dos_alloc(nbytes)	/* allocate memory */
	LONG		nbytes;
{
	LONG		maddr;

	DOS_AX = 0x4800;
	if (nbytes == 0xFFFFFFFFL)	/* convert number */
	  DOS_BX = 0xffff;		/* 	of bytes  */
	else				/*	to	  */
	  DOS_BX = (nbytes + 15L) >> 4L;/*	paragraphs*/
	__DOS();

	if (DOS_ERR)
	  maddr = 0x0L;
	else
	  maddr = HW(DOS_AX) & 0xFFFF0000L;

	return(maddr); /* return location of allocated memory block */
}
