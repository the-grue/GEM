#include <dosglob.h>


    WORD
dos_ioctl(handle, func, length, buff) /* Device I/O control */
    WORD        handle;
    WORD        func, length;
    BYTE        *buff;
{
    LONG    ad_buff;
    
    DOS_AX = 0x4400;
    DOS_AX += func;
    DOS_BX = handle;
    DOS_CX = length;
    ad_buff = ADDR( buff );
    DOS_DS = LHIWD(ad_buff);
    DOS_DX = LLOWD(ad_buff);

    __DOS();

    return( !DOS_ERR );
}
