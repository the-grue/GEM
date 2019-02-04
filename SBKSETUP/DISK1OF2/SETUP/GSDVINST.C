/************************************************************************/
/**       Copyright 1999, Caldera Thin Clients, Inc.                     **/ 
/**       This software is licenced under the GNU Public License.        **/ 
/**       Please see LICENSE.TXT for further information.                **/ 
/**                                                                      **/ 
/**                  Historical Copyright                                **/ 
/**									**/
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

/* gsdvinst.c		install device drivers on target disk.		*/

#include "portab.h"
#include "machine.h"
#include "dosbind.h"
#include "gsdefs.h"
#include "gstxtdef.h"
#include "gsevars.h"

#define		SCRN_11		scrn_node[11-1]
#define		SCRN_17		scrn_node[17-1]

EXTERN VOID	strcpy() ;			/* gsutil.c */
EXTERN VOID	strcat() ;			/* gsutil.c */
EXTERN BOOLEAN	strcmp() ;			/* gsutil.c */
EXTERN BOOLEAN	file_copy() ;			/* gsfilcpy.c */
EXTERN BOOLEAN	get_vol() ;			/* gsfilcpy.c */
EXTERN BOOLEAN	get_vol2() ;			/* gsfilcpy.c */
EXTERN VOID	init_copy_echo() ;		/* gsfilcpy.c */
EXTERN VOID	strtup_dirs() ;			/* gsnewcfg.c */
EXTERN VOID	cpy_strtup() ;			/* gsnewcfg.c */
EXTERN VOID	dsktop_dirs() ;			/* gsnewcfg.c */
EXTERN VOID	cpy_dsktop() ;			/* gsnewcfg.c */
EXTERN VOID	do_floppies();			/* gsdualfp.c */
EXTERN VOID	beeper() ;			/* scrnutil.asm */

/*----------------------------------------------------------------------*/ 
/*  Finds disk label of diskette containing next driver to be installed */
    WORD
next_driver( label )
BYTE	*label ; 
{
WORD		num ;
DEV_STRUCT	*ptr ;

    num = 0 ;
    while ( num < num_devs )
        {
	ptr = &devices[ num ] ;
	if ( devices[ num ].installed == FALSE)
	    if ( strcmp( label, ptr->src_disk ) )
	        {
		cur_dev = num ;
		return( TRUE ) ;
		}
	if ( devices[ num ].fnts_installed == FALSE)
	    {   
	    if ( ( *ptr->font_disk && strcmp( label, ptr->font_disk ) )
		    || ( *ptr->font_disk == NULL && strcmp( label, ptr->src_disk ) ) )
	        {
		cur_dev = num ;
		return( -1 ) ;
		}
            }
	++num ;	
	}
    return( FALSE ) ;
} /* next_driver() */

/*----------------------------------------------------------------------*/ 
    BYTE
*next_dlabel()
{
WORD	num ;

    cur_dlabel = NULLPTR ;
    for ( num = 0; num < num_devs ; num++ )
        if ( devices[ num ].installed == FALSE)
	    {
	    cur_dlabel = devices[ num ].src_disk ;
	    num = num_devs ;		/* break out of loop */
	    }
    if ( cur_dlabel == NULLPTR )
        {
    	for ( num = 0; num < num_devs ; num++ )
            if ( devices[ num ].fnts_installed == FALSE)
	    	{
	    	cur_dlabel = devices[ num ].font_disk ;
	    	num = num_devs ;		/* break out of loop */
	    	}
	}
    return( cur_dlabel ) ;	
} /* next_dlabel() */

/*----------------------------------------------------------------------*/
    VOID
do_aux_file()
{
BYTE  	*begin, *end ;
BOOLEAN done;

	done = FALSE;
	begin = devices[ cur_dev ].aux_file;
	end = begin;
	
	while (!done)
		{
		if (*end == SEMICOLON)
			{
			*end = NULL;
			file_copy( src_path, gemsys, begin, FALSE );
			*end = SEMICOLON;
			end++;
			begin = end;
			}
		else if (*end == NULL)
			{
			file_copy( src_path, gemsys, begin, FALSE );
			done = TRUE;
			}
		else
			end++;
		}
}

/*----------------------------------------------------------------------*/
    BOOLEAN
do_drv_copy()
{
    file_copy( src_path, gemsys, devices[ cur_dev ].f_name, TRUE ) ;
    if ( devices[ cur_dev ].aux_file[ 0 ] != NULL )
	    do_aux_file();
    devices[ cur_dev ].installed = TRUE ;
    devices[ cur_dev ].change = FALSE ;
    return( TRUE ) ;
} /* do_drv_copy() */

