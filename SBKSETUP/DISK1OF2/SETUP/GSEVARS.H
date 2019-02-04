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

/* gsevars.h 	EXTERN variable declarations for GEMSETUP.EXE.		*/

EXTERN	WORD	cur_screen ;
EXTERN	WORD	prev_screen ;
EXTERN	WORD	prev_16_screen ;
EXTERN	WORD	picked ;
EXTERN  WORD	offset ;
EXTERN  LONG    txt_bsiz ;		/* dot txt file size */
EXTERN  FBYTE 	*txt_bptr;		/* Pointer to data files */
EXTERN  LONG	mem_avail;
EXTERN  FBYTE	*cpy_bptr ;		/* ptr to copy buffer in memory */
EXTERN  BOOLEAN is_bus_mouse ;		/* Flag for screen 32 */
EXTERN  BOOLEAN is_scan_com ;
EXTERN  BOOLEAN empty_dest_drv ;	/* Flag for dual floppy installation*/
EXTERN  WORD	num_drvr_paks ;		/* current number of driver packs */
EXTERN  WORD	max_devs ;		/* max num of devices */
EXTERN  WORD    colors[5] ;

EXTERN  WORD    config ;
EXTERN  BOOLEAN	is_new_config ;
EXTERN	BOOLEAN	past_sv_and_ex ;	/* TRUE if installation started */
EXTERN  UWORD	harddrives ;
EXTERN  UWORD	flpydrives ;
EXTERN  WORD	g_num_choices ;

EXTERN  BOOLEAN	error ;
EXTERN  BOOLEAN	help ;
EXTERN	BOOLEAN	escape ;

EXTERN  BYTE	*last_footer ;
EXTERN  BYTE	check[2];
EXTERN  BYTE	bullet[2];
EXTERN  BYTE	logo1[2];
EXTERN  BYTE	logo2[2];

EXTERN WORD 		num_devs ; 	/* number of devices chosen */
EXTERN WORD		num_delete ;
EXTERN WORD 		cur_dev ; /* current device */

EXTERN DEV_STRUCT	devices[ 5 ] ;
EXTERN FBYTE		*cat_ptr[ MX_CATEGORIES ] ;

EXTERN BYTE	gemroot[ NAME_LENGTH ] ;
EXTERN BYTE	gemapps[ NAME_LENGTH * 2 ] ;
EXTERN BYTE     gemfonts[ NAME_LENGTH * 2 ] ;
EXTERN BYTE	gemsys[ NAME_LENGTH * 2 ] ;
EXTERN BYTE	clipbrd[ NAME_LENGTH * 2 ] ;
EXTERN BYTE	gembat[ NAME_LENGTH ] ;
EXTERN BYTE	gemsettxt[ NAME_LENGTH ] ;
EXTERN BYTE	gemsetmsg[ NAME_LENGTH ] ;
EXTERN BYTE	gemvdiexe[ NAME_LENGTH ] ;
EXTERN BYTE	calclockacc[ NAME_LENGTH ] ;
EXTERN BYTE	desktopinf[ NAME_LENGTH ] ;
EXTERN BYTE	desk2topinf[ NAME_LENGTH ] ;
EXTERN BYTE	deskhiicn[ NAME_LENGTH ] ;
EXTERN BYTE	deskloicn[ NAME_LENGTH ] ;
EXTERN BYTE	desktopapp[ NAME_LENGTH ] ;
EXTERN BYTE	desktoprsc[ NAME_LENGTH ] ;
EXTERN BYTE	gemexe[ NAME_LENGTH ] ;
EXTERN BYTE	gemrsc[ NAME_LENGTH ] ;
EXTERN BYTE	outputapp[ NAME_LENGTH ] ;
EXTERN BYTE	outputrsc[ NAME_LENGTH ] ;
EXTERN BYTE	mdsys[ NAME_LENGTH ] ;

