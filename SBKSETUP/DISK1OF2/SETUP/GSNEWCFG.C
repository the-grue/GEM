/************************************************************************/
/*       Copyright 1999, Caldera Thin Clients, Inc.                     */ 
/*       This software is licenced under the GNU Public License.        */ 
/*       Please see LICENSE.TXT for further information.                */ 
/*                                                                      */ 
/*                  Historical Copyright                                */ 
/*									*/
/*	Copyright (c) 1987 Digital Research Inc.		        */
/*    The software contained in this listing is proprietary to       	*/
/*    Digital Research Inc., Pacific Grove, California and is        	*/
/*    covered by U.S. and other copyright protection.  Unauthorized  	*/
/*    copying, adaptation, distribution, use or display is prohibited	*/
/*    and may be subject to civil and criminal penalties.  Disclosure	*/
/*    to others is prohibited.  For the terms and conditions of soft-	*/
/*    ware code use refer to the appropriate Digital Research        	*/
/*    license agreement.						*/
/************************************************************************/

/* gsnewcfg.c		routines for setting up a new configuration.	*/

#include "portab.h"
#include "machine.h"
#include "dosbind.h"
#include "gsdefs.h"
#include "gstxtdef.h"
#include "gsevars.h"

EXTERN WORD	N_FDRV() ;			/* dosasm.asm */
EXTERN VOID	strcpy() ;			/* gsutil.c */
EXTERN WORD	strlen() ;			/* gsutil.c */
EXTERN WORD 	ibm_type() ;    		/* scrnutil.asm */
EXTERN BOOLEAN	file_copy() ;			/* gsfilcpy.c */

/*----------------------------------------------------------------------*/
    VOID
cpy_strtup()
{
    file_copy( src_path, gemroot, gembat, FALSE ) ; 
    file_copy( src_path, gemsys, gemexe, FALSE ) ; 
    file_copy( src_path, gemsys, gemrsc, FALSE ) ; 
    file_copy( src_path, gemsys, calclockacc, FALSE ) ;
} /* cpy_strtup() */

/*----------------------------------------------------------------------*/
    VOID
cpy_dsktop()
{
    file_copy( src_path, gemsys, desktopinf, TRUE ) ;
    file_copy( src_path, gemsys, deskhiicn, FALSE ) ;
    file_copy( src_path, gemsys, deskloicn, FALSE ) ;
    file_copy( src_path, gemsys, desktopapp, FALSE ) ;
    file_copy( src_path, gemsys, desktoprsc, FALSE ) ;
    file_copy( src_path, gemapps, outputapp, FALSE ) ;
    file_copy( src_path, gemapps, outputrsc, FALSE ) ;
} /* cpy_dsktop() */

/*----------------------------------------------------------------------*/
    VOID
do_mkdir( dir )
BYTE	*dir ;
{
	BYTE	tmp_str[ FNAME_LENGTH ] ;
	
	strcpy( tmp_str, dir );
	tmp_str[ strlen( tmp_str ) - 1 ] = NULL ;
	dos_mkdir( ADDR( tmp_str ));
}  /*  END do_mkdir */
	
/*----------------------------------------------------------------------*/
    VOID
strtup_dirs()
{
    gemsys[ 0 ] = clipbrd[ 0 ] = gemfonts[ 0 ] = 
    					gemapps[ 0 ] = DEFAULT_DRV[ 0 ] ;
    do_mkdir( gemapps ) ;
    do_mkdir( gemsys) ;
}  /*  END strtup_dirs */
	
/*----------------------------------------------------------------------*/
    VOID
dsktop_dirs()
{
    gemsys[ 0 ] = clipbrd[ 0 ] = gemfonts[ 0 ] = 
    					gemapps[ 0 ] = DEFAULT_DRV[ 0 ] ;
    do_mkdir( gemapps ) ;
    do_mkdir( gemsys) ;
    do_mkdir( clipbrd) ;
    do_mkdir( gemfonts ) ;
}  /*  END dsktop_dirs */
	
/*----------------------------------------------------------------------*/
    WORD
is_360k()
{
	WORD ii;
	
	ii = ibm_type();
	if (ii == 0x00ff)
		return(TRUE);
	return(FALSE);
} /* is_360k */
            
/*----------------------------------------------------------------------*/
    VOID
