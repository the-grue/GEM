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

/* gsgvars.h 	GLOBAL variable declarations for GEMSETUP.EXE.		*/

GLOBAL	WORD	cur_screen ;		/* the current screen node */
GLOBAL	WORD	prev_screen ;        	/* the previous screen node */
GLOBAL	WORD	prev_16_screen ;	/* the previous relative to node 16 */
GLOBAL	WORD	picked ;		/* choice picked */
GLOBAL  WORD	offset ;		/* row offset from top of screen */
GLOBAL  LONG    txt_bsiz ;		/* dot txt file size */
GLOBAL  FBYTE 	*txt_bptr;		/* Pointer to data files */
GLOBAL  LONG	mem_avail;		/* amount of unallocated memory left*/
GLOBAL  FBYTE	*cpy_bptr ;		/* ptr to copy buffer in memory */
GLOBAL  BOOLEAN is_bus_mouse ;		/* Flag for screen 32 */
GLOBAL  BOOLEAN is_scan_com ;		/* Flag for screen 30, scan.com */
GLOBAL  BOOLEAN empty_dest_drv ;	/* Flag for dual floppy installation*/
GLOBAL  WORD	num_drvr_paks ;		/* current number of driver packs */
GLOBAL  WORD	max_devs ;		/* max num of devices allowed	*/

GLOBAL  WORD    config ;		/* HARD, DUAL, of SING */
GLOBAL	BOOLEAN	is_new_config ;		/* TRUE if new install */
GLOBAL	BOOLEAN	past_sv_and_ex ;	/* TRUE if installation started */
GLOBAL  UWORD	harddrives ;		/* num of hard drives */
GLOBAL  UWORD	flpydrives ;		/* num of floppy drives */
GLOBAL  WORD	g_num_choices ;		/* num of choices to choose from */
GLOBAL  BYTE	into_drive ;		/* target drive for inserting disks */
GLOBAL  BYTE	*insert_disk_labeled = NULLPTR ;

GLOBAL  BOOLEAN	error ;			/* TRUE if errors occur in todo */
GLOBAL  BOOLEAN	help ;			/* TRUE if F1 pressed */
GLOBAL	BOOLEAN	escape ;		/* TRUE if esc key pressed */

GLOBAL  BYTE	*cur_dlabel = &disk_label[0] ;
GLOBAL  BYTE	*last_footer = NULLPTR ;
GLOBAL  BYTE	check[2];
GLOBAL  BYTE	bullet[2];
GLOBAL  BYTE	logo1[2];
GLOBAL  BYTE	logo2[2];


GLOBAL WORD 		num_devs = 0 ; 	/* number of devices chosen */
GLOBAL WORD		num_delete = 0 ;
GLOBAL WORD 		cur_dev = -1 ; /* current device */
GLOBAL WORD 		colors[5];

GLOBAL DEV_STRUCT	devices[ 5 ] = { 0 } ;
GLOBAL FBYTE		*cat_ptr[ MX_CATEGORIES ] = { 0 } ;

/* end of gsgvars.h */