/*----------------------------------------------------------------------*/
    BOOLEAN
do_fnt_copy()
{
BYTE		tmp[ FNAME_LENGTH ] ;
BOOLEAN 	ok ;
 
    strcpy( tmp, src_path ) ;
    strcat( tmp, devices[ cur_dev ].font_wcard ) ;
    dos_sdta( ADDR( gl_dta ) ) ;
    ok = dos_sfirst( ADDR( tmp ), 0x0 ) ;
    while ( ok )
	{
	if ( gl_dta[ 31 ]!= UCASE_D )	/* is not a driver */
            file_copy( src_path, gemfonts, &gl_dta[ 30 ], FALSE ) ; 
    	ok = dos_snext() ;
	}
    devices[ cur_dev ].fnts_installed = TRUE ;
    return( TRUE ) ;
} /* do_fnt_copy() */

/*----------------------------------------------------------------------*/
    VOID
do_change()
{
    file_copy( gemsys, gemsys, devices[ cur_dev ].f_name, TRUE ) ; 
    devices[ cur_dev ].change = FALSE ;
} /* do_change() */

/*----------------------------------------------------------------------*/
    VOID
dev_change() 
{
WORD	num ;

    for ( num = 0; num < num_devs; num ++ )
	if ( devices[ num ].change == TRUE)
	    {
   	    cur_dev = num ;
	    do_change() ;
	    }
} /* dev_change() */

/*----------------------------------------------------------------------*/
    VOID
dev_install( first )
BOOLEAN first ;
{
WORD	kind ;
BOOLEAN	ok ;

    init_copy_echo() ;
    if ( first )
    {
    	file_copy( src_path, gemsys, gemvdiexe, TRUE ) ; 
	file_copy( src_path, gemsys, mdsys, FALSE ) ;
    }
    kind = next_driver( cur_dlabel ) ; /* 1 = driver, -1 = fonts */
    while ( kind != 0 )
	{	    
	if ( kind == 1 )
	    ok = do_drv_copy() ;
	else 
	    ok = do_fnt_copy() ;
	kind = next_driver( cur_dlabel ) ;
	}
} /* dev_install() */

/*----------------------------------------------------------------------*/
    BYTE
*pos_dot( s )
BYTE *s;
{
	BYTE *p;
	
	p = s;
	while (*p && (*p != DOT))
		p++;
	return( p );
}
/*----------------------------------------------------------------------*/
    VOID
del_file( path, file)
BYTE	*path, *file;
{
BYTE	tmp[ FNAME_LENGTH ] ;

	strcpy( tmp, path ) ;
	strcat( tmp, file);
	dos_dele(ADDR( tmp ));
	if (DOS_ERR);
		/* ERROR */
}
		
/*----------------------------------------------------------------------*/
    VOID
rm_devices() 
{
WORD	ii, jj ;
BYTE	wildcard[ 6 ] = "*.*" ;
BOOLEAN is_being_used, ok ;
BYTE	tmp[ FNAME_LENGTH ] ;
   
    for ( ii = 0; ii < num_delete; ii++ )
        {
	del_file( gemsys, to_delete[ ii ] ) ;
	strcpy( pos_dot( wildcard ), pos_dot( to_delete[ ii ] ) ) ;
	is_being_used = FALSE ;
	for ( jj = 0; jj < num_devs; jj++ )
	    {
	    if (strcmp(pos_dot(wildcard), pos_dot(devices[jj].font_wcard)))
			is_being_used = TRUE;
	    if (strcmp(pos_dot(devices[jj].f_name),pos_dot(wildcard))) 
			is_being_used = TRUE;
	    }
	if ( !is_being_used )
	    {
    	    strcpy( tmp, gemfonts ) ;
	    strcat( tmp, wildcard ) ;
	    dos_sdta( ADDR( gl_dta ) ) ;
	    ok = dos_sfirst( ADDR( tmp ), 0x0 ) ;
	    while ( ok )
		{
		del_file( gemfonts, &gl_dta[ 30 ] ) ;
	    	ok = dos_snext() ;
		}
	    }
	}
} /* rm_devices() */

/*----------------------------------------------------------------------*/
/* This routine clears out the GEMAPPS/GEMSYS and GEMAPPS/FONTS dirs    */
    VOID
