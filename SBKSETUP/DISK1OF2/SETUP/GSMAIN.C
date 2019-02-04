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

/* gsmain.c 	MAIN module for GEMSETUP.EXE.				*/

#include "portab.h"
#include "machine.h"
#include "dosbind.h"
#include "gsdefs.h"
#include "gstxtdef.h"
#include "gstrings.h"
#include "gscreens.h"
#include "gsgvars.h"

#define	SCRN_1	scrn_node[1-1]
#define SCRN_11 scrn_node[11-1]

EXTERN 	VOID 	cls() ;				/* scrnutil.asm */
EXTERN 	VOID 	beeper() ;			/* scrnutil.asm */
EXTERN  VOID    prn_logo() ;			/* scrnutil.asm */
EXTERN  VOID    pos_cur() ;			/* scrnutil.asm */
EXTERN  WORD    get_key() ;	/* returns keybrd  scrnutil.asm */
EXTERN  WORD    read_chr() ;	/* returns screen  scrnutil.asm */
EXTERN  WORD    get_row() ;			/* scrnutil.asm */
EXTERN  VOID    cur_set();			/* scrnutil.asm */
EXTERN  VOID    printa();			/* scrnutil.asm */
EXTERN  VOID    printn();			/* scrnutil.asm */
EXTERN  VOID    b_stuff();			/* scrnutil.asm */
EXTERN  WORD    egacard();			/* scrnutil.asm */
EXTERN  VOID    strcpy() ; 			/* gsutil.c	*/
EXTERN  VOID    strcat() ; 			/* gsutil.c	*/
EXTERN  WORD    strlen() ; 			/* gsutil.c	*/
EXTERN  WORD    l_atoi() ; 			/* gsutil.c	*/
EXTERN  FBYTE	*get_curly_brace() ;		/* gsnewdev.c */
EXTERN 	FBYTE	*find_label() ;			/* gsnewdev.c */
EXTERN 	BYTE	*get_cat_string() ;		/* gsnewdev.c */
EXTERN 	WORD	get_type() ;	                /* gsnewdev.c */
EXTERN 	VOID	label_copy() ;			/* gsnewdev.c */
EXTERN 	VOID	do_lbcopy() ;			/* gsnewdev.c */
EXTERN  WORD	open_file() ;			/* gsfilcpy.c */
EXTERN  VOID	get_drvr() ;			/* gsfilcpy.c */
EXTERN  BOOLEAN	get_vol2() ;			/* gsfilcpy.c */
EXTERN  VOID	set_drives() ;			/* gsnewcfg.c */
EXTERN  VOID	setup_16() ;			/* gssetup.c */

/*--------------------------------------------------------------*/
/* reverses bytes in string s					*/
    VOID
reverse( s )
	BYTE s[];
{
	WORD c, i, j;

	for(i=0, j=strlen(s)-1; i<j; i++, j--)
	{
	    c = s[i];
	    s[i] = s[j];
	    s[j] = c;
	}
} /* reverse */

/*--------------------------------------------------------------*/
/* converts integer to ascii: s[] had best be long enough.	*/
    VOID
itoa( i, s )
	WORD i;
	BYTE s[];
{
	WORD cnt, sign;

	if ( ( sign = i ) < 0 )
	    i = -i ;
	cnt = 0 ;
	do
	{
	    s[ cnt++ ] = i % 10 + '0' ;
	} while ( (i /= 10 ) > 0 ) ;
	if ( sign < 0 )
	    s[ cnt++ ] = '-' ;
	s[ cnt ] = NULL ;
	reverse( s ) ;
} /* itoa */
/*----------------------------------------------------------------------*/
/*sets up abort screen 35. should be in gssetup.c			*/
	VOID
abort_setup()
{
        scrn_node[ 35 - 1 ].choice[1].next = cur_screen;
}

/*----------------------------------------------------------------------*/
    VOID
abort_cancel()
{
	strcpy(W35_CHOICES+80, choices[5]);
}

/*----------------------------------------------------------------------*/
/* clears out the misc_choices array (inserts spaces).			*/
    VOID
clear_misc_choices()
{
    b_stuff( ADDR( misc_choices ), sizeof( misc_choices ), SPACE ) ;
} /* END clear_misc_choices */

/*----------------------------------------------------------------------*/
/* copies device choices of catagory from gemsetup.txt file which at    */
/* point is in memory.	sets g_num_choices to the number of device      */
/* choices in the catagory.						*/
    VOID