EXTERN BYTE	sd_wildcard[ NAME_LENGTH ] ;
EXTERN BYTE	vd_wildcard[ NAME_LENGTH ] ;
EXTERN BYTE	pd_wildcard[ NAME_LENGTH ] ;
EXTERN BYTE	cd_wildcard[ NAME_LENGTH ] ;
EXTERN BYTE	id_wildcard[ NAME_LENGTH ] ;
EXTERN BYTE	drvr_wcard[ NAME_LENGTH ] ;

EXTERN BYTE	src_path[ FNAME_LENGTH ] ;

EXTERN BYTE     no_choices[20] ;
EXTERN BYTE	scanner[20] ;
EXTERN BYTE	printer[20] ;
EXTERN BYTE	camera[20] ;
EXTERN BYTE	screen[20] ;
EXTERN BYTE	mouse[20] ;
EXTERN BYTE	plotter[20] ;
EXTERN BYTE	metafile[20] ;
EXTERN BYTE	promptsx[20] ;
EXTERN BYTE	choicesx[20] ;
EXTERN BYTE	footersx[20] ;
EXTERN BYTE	hard_vol[20] ;
EXTERN BYTE	hard_lab[20] ;
EXTERN BYTE	keywords[20] ;
EXTERN BYTE	flop_lab[20] ;
EXTERN BYTE	flop_vol[20] ;
EXTERN BYTE	*cur_dlabel ;
EXTERN BYTE	*insert_disk_labeled ;
EXTERN BYTE	into_drive ;

EXTERN BYTE	CRLF[3] ;
EXTERN BYTE	five_spaces[6] ;
EXTERN BYTE	zyxg[ 5 ] ;
EXTERN BYTE	dtinf_hdisk[ 11 ] ;
EXTERN BYTE	dtinf_fdisk[ 11 ] ;
EXTERN BYTE	floppydisk[ NAME_LENGTH ] ;
EXTERN BYTE	harddisk[ NAME_LENGTH ] ;
EXTERN BYTE	copy_echo[ 80 ] ;
EXTERN BYTE	failed[ 20 ] ;
EXTERN BYTE	done[ 20 ] ;
EXTERN BYTE	wait[ PROMPT_LEN ];
EXTERN BYTE	go_on[2][PROMPT_LEN];
EXTERN BYTE	too_many_devices[2][ PROMPT_LEN ];
EXTERN BYTE	scan_st1[ HALF_LINE ];
EXTERN BYTE	scan_st2[ HALF_LINE ];
EXTERN BYTE     init_1[ PROMPT_LEN ];
EXTERN BYTE     init_2[ PROMPT_LEN ];

EXTERN BYTE	to_delete[ 5 ][ NAME_LENGTH ] ; 

EXTERN BYTE	prompts[ PROMPTS_SZ ][ PROMPT_LEN ] ;
/*
"1234567890123456789012345678901234567890123456789012345678901234567890123456789"
*/
EXTERN BYTE	choices[ CHOICES_SZ ][ PROMPT_LEN ] ;

EXTERN BYTE	footers[ FOOTERS_SZ ][ PROMPT_LEN ] ;

EXTERN BYTE	misc_choices[MX_CHOICES][PROMPT_LEN] ;


EXTERN BYTE     header1[] ;

EXTERN BYTE   	header2[] ;

EXTERN BYTE	header3[] ;

EXTERN	BYTE	gs_keywds[ 12 ][ 15 ] ;

EXTERN	BYTE	disk_label[ NUM_REL_DSKS + 6][ HALF_LINE ] ; 
EXTERN	BYTE	vol_label[ NUM_REL_DSKS + 6][ 12 ] ;
EXTERN	BYTE	label_floppies[ 2 ][ HALF_LINE ] ;
EXTERN	BYTE	vol_floppies[ 2 ][ 12 ] ;

EXTERN	SCREEN_REC	scrn_node[ MX_SCREENS ] ;

/* end of gsevars.h */