set_drives( harddrives, flpydrives )
WORD	*harddrives, *flpydrives ;
{
    WORD    cur_drv ;
    WORD    num_fd, num_ld, drive;
    UWORD   mask, drv, hdrv;
    FCB     fcb1;
    BYTE    fname[8];
    
    cur_drv = dos_gdrv();
    dos_sdrv( cur_drv );
    num_ld = DOS_AX & 0x00FF;
    num_fd = N_FDRV();
    drv =  (UWORD)((WORD)0x8000 >> (num_fd-1) );
    if( num_fd == 1 )
    {
        mask = 0x2000;
        num_ld -= 2;
        drive = 3;
    }
    else
    {
        mask = (UWORD)0x8000 >> num_fd;
        num_ld -= num_fd;
        drive = num_fd + 1;
    }
    hdrv = 0;
    if( dos_version() >= 0x0300 )
    {
        while( mask && num_ld )
        {
            dos_ioctl( drive, 8, 0, (BYTE *)0 );
            if( !DOS_ERR )
            {
                num_ld--;
                drv |= mask;
                if( DOS_AX )
                    hdrv |= mask;
            }
            mask >>= 1;
            drive++;
        }
    }
    else
    {
        /* strcpy( fname, "A:\\*.*" ); */
	fname[0] = UCASE_A;
	fname[1] = COLON;
	fname[2] = SLASH;
	fname[3] = fname[5] = ASTERISK;
	fname[4] = DOT;

        while( mask && num_ld )
        {
            fname[0] = drive + '@';
            dos_parse( &fcb1, fname, 1 );
            if( !DOS_ERR && (DOS_AX & 0x00FF) != 0x00FF )
            {
                num_ld--;
                drv |= mask;
                hdrv |= mask;
            }
            mask >>= 1;
            drive++;
        }
    }
    *harddrives = hdrv;
    *flpydrives = ( drv & ~hdrv ) ;
} /* set_drives() */

#define	SCRN3	scrn_node[3-1]
#define SCRN1	scrn_node[1-1]
/*----------------------------------------------------------------------*/
/* This procedure is always called					*/
    BOOLEAN
find_drives()
{
WORD	drives ;
BYTE 	bb ;
UWORD	cur_drive, mask ;

    config = HARD_DISK;
    SCRN1.choice[ 0 ].next = 12 ;
    drives =  ( harddrives | flpydrives ) ;
    cur_drive = 0x8000 ;
    for ( bb = UCASE_A; bb < src_path[ 0 ]; bb++ )
        cur_drive >>= 1 ;
    mask = (drives & ~cur_drive ) ;
    if ( ! mask )
        return( FALSE ) ;		/* only one drive ! */
    if ( harddrives == 0x0000 )
        {
	DEFAULT_DRV[ 0 ] = UCASE_A ;
	while ( !( mask & 0x8000 ) )
	    {
	    ++DEFAULT_DRV[ 0 ] ;
	    mask <<= 1 ;
	    }
	if (is_360k())
	    {
	    config = DUAL_FLOPPY ;	/* Dual 360k floppies */
	    SCRN1.choice[ 0 ].next = 6 ;
	    }
        else
	    {
	    config = SING_FLOPPY ;	/* At least 1 high capacity */
	    SCRN1.choice[ 0 ].next = 5 ;
	    }
	}
    else
    	{
	mask = 0x2000 ;		/* start at C */
	DEFAULT_DRV[ 0 ] = UCASE_C ;
	while ( !( mask & harddrives ) )
	    {
	    mask >>= 1 ;
	    DEFAULT_DRV[ 0 ] += 1 ;
	    }
	if ( !( harddrives & ~mask ) )		/* only one hard disk */
	    return( TRUE ) ;

	SCRN1.choice[ 0 ].next = 3 ;
	}
	return( TRUE ) ;
} /* find_drives() */

/*----------------------------------------------------------------------*/
/* call this routine after destination disk is established. 		*/
    VOID
setup_src()
{
    BYTE	drive ;
    WORD	mask ;
    
    if ( config == HARD_DISK )
	    {
	    drive = src_path[0] - UCASE_A;
	    mask = 0x8000;
	    while ( drive ) 
		    {
		    mask >>= 1 ;
		    --drive ;
		    }
	    if ( mask & harddrives )    
		{  /* we're being run from an existing hard disk system */
		src_path[ 0 ] = UCASE_A ;
		src_path[ 3 ] = NULL ;
		}
	    }
} /* setup_src() */

/*----------------------------------------------------------------------*/
    VOID
set_hdrive_id()
{
    DEFAULT_DRV[ 0 ] = misc_choices[ picked ][ 0 ] ;
    setup_src() ;
} /* set_hdrive_id() */

/*----------------------------------------------------------------------*/
/* next choice is set to 12 ONLY if find_drives() was able to determine	*/
/* destination drive id.						*/
    VOID
init_drives()
{
    is_new_config = TRUE ;
    find_drives() ;
    setup_src() ;
} /* init_drives() */

/* end of setdrive.c */