cpy_txt_choices(category)
BYTE	*category;		/* SCREEN, PRINTER, ETC... */
{
	FBYTE	*cur_pos, *temptr;
	WORD	num_choices, ii, jj;
	
	clear_misc_choices();
	cur_pos = txt_bptr;
	num_choices = 0 ;
	while ( cur_pos != LNULLPTR && cur_pos < txt_bptr + txt_bsiz )
	    {
	    cur_pos = get_curly_brace( num_choices, category ) ;
	    if ( cur_pos != LNULLPTR )
	        {
		temptr = find_label( cur_pos, DESCRIPTION ) ;
	    	if ( temptr != LNULLPTR )
	            {
		    label_copy(PROMPT_LEN, ADDR( misc_choices[ num_choices ] ), temptr ) ;
		    cur_pos = temptr ;
		    ++num_choices ;
		    }
		}
	    }
       	for (ii = 0; ii < num_choices; ii++)
		for (jj = 77; jj > -1; jj--)
			if (misc_choices[ii][jj] != SPACE)
			{
				misc_choices[ii][jj + 2] = NULL;
				jj = -1;
			}

	g_num_choices = num_choices;
} /* END cpy_txt_choices */
    
/*----------------------------------------------------------------------*/
/* copies choices from choices array in gstrings.h			*/
    VOID
cpy_gen_choices(num_choices, choices_str)
BYTE	*choices_str;		/* the string of choices */ 
WORD	num_choices;		/* the number of choices */
{
	WORD 	i;
	
	for (i = 0; i < num_choices; i++)
	    strcpy(misc_choices[i], choices_str + i*80 );
}

/*----------------------------------------------------------------------*/
/* draws the header you see at the top of gemsetup. 			*/
    VOID
draw_header()
{
	BYTE	bar[2];
/*	WORD    ii, jj, kk;	*/
	
	bar[0] = 186;
	bar[1] = NULL;
	cls(0,0,24,79);
	pos_cur(1,2);
	printa(12,logo1);
	pos_cur(1,3);
	printa(12,logo2);
	pos_cur(0,0);
	printa(3,header1);
	printa(3,bar);
	pos_cur(1,5);
	printa(3,header2);
	printa(3,bar);
	printa(3,header3);
/*	pos_cur(24,0);
	for (jj = 0; jj < 20; jj += 2)
	{
		for (ii = 0; ii < 20-jj; ii++)
		{
			scr_dn();
			for (kk = 0; kk < 10*(20-jj); kk++);
		}
		for (ii = 0; ii < 20-jj; ii++)
		{
			scr_up();
			for (kk = 0; kk < 10*(20-jj); kk++);
		}
	}*/
	cls(3,0,24,79);
	
} /* END draw_header */

/*----------------------------------------------------------------------*/
/* determines the current path.						*/
    VOID
find_spath() 
{
BYTE	*end ;

    src_path[ 0 ] = (BYTE)(dos_gdrv()) + UCASE_A ;
    src_path[ 1 ] = COLON ;
    src_path[ 2 ] = SLASH ; 
    dos_gdir( 0, ADDR( &src_path[ 3 ] ) ) ;
    end = &src_path[ strlen( src_path ) - 1 ] ;
    if ( *end != SLASH )
        *(end + 1) = SLASH ;
    set_drives( &harddrives, &flpydrives ) ;
} /* find_spath() */

/*----------------------------------------------------------------------*/
/* builds array of pointers which point to the different catagories	*/
/* such as SCREEN, PRINTER, etc. so that the whole .txt file won't	*/
/* have to be searched.  Instead, catagory pointers are used to get	*/
/* direct access.							*/
    VOID
get_catptrs() 
{
WORD	ii ;
FBYTE	*cur_ptr ;

    ii = 0 ;
    cur_ptr = txt_bptr ;
    while ( ii < MX_CATEGORIES-1 && cur_ptr != LNULLPTR 
    		&& cur_ptr < txt_bptr + txt_bsiz )
	{
	if ( *cur_ptr == AT_SIGN )
	    cat_ptr[ ii++ ] = cur_ptr ;
    	++cur_ptr ;
	}
    cat_ptr[ ii ] = txt_bptr + txt_bsiz ;	
} /* get_catptrs() */

/*----------------------------------------------------------------------*/
/* get choices from gemsetup.txt file under screen catagories.		*/
/* if no choices, then call screen 16, driver pack insert.		*/
/*   e nxt_scrn for cont of scrn 16 */
    VOID
