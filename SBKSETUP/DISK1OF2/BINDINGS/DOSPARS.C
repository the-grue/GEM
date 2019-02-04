#include <dosglob.h>

    BYTE
*dos_parse( fcb1, tail, flag )
    FCB     *fcb1;
    BYTE    *tail;
    WORD    flag;
{
    DOS_AX = ( 0x2900 | ( flag & 0x00FF ) );

    DOS_DS = LHIWD(ADDR(tail));
    DOS_SI = LLOWD(ADDR(tail));
    DOS_ES = LHIWD(ADDR(fcb1));
    DOS_DI = LLOWD(ADDR(fcb1));
    
    __DOS();
    
    return( (BYTE *)DOS_SI );
}

