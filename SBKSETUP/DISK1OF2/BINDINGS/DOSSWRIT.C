#include "dosglob.h"


	UWORD
write_piece(handle, cnt, pbuffer)	/*  write to a file */
	WORD		handle;
	UWORD		cnt;
	LONG		pbuffer;
{
	DOS_CX = cnt;
	DOS_BX = handle;
	dos_func(0x4000, LLOWD(pbuffer), LHIWD(pbuffer));
	return( DOS_AX ) ;
}
	LONG
dos_write(handle, cnt, pbuffer)	/* write 32k or less to a file */

	WORD		handle;
	LONG		cnt;
	LONG		pbuffer;
{
	UWORD	buff_piece;
	ULONG	wt_cnt;

	buff_piece = 0x8000;
	wt_cnt = 0L;
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
		wt_cnt += (ULONG)write_piece(handle, buff_piece, pbuffer);
#if	I8086
		pbuffer += 0x08000000L;
#else
		pbuffer += 0x00008000L;
#endif
	}
	return( wt_cnt );
}
