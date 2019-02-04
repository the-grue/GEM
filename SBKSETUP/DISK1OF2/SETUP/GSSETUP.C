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

/* gssetup.c 		routines for setting up custom screens.		*/

#include "portab.h"
#include "machine.h"
#include "dosbind.h"
#include "gsdefs.h"
#include "gstxtdef.h"
#include "gsevars.h"

EXTERN VOID	strcpy() ;		/* gsutil.c */
EXTERN VOID	strcat() ;		/* gsutil.c */
EXTERN WORD	strlen() ;		/* gsutil.c */
EXTERN WORD	set_vol() ;		/* gsfilcpy.c */
EXTERN VOID	b_stuff() ;		/* scrnutil.asm */
EXTERN VOID	new_device() ;		/* gsnewdev.c */
EXTERN VOID	ch_device() ;		/* gsnewdev.c */

/*----------------------------------------------------------------------*/
    VOID
clear_devs()
{
	num_devs = 0;
	b_stuff( ADDR( devices ), sizeof( devices ), NULL ) ;
	num_delete = 0;
}

/*----------------------------------------------------------------------*/
    VOID
setup_2()
{
}

/*----------------------------------------------------------------------*/
    VOID
setup_3()
{
	WORD	ii, mask;
	
	mask = 0x2000 ;		/* start at C */
	while ( !( mask & harddrives ) )
	    mask >>= 1 ;

	ii = 0 ;
	
	b_stuff( ADDR( misc_choices ), sizeof( misc_choices ), SPACE ) ;
	scrn_node[3 - 1].num_choices = 0 ;
	
	while ( mask )
	    {
	    if ( harddrives & mask )
	        {
		misc_choices[ scrn_node[3 - 1].num_choices ][ 0 ] = UCASE_C + ii ;
		misc_choices[ scrn_node[3 - 1].num_choices ][ 1 ] = NULL ;
	   	scrn_node[3 - 1].num_choices += 1 ;
		}	    
	    ++ii ;
	    mask >>= 1 ;
	    }
}

/*----------------------------------------------------------------------*/
    VOID
set_dt_vol()
{
	set_vol(DT_VOL, DEFAULT_DRV[0]);
} /* set_dt_vol */
	
/*----------------------------------------------------------------------*/
    VOID
setup_78(prompt,patch)
BYTE prompt[];
BYTE patch;
{
	WORD ii;
	
	for (ii = 0; ii < PROMPT_LEN; ii++)
		if (prompt[ii] == PIPE)
			prompt[ii] = patch;
}
/*----------------------------------------------------------------------*/
/* "Label your disk GEM DESKTOP and insert it in drive (drive )."	*/
    VOID
setup_7()
{
	setup_78(W7_PROMPT,DEFAULT_DRV[0]);
} /* setup_7() */

/*----------------------------------------------------------------------*/
/* "Label one disk GEM DESKTOP and the other GEM STARTUP		*/
/*  insert...in drive (drive )."					*/
    VOID
setup_8()
{
	setup_78(W8_PROMPT + PROMPT_LEN,DEFAULT_DRV[0]);
} /* setup_7() */

/*----------------------------------------------------------------------*/
/* "Insert the disk labeled (label) into drive (drive )."		*/
    VOID
setup_11()
{
BYTE	*ptr1, *ptr2, *end ;
BOOLEAN	found_1st;

    
   if (prev_screen != 11)
	   {
	   if (past_sv_and_ex)
		   scrn_node[11-1].esc_next = 35 ;
	   else 
		   scrn_node[11-1].esc_next = prev_screen ;
	   }

    b_stuff( ADDR( misc_choices ), sizeof( misc_choices ), NULL ) ;
    found_1st = FALSE;
    end = W11_PROMPT + strlen( W11_PROMPT ) ;
    for (ptr1 = W11_PROMPT; ptr1 < end; ptr1++)
	    {
		if ( *ptr1 == PIPE ) 
		    {
		    *ptr1 = NULL ;
		    if ( !found_1st )
			{
			ptr2 = ptr1 + 1 ;
			strcpy( &misc_choices[0], W11_PROMPT ) ;
			strcat( &misc_choices[0], insert_disk_labeled ) ;
			found_1st = TRUE;
			*ptr1 = PIPE ;
			}
		    else 
		        {
			strcat( &misc_choices[0], ptr2 ) ;    
			*((BYTE*)(misc_choices) + strlen( misc_choices ) ) 
								= into_drive;
			strcat( &misc_choices[0], ptr1 + 1 ) ;		
			*ptr1 = PIPE ;
			}
		    }
	    }
} /* setup_11() */
/*----------------------------------------------------------------------*/
/* Insert driver pack screen						*/
#define SCRN_16		scrn_node[16-1]
    VOID
