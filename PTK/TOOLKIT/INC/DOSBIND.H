/*****************************************************************************/
/*   DOSBIND.H   -  Common dosbind definitions and structures.               */
/*****************************************************************************/
/*                                                                           */
/*    GEM/3 Programmers Toolkit - Digital Research, Inc.                     */
/*                                                                           */
/*    Author:  Robert Schneider                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef __DOSBIND__
#define __DOSBIND__

EXTERN UWORD DOS_ERR;

/*
 * Common DOS-Interrupts
 */
VOID  __dos          _((VOID));
VOID  dos_func       _((UWORD ax, UWORD lodsdx, UWORD hidsdx));

/*
 * Drive functions
 */
WORD  dos_gdrv       _((VOID));
WORD  dos_sdrv       _((WORD newdrv));
WORD  dos_space      _((WORD drv, LONG FAR *ptotal, LONG FAR *pavail));

/*
 * Directory functions
 */
WORD  dos_chdir      _((BYTE FAR *pdrvpath));
WORD  dos_gdir       _((WORD drive, BYTE FAR *pdrvpath));
WORD  dos_rmdir      _((BYTE FAR *ppath));
WORD  dos_mkdir      _((BYTE FAR *ppath));

/*
 * File maintenance
 */
WORD  dos_setdt      _((WORD handle, WORD time, WORD date));
WORD  dos_getdt      _((WORD handle, WORD FAR *time, WORD FAR *date));
WORD  dos_delete     _((BYTE FAR *pname));
WORD  dos_rename     _((BYTE FAR *poname, BYTE FAR *pnname));
WORD  dos_chmod      _((BYTE FAR *pname, WORD func, WORD attr));

/*
 * Find files
 */
WORD  dos_sdta       _((VOID FAR *ldta));
VOID FAR *dos_gdta   _((VOID));
WORD  dos_sfirst     _((BYTE FAR *pspec, WORD attr));
WORD  dos_snext      _((VOID));

/*
 * File I/O
 */
WORD  dos_create     _((BYTE FAR *pname, WORD attr));
WORD  dos_open       _((BYTE FAR *pname, WORD access));
UWORD  read_piece    _((WORD handle, UWORD cnt, VOID FAR *pbuffer));
LONG  dos_read       _((WORD handle, LONG cnt, VOID FAR *pbuffer));
UWORD  write_piece   _((WORD handle, UWORD cnt, VOID FAR *pbuffer));
LONG  dos_write      _((WORD handle, LONG cnt, VOID FAR *pbuffer));
LONG  dos_lseek      _((WORD handle, WORD smode, LONG sofst));
WORD  dos_close      _((WORD handle));

/*
 * Memory
 */
VOID FAR *dos_alloc  _((LONG nbytes));
LONG  dos_avail      _((VOID));
WORD  dos_free       _((VOID FAR *maddr));

/*
 * Process managment
 */
WORD  dos_exec       _((BYTE FAR *pname, BYTE FAR *para, WORD envrn));
WORD  dos_wait       _((VOID));

/*
 * Date and Time
 */
VOID  dos_getdate    _((WORD FAR *yr, WORD FAR *mo,
                        WORD FAR *dy, WORD FAR *dw));
VOID  dos_setdate    _((WORD yr, WORD mo, WORD dy));
VOID  dos_gettime    _((WORD FAR *hr, WORD FAR *mi,
                        WORD FAR *se, WORD FAR *hn));
VOID  dos_settime    _((WORD hr, WORD mi, WORD se, WORD hn));

/*
 * Miscellanous
 */
VOID  dos_version    _((WORD FAR *vh, WORD FAR *vl,
                        WORD FAR *oem, LONG FAR *user));

/*
 * Standard handles
 */
#define STDIN           0x0000
#define STDOUT          0x0001
#define STDERR          0x0002
#define STDAUX          0x0003
#define STDPRN          0x0004

/*
 * DOS error codes
 */
#define E_BADFUNC       1
#define E_FILENOTFND    2
#define E_PATHNOTFND    3
#define E_NOHANDLES     4
#define E_NOACCESS      5
#define E_BADHANDLE     6
#define E_MEMBLKERR     7
#define E_NOMEMORY      8
#define E_BADMEMBLK     9
#define E_BADENVIR      10
#define E_BADFORMAT     11
#define E_BADACCESS     12
#define E_BADDATA       13
#define E_BADDRIVE      15
#define E_NODELDIR      16
#define E_NOTDEVICE     17
#define E_NOFILES       18

/*
 * Parameter for dos_sfirst
 */
#define F_RDONLY        0x01
#define F_HIDDEN        0x02
#define F_SYSTEM        0x04
#define F_VOLUME        0x08
#define F_SUBDIR        0x10
#define F_ARCHIVE       0x20

/*
 * Parameter for dos_chmod
 */
#define F_GETMOD        0x0
#define F_SETMOD        0x1

/*
 * Paramter for dos_exec (Start sub-process)
 */
typedef struct exec_blk
{

   WORD  eb_segenv;
   BYTE FAR *eb_pcmdln;
   LONG  eb_pfcb1;
   LONG  eb_pfcb2;

} EXEC_BLK;

/*
 * Parameter for dos_exec (Load overlay)
 */
typedef struct over_blk
{

   WORD  ob_seglod;
   WORD  ob_relfac;

} OVER_BLK;

#endif /* __DOSBIND__ */
