/************************************************************************/
/**       Copyright 1999, Caldera Thin Clients, Inc.                     **/ 
/**       This software is licenced under the GNU Public License.        **/ 
/**       Please see LICENSE.TXT for further information.                **/
/**                                                                      **/ 
/**                  Historical Copyright                                **/ 
/**									**/
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

/* gsdualfp.c 		routines for installing/changing onto dual floppy
			systems.					*/

#include "portab.h"
#include "machine.h"
#include "dosbind.h"
#include "gsdefs.h"
#include "gstxtdef.h"
#include "gsevars.h"

EXTERN BOOLEAN	get_vol2();		/* gsfilcpy.c */
EXTERN WORD	is_vol();		/* gsfilcpy.c */
EXTERN BOOLEAN	strcmp() ;		/* gsutil.c */
EXTERN VOID	init_copy_echo();	/* gsdvinst.c */
EXTERN BYTE	*next_dlabel() ;	/* gsdvinst.c */
EXTERN VOID	dev_change();		/* gsdvinst.c */
EXTERN VOID	file_copy();		/* gsfilcpy.c */
EXTERN VOID	strtup_dirs();  	/* gsnewdev.c */
EXTERN VOID	cpy_strtup();		/* gsnewdev.c */
EXTERN VOID	dsktop_dirs();  	/* gsnewdev.c */
EXTERN VOID	cpy_dsktop();	  	/* gsnewdev.c */
EXTERN VOID	dev_install();		/* gsnewdev.c */
EXTERN VOID	clear_system();		/* gsdvinst.c */
EXTERN VOID	rm_devices();		/* gsdvinst.c */
EXTERN BOOLEAN	set_vol();	  	/* gsfilcpy.c */
EXTERN VOID     beeper() ;		/* scrnutil.asm */

#define		SCRN_11			scrn_node[11-1]
		   
/*----------------------------------------------------------------------*/
/*  This routine finds the disk label of the screen driver for new config */
    BYTE
*get_scrn_label()
{
	WORD	num;
	
	cur_dlabel = NULLPTR ;
	for (num = 0; num < num_devs ; num++)
		if ((devices[num].type == SCRN_TYPE ) && 
						(!devices[num].installed))
		{
			cur_dlabel = devices[num].src_disk;
			cur_dev = num ;
		}
	return(cur_dlabel);
} /* get_scrn_label() */

/*----------------------------------------------------------------------*/
/*  On dual floppy systems, some drivers are hidden in order to skip    */
/*  over or save the file copies.  This routine 'unhides' them.		*/
    VOID
undo_hides()
{
	WORD	ii ;
	
	for ( ii = 0 ; ii < num_devs; ii++)
		if (devices[ii].type == SCRN_TYPE)
			{
			/* UNHIDE SCREEN INSTALLS */
			if (devices[ii].installed == HIDE_FALSE)
			    devices[ii].installed = FALSE;

			/* UNHIDE SCREEN CHANGES */
			if (devices[ii].change == HIDE_TRUE)
			    devices[ii].change = TRUE;
		        }
		else
			{
			/* UNHIDE ALREADY INSTALLED DEVICES */
			if (devices[ii].installed == HIDE_TRUE)
				{
				devices[ii].installed = TRUE ;
				devices[ii].fnts_installed = TRUE ;
				}

			/* UNINSTALL INSTALLED DEVICES */
			else if (devices[ii].installed == TRUE )
				devices[ii].installed = FALSE ;
			
			/* UNHIDE UNCHANGING DEVICES */
			if (devices[ii].change == HIDE_FALSE )
				devices[ii].change = FALSE ;
			
			/* UNCHANGE CHANGED DEVICES */
			else if (devices[ii].change == FALSE )
				devices[ii].change = TRUE ;
			}
} /* undo_hides */
		    
			    
/*----------------------------------------------------------------------*/
/*  This routine hides driver states in order to skip file copies or    */
/*  save the current state in the event it gets changed unwantingly (?) */
    VOID
