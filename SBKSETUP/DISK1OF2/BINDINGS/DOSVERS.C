#include <dosglob.h>


    WORD
dos_version()    /* Get DOS version number  */
{
    UWORD   version;
    
    DOS_AX = 0x3000;
    __DOS();
    version = ( DOS_AX << 8 ) | ( (UWORD)DOS_AX >> 8 );
    return( version );
}
