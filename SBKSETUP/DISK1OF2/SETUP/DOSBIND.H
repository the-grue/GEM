/*	DOSBIND.H	4/18/84 - 9/07/84	Lee Lorenzen		*/

EXTERN WORD	__DOS();
EXTERN WORD	__EXEC() ;
EXTERN BOOLEAN	DOS_ERR;
EXTERN UWORD	DOS_AX;
EXTERN UWORD	DOS_BX;
EXTERN UWORD	DOS_CX;
EXTERN UWORD	DOS_DX;
EXTERN UWORD	DOS_ES;
EXTERN UWORD	DOS_DS;
EXTERN UWORD    DOS_DI;
EXTERN BYTE	gl_dta[] ;

#define STDIN 0x0000
#define STDOUT 0x0001
#define STDERR 0x0002
#define STDAUX 0x0003
#define STDPRN 0x0004

#define E_BADFUNC 1
#define E_FILENOTFND 2
#define E_PATHNOTFND 3
#define E_NOHANDLES 4
#define E_NOACCESS 5
#define E_BADHANDLE 6
#define E_MEMBLKERR 7
#define E_NOMEMORY 8
#define E_BADMEMBLK 9
#define E_BADENVIR 10
#define E_BADFORMAT 11
#define E_BADACCESS 12
#define E_BADDATA 13
#define E_BADDRIVE 15
#define E_NODELDIR 16
#define E_NOTDEVICE 17
#define E_NOFILES 18

#define F_RDONLY 0x01
#define F_HIDDEN 0x02
#define F_SYSTEM 0x04
#define F_VOLUME 0x08
#define F_SUBDIR 0x10
#define F_ARCHIVE 0x20

#define F_GETMOD 0x0
#define F_SETMOD 0x1

typedef struct Xfcb_blk
{
    BYTE fc_flag;
    BYTE fc_rsv[5];
    BYTE fc_vol_attr;
    BYTE fc_drv;
    BYTE fc_name[11];
    WORD fc_cblock;
    WORD fc_rsize;
    LONG fc_fsize;
    WORD fc_date;
    WORD fc_time;
    BYTE fc_resv[8];
    BYTE fc_crec;
    LONG fc_rnum;
} XFCB;

typedef struct data_trans
{
    BYTE dta_reserved[21];
    BYTE dta_attr;
    BYTE dta_time[2];
    BYTE dta_date[2];
    LONG dta_size;
    BYTE dta_name[13];
} DTA;

typedef struct fcb_blk
{
    BYTE fc_drv;
    BYTE fc_name[11];
    WORD fc_cblock;
    WORD fc_rsize;
    LONG fc_fsize;
    WORD fc_date;
    WORD fc_time;
    BYTE fc_resv[8];
    BYTE fc_crec;
    LONG fc_rnum;
} FCB;

typedef struct exec_blk
{
	WORD		eb_segenv;
	LONG		eb_pcmdln;
	LONG		eb_pfcb1;
	LONG		eb_pfcb2;
} EXEC_BLK;


typedef struct over_blk
{
	WORD		ob_seglod;
	WORD		ob_relfac;
} OVER_BLK;

EXTERN  WORD dos_cin() ;	/* tcrtl.c */
EXTERN  WORD dos_getim() ;	/* tcrtl.c */
EXTERN  WORD dos_getdt() ;	/* tcrtl.c */
EXTERN  UWORD write_piece() ;    /* dos_writ.c in dosbind.lib */
EXTERN 	WORD dos_label();	/* dosbind.lib */
EXTERN 	WORD dos_space();
EXTERN 	WORD dos_rmdir();
EXTERN 	WORD dos_chdir();
EXTERN 	WORD dos_gdir();
EXTERN 	WORD dos_sdta();
EXTERN 	WORD dos_sfirst();
EXTERN 	WORD dos_snext();
EXTERN 	WORD dos_gdrv();
EXTERN 	WORD dos_sdrv();
EXTERN 	WORD dos_create();
EXTERN 	WORD dos_mkdir();
EXTERN 	WORD dos_open();
EXTERN 	WORD dos_delete();
EXTERN 	WORD dos_rename();
EXTERN 	WORD dos_close();
EXTERN 	LONG dos_read();
EXTERN 	LONG dos_write();
EXTERN 	FBYTE *dos_lseek();
EXTERN 	VOID dos_exec();
EXTERN 	WORD dos_wait();
EXTERN 	LONG dos_envir();
EXTERN 	WORD dos_chmod();
EXTERN 	FBYTE *dos_alloc();
EXTERN 	WORD dos_free();
EXTERN 	WORD dos_setdt();
EXTERN 	WORD dos_find();
EXTERN  LONG dos_gint();
EXTERN  VOID dos_sint();
EXTERN	VOID dos_fdup();
EXTERN  LONG dos_avail() ;
EXTERN  LONG dos_fsize() ;
EXTERN  WORD dos_version() ;
EXTERN  WORD dos_ioctl() ;
EXTERN  WORD dos_xfcb() ;
EXTERN  WORD dos_16h() ;
EXTERN  WORD dos_dele() ;
EXTERN  WORD dos_ren() ;
EXTERN  BYTE *dos_parse() ;