hide_drivers()
{
	WORD	ii ;
	
	for ( ii = 0 ; ii < num_devs; ii++)
		if (devices[ii].type == SCRN_TYPE)
			{
			/* HIDE UNINSTALLED NEW SCREEN */
			if (devices[ii].installed == FALSE)
				devices[ii].installed = HIDE_FALSE ;
			
			/* HIDE SCREEN CHANGES */
			if (devices[ii].change == TRUE)
				devices[ii].change = HIDE_TRUE;
			}
		else if (devices[ii].installed == TRUE)
			{
			/* HIDE ALREADY INSTALLED DEVICES */
			devices[ii].installed = HIDE_TRUE;
			devices[ii].fnts_installed = HIDE_TRUE;
			}
			/* HIDE UNCHANGES */
		else if (devices[ii].change == FALSE)
			devices[ii].change = HIDE_FALSE;
} /* hide_drivers */
				
/*----------------------------------------------------------------------*/
/*  For dual floppy configs, this routine determines the exit screen    */
/* dependings on mouse type and scanner.				*/
    VOID
finish_floppies()
{
      WORD	ch;
      
        ch = 0 ;
	
	if (cur_screen == 17)
		ch = 1 ;
	      
	if (is_scan_com)
		scrn_node[ cur_screen - 1 ].choice[ ch ].next = 30 ;
	else if (is_bus_mouse)
		scrn_node[ cur_screen - 1 ].choice[ ch ].next = 32 ;
	else
		scrn_node[ cur_screen - 1 ].choice[ ch ].next = 31 ;
		
} /* finish_floppies */

/*----------------------------------------------------------------------*/
/* This routine copies device drivers to startup or changes existing 	*/
/* devices or both.  Then passes control to exit screen.		*/
    VOID
finish_startup()
{
	WORD    ii ;
	
	SCRN_11.choice[ 0 ].next = 11;

/* CHECK FOR DRIVER DISK */
	for (ii = 0; ii < NUM_REL_DSKS + num_drvr_paks; ii++)
		if ( strcmp(disk_label[ii], cur_dlabel))
			break;
		
	if (!get_vol2( vol_label[ii], src_path[0]))
	{
		SCRN_11.choice[0].todo_func = finish_startup ;
		into_drive = src_path[0];
		insert_disk_labeled = cur_dlabel ;
		beeper() ;
		return ;
	}
	
/* CHECK FOR STARTUP DISK */
	if (!get_vol2(SU_VOL, DEFAULT_DRV[0]))
	{
		SCRN_11.choice[0].todo_func = finish_startup ;
		into_drive = DEFAULT_DRV[0];
		insert_disk_labeled = &SU_LABEL;
		beeper();
		return ;
	}
	init_copy_echo();
	if (( strcmp( cur_dlabel, disk_label[1])) || /* GEM SCREEN DISK #1 */
	    ( strcmp( cur_dlabel, disk_label[2])))   /* GEM SCREEN DISK #2 */
		dev_install(TRUE);
	else
		dev_install(FALSE);
	
	insert_disk_labeled = next_dlabel() ;
	
	if (insert_disk_labeled != NULLPTR )
		{
		into_drive = src_path[0];
		SCRN_11.choice[0].todo_func = finish_startup ;
		}
	else
		{
		dev_change() ;
		finish_floppies() ;
		}

} /* finish_startup */

/*----------------------------------------------------------------------*/
/*  In case of new_config, this routine does the disk build.		*/
    VOID
