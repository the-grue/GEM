/************************************************************************/
/*       Copyright 1999, Caldera Thin Clients, Inc.                     */ 
/*       This software is licenced under the GNU Public License.        */ 
/*       Please see LICENSE.TXT for further information.                */ 
/*                                                                      */ 
/*                  Historical Copyright                                */ 
/*									*/
/*	COPYRIGHT (c) 1987 Digital Research Inc.		        */
/*    The software contained in this listing is proprietary to       	*/
/*    Digital Research Inc., Pacific Grove, California and is        	*/
/*    covered by U.S. and other copyright protection.  Unauthorized  	*/
/*    copying, adaptation, distribution, use or display is prohibited	*/
/*    and may be subject to civil and criminal penalties.  Disclosure	*/
/*    to others is prohibited.  For the terms and conditions of soft-	*/
/*    ware code use refer to the appropriate Digital Research        	*/
/*    license agreement.						*/
/************************************************************************/

/* gsnewdev.c 		routines for adding a new device.		*/

#include "portab.h"
#include "machine.h"
#include "dosbind.h"
#include "gsdefs.h"
#include "gstxtdef.h"
#include "gsevars.h"

EXTERN BOOLEAN	lstr_cmp() ;		/* gsutil.c */
EXTERN BOOLEAN	get_vol2();		/* gsfilcpy.c */
EXTERN WORD	l_atoi() ;		/* gsutil.c */
EXTERN WORD	open_file() ;		/* gsfilcpy.c */
EXTERN WORD	min() ;			/* gsutil.c */
EXTERN VOID	strcpy() ;		/* gsutil.c */
EXTERN VOID	strcat() ;		/* gsutil.c */
EXTERN VOID	get_catptrs();		/* gsmain.c */
EXTERN VOID     printa();		/* scrnutil.asm */
EXTERN VOID     pos_cur();		/* scrnutil.asm */
EXTERN BOOLEAN	strcmp() ;		/* gsutil.c */

/*----------------------------------------------------------------------*/
    VOID
new_port() 
{
    WORD  	ii, port ;
    BOOLEAN 	no_match ;
    
    no_match = TRUE;
    
    port = picked ;
    if ( devices[ cur_dev ].type == PRNT_TYPE )
    	devices[ cur_dev ].port = picked ;
    else
	{
    	devices[ cur_dev ].port = picked + 3 ;	/* jump past LPTs */
	port = picked + 3;
	}
	

    if (devices[cur_dev].installed == TRUE)
	    devices[ cur_dev ].change = TRUE ;
    else
	    devices[ cur_dev ].change = FALSE ;
    
    for (ii = 0; ii < num_devs ; ii++)
	    if  (ii != cur_dev) 
		    if (devices[ii].port != NO_PORT)
			    if (devices[ii].port == port)
				    no_match = FALSE;
			 
    if (!no_match)
	    {
/*	    scrn_node[2-1].esc_next = cur_screen ;  */
	    error = TRUE;
	    }
			    
    
} /* new_port() */

/*----------------------------------------------------------------------*/
    WORD
get_type( scrn_num )
WORD	scrn_num ;
{
WORD	ret ;

    if ( scrn_num == 12 || scrn_num == 37)
	ret = SCRN_TYPE ;
    else if ( scrn_num == 14 || scrn_num == 42)
	ret = MOUS_TYPE ;
    else if ( scrn_num == 21 || scrn_num == 38)
	ret = PRNT_TYPE ;
    else if ( scrn_num == 23 || scrn_num == 39)
	ret = PLOT_TYPE ;
    else if ( scrn_num == 25 || scrn_num == 40)
	ret = CMRA_TYPE ;
    else if ( scrn_num == 27 || scrn_num == 41)
	ret = SCAN_TYPE ;
    return( ret ) ;
} /* get_type() */

/*----------------------------------------------------------------------*/
    BYTE