get_screens()
{
	WORD ii;
	
	for (ii = 0; ii < 20; ii++)
		{
		scrn_node[12-1].choice[g_num_choices].todo_func = new_device;
		scrn_node[37-1].choice[g_num_choices].todo_func = ch_device;
		scrn_node[12-1].choice[ii].next = 14;
		scrn_node[37-1].choice[ii].next = 17;
		}
	
	cpy_txt_choices( screen );
	prev_16_screen = cur_screen;
	if (g_num_choices > 0 )
		{
		strcpy(misc_choices[ g_num_choices ], OTHER);
		scrn_node[12-1].choice[g_num_choices ].next = 16; 
		scrn_node[37-1].choice[g_num_choices ].next = 16;
		g_num_choices++;
		}
	else
		{
		cur_screen = 16; 
		setup_16();
		}	
} /* END get_screens  */

/*----------------------------------------------------------------------*/
/* Get mouse choices from gemsetup.txt.					*/
/* 									*/
    VOID
get_mouse()
{
	WORD	ii, ms_id ;
        FBYTE	*strt, *temptr ;
	BYTE	temp_buf[4] ;

   cpy_txt_choices( mouse );
   for (ii = 0; ii < g_num_choices ; ii++)
	{
	strt = get_curly_brace( ii, mouse );
	if ( strt != LNULLPTR )
		{
		temptr = find_label( strt, MOUSE_ID );
		if ( temptr != LNULLPTR )
			{
			label_copy(3, ADDR( temp_buf ), temptr ) ;
			ms_id = l_atoi(ADDR(temp_buf) );
			if (ms_id == NO_MOUSE || ms_id == PS2_MOUSE ||
				ms_id == BUS_MOUSE)
				{
				scrn_node[14 -1].choice[ii].next = 17 ;
				scrn_node[42 -1].choice[ii].next = 17 ;
				}
			else
				{
				scrn_node[14 -1].choice[ii].next = 15 ;
				scrn_node[42 -1].choice[ii].next = 15 ;
				}
			}
		}
	}
		
}/* END get_mouse */

/*----------------------------------------------------------------------*/
/* get choices from gemsetup.txt file under printer catagories.		*/
/* if no choices, then call screen 16, driver pack insert.		*/
    VOID
get_printer()
{
	WORD ii;
	
	for (ii = 0; ii < 20; ii++)
		{
		scrn_node[21-1].choice[g_num_choices].todo_func = new_device;
		scrn_node[38-1].choice[g_num_choices].todo_func = ch_device;
		scrn_node[21-1].choice[ii].next = 22;
		scrn_node[38-1].choice[ii].next = 17;
		}
	
        prev_16_screen = cur_screen;
	cpy_txt_choices( printer );
	if (g_num_choices > 0 )
		{
		strcpy(misc_choices[ g_num_choices ], OTHER);
	      scrn_node[21-1].choice[g_num_choices].todo_func =null_procedure;
	      scrn_node[38-1].choice[g_num_choices].todo_func =null_procedure;
		scrn_node[21-1].choice[g_num_choices ].next = 16;
		scrn_node[38-1].choice[g_num_choices ].next = 16;
		g_num_choices++;
		}
	else
		{
		cur_screen = 16; 
		setup_16();
		}
} /* END get_printer */

/*----------------------------------------------------------------------*/
/* get choices from gemsetup.txt file under plotter catagories.		*/
/* if no choices, then call screen 16, driver pack insert.		*/
    VOID
get_plotter()
{
	WORD ii;
	
	for (ii = 0; ii < 20; ii++)
		{
		scrn_node[23-1].choice[g_num_choices].todo_func = new_device;
		scrn_node[39-1].choice[g_num_choices].todo_func = ch_device;
		scrn_node[23-1].choice[ii].next = 24;
		scrn_node[39-1].choice[ii].next = 17;
		}
	
	cpy_txt_choices( plotter );
        prev_16_screen = cur_screen;
	if (g_num_choices > 0 )
		{
		strcpy(misc_choices[ g_num_choices ], OTHER);
	      scrn_node[23-1].choice[g_num_choices].todo_func =null_procedure;
	      scrn_node[39-1].choice[g_num_choices].todo_func =null_procedure;
		scrn_node[23-1].choice[g_num_choices ].next = 16;
		scrn_node[39-1].choice[g_num_choices ].next = 16;
		g_num_choices++;
		}
	else
		{
		cur_screen = 16; 
		setup_16();
		}
} /* END get_plotter */

/*----------------------------------------------------------------------*/
/* get choices from gemsetup.txt file under camera catagories.		*/
/* if no choices, then call screen 16, driver pack insert.		*/
    VOID
