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

/* GSDEFS.h	typedefs and defines for GEMSETUP.EXE.			*/

#define		COPY_BSIZE	0x10000L
#define		MIN_MEM		0xA000L

#define		NUM_REL_DSKS	8
#define		FOOTERS_SZ	3
#define		PROMPTS_SZ	69
#define		CHOICES_SZ	28
#define		MX_CHOICES	20
#define		MX_SCREENS	45
#define		MX_MICE		10
#define		MX_CATEGORIES	15 	/* how many '@' in gemsetup.txt? */
					/* also add '@' in any driver pks */
#define		HELP_SCREEN	36

#define		INDENT		5		/* Column of choice print */
#define		REG_COL		colors[0]	/* Regular text attribute */
#define		CHOS_COL	colors[1]	/* Red	  		  */
#define		CHK_COL		colors[2]	/* Reverse video red	  */
#define		REV_COL		colors[3]	/* Reverse video regular  */

#define		NO_ERR_EXPECTED	1 
#define		END_OF_SCRNS	-1
#define		NO_ESC_DEFINED  1

#define		PIPE		'|'
#define		BEG_BRACE       '{'
#define		END_BRACE       '}'
#define		SLASH		'\\'
#define		UCASE_A		'A'
#define 	UCASE_C 	'C'
#define		LCASE_Z		'z'
#define		ASTERISK	'*'
#define		COLON		':'
#define		SEMICOLON	';'
#define		DOT	 	'.'
#define		COMMA		','
#define		W_DOT		"."
#define 	UCASE_D 	'D'
#define 	UCASE_W 	'W'
#define 	UCASE_M 	'M'
#define		AT_SIGN		'@'
#define		SHARP_SIGN	'#'
#define		SPACE		0x20
#define		TAB		0x09
#define		CR		0x0d
#define		LF		0x0a
#define		CNTL_Z		0x1a
#define		ESCAPE		0x011b
#define		F1		0x3B00
#define		UP_ARROW	0x4800
#define		DOWN_ARROW	0x5000

#define		SCRN_TYPE	1
#define		PRNT_TYPE	21
#define		PLOT_TYPE	11
#define		CMRA_TYPE	41
#define		SCAN_TYPE	51
#define		MOUS_TYPE	301
#define		META_TYPE	31
#define		NO_MOUSE	0 
#define		BUS_MOUSE	2
#define		PS2_MOUSE	10
#define		NO_PORT		0xFF

#define		DUAL_FLOPPY	1
#define		SING_FLOPPY	21
#define		HARD_DISK	41

#define		HIDE_FALSE	-6
#define		HIDE_TRUE	-5

#define		MX_DEVICES	max_devs
#define		FNAME_LENGTH	67
#define		NAME_LENGTH	13
#define		DESC_LENGTH	80
#define		PROMPT_LEN	80
#define		CHOICE_LEN	80
#define		MX_HEX		10	/* ~max length of a hex string */
#define		HALF_LINE	40	

#define		READ		0 	/* file access code for dos_open() */
#define		READWRITE	0	/* file ATTRIBUTE code dos_create() */

typedef struct _dev_struct
{
    WORD	type ;
    BOOLEAN	installed ;
    BOOLEAN	fnts_installed ;
    BOOLEAN	change ;
    WORD	port ;
    WORD	mouse ;
    BYTE	f_name[ NAME_LENGTH ] ;
    BYTE	s_desc[ NAME_LENGTH ] ;
    BYTE	desc[ DESC_LENGTH + 1 ] ;
    BYTE	m_desc[ DESC_LENGTH + 1 ] ;
    BYTE	src_disk[ HALF_LINE ] ;
    BYTE	font_disk[ HALF_LINE ] ;
    BYTE	font_wcard[ HALF_LINE ] ;
    BYTE	scan_port[ FNAME_LENGTH ] ;
    BYTE	scan_xfer[ FNAME_LENGTH ] ;
    BYTE	scan_com[ NAME_LENGTH ] ;
    BYTE	aux_file[ FNAME_LENGTH ] ;
} DEV_STRUCT ;

typedef struct _action
{
    WORD	(*todo_func)() ;
    WORD	next ;
    WORD	err_next ;
} ACTION_REC ;

typedef struct _screen_record
{
    BYTE	(*setup_func)() ;
    WORD	num_choices ;
    WORD	strt_choice ;
    BYTE	*prompt ;
    BYTE	*choices ;
    BYTE	*footer ;
    WORD	esc_next ;
    ACTION_REC	choice[ MX_CHOICES ] ;
} SCREEN_REC ;    

/* end of gsdefs.h */