*get_cat_string( type )
WORD	type ;
{
BYTE	*category ;

    switch( type )
        {
	case SCRN_TYPE : category = screen ;
			 break ;
	case PRNT_TYPE : category = printer ;
			 break ;
	case PLOT_TYPE : category = plotter ;
			 break ;
	case CMRA_TYPE : category = camera ;
			 break ;
	case SCAN_TYPE : category = scanner ;
			 break ;
	case MOUS_TYPE : category = mouse ;  
			 break ;
	case META_TYPE : category = metafile ;  
			 break ;
	} /* switch */
    return( category ) ;
} /* get_cat_string() */

/*----------------------------------------------------------------------*/
    WORD
match_category( start, category )
WORD	start ;
BYTE	*category ;
{
WORD	cat_num ;
FBYTE	*ptr ;

    ptr = cat_ptr[ start ] ;
    cat_num = start ;
    while ( cat_num < MX_CATEGORIES-1 && ptr < txt_bptr + txt_bsiz )
        {
	ptr = cat_ptr[ cat_num ] ;
	if ( ptr != LNULLPTR && lstr_cmp( ADDR( category ), ptr+1 ) )
	    return( cat_num ) ;
	++cat_num ;
	} /* while */
    return( -1 ) ;	
} /* match_category() */

/*----------------------------------------------------------------------*/
    FBYTE
*get_curly_brace( picked, category )
WORD	picked ;
BYTE	*category ;
{
WORD	ii, cat_num ;
FBYTE	*ptr ;

    ptr = txt_bptr ;
    ii = picked ;
    cat_num = -1 ;
    while ( ii >= 0 )
        {
	cat_num = match_category( cat_num+1, category ) ;
	if ( cat_num < 0 )
	    ii = -1 ;
	else
	    {
	    ptr = cat_ptr[ cat_num ] ;
            while ( ii >= 0 && ptr < cat_ptr[ cat_num+1 ] )
		{
		++ptr ;
		while ( *ptr != BEG_BRACE &&
					ptr < cat_ptr[ cat_num+1 ] )
		    ++ptr ;
		if ( ptr < cat_ptr[ cat_num+1 ] )
		    --ii ;
		}
	    }
	} /* while */
    if ( *ptr == BEG_BRACE && ptr < txt_bptr + txt_bsiz )
        return( ptr ) ;
    return( LNULLPTR ) ;	
} /* get_curly_brace() */

/*----------------------------------------------------------------------*/
    FBYTE
*find_label( strt_ptr, label )
FBYTE	*strt_ptr ;
BYTE	*label ;
{
FBYTE	*ptr ;
BYTE	b ;
BOOLEAN	found ;

    found = FALSE ;

    ptr = strt_ptr ;
    b = *ptr ;
    while ( b != END_BRACE )
        {
	while ( b != PIPE && b != END_BRACE )
	    {
	    ++ptr ;
	    b = *ptr ;
	    }
	if ( b == PIPE )
	    {
	    ++ptr ;
	    found = ( lstr_cmp( ADDR( label ), ptr ) );
	    b = *ptr ;
	    while ( b != PIPE )
		    {
		    ++ptr ;
		    b = *ptr ;
		    }
	   ++ptr;
	   if (found)
		    return( ptr ) ;
	   }
	b = *ptr ;
	}
    return( LNULLPTR ) ;
} /* find_label() */

/*----------------------------------------------------------------------*/
    VOID
label_copy( size, dst, src )
WORD	size ;
FBYTE	*dst, *src ;
{
FBYTE	*ii ;
BYTE	b ;
WORD	num_bytes ;

    ii = src ;
    do
        {
	b = *ii ;
	++ii ;
	}
    while ( b!= PIPE && b != CR && b != LF && b != CNTL_Z && 
					    b != END_BRACE && b != NULL) ;
				    
    num_bytes = min( (ii - src), size ) ;
    LBCOPY( dst, src, num_bytes ) ;
    dst[num_bytes - 1 ] = NULL ;
} /* label_copy() */

