#include "dosglob.h"

	UWORD
read_piece(handle, cnt, pbuffer)	/* read file */
	WORD		handle;
	UWORD		cnt;		/* number of bytes to read */
	LONG		pbuffer;	/* buffer to read into 	   */
{
	DOS_CX = cnt;
	DOS_BX = handle;
	dos_func(0x3f00, LLOWD(pbuffer), LHIWD(pbuffer));
	return( DOS_AX ) ; 	/* DOS_AX = number of bytes actually read */
}

	LONG
dos_read(handle, cnt, pbuffer) /* read complete file 32k at a time */
	WORD		handle;
	LONG		cnt;
	LONG		pbuffer;
{
	UWORD	buff_piece;
	LONG	rd_cnt;

	buff_piece = 0x8000; /* 32k */
	rd_cnt = 0L;
	DOS_ERR = FALSE;
	while (cnt && !DOS_ERR)
	{
		if (cnt > 0x00008000L)
			cnt -= 0x00008000L;
		else
		{
			buff_piece = cnt;
			cnt = 0;
		}
						 /* read 32k or less */
		rd_cnt += (ULONG)read_piece(handle, buff_piece, pbuffer);
#if	I8086
		pbuffer += 0x08000000L;
#else
		pbuffer += 0x00008000L;
#endif
	}
	return( rd_cnt );
}