get_camera()
{
	WORD ii;
	
	for (ii = 0; ii < 20; ii++)
		{
		scrn_node[25-1].choice[g_num_choices].todo_func = new_device;
		scrn_node[40-1].choice[g_num_choices].todo_func = ch_device;
		scrn_node[25-1].choice[ii].next = 26;
		scrn_node[40-1].choice[ii].next = 17;
		}
	
	cpy_txt_choices( camera );
	prev_16_screen = cur_screen;
	if (g_num_choices > 0 )
		{
		strcpy(misc_choices[ g_num_choices ], OTHER);
	      scrn_node[25-1].choice[g_num_choices].todo_func =null_procedure;
	      scrn_node[40-1].choice[g_num_choices].todo_func =null_procedure;
		scrn_node[25-1].choice[g_num_choices ].next = 16;
		scrn_node[40-1].choice[g_num_choices ].next = 16;
		g_num_choices++;
		}
	else
		{
		cur_screen = 16; 
		setup_16();
		}
} /* END get_camera */

/*----------------------------------------------------------------------*/
/* get choices from gemsetup.txt file under scanner catagories.		*/
/* if no choices, then call screen 16, driver pack insert.		*/
    VOID
get_scanner()
{
	WORD ii;
	
	for (ii = 0; ii < 20; ii++)
		{
		scrn_node[27-1].choice[g_num_choices].todo_func = new_scanner;
		scrn_node[41-1].choice[g_num_choices].todo_func = ch_scanner;
		scrn_node[27-1].choice[ii].next = 17;
		scrn_node[41-1].choice[ii].next = 17;
		}
	
	cpy_txt_choices( scanner );
	prev_16_screen = cur_screen;
	if (g_num_choices > 0 )
		{
		strcpy(misc_choices[ g_num_choices ], OTHER);
	      scrn_node[27-1].choice[g_num_choices].todo_func =null_procedure;
	      scrn_node[41-1].choice[g_num_choices].todo_func =null_procedure;
		scrn_node[27-1].choice[g_num_choices ].next = 16;
		scrn_node[41-1].choice[g_num_choices ].next = 16;
		g_num_choices++;
		}
	else
		{
		cur_screen = 16; 
		setup_16();
		}
} /* END get_scanner */

/*----------------------------------------------------------------------*/
    VOID
null_procedure()
{
    error = FALSE ;
} /* null_procedure() */

/*----------------------------------------------------------------------*/
/* converts chr to capital letters.					*/
    BYTE
to_caps( chr )
BYTE	chr;
{
	return(chr & 0xDF);
}

/*----------------------------------------------------------------------*/
/* Draws the next screen.						*/
    VOID
draw_screen( num_choices, prompt_str, choices_str, footer_str )
WORD	num_choices ;
BYTE	*prompt_str, *choices_str, *footer_str ;
{
	WORD	row, print_foot, i ,chr_att ;
	row = 23;
	print_foot = FALSE;
	
	if ( num_choices == -1 )
		num_choices = g_num_choices;
	
	if (last_footer != footer_str)
	{
		row = 24;
		print_foot = TRUE;
		last_footer = footer_str;
	}
	
	chr_att = read_chr();		/* make sure screen is intact */
	if (chr_att != 0x03C9)		/* ie. no critical error handler */
		draw_header();
	
	cls(3, 0, row, 79);
	
	if (print_foot)
	{
		pos_cur(row,0);
		printa(REV_COL, footer_str);
	}

	pos_cur(4,0);
	printn( prompt_str );
	printn("  \n\r");
	offset = get_row();
	
	for (i = 0; i < num_choices; i++)
	{
		pos_cur(offset + i, INDENT);
		printa(4, bullet);
		printn( "  " );
		printn( choices_str + i*80 );
	}
} /* draw_screen() */

/*----------------------------------------------------------------------*/
/* gets the picked choice from user.					*/
    WORD