/*----------------------------------------------------------------------*/
/* same as above, but don't stop at CRLF. Copies whole paragraphs.	*/
    VOID
llabel_copy( size, dst, src )
WORD    size ;
FBYTE	*dst, *src ;
{
FBYTE	*ii ;
BYTE	b ;
WORD	num_bytes;

    ii = src ;
    do
        {
	b = *ii ;
	++ii ;
	}
    while ( b!= PIPE && b != CNTL_Z && b != END_BRACE && b != NULL) ;

    num_bytes = min( (ii - src), size ) ;
    LBCOPY( dst, src, num_bytes ) ;
    dst[num_bytes - 1 ] = NULL ;
} /* llabel_copy() */

/*----------------------------------------------------------------------*/
/* LBCOPY() is defined in machine.h as LBCOPY( dlp, slp, n ).		*/
    VOID
do_lbcopy( strt_ptr, label, size, dest )
FBYTE	*strt_ptr ;
BYTE	*label ;
WORD    size ;
BYTE	*dest ;
{
FBYTE	*ptr ;

    ptr = find_label( strt_ptr, label ) ;
    if ( ptr == LNULLPTR )
	{
	*dest = NULL;
	return ;
	}
    if ( strcmp( label, L_DESCRIPT )  )
	llabel_copy( size, ADDR( dest ), ptr ) ;
     else
	label_copy( size, ADDR( dest ), ptr ) ;

} /* do_lbcopy() */

/*----------------------------------------------------------------------*/
    VOID
get_gstxt_vals( picked )
WORD	picked ;
{
BYTE	*category ;
FBYTE	*strt_ptr ;
WORD	ii, found ;

    category = get_cat_string( devices[ cur_dev ].type ) ;
    strt_ptr = get_curly_brace( picked, category ) ;
    if ( strt_ptr == LNULLPTR )
        return ;

do_lbcopy( strt_ptr, DESCRIPTION, DESC_LENGTH, devices[ cur_dev ].desc ) ;
do_lbcopy( strt_ptr, S_DESCRIPT, NAME_LENGTH, devices[ cur_dev ].s_desc );
do_lbcopy( strt_ptr, DRIVER_NAME, NAME_LENGTH, devices[ cur_dev ].f_name);
do_lbcopy( strt_ptr, FONT_DISK, HALF_LINE, devices[ cur_dev ].font_disk );
do_lbcopy( strt_ptr, SRC_DISK, HALF_LINE, devices[ cur_dev ].src_disk ) ;
do_lbcopy( strt_ptr, FNT_WILDCRD, HALF_LINE, devices[ cur_dev ].font_wcard ) ;
do_lbcopy( strt_ptr, SCAN_PORT, FNAME_LENGTH, devices[ cur_dev ].scan_port ) ;
do_lbcopy( strt_ptr, SCAN_XFER, FNAME_LENGTH, devices[ cur_dev ].scan_xfer );
do_lbcopy( strt_ptr, SCAN_COM, NAME_LENGTH, devices[ cur_dev ].scan_com );
do_lbcopy( strt_ptr, AUX_FILE, FNAME_LENGTH, devices[ cur_dev ].aux_file ) ;

    found = FALSE;
    for ( ii = 0; ii < NUM_REL_DSKS + num_drvr_paks; ii++ )
	    if ( strcmp( disk_label[ ii ], devices[ cur_dev ].src_disk ))
		    {
		        found = TRUE;
			ii = NUM_REL_DSKS + num_drvr_paks ;
		    }
    if (!found)
	    {
	    strcpy(disk_label[NUM_REL_DSKS + num_drvr_paks ], devices[ cur_dev ].src_disk);
	    num_drvr_paks++;
	    }

} /* get_gstxt_vals() */

/*----------------------------------------------------------------------*/
    VOID
