/*	DESKWIN.H	06/11/84 - 01/04/85		Lee Lorenzen	*/
/*	changed NUM_WOBS from 128 to 300	11/19/87	mdf	*/
/*
*       Copyright 1999, Caldera Thin Clients, Inc.                      
*       This software is licenced under the GNU Public License.         
*       Please see LICENSE.TXT for further information.                 
*                                                                       
*                  Historical Copyright                                 
*	-------------------------------------------------------------
*	GEM Desktop					  Version 2.3
*	Serial No.  XXXX-0000-654321		  All Rights Reserved
*	Copyright (C) 1987			Digital Research Inc.
*	-------------------------------------------------------------
*/

#define DROOT 1
#define NUM_WNODES 2
#define NUM_WOBS 300
#define NUM_SOBS (NUM_WOBS + NUM_WNODES + 1)

#define WNODE struct windnode

WNODE
{
	WNODE		*w_next;
	WORD		w_flags;
	WORD		w_id;			/* window handle id #	*/
	WORD		w_obid;			/* desktop object id	*/
	WORD		w_root;			/* pseudo root ob. in	*/
						/*   gl_screen for this	*/
						/*   windows objects	*/
	WORD		w_cvcol;		/* current virt. col	*/
	WORD		w_cvrow;		/* current virt. row	*/
	WORD		w_pncol;		/* physical # of cols	*/
	WORD		w_pnrow;		/* physical # of rows	*/
	WORD		w_vncol;		/* virtual # of cols 	*/
	WORD		w_vnrow;		/* virtual # of rows	*/
	PNODE		*w_path;
	BYTE		w_name[LEN_ZPATH];
/*	BYTE		w_info[81];		NOT USED v2.1		*/
};