start_startup()
{
	WORD 	ii, vol;
	BYTE	temp[12] ;
	BOOLEAN	ok_disk ;
	
	SCRN_11.choice[ 0 ].next = 11;

/* CHECK SYSTEM DISK */
	if (!get_vol2( SYS_VOL, src_path[0]))
		{
		SCRN_11.choice[0].todo_func = start_startup ;
		into_drive = src_path[0];
		insert_disk_labeled = &SYS_LABEL ;
		beeper();
		return ;
	}

/* CHECK THAT NEW STARTUP DISK IS A REL OR DRVR OR DT DISK */
	ok_disk = TRUE;
	vol = is_vol(SYS_VOL, DEFAULT_DRV[0]);
	if (vol == 0)
		{
		for (ii = 0; ii < 11; ii++)
			temp[ii] = gl_dta[ii+8];
		temp[11] = NULL;
		for (ii = 1; ii < NUM_REL_DSKS + num_drvr_paks; ii++)
			if (strcmp(vol_label[ii], temp))
				ok_disk = FALSE ;
		if (strcmp(DT_VOL, temp))
				ok_disk = FALSE ;
		}
	else if (vol == 1)
		ok_disk = FALSE;
		
	if (ok_disk)
		{
		undo_hides();
		if (is_new_config)
			{
			set_vol(SU_VOL, DEFAULT_DRV[0]);
			strtup_dirs();
			clear_system();
			init_copy_echo();
			cpy_strtup();
			insert_disk_labeled = get_scrn_label();
			into_drive = src_path[0];
			SCRN_11.choice[0].todo_func = finish_startup ;
			}
		else
			{
			rm_devices() ;
			insert_disk_labeled = next_dlabel() ;
			if (insert_disk_labeled != NULLPTR )
				{
				into_drive = src_path[0];
				SCRN_11.choice[0].todo_func = finish_startup ;
				}
			else
				{
				dev_change() ;
				finish_floppies() ;
				}
			}
		}
	else
		{
		SCRN_11.choice[0].todo_func = start_startup ;
		into_drive = DEFAULT_DRV[0];
		insert_disk_labeled = &SU_LABEL;
		beeper();
		}

}/* start_startup */

/*----------------------------------------------------------------------*/
    VOID
finish_desktop()
{
	WORD    ii ;
	
	SCRN_11.choice[ 0 ].next = 11;

/* CHECK FOR DRIVER DISK */
	for (ii = 0; ii < NUM_REL_DSKS + num_drvr_paks; ii++)
		if ( strcmp(disk_label[ii], cur_dlabel))
			break;
		
	if (!get_vol2( vol_label[ii], src_path[0]))
		{
		SCRN_11.choice[0].todo_func = finish_desktop ;
		into_drive = src_path[0];
		insert_disk_labeled = cur_dlabel ;
		beeper();
		return ;
		}
	
/* CHECK FOR DESKTOP DISK */
	if (!get_vol2(DT_VOL, DEFAULT_DRV[0]))
		{
		SCRN_11.choice[0].todo_func = finish_desktop ;
		into_drive = DEFAULT_DRV[0];
		insert_disk_labeled = &DT_LABEL;
		beeper();
		return ;
		}

	init_copy_echo();
	dev_install(FALSE);

	insert_disk_labeled = next_dlabel() ;
	
	if (insert_disk_labeled != NULLPTR )
		{
		into_drive = src_path[0];
		SCRN_11.choice[0].todo_func = finish_desktop ;
		}
	else
		{
		SCRN_11.choice[0].todo_func = start_startup ;
		into_drive = DEFAULT_DRV[0];
		insert_disk_labeled = &SU_LABEL;
		}
} /* finish_desktop */
	
/*----------------------------------------------------------------------*/
    VOID
start2_desktop()
{
	WORD    ii ;
	
	SCRN_11.choice[ 0 ].next = 11;
		
/* CHECK FOR SCREEN DRIVER DISK */
	for (ii = 0; ii < NUM_REL_DSKS + num_drvr_paks; ii++)
		if ( strcmp(disk_label[ii], cur_dlabel))
			break;
		
	if (!get_vol2( vol_label[ii], src_path[0]))
	{
		SCRN_11.choice[0].todo_func = start2_desktop ;
		into_drive = src_path[0];
		insert_disk_labeled = cur_dlabel ;
		beeper();
		return ;
	}
	
/* CHECK FOR DESKTOP DISK */
	if (!get_vol2(DT_VOL, DEFAULT_DRV[0]))
	{
		SCRN_11.choice[0].todo_func = start2_desktop ;
		into_drive = DEFAULT_DRV[0];
		insert_disk_labeled = &DT_LABEL;
		beeper();
		return ;
	}
	
	init_copy_echo();
	file_copy(src_path, gemsys, mdsys, FALSE);
	hide_drivers();
	dev_install(FALSE);
	dev_change();
	
	insert_disk_labeled = next_dlabel() ;
	
	if (insert_disk_labeled != NULLPTR )
		{
		into_drive = src_path[0];
		SCRN_11.choice[0].todo_func = finish_desktop ;
		}
	else
		{
		insert_disk_labeled = &SU_LABEL ;
		into_drive = DEFAULT_DRV[0] ;
		SCRN_11.choice[0].todo_func = start_startup ;
		}
	
} /* start2_desktop() */