new_mouse() 
{
BYTE	*category ;
FBYTE	*strt_ptr ;
BYTE	tmp[ NAME_LENGTH ] ;

    strcpy( devices[ cur_dev ].m_desc, W14_CHOICES + picked * 80 ) ;
    category = get_cat_string( MOUS_TYPE ) ;
    strt_ptr = get_curly_brace( picked, category ) ;
    do_lbcopy( strt_ptr, MOUSE_ID, NAME_LENGTH, tmp ) ;
    devices[ cur_dev ].mouse = l_atoi( ADDR( tmp ) ) ;
    if ( devices[ cur_dev ].mouse == NO_MOUSE || 
    		devices[ cur_dev ].mouse == PS2_MOUSE ||
    		devices[ cur_dev ].mouse == BUS_MOUSE )
	devices[ cur_dev ].port = NO_PORT ;

    if ( devices[ cur_dev].mouse == BUS_MOUSE )
	    is_bus_mouse = TRUE;
    if (devices[cur_dev].installed == TRUE)
	    devices[ cur_dev ].change = TRUE ;
    else
	    devices[ cur_dev ].change = FALSE ;
    
} /* new_mouse() */

/*----------------------------------------------------------------------*/
    VOID
ch_device()
{
DEV_STRUCT	*dev_ptr ;

    dev_ptr = &devices[ cur_dev ] ;
    if ( dev_ptr->installed )
	strcpy( to_delete[ num_delete++ ], dev_ptr->f_name ) ;
    devices[ cur_dev ].installed = FALSE ;
    devices[ cur_dev ].change = FALSE ;
    get_gstxt_vals( picked ) ;
    devices[ cur_dev ].fnts_installed = 
    			( devices[ cur_dev ].font_wcard[ 0 ] == NULL ) ;
} /* ch_device() */

/*----------------------------------------------------------------------*/
    VOID
ch_scanner()
{
	
    ch_device();
    if (*devices[cur_dev].scan_port != NULL)
	    scrn_node[27-1].choice[picked].next = 28;
    else if (*devices[cur_dev].scan_xfer != NULL)
	    scrn_node[27-1].choice[picked].next = 29;

}

/*----------------------------------------------------------------------*/
    VOID
new_device()
{
    WORD	ii, sav_cur;
    BOOLEAN	unique_dev;
    
    if ( num_devs >= MX_DEVICES )
	{
	error = TRUE ;
	return ;
	}
	
    unique_dev = TRUE;
    sav_cur = cur_dev ;
    cur_dev = num_devs ;

    devices[ cur_dev ].type = get_type( cur_screen ) ;
    devices[ cur_dev ].installed = FALSE ;
    devices[ cur_dev ].change = FALSE ;

    get_gstxt_vals( picked ) ;
    devices[ cur_dev ].fnts_installed = 
    			( devices[ cur_dev ].font_wcard[ 0 ] == NULL ) ;

    for (ii = 0; ii < num_devs; ii++)
	    if (strcmp(devices[ii].f_name, devices[cur_dev].f_name))
		    unique_dev = FALSE ;
    if (unique_dev)	    
	    num_devs += 1 ;
    else
	    {
	    picked = 0;		/* Choice 0 of screen 14 goes to screen 17. */
	    cur_screen = 14;	/* This is a trickey way of bypassing ports */
	    cur_dev = sav_cur;
	    }
    
    error = FALSE ;
} /* new_device() */

/*----------------------------------------------------------------------*/
    VOID
find_mdesc( dev_ptr )
DEV_STRUCT	*dev_ptr ;
{
BYTE	*category, tmp[ NAME_LENGTH ] ;
FBYTE	*strt_ptr ;
WORD	ii ;
BOOLEAN	found ;

    category = get_cat_string( MOUS_TYPE ) ;
    found = FALSE ;
    ii = 0 ;
    strt_ptr = get_curly_brace( 0, category ) ;
    while ( strt_ptr != LNULLPTR && !found )
        {
    	do_lbcopy( strt_ptr, MOUSE_ID, NAME_LENGTH, tmp ) ;
    	found = ( dev_ptr->mouse == l_atoi( ADDR( tmp ) ) ) ;
	if ( !found )
	    strt_ptr = get_curly_brace( ++ii, category ) ;
	}
    if ( !found )
        return ;
    do_lbcopy( strt_ptr, DESCRIPTION, DESC_LENGTH, dev_ptr->m_desc ) ;
    if ( dev_ptr->mouse == NO_MOUSE || dev_ptr->mouse == BUS_MOUSE 
				    || dev_ptr->mouse == PS2_MOUSE )
	dev_ptr->port = NO_PORT ;
} /* find_mdesc() */