setup_16()
{
	setup_78(W16_PROMPT,src_path[0]);
	SCRN_16.esc_next = prev_screen;
	SCRN_16.choice[0].next = prev_screen;
	SCRN_16.choice[1].next = prev_16_screen;
	if (prev_screen == 12 || prev_screen == 21 || prev_screen == 23 ||
	    prev_screen == 25 || prev_screen == 27)
		scrn_node[prev_screen-1].choice[picked].todo_func =new_device;
	else
	        scrn_node[prev_screen-1].choice[picked].todo_func = ch_device;

} /* setup_16 */

/*----------------------------------------------------------------------*/
    VOID
show_scan_stuff( dev )
WORD dev;
{
	BYTE	*start, *end;
	WORD 	ii;
	BOOLEAN spaced;
	
	spaced = FALSE ;
	

	if (*devices[dev].scan_port != NULL)
		{
		if (!spaced)
			{
			strcat( W17_PROMPT, five_spaces ) ;
			strcat( W17_PROMPT, five_spaces ) ;
			spaced = TRUE ;
			}
		strcat(W17_PROMPT, scan_st1);
		end = devices[dev].scan_port;
		for (ii = 0; ii < dev; ii++)
			{
			start = end ;
			while (*end != COMMA)
				end++;
			}
		*end = NULL;
		strcat(W17_PROMPT, start);
		*end = COMMA ;
		}
	if (*devices[dev].scan_xfer != NULL)
		{
		if (!spaced)
			{
			strcat( W17_PROMPT, five_spaces ) ;
			strcat( W17_PROMPT, five_spaces ) ;
			spaced = TRUE ;
			}
		strcat(W17_PROMPT, scan_st2);
		end = devices[dev].scan_xfer;
		for (ii = 0; ii < dev; ii++)
			{
			start = end ;
			while (*end != COMMA)
				end++;
			}
		*end = NULL;
		strcat(W17_PROMPT, start);
		*end = COMMA ;
		}
    strcat( W17_PROMPT, CRLF ) ;
} /* show_scan_stuff */

/*----------------------------------------------------------------------*/
/* setup custom prompt for screen #17.					*/
    VOID
setup_17()
{
WORD	ii ;

    strcpy( W17_PROMPT, prompts[ 27 ] ) ;
    strcat( W17_PROMPT, CRLF ) ;
    strcat( W17_PROMPT, CRLF ) ;
    ii = 0 ;
    while ( ii < num_devs )
        { 
	strcat( W17_PROMPT, five_spaces ) ;
	strcat( W17_PROMPT, devices[ ii ].desc ) ;
	strcat( W17_PROMPT, CRLF ) ;
	if ( devices[ ii ].type == SCRN_TYPE )
	    {
	    strcat( W17_PROMPT, five_spaces ) ;
	    strcat( W17_PROMPT, devices[ ii ].m_desc ) ;
	    strcat( W17_PROMPT, CRLF ) ;
	    }
	if ((devices[ii].type != SCAN_TYPE) && 
		(!( devices[ ii ].type == SCRN_TYPE && 
			( devices[ ii ].mouse == NO_MOUSE || 
			  devices[ ii ].mouse == PS2_MOUSE || 
			  devices[ ii ].mouse == BUS_MOUSE) ) ) ) 
	    {
	    strcat( W17_PROMPT, five_spaces ) ;
	    strcat( W17_PROMPT, five_spaces ) ;
	    strcat( W17_PROMPT, choices[ 8 + devices[ ii ].port ] ) ;
	    strcat( W17_PROMPT, CRLF ) ;
	    }
        if ( devices[ ii ].type == SCAN_TYPE)
		    show_scan_stuff( ii );		
	++ii ;
	}
    strcat( W17_PROMPT, CRLF ) ;
    strcat( W17_PROMPT, prompts[ 28 ] ) ;
    
} /* setup_17() */

