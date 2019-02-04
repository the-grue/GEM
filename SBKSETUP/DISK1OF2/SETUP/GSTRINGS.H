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

/************************************************************************/
/* gstrings.h	GLOBAL string type variables for GEMSETUP.EXE		*/
/*                                                                      */
/* All the text strings used in the GEM Setup program R3.0, are con-    */
/* tained in this include file.  By declaring all the texual strings    */
/* together in one continuous area, translating a string to a different */
/* language is easier.  The translator doesn't have to search through   */
/* the entire executable file looking for text strings.  Also, the      */
/* strings have been declared 30-50% longer than what is needed for     */
/* english so that the translator has more room to work with for writing*/
/* more meaningful translations.                                        */
/************************************************************************/

GLOBAL BYTE     header1[] =  {201,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
		              205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,187,0};
GLOBAL BYTE   	header2[] =  "Digital Research Inc.                                   GEM Setup R3.0B13 "; 
GLOBAL BYTE	header3[] =  {200,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,205,
			      205,205,205,205,205,205,205,205,205,188,0};
		      
GLOBAL BYTE	misc_choices[MX_CHOICES][PROMPT_LEN] = { 0 } ;
GLOBAL BYTE	to_delete[ 5 ][ NAME_LENGTH ] = { 0 } ; 
GLOBAL BYTE	src_path[ FNAME_LENGTH ] = "c:\\";
GLOBAL BYTE	gemroot[ NAME_LENGTH ] = "c:\\";
GLOBAL BYTE	gemapps[ NAME_LENGTH * 2 ] = "c:\\GEMAPPS\\";
GLOBAL BYTE     gemfonts[ NAME_LENGTH * 2 ] = "c:\\GEMAPPS\\FONTS\\";
GLOBAL BYTE	gemsys[ NAME_LENGTH * 2 ] = "c:\\GEMAPPS\\GEMSYS\\";
GLOBAL BYTE	clipbrd[ NAME_LENGTH * 2 ] = "c:\\GEMAPPS\\CLIPBRD\\";
GLOBAL BYTE	sd_wildcard[ NAME_LENGTH ] = "SD*.*" ;
GLOBAL BYTE	vd_wildcard[ NAME_LENGTH ] = "VD*.*" ;
GLOBAL BYTE	pd_wildcard[ NAME_LENGTH ] = "PD*.*" ;
GLOBAL BYTE	cd_wildcard[ NAME_LENGTH ] = "CD*.*" ;
GLOBAL BYTE	id_wildcard[ NAME_LENGTH ] = "ID*.*" ;
GLOBAL BYTE	drvr_wcard[ NAME_LENGTH ]  = "?D*.*" ;
GLOBAL BYTE	CRLF[3] = { 0x0d, 0x0a, NULL } ;
GLOBAL BYTE	five_spaces[6] = "     " ;
GLOBAL BYTE	zyxg[ 5 ] = "zyxg" ;
GLOBAL BYTE	dtinf_hdisk[ 11 ] = "#M000000FF" ;
GLOBAL BYTE	dtinf_fdisk[ 11 ] = "#M000001FF" ;

GLOBAL BYTE	floppydisk[ NAME_LENGTH ] = "FLOPPY DISK" ;
GLOBAL BYTE	harddisk[ NAME_LENGTH ] = "HARD DISK" ;
GLOBAL BYTE	wait[ PROMPT_LEN ]    = "Busy, please wait...\n";
GLOBAL BYTE	scan_st1[ HALF_LINE ] = " SCAN PORT = ";
GLOBAL BYTE	scan_st2[ HALF_LINE ] = " SCAN XFER = ";
GLOBAL BYTE	failed[ 20 ]          = "  failed \0" ;
GLOBAL BYTE	done[ 20 ]            = "  done\0" ;