get_choice( num_choices, start_choice, choice_str )
WORD	num_choices, start_choice;
BYTE	*choice_str ;
{
	WORD	valid, in, row;
	BOOLEAN	kill_dev ;
	
	
	if ( num_choices == 0 )   /*If you don't want any choices displayed */
		return( 0 );  	  /* then set num_choices = 0.              */
	
	if ( num_choices == -1 )  /*If you don't know how many choices...-1 */
		num_choices = g_num_choices;

	row = offset + start_choice - 1;
	pos_cur(row, INDENT);
	printa(CHK_COL, check);
	printa(REG_COL,"  ");
	printa(CHOS_COL,choice_str + 80 * (row - offset ) );
	valid = FALSE;
	
	while(!valid)
	{
		pos_cur(24,0);			/* cursor off */
		in = get_key();

		if (in == DOWN_ARROW)
		{
			pos_cur(row,INDENT);
			printa(4, bullet);
			printa(REG_COL,"  ");
			printa(REG_COL,choice_str + 80 * ( row - offset ));
			row++;
			if (row > offset + num_choices - 1 )
				row = offset;
			pos_cur(row,INDENT);
			printa(CHK_COL,check);
			printa(REG_COL,"  ");
			printa(CHOS_COL,choice_str + 80 * ( row - offset ));
		}
		else if (in == UP_ARROW)
		{
			pos_cur(row, INDENT);
			printa(4, bullet);
			printa(REG_COL,"  ");
			printa(REG_COL,choice_str + 80 * ( row - offset ));
			row--;
			if (row < offset)
				row = offset + num_choices - 1;
			pos_cur(row,INDENT);
			printa(CHK_COL, check);
			printa(REG_COL,"  ");
			printa(CHOS_COL,choice_str + 80 * ( row - offset ));

		}
		else if (LLOBT(in) == CR)  
			valid = row - offset + 1;
		else if ( in == F1) 
			{
			if ((cur_screen == 12 || cur_screen == 13 || 
			     cur_screen == 23 || cur_screen == 25 ||
			     cur_screen == 27 || cur_screen == 37 ||
			     cur_screen == 38 || cur_screen == 39 ||
			     cur_screen == 40 || cur_screen == 41 ||
			     cur_screen == 21 ) 
			    && (row - offset + 1 != num_choices))
			    {
				valid = row - offset + 1 ;
				help = TRUE ;
			    }
			else if (cur_screen == 14 || cur_screen == 42)    
			    {
				valid = row - offset + 1 ;
				help = TRUE ;
			    }
			}
		else if ( in == ESCAPE)
			{
			kill_dev = FALSE ;
			if (prev_screen == 12 && cur_screen == 14)
				kill_dev = TRUE;
			if ((prev_screen == 21 && cur_screen == 22) ||
			    (prev_screen == 23 && cur_screen == 24) ||
			    (prev_screen == 25 && cur_screen == 26 ))
					     kill_dev = TRUE;
			if (kill_dev)
				{
				num_devs--;
				devices[cur_dev].type = 0x0000;
				devices[cur_dev].installed = 0x0000;
				devices[cur_dev].fnts_installed = 0x0000;
				devices[cur_dev].change = 0x0000;
				devices[cur_dev].port = 0x0000;
				devices[cur_dev].mouse = 0x0000;
				}
			escape = TRUE;
			return(row - offset);
			}
	}
	error = FALSE ;
return(valid-1);	
} /* END get_choice */

/*----------------------------------------------------------------------*/
    VOID
cpy_strings(d2_arry, catagory, do_delimit, num_strings, strng_len)
BYTE	*d2_arry, *catagory ;
BOOLEAN do_delimit ;
WORD    num_strings ;
{
	BYTE	ch, *cat_ptr;
	FBYTE   *f_ptr ;
	WORD	ii , pp, qq;
	BOOLEAN end ;
	
	pp = 0;
	cat_ptr = catagory;
	for (ii = 0; ii < num_strings; ii++)
	   {
	   qq = 0;
	   f_ptr = get_curly_brace( ii, cat_ptr );
	   end = ( f_ptr == LNULLPTR ) ;
	   f_ptr++;
	   while (!end)
		{
		LBCOPY(ADDR(&ch), f_ptr, 1);
		f_ptr++;
		if (ch == CR )
			{
			f_ptr++;
			while (qq < strng_len)
				{
				*(d2_arry + (pp*strng_len + qq)) = SPACE;
				qq++;
				}
			if (do_delimit)
				{
				qq--;
				while (qq >= 0)
				  {
				  if (*(d2_arry+(pp*strng_len + qq)) != SPACE)
				    {
				    *(d2_arry+(pp*strng_len + qq + 1)) = NULL;
				    qq = -1;
				    }
				  else 
				    qq--;
				  }
				}
			qq = 0;
			pp++;
			}
		else if (ch == END_BRACE)
			{
			*(d2_arry + ((pp-1)*strng_len + strng_len-1)) = NULL ;
			end = TRUE ;
			}
		else
			{
			*(d2_arry + (pp*strng_len + qq)) = ch ;
			qq++ ;
			}
		}
	   }
}/* cpy_strings() */
	