/*----------------------------------------------------------------------*/
/* return device number of device choosen in screen 18 or 19 to change	*/
/* or remove, respectively.						*/
    WORD
get_devnum()
{
WORD		num, picked_num ;
DEV_STRUCT	*dev_ptr ;

    num = 0 ;
    picked_num = 0 ;
    do
    	{ 
	dev_ptr = &devices[ num ] ;
	if ( cur_screen == 18 )		/* change */
	    {
	    if ( dev_ptr->type == SCRN_TYPE && dev_ptr->mouse != NO_MOUSE
	       && dev_ptr->mouse != BUS_MOUSE && dev_ptr->mouse != PS2_MOUSE)
		picked_num += 3 ;
	    else
	        picked_num += 2 ;
	    }
	else if ( cur_screen == 19 )	/* remove */
	    {
	   /* if ( !( (dev_ptr->type == SCRN_TYPE) && 
		    (dev_ptr->mouse == NO_MOUSE  || 
		     dev_ptr->mouse == PS2_MOUSE || 
		     dev_ptr->mouse == BUS_MOUSE) ) )*/
	        picked_num += 1 ;
	    }
	++num ;
        } while ( picked_num <= picked ) ;
    return( --num ) ;
} /* get_devnum() */
    
/*----------------------------------------------------------------------*/
    VOID
set_curdev()
{
    cur_dev = get_devnum() ;
} /* set_curdev() */

/*----------------------------------------------------------------------*/
    VOID
remove_device()
{
WORD		num ;
DEV_STRUCT	*dev_ptr ;

    num = get_devnum() ;
    dev_ptr = &devices[ num ] ;
    if ( dev_ptr->type == SCRN_TYPE )	/* removing a mouse only */    
	{
	dev_ptr->mouse = NO_MOUSE ;
	find_mdesc( dev_ptr ) ;
	devices[ num ].change = TRUE ;
	return ;
	}
    if ( dev_ptr->installed )
	strcpy( to_delete[ num_delete++ ], dev_ptr->f_name ) ;
    if ( num < num_devs - 1 )
        LBCOPY( ADDR( dev_ptr ), ADDR( &devices[ num+1 ] ), 
			sizeof( DEV_STRUCT ) * (num_devs - num - 1 ) ) ;
    --num_devs ;
} /* remove_device() */
    
/*----------------------------------------------------------------------*/
    VOID
add_device( ptr )
FBYTE	*ptr ;
{
    if ( num_devs >= MX_DEVICES )
	return ;
    devices[ num_devs ].installed = TRUE ;
    devices[ num_devs ].change = FALSE ;
    devices[ num_devs ].fnts_installed = TRUE ;
    devices[ num_devs ].port  = (WORD)( *ptr++ ) ;
    if ( devices[ num_devs ].type == SCRN_TYPE )
    	{
        devices[ num_devs ].mouse = (WORD)( *ptr++ ) ;
	find_mdesc( &devices[ num_devs ] ) ;
	}
    LBCOPY( ADDR( devices[ num_devs ].s_desc ), ptr, NAME_LENGTH ) ;
    ptr += NAME_LENGTH ;
    LBCOPY( ADDR( devices[ num_devs ].desc ), ptr, DESC_LENGTH + 1 ) ;
    cur_dev = num_devs ;
    num_devs += 1 ;
} /* add_device() */

/*----------------------------------------------------------------------*/
    VOID