GLOBAL BYTE	gembat[ NAME_LENGTH ] = "GEM.BAT";
GLOBAL BYTE	gemsettxt[ NAME_LENGTH ] = "GEMSETUP.TXT";
GLOBAL BYTE	gemsetmsg[ NAME_LENGTH ] = "GEMSETUP.MSG";
GLOBAL BYTE	gemvdiexe[ NAME_LENGTH ] = "GEMVDI.EXE" ;
GLOBAL BYTE	calclockacc[ NAME_LENGTH ] = "CALCLOCK.ACC" ;
GLOBAL BYTE	desktopinf[ NAME_LENGTH ] = "DESKTOP.INF" ;
GLOBAL BYTE	desk2topinf[ NAME_LENGTH ] = "DESK2TOP.INF" ;
GLOBAL BYTE	deskhiicn[ NAME_LENGTH ] = "DESKHI.ICN" ;
GLOBAL BYTE	deskloicn[ NAME_LENGTH ] = "DESKLO.ICN" ;
GLOBAL BYTE	desktopapp[ NAME_LENGTH ] = "DESKTOP.APP" ;
GLOBAL BYTE	desktoprsc[ NAME_LENGTH ] = "DESKTOP.RSC" ;
GLOBAL BYTE	gemexe[ NAME_LENGTH ] = "GEM.EXE" ;
GLOBAL BYTE	gemrsc[ NAME_LENGTH ] = "GEM.RSC" ;
GLOBAL BYTE	outputapp[ NAME_LENGTH ] = "OUTPUT.APP" ;
GLOBAL BYTE	outputrsc[ NAME_LENGTH ] = "OUTPUT.RSC" ;
GLOBAL BYTE	mdsys[ NAME_LENGTH ] = "MDGEM8.SYS" ;

GLOBAL BYTE	scanner[20] = "SCANNER";
GLOBAL BYTE	printer[20] = "PRINTER";
GLOBAL BYTE	camera[20] = "CAMERA";
GLOBAL BYTE	screen[20] = "SCREEN";
GLOBAL BYTE	mouse[20] = "MOUSE";
GLOBAL BYTE	plotter[20] = "PLOTTER";
GLOBAL BYTE	metafile[20] = "METAFILE" ;
GLOBAL BYTE	promptsx[20] = "PROMPT_PTR" ;
GLOBAL BYTE	choicesx[20] = "CHOICE_PTR" ;
GLOBAL BYTE	footersx[20] = "FOOTER_PTR" ;
GLOBAL BYTE	hard_vol[20] = "HVOL_PTR" ;
GLOBAL BYTE	hard_lab[20] = "HLAB_PTR" ;
GLOBAL BYTE	keywords[20] = "KEYWORDS_PTR" ;
GLOBAL BYTE	flop_lab[20] = "FLAB_PTR" ;
GLOBAL BYTE	flop_vol[20] = "FVOL_PTR" ;
GLOBAL BYTE	msg1[20] = "COPY_PTR" ;
GLOBAL BYTE	msg2[20] = "TOO_MANY" ;
GLOBAL BYTE	msg3[20] = "D_SPACE" ;

/* NUM_REL_DSKS = 8 */

GLOBAL BYTE	prompts[ PROMPTS_SZ ][ PROMPT_LEN ] ;
GLOBAL BYTE	choices[ CHOICES_SZ ][ PROMPT_LEN ] ;
GLOBAL BYTE	footers[ FOOTERS_SZ ][ PROMPT_LEN ] ;
GLOBAL BYTE	vol_label[ NUM_REL_DSKS + 6][ 12 ] ;
GLOBAL BYTE	disk_label[ NUM_REL_DSKS + 6][ HALF_LINE ] ;
GLOBAL BYTE	gs_keywds[ 12 ][ 15 ] ;
GLOBAL BYTE	label_floppies[ 2 ][ HALF_LINE ] ;
GLOBAL BYTE     vol_floppies[ 2 ][ 12 ] ;
GLOBAL BYTE	copy_echo[ PROMPT_LEN ] ;
GLOBAL BYTE	too_many_devices[2][PROMPT_LEN]; 
GLOBAL BYTE	go_on[2][PROMPT_LEN] ; 


/* end of gstrings.h */