/*----------------------------------------------------------------------*/
/* Initializes copy file buffer and .txt file buffer.			*/
init_bufs(file, meta)
BYTE	*file ;
BOOLEAN meta ;
{
    LONG    count ;
    WORD	handle, ok ;
    BYTE	tmp_str[ FNAME_LENGTH ] ;
    BYTE	*cat_ptr;
    FBYTE	*f_ptr;

    strcpy( tmp_str, src_path ) ;
    strcat( tmp_str, file ) ;
	
    if (!meta)
        {
	printn(" Loading GEMSETUP...                                      " );
	printn("  \n\r");
	mem_avail = dos_avail() ;
	if (mem_avail < MIN_MEM)
		return(FALSE);

	/* Allocate copy_buffer */	
	cpy_bptr = dos_alloc( COPY_BSIZE ) ;
	mem_avail = dos_avail();
	txt_bptr = dos_alloc( mem_avail ) ;
	}	
    ok = dos_sfirst( ADDR( tmp_str ), 0x0 ) ;
    if ( !ok )
        {
	if ( meta )
  	   {
	   beeper();
	   draw_screen( scrn_node[8-1].num_choices, scrn_node[8-1].prompt, 
			scrn_node[8-1].choices, scrn_node[8-1].footer ) ;
	   }
	else	/* no messages file yet... */
	   printn("  GEMSETUP.MSG?                                   \n\r" ) ;
	beeper();
	return( FALSE ) ;
	}
    
    txt_bsiz = dos_fsize( tmp_str ) ;
	
    /* Read in GEMSETUP.??? */
    handle = open_file( src_path, file, 0 ) ;
    count = dos_read( handle, txt_bsiz, txt_bptr ) ;
    dos_close( handle ) ;
    get_catptrs() ;

	/* Copy in meta_file */
    if (meta)
		{
		mem_avail -= txt_bsiz;
		cat_ptr = get_cat_string( META_TYPE ) ;
		f_ptr = get_curly_brace( 0, cat_ptr ) ;
		do_lbcopy( f_ptr, DRIVER_NAME, NAME_LENGTH, mdsys )  ;
		}
    return( TRUE ) ;
} /* init_bufs */

/*----------------------------------------------------------------------*/
/* Initializes some variables, then allocates buffer space for file 	*/
/* copy and patch, then allocates memory for and loads gemsetup.txt	*/
/* then copies metafile.						*/
    BOOLEAN
init()
{
	WORD 	corner ;
	
	find_spath() ;
	if (!init_bufs(gemsetmsg, FALSE))
		return(FALSE);

	cpy_strings(prompts, promptsx, FALSE, 48, PROMPT_LEN);
	cpy_strings(choices, choicesx, TRUE, 10, PROMPT_LEN);
	cpy_strings(footers, footersx, FALSE, 3, PROMPT_LEN);
	cpy_strings(vol_label, hard_vol, TRUE, 1, 12);
	cpy_strings(disk_label, hard_lab, TRUE, 1, HALF_LINE);
	cpy_strings(gs_keywds, keywords, TRUE, 1, 15);
	cpy_strings(label_floppies, flop_lab, TRUE, 1, HALF_LINE);
	cpy_strings(vol_floppies, flop_vol, TRUE, 1, 12);
	cpy_strings(copy_echo, msg1, TRUE, 1, PROMPT_LEN);
	cpy_strings(too_many_devices, msg2, FALSE, 1, PROMPT_LEN);
	cpy_strings(go_on, msg3, FALSE, 1, PROMPT_LEN);
	
	if (!init_bufs(gemsettxt, TRUE))
		return(FALSE);
	
	REG_COL = 7;
	REV_COL = 112;
	CHK_COL = 244;
	bullet[1] = NULL;
	check[1] = NULL;
	logo1[1] = NULL;
	logo2[1] = NULL;
	
	pos_cur(0,0);
	printa(3,header1);
	corner = egacard();
	if (!(LLOBT(corner) == header1[0]))
		{
		CHOS_COL = 12;
		prn_logo();
		bullet[0] = 0x11;
		check[0] = 0x10;
		logo1[0] = 0x1e;
		logo2[0] = 0x1f;
		}
	else
		{
		CHOS_COL = 7;
		bullet[0] = SPACE;
		check[0] = 0x10;
		logo1[0] = SPACE;
		logo2[0] = SPACE;
		}
		
		
	num_drvr_paks = 0;
	MX_DEVICES = 5;		/* all that will fit in summary screen #17 */
	is_bus_mouse = FALSE;	/* Flag for displaying screen 32 */
	is_scan_com = FALSE;	/* Flag for displaying screen 30 */
	empty_dest_drv = TRUE;  /* Flag for dual floppy stuff */
	past_sv_and_ex = FALSE ; /*flag for screen 11 esc_next */
	

	draw_header();
	cur_set(13,13);
	return(TRUE);

} /* END init() */