new_scanner()
{
    new_device() ;
    if (*devices[cur_dev].scan_port != NULL)
	    scrn_node[27-1].choice[picked].next = 28;
    else if (*devices[cur_dev].scan_xfer != NULL)
	    scrn_node[27-1].choice[picked].next = 29;

} /* new_scanner() */

/*----------------------------------------------------------------------*/
    BOOLEAN
found_match(ptr, new_cat, label, size)
FBYTE	*ptr;
WORD	new_cat;
BYTE	*label;
LONG	size;
{
	FBYTE   *strt;
	BYTE	f_name1[ FNAME_LENGTH ], f_name2[ FNAME_LENGTH ];
	LONG	old_txtbsize;
	WORD	num;
	
	do_lbcopy(ptr, DRIVER_NAME, 100, f_name1);
	num = 0;
	old_txtbsize = txt_bsiz;
	txt_bsiz += size;
	strt = get_curly_brace(num, label);
	while (strt < cat_ptr[new_cat])
		strt = get_curly_brace(++num, label);
	while (strt != LNULLPTR)
		{
		printa(REG_COL,W_DOT);
		do_lbcopy(strt, DRIVER_NAME,100, f_name2);
		if (strcmp(f_name1, f_name2))
			{
			txt_bsiz = old_txtbsize;
			return(TRUE);
			}
		strt = get_curly_brace(++num, label);
		}
	txt_bsiz = old_txtbsize;
	return(FALSE);
} /* found_match() ; */
	    
/*----------------------------------------------------------------------*/
    BOOLEAN
unique_drivers( size )
LONG	size;
{
	BOOLEAN	match;
	BYTE	tmp[20], *tmp_ptr;
	FBYTE	*nxt_cat, *ptr;
	WORD	ii, num;

	match = FALSE;
	
	txt_bsiz += size;
	get_catptrs();
	
	for (ii = 0; cat_ptr[ii] < txt_bptr + txt_bsiz; ii++);
	txt_bsiz -= size;
	
	nxt_cat = cat_ptr[--ii];
	
	while ( nxt_cat != txt_bptr + txt_bsiz + size)
		{
		tmp_ptr = &tmp[0];
		ptr = nxt_cat + 1;
		while (*ptr != SPACE && *ptr != CR && *ptr != LF 
						   && *ptr != TAB)
			*tmp_ptr++ = *ptr++;
		*tmp_ptr = NULL;
		num = 0;
		ptr = get_curly_brace(num, tmp);
		while ( !match && ptr != LNULLPTR )
			{
			match |= found_match(ptr, ii, tmp, size);
			ptr = get_curly_brace(++num, tmp);
			}
		nxt_cat = cat_ptr[++ii];
		}
	return(!match);
}

/*----------------------------------------------------------------------*/
    BOOLEAN
append_txt_buf()
{
	LONG    size, count ;
	WORD	handle ;
	BYTE	tmp_str[ FNAME_LENGTH ] ;

	strcpy( tmp_str, src_path ) ;
	strcat( tmp_str, gemsettxt ) ;

	size = dos_fsize( tmp_str );
	if ( size > mem_avail )
		return(FALSE);
	
	handle = open_file( src_path, gemsettxt, 0 ) ;
	count = dos_read( handle, size, txt_bsiz + txt_bptr ) ;
	dos_close( handle ) ;
	
	pos_cur(10,10);
	printa(REG_COL, wait);
	
	if (!unique_drivers(size))
		{
		get_catptrs();
		return(FALSE);
		}
	else
		{
		txt_bsiz += size;
		mem_avail -= size;
		return(TRUE);
		}
}/* append_txt_buf */
	
/*----------------------------------------------------------------------*/
	VOID
get_new_txt()
{
	BYTE drive_id;
	
	drive_id = src_path[0];
	if (!(get_vol2(DPK_VOL, drive_id)))
		scrn_node[16-1].choice[1].next = 16;
	else if (!append_txt_buf())
		/* WRONG TXT FILE */;
} /*get_new_txt*/
	

/* end of gsnewdev.c */