/*----------------------------------------------------------------------*/
    VOID
start1_desktop()
{
	WORD 	ii,vol;
	BYTE	temp[12] ;
	BOOLEAN	ok_disk ;
	
	SCRN_11.choice[ 0 ].next = 11;
		
/* CHECK FOR SYSTEM DISK */
	if (!get_vol2( SYS_VOL, src_path[0]))
		{
		SCRN_11.choice[0].todo_func = start1_desktop;
		into_drive = src_path[0];
		insert_disk_labeled = &SYS_LABEL ;
		beeper();
		return ;
		}

/* CHECK THAT NEW DESKTOP ISN'T A REL OR DRIVR DISK */
	ok_disk = TRUE;
	vol = is_vol(SYS_VOL, DEFAULT_DRV[0]);
	if (vol == 0)
		{
		for (ii = 0; ii < 11; ii++)
			temp[ii] = gl_dta[ii+8];
		temp[11] = NULL;
		for (ii = 1; ii < NUM_REL_DSKS + num_drvr_paks; ii++)
			if (strcmp(vol_label[ii], temp))
				ok_disk = FALSE ;
		}
	else if (vol == 1)
		ok_disk = FALSE;

/* BUILD DESKTOP DISK (ALWAYS ASSUMING NEW DISK BEING BUILT) */
	if (ok_disk)
		{
		set_vol(DT_VOL, DEFAULT_DRV[0]);
		dsktop_dirs();
		if (is_new_config)
			clear_system();
		else 
			rm_devices();
		init_copy_echo();
		cpy_dsktop();
		
/* GET SCREEN DISK (one that's needed or any) */
		insert_disk_labeled = NULLPTR ;
		for (ii = 0; ii < num_devs ; ii++)
			if ((devices[ii].type == SCRN_TYPE ) &&
				 (devices[ii].installed == FALSE))
				  insert_disk_labeled = devices[ii].src_disk;

		if (insert_disk_labeled == NULLPTR )
			insert_disk_labeled = &disk_label[1] ;

	        cur_dlabel = insert_disk_labeled ;
		into_drive = src_path[0] ;
		SCRN_11.choice[0].todo_func = start2_desktop ;
		}
	else
		{
			SCRN_11.choice[0].todo_func = start1_desktop ;
			insert_disk_labeled = &DT_LABEL ;
			into_drive = DEFAULT_DRV[0];
			beeper();
		}
} /*start1_desktop */

/*----------------------------------------------------------------------*/
    VOID
do_floppies()
{
	WORD ii ;
	BOOLEAN exit, do_desktop ;
	
	exit = TRUE ;
	do_desktop = FALSE;
	
	for (ii = 0 ; ii < num_devs ; ii ++ )
		if (( devices[ii].installed == FALSE ) ||
		    ( devices[ii].change == TRUE ))
			    {
			    exit = FALSE ;
			    if (!((devices[ii].type == SCRN_TYPE) &&
				  (devices[ii].change == TRUE)))
				    do_desktop =  TRUE;
			    }
			    
	if (num_delete > 0)
		exit = FALSE ;
	
	if ( !exit )
		{
		if (do_desktop)
			{
			SCRN_11.choice[0].todo_func = start1_desktop ;
			insert_disk_labeled = &DT_LABEL;
			into_drive = DEFAULT_DRV[0];
			}
		else
			{
			hide_drivers();
			SCRN_11.choice[0].todo_func = start_startup ;
			insert_disk_labeled = &SU_LABEL;
			into_drive = DEFAULT_DRV[0];
			}
		}
	else
		finish_floppies() ;
}/* do_floppies */
		
/* end of gsdualfp.c */