#define SCRN_18		scrn_node[18-1]
/*----------------------------------------------------------------------*/
/* setup custom prompt for screen #18.					*/
    VOID
setup_18()
{
    WORD	ii, jj, kk ;
    WORD	dev_next, port_next ;

    kk = CHOICE_LEN;
    jj = ii = 0;
    while ( ii < num_devs )
    { 
	switch ( devices[ ii ].type )
	    {
	    case SCRN_TYPE :	dev_next = 37 ;
	    			port_next = 15 ;
				break ;
	    case PRNT_TYPE :	dev_next = 38 ;
	    			port_next = 22 ;
				break ;
	    case PLOT_TYPE :	dev_next = 39 ;
	    			port_next = 24 ;
				break ;
	    case CMRA_TYPE :	dev_next = 40 ;
	    			port_next = 26 ;
				break ;
	    case SCAN_TYPE :	dev_next = 41 ;
	    			port_next = 28 ;
				break ;
	    }
	strcpy( W18_CHOICES + jj*kk, devices[ ii ].desc ) ;
    	SCRN_18.choice[ jj ].next = dev_next ;
    	jj++;
	if ( devices[ ii ].type == SCRN_TYPE )
	    {
	    strcpy( W18_CHOICES + jj*kk, devices[ ii ].m_desc ) ;
            SCRN_18.choice[ jj ].next = 42;
	    jj++;
	    }
	if (( devices[ ii ].type != SCAN_TYPE ) && 
		(devices[ ii ].type != SCRN_TYPE ||
			(devices[ ii ].mouse != NO_MOUSE &&
			 devices[ ii ].mouse != PS2_MOUSE &&
			 devices[ ii ].mouse != BUS_MOUSE)))
	    {
	    strcpy( W18_CHOICES + jj*kk, five_spaces );
	    strcat(W18_CHOICES+jj*kk,choices[8+devices[ii].port]);
	    SCRN_18.choice[ jj ].next = port_next ;
	    jj++;
	    }

    ++ii ;
    }
        SCRN_18.num_choices = jj ;
	
} /* setup_18() */


/*----------------------------------------------------------------------*/
/* setup custom prompt for screen #19.					*/
    VOID
setup_19()
{
WORD		num, num_choices ;
DEV_STRUCT	*dev_ptr ;

    num = 0 ;
    num_choices = 0 ;
    while ( num < num_devs )
    	{ 
	dev_ptr = &devices[ num ] ;
	if ( dev_ptr->type != SCRN_TYPE )	/* can't remove screen */
	    {
	    strcpy( W19_CHOICES + num_choices * CHOICE_LEN, dev_ptr->desc ) ;
	    ++num_choices ;
	    }
	else /*( dev_ptr->mouse != NO_MOUSE && dev_ptr->mouse != PS2_MOUSE &&
		   dev_ptr->mouse != BUS_MOUSE)   you can remove a mouse */
	    {
	    strcpy( W19_CHOICES + num_choices * CHOICE_LEN, dev_ptr->m_desc );
	    ++num_choices ;
	    }
	++num ;
        }
    scrn_node[ 19 - 1 ].num_choices = num_choices ;
} /* setup_19() */

/*----------------------------------------------------------------------*/
/* setup custom prompt for screens #28 and #29.				*/
    VOID
do_scan_setup( ptr )
BYTE *ptr;
{
	BYTE 	*start, *end;
	BOOLEAN	done;
	
	g_num_choices = 0;
	
	start = ptr;
	end = start;
	done = FALSE;
	
	while (!done)
		if (*end == COMMA )
			{
			*end = NULL;
			strcpy(misc_choices[g_num_choices], start);
			++g_num_choices;
			*end = COMMA;
			++end;
			start = end;
			}
		else if (*end == NULL)
			{
			strcpy(misc_choices[g_num_choices], start);
			++g_num_choices;
			done = TRUE;
			}
		else 
			++end;
} /* do_scan_setup() */
			
/*----------------------------------------------------------------------*/
/* setup custom prompt for screen #28, get scanner_port.		*/
    VOID
setup_28()
{
	do_scan_setup(devices[cur_dev].scan_port);
}
/*----------------------------------------------------------------------*/
/* setup custom prompt for screen #29, get scanner_xfer.		*/
    VOID
