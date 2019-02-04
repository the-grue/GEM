#include "dosglob.h"

EXTERN WORD	dos_sfirst() ;
EXTERN WORD	dos_sdta() ;

/*----------------------------------------------------------------------*/
/* get file size with DOS FIND FIRST call.				*/
    LONG
dos_fsize( name )
    BYTE	*name ;
{
    BYTE	dta[ 128 ] ;
    LONG	size ;

    dos_sdta( ADDR( dta ) ) ;
    dos_sfirst( ADDR( name ), 0 ) ;
    if (DOS_ERR)
	return ( 0x00L ) ;
    size = ((LONG)(dta[26])      ) + ((LONG)(dta[27]) << 8 ) +
	   ((LONG)(dta[28]) << 16) + ((LONG)(dta[29]) << 24) ;
    dos_sdta( ADDR( &gl_dta[ 0 ] ) ) ;
    return( size ) ;
} /* dos_fsize() */