clear_system()
{
	BOOLEAN	ok;
	BYTE	file_buf[ FNAME_LENGTH ];
	
	strcpy(file_buf, gemsys);	/* Clear \GEMAPPS\GEMSYS */
	strcat(file_buf, drvr_wcard);
	dos_sdta( ADDR( gl_dta ) );
	ok = dos_sfirst( ADDR(file_buf), 0x0);
	while ( ok )
		{
		if (gl_dta[30] == 'S' || gl_dta[30] == 'P' ||
		    gl_dta[30] == 'V' || gl_dta[30] == 'C' ||
		    gl_dta[30] == 'I')
			    {
			    strcpy(to_delete[num_delete], &gl_dta[30]);
			    num_delete++;
		    }
		 ok = dos_snext();   
		 }
	rm_devices();
	num_delete = 0 ;
} /* clear_system */
/*----------------------------------------------------------------------*/
    VOID
wrong_disk() 
{
	beeper() ;
} /* wrong_disk() */

/*----------------------------------------------------------------------*/
    VOID
next_disk()
{
    if ( !get_vol( insert_disk_labeled, into_drive ) )
        {
	wrong_disk() ;
	return ;
	}
    dev_install( FALSE ) ;
    insert_disk_labeled = next_dlabel() ;
    if ( insert_disk_labeled != NULLPTR )
    	into_drive = src_path[ 0 ] ;
    else
        {
	dev_change() ;
        if (is_scan_com)
		SCRN_11.choice[ 0 ].next = 30 ;
	else if (is_bus_mouse)
		SCRN_11.choice[ 0 ].next = 32 ;
	else
		SCRN_11.choice[ 0 ].next = 31 ;
	}
} /* next_disk() */

/*----------------------------------------------------------------------*/
    VOID
first_disk()
{
    if ( !get_vol( insert_disk_labeled, into_drive ) )
        {
	wrong_disk() ;
	return ;
	}
    dev_install( TRUE ) ;
    insert_disk_labeled = next_dlabel() ;
    if ( insert_disk_labeled != NULLPTR )
        {
    	into_drive = src_path[ 0 ] ;
    	SCRN_11.choice[ 0 ].todo_func = next_disk ;
	}
    else
        {
	dev_change() ;
        if (is_scan_com)
		SCRN_11.choice[ 0 ].next = 30 ;
	else if (is_bus_mouse )
		SCRN_11.choice[ 0 ].next = 32 ;
        else
		SCRN_11.choice[ 0 ].next = 31 ;
	}
} /* first_disk() */

/*----------------------------------------------------------------------*/
    VOID
do_sing_floppy()
{
	if ( is_new_config )
	   {
           strtup_dirs() ;
           dsktop_dirs() ;
	   clear_system();
	   init_copy_echo() ;
	   cpy_strtup() ;
	   cpy_dsktop() ;
	   SCRN_11.choice[ 0 ].todo_func = first_disk ;
	   }
	else
	   SCRN_11.choice[ 0 ].todo_func = next_disk ;
}/* do_sing_floppy() */

/*----------------------------------------------------------------------*/
    VOID
sv_and_exit()
{
    past_sv_and_ex = TRUE ;
    if ( config == HARD_DISK || config == SING_FLOPPY)
        {
	if ( is_new_config )
	    {
	    if (!get_vol2(vol_label[0], src_path[0]))
		    {
		    SCRN_11.choice[0].todo_func = sv_and_exit ;
		    into_drive = src_path[ 0 ] ;
		    insert_disk_labeled = &disk_label[0] ;
		    return;
		    }
		    
	    strtup_dirs() ;
	    dsktop_dirs() ;
	    clear_system();
	    init_copy_echo() ;
    	    cpy_strtup() ;
	    cpy_dsktop() ;
	    SCRN_11.choice[ 0 ].todo_func = first_disk ;
	    }
	else
	    SCRN_11.choice[ 0 ].todo_func = next_disk ;
	rm_devices() ;
    	insert_disk_labeled = next_dlabel() ;
    	if ( insert_disk_labeled != NULLPTR )
	    {
    	    into_drive = src_path[ 0 ] ;
	    SCRN_11.choice[ 0 ].next = 11 ;
	    }
    	else
            {
	    if (is_bus_mouse)
		    SCRN_17.choice[ 1 ].next = 32 ;
	    else
		    SCRN_17.choice[ 1 ].next = 31 ;
	    dev_change() ;
	    }
	}
    else if ( config == DUAL_FLOPPY )
	    do_floppies();
    
}/* sv_and_exit() */


/* end of gsdvinst.c */