setup_29()
{
	do_scan_setup(devices[cur_dev].scan_xfer);
}
			
/*----------------------------------------------------------------------*/
/* setup custom prompt for screen #30.					*/
    VOID
setup_30()
{
	WORD 	ii;
	
	
	for (ii = 0; ii < PROMPT_LEN; ii++)
		if (*(W30_PROMPT + ii) == PIPE)
			 *(W30_PROMPT + ii) = src_path[0];
		 			   into_drive = DEFAULT_DRV[0];
					   
	if ( is_bus_mouse )
		scrn_node[30-1].choice[0].next = 32 ;

	
} /* setup_30() */

/*----------------------------------------------------------------------*/
    VOID
get_s_address()
{
	devices[cur_dev].W_SCAN_XFER = picked + 1;
	if (*devices[cur_dev].scan_com != NULL)
		is_scan_com = TRUE ;

} /* get_s_address() */

/*----------------------------------------------------------------------*/
    VOID
get_s_port()
{
	devices[cur_dev].W_SCAN_PORT = picked;

	if (*devices[cur_dev].scan_xfer != NULL)
		scrn_node[28-1].choice[picked].next = 29;

} /* get_s_port() */

/*----------------------------------------------------------------------*/
/* setup custom prompt for screen #31, exit screen.    			*/
    VOID
setup_31()
{
	WORD	ii, jj, kk ;

	kk = CHOICE_LEN;
	ii = 0;
    
	strcpy( W31_PROMPT + ii*kk, prompts[43]);
    	ii++;
    	strcpy(W31_PROMPT + ii*kk, prompts[44]);
    	ii++;
	
	if (config == DUAL_FLOPPY)
	{
		strcpy(W31_PROMPT + ii*kk, prompts[62]);
		ii++;
		strcpy(W31_PROMPT + ii*kk, prompts[63]);
		ii++;
		strcpy(W31_PROMPT + ii*kk, prompts[66]);
		for (jj = 0; jj < 80 ; jj++)
			if (misc_choices[ii][jj] == PIPE)
				misc_choices[ii][jj] = UCASE_A;
		ii++;
		strcpy(W31_PROMPT + ii*kk, prompts[67]);

		for (jj = 0; jj < 80 ; jj++)
			if (misc_choices[ii][jj] == PIPE)
				misc_choices[ii][jj] = UCASE_A;
	}
	else if (config == SING_FLOPPY)
	{
		strcpy(W31_PROMPT + ii*kk, prompts[6]);
		ii++;
		strcpy(W31_PROMPT + ii*kk, prompts[19]);
		ii++;
		strcpy(W31_PROMPT + ii*kk, prompts[20]);
		for (jj = 0; jj < 80 ; jj++)
			if (misc_choices[ii][jj] == PIPE)
				misc_choices[ii][jj] = UCASE_A;
		ii++;
		strcpy(W31_PROMPT + ii*kk, prompts[67]);

		for (jj = 0; jj < 80 ; jj++)
			if (misc_choices[ii][jj] == PIPE)
				misc_choices[ii][jj] = UCASE_A;
			
	}

	else if (config == HARD_DISK)
	     {	
		strcpy(W31_PROMPT + ii*kk, prompts[68]);

		for (jj = 0; jj < 80 ; jj++)
			if (misc_choices[ii][jj] == PIPE)
				misc_choices[ii][jj] = DEFAULT_DRV[0];
	     }
       			
       	for (kk = 0; kk <= ii; kk++)
		for (jj = 0; jj < 80; jj++)
			if (misc_choices[ii][jj] == NULL)
					misc_choices[kk][jj] = SPACE;
                                
	misc_choices[ii][79] = NULL;                        
} /* setup_31() */

/*----------------------------------------------------------------------*/
/* setup custom prompt for screen #31, exit screen.    			*/
    VOID
setup_32()
{
		scrn_node[32-1].choice[0].next = 31 ;
}

/*----------------------------------------------------------------------*/
/* setup custom prompt for screen #35.					*/
    VOID
setup_35()
{
	if (past_sv_and_ex)
		scrn_node[35-1].choice[1].next = 11;
}

/*----------------------------------------------------------------------*/
/* setup custom prompt for help screen.					*/
    VOID
setup_help()
{
}
/* end of gssetup.c */