/*----------------------------------------------------------------------*/
    VOID
look_for_drvr( wildcard )
{
BYTE		tmp[ FNAME_LENGTH ] ;
BOOLEAN 	ok ;
 
    strcpy( tmp, gemsys ) ;
    strcat( tmp, wildcard ) ;
    dos_sdta( ADDR( gl_dta ) ) ;
    ok = dos_sfirst( ADDR( tmp ), 0x0 ) ;
    while ( ok )
	{
        get_drvr( &gl_dta[ 30 ] ) ; 
	if ( num_devs < MX_DEVICES )
	    devices[ num_devs ].type = devices[ num_devs-1 ].type ; 
    	ok = dos_snext() ;
	}
} /* look_for_drvr() */

/*----------------------------------------------------------------------*/
    VOID
set_new_cfg()
{
    WORD	mask ;
    
    if (config == HARD_DISK)
	    {
	    is_new_config = TRUE ;

	    mask = 0x2000;
	    while( !(mask & harddrives) )
		    mask >>= 1 ;
	    if ( harddrives & (~mask))
		    SCRN_1.choice[ 1 ].next = 3 ;
	    else
		    SCRN_1.choice[ 1 ].next = 12 ;
	    error = FALSE ;
	    }
    else
	    {
	    is_new_config = TRUE ;
	    SCRN_11.choice[ 0 ].next = 12 ;
	    error = FALSE ;
	    }
} /* set_new_cfg() */

/*----------------------------------------------------------------------*/
    VOID
drvr_look()
{
	if (config == SING_FLOPPY)
		{
		if ( !get_vol2(DT_VOL, DEFAULT_DRV[0]))
			{
			beeper();
			SCRN_11.choice[0].todo_func = drvr_look ;
			SCRN_11.choice[0].next = 11 ;
			return;
			}
		else
			SCRN_11.choice[0].next = 17 ;
		}
	if ( num_devs < MX_DEVICES )
		{
	        devices[ num_devs ].type = SCRN_TYPE ;
		look_for_drvr( sd_wildcard ) ;	/* screen drivers */
		}

	if ( num_devs < MX_DEVICES )
		{
		devices[ num_devs ].type = PRNT_TYPE ;
		look_for_drvr( pd_wildcard ) ;	/* printer drivers */
		}

	if ( num_devs < MX_DEVICES )
		{
		devices[ num_devs ].type = PLOT_TYPE ;
		look_for_drvr( vd_wildcard ) ;	/* plotter drivers */
		}
   	
	if ( num_devs < MX_DEVICES )
		{
		devices[ num_devs ].type = SCAN_TYPE ;
		look_for_drvr( id_wildcard ) ;	/* scanner drivers */
		}

	if ( num_devs < MX_DEVICES )
		{
		devices[ num_devs ].type = CMRA_TYPE ;
		look_for_drvr( cd_wildcard ) ;	/* camera drivers  */
		}


	if ( num_devs > 0 )
		{
		is_new_config = FALSE ;    
		if (config == HARD_DISK )
			SCRN_1.choice[ 1 ].next = 17 ;
		else 
			SCRN_11.choice[ 0 ].next = 17 ;
		}
	else 
		set_new_cfg();
	
}/* drvr_look() */

/*----------------------------------------------------------------------*/
    BOOLEAN
find_gemsys()
{
UWORD	mask ;
BOOLEAN	found ;

    found = FALSE ;
    mask = 0x8000 ;
    gemsys[ 0 ] = UCASE_A ;
    gemsys[ strlen( gemsys ) - 1 ] = NULL ;	/* rm last backslash */
    while ( !found && mask != 0x0000 )
        {
	if ( mask & harddrives )
	    {
	    found = dos_sfirst( ADDR( gemsys ), 0x10 ) ;
	    }
	mask >>= 1 ;
	gemsys[ 0 ] += 1 ;
	}
    --gemsys[ 0 ] ;
    gemsys[ strlen( gemsys ) ] = SLASH ;
    return(found);
} /* find_gemsys */

/*----------------------------------------------------------------------*/
/* This routine searches for \GEMAPPS\GEMSYS\SD*.*,PD*.*,VD*.* etc.	*/
/* after finding each driver, its zyxg area is read and the info	*/
/* put into the prompt area for screen #15.				*/
    VOID
find_cur_setup()
{
BOOLEAN	found ;

    found = FALSE ;
    num_devs = 0;
    b_stuff( ADDR( devices ), sizeof( devices ), NULL ) ;
    
    if (config == HARD_DISK)
	    found = find_gemsys();

    if ( !found )
	gemsys[ 0 ] = DEFAULT_DRV[ 0 ] ;

    clipbrd[0] = gemfonts[0] = gemapps[0] = DEFAULT_DRV[0] = gemsys[0] ;

    if ( config == HARD_DISK )
	    if (!found)
		    set_new_cfg();
	    else
		    drvr_look();

     else if (config == SING_FLOPPY)
	    {
	    SCRN_11.choice[0].todo_func = drvr_look ;
	    insert_disk_labeled = &DT_LABEL ;
	    into_drive = DEFAULT_DRV[0] ;
	    SCRN_1.choice[1].next = 11 ;
	    }
    else if (config == DUAL_FLOPPY )
	    {
	    SCRN_11.choice[0].todo_func = drvr_look ;
	    insert_disk_labeled = &SU_LABEL ;
	    into_drive = DEFAULT_DRV[0] ;
	    SCRN_1.choice[1].next = 11 ;
	    }
} /* find_cur_setup () */

/*----------------------------------------------------------------------*/
/* displays help screen.						*/
    VOID
do_help()
{
    BYTE	*cat_ptr, desc_buf[PROMPT_LEN];
    FBYTE	*f_ptr;
    WORD	ii;

    strcpy( desc_buf, misc_choices[picked] );
    clear_misc_choices();
    strcpy( misc_choices[0], desc_buf );
    
    cat_ptr = get_cat_string(get_type(cur_screen));
    f_ptr = get_curly_brace( picked, cat_ptr);
    
    for (ii = 0; ii < 80; ii++)
	    if (misc_choices[0][ii] == NULL )
		    misc_choices[0][ii] = SPACE;
		    
    for (ii = 0; ii < 80; ii++)
	    misc_choices[1][ii] = SPACE;
	    
    do_lbcopy( f_ptr, L_DESCRIPT, sizeof(misc_choices), misc_choices[2]);
    CUR_SCRN.strt_choice = picked + 1 ;
    scrn_node[ HELP_SCREEN-1 ].choice[ 0 ].next = cur_screen ;
    scrn_node[ HELP_SCREEN-1 ].esc_next = cur_screen ;

} /* do_help() */

/*----------------------------------------------------------------------*/
/* clears screen, positions cursor then exits program.			*/
    VOID
clean_up() 
{
    dos_free( txt_bptr ) ;
    dos_free( cpy_bptr ) ;
} /* clean_up() */

/*----------------------------------------------------------------------*/
/* the main driver routine.  control is dependent on definitions of 	*/
/* screen nodes in screens.h						*/
    VOID
gemain() 
{
	
    if ( !init() )		/* if initialization not successful, bail */
        {
	clean_up() ;
        return ;
	}

    cur_screen = 1 ;		/* begin with screen 1 */
    while ( cur_screen != END_OF_SCRNS )
        {
	(*CUR_SCRN.setup_func)() ;
	draw_screen( CUR_SCRN.num_choices, CUR_SCRN.prompt, 
					CUR_SCRN.choices, CUR_SCRN.footer ) ;
	help = FALSE ;
	escape = FALSE ;
	picked = get_choice( CUR_SCRN.num_choices, CUR_SCRN.strt_choice,
						CUR_SCRN.choices ) ;
	if (cur_screen != 16)
		prev_screen = cur_screen;	/*save prev screen */
	
	if ( help )		/* F1 key pushed */
	   do_help() ;
	else if ( !escape )
     	   (*CUR_SCRN.choice[ picked ].todo_func)() ;
	if ( error )		/* An error occured */
	    cur_screen = CUR_SCRN.choice[ picked ].err_next ;
	else if ( help )
	    cur_screen = HELP_SCREEN ;
	else if ( escape )	/* ESC key pushed */
	    cur_screen = CUR_SCRN.esc_next ;
	else
	    cur_screen = CUR_SCRN.choice[ picked ].next ;
	}
    cur_set(5,7);
    cls(15,0,24,79);
    pos_cur(15,0);
    clean_up() ;
}/* gemain() */

/* end of gsmain.c */