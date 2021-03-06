/*	SPOL.C		08/03/84 - 06/24/85	Lee Lorenzen		*/
/*	added printer # and text flag	11/18/87	mdf		*/

/*** INCLUDE FILES ******************************************************/

#include <portab.h>
#include <machine.h>
#include <obdefs.h>
#include <taddr.h>
#include <crysbind.h>
#include <spol.h>

/*** DEFINES ***********************************************************/

#define SPLSIZE 2048
#define NUM_SPOL  12
#define SPOL_SND 8

/*** GLOBAL VARIABLES ***************************************************/
EXTERN WORD	gl_apid;
EXTERN WORD	color;

GLOBAL BYTE	spol_bufr[SPLSIZE];
GLOBAL UWORD	spol_cntr;
GLOBAL BYTE	*spol_ptr;
GLOBAL LONG	spol_fcnt,spol_path,spol_pbuf;
GLOBAL WORD	spol_sts;
GLOBAL WORD	spol_texp;
GLOBAL WORD	spol_tcnt;
GLOBAL WORD	spol_prn;

GLOBAL BYTE	gl_fspol[82];
GLOBAL BYTE	gl_fspec[68];
GLOBAL BYTE	gl_fname[14];

GLOBAL LONG	ad_spol;
GLOBAL WORD	wh_spol;
GLOBAL WORD	gl_lastch;
GLOBAL WORD	gl_itspol;	
GLOBAL WORD	gl_inspol;
GLOBAL WORD	gl_spnxt;
GLOBAL BYTE	*gl_sppfn[12];
GLOBAL WORD	gl_spdel[12];
GLOBAL WORD	gl_sptab[12];
GLOBAL WORD	gl_spcnt[12];
GLOBAL WORD	gl_spprn[12];
GLOBAL BYTE	*gl_splst[12] = 
{
"01_\1234567890123456789012345678901234567890123456789012345678901234567890",
"02_\1234567890123456789012345678901234567890123456789012345678901234567890",
"03_\1234567890123456789012345678901234567890123456789012345678901234567890",
"04_\1234567890123456789012345678901234567890123456789012345678901234567890",
"05_\1234567890123456789012345678901234567890123456789012345678901234567890",
"06_\1234567890123456789012345678901234567890123456789012345678901234567890",
"07_\1234567890123456789012345678901234567890123456789012345678901234567890",
"08_\1234567890123456789012345678901234567890123456789012345678901234567890",
"09_\1234567890123456789012345678901234567890123456789012345678901234567890",
"10_\1234567890123456789012345678901234567890123456789012345678901234567890",
"11_\1234567890123456789012345678901234567890123456789012345678901234567890",
"12_\1234567890123456789012345678901234567890123456789012345678901234567890",
};


	VOID
spol_file( fname, tabexp, prntr)
	BYTE		*fname;
	WORD		tabexp;
	WORD		prntr;
{
	spol_path = ADDR(fname);
	spol_pbuf = ADDR( spol_bufr );
	spol_prn = prntr;
	spol_sts = FALSE;
	spol_cntr = 0;
	spol_fcnt = 0;
	spol_texp = tabexp;
	spol_tcnt = 0;
	spol_gblk();	
}


	VOID
spol_gblk()
{
	BYTE    handle;

	handle = dos_open( spol_path, 0x0000 );
	if (handle)
	{
	  dos_lseek( handle, 0x0000, spol_fcnt );
	  spol_cntr = dos_read( handle, SPLSIZE, spol_pbuf );
	  if ( spol_cntr != SPLSIZE )
	      spol_sts = TRUE;
 	  spol_fcnt += LW( spol_cntr );
	  dos_close( handle );
	  spol_ptr = spol_bufr;
	}
	else
	{
	  spol_sts = TRUE;
	  spol_cntr = 0;
	}
}


	WORD
spol_tab( thechar )
	WORD		thechar;
{
	WORD		out;

	if (spol_texp)
	{
	  if ( thechar == 0x09 )
	  {
	    out = 8 - (spol_tcnt & 0x0007);
	    if ( spol_out( gl_lastch = ' ' ) )
	    {
	      spol_tcnt++;
	      out--;
	      return( !out );
	    }
	    return(FALSE);
	  }
	  else
	  {
	    if ( spol_out( gl_lastch = thechar) )
	    {
	      if ( thechar == 0x0D )
	        spol_tcnt = 0;
	      if (thechar >= ' ')
	        spol_tcnt++;
	      return( TRUE );
	    }
	    return( FALSE );
	  }
	}
	else
	  return( spol_out( gl_lastch = thechar) );
}


	WORD
spol_doit()
{
	WORD		i;

	if ( spol_cntr )
	{
	  for (i=0; i<SPOL_SND; i++)
	  {
	    if ( spol_tab( *spol_ptr ) )
	    {
	      spol_ptr++;
	      spol_cntr--;
	    }
	    else
	      break;
	    if (!spol_cntr)
	      break;
	  }
	}
	else
	{
	  if ( spol_sts )
	    return( FALSE );
	  else
	    spol_gblk();
	}
	return( TRUE );
}


/*
*	Initializes the spooler. Assigns the ted infos to the proper objects
*	and the strings to the proper ted infos.
*/
	LONG
ini_spol()
{
	LONG		tree;
	WORD		i, j;

	tree = ADDR(&the_spol[0]);

	j = 0;
	for (i=0; i < SPOL_OBS; i++)
	{
	  rsrc_obfix(tree, i);
	  if (the_spol[i].ob_type == G_STRING)
	  {
	    the_spol[i].ob_spec = ADDR(spol_str[j]);
	    *spol_str[j] = NULL;
	    j++;
	  }
	  if (the_spol[i].ob_type == G_BUTTON) 
	  {
	    the_spol[i].ob_spec = ADDR(spol_str[j]);
	    j++;
	  }
	}

	if (color == 2)
	{	
	  the_spol[0].ob_spec = 0x0173L;	
	}  
	else
 	  if (color >= 3)
	  {	
	    the_spol[0].ob_spec = 0x0175L;	
	  }  

	for (i=0; i<NUM_SPOL; i++)
	{
	  gl_sppfn[i] = NULLPTR;
	  *gl_splst[i] = NULL;
	  gl_spdel[i] = FALSE;
	  gl_sptab[i] = FALSE;
	}
	gl_spnxt = 0;

	the_spol[ADDNAME].ob_state = NORMAL;
	the_spol[REMNAME].ob_state = DISABLED;

 	strcpy("A:\*.*", &gl_fspec[0]);
	gl_fspec[0] += dos_gdrv();

	spol_setup();
	return(tree);
}


spol_setup()
{
	WORD		i, j;
	BYTE		*pname, *pdst;

	j = 0;
	for (i=0; i < SPOL_OBS; i++)
	{
	  if (the_spol[i].ob_type == G_STRING)
	  {
	    pname = gl_sppfn[j];
	    if (!pname)
	      pname = "";
	    if (*pname)
	    {
	      while(*pname)
	        pname++;
	      pname--;
	      while( (*pname) &&
		     (*pname != '\\') &&
		     (*pname != ':') )
	       pname--;
	     if (*pname)
	       pname++;
	    }	
	    pdst = spol_str[j];
	    if (*pname)
	    {
	      *pdst++ = ' ';
	      while( (*pname) && 
		     (*pname != '.') )
	        *pdst++ = *pname++;
	      while ( (pdst - spol_str[j]) < 9 )
		*pdst++ = ' ';
	      while(*pname)
	        *pdst++ = *pname++;
	      while ( (pdst - spol_str[j]) < 14 )
		*pdst++ = ' ';
	    }
	    *pdst = NULL;
	    j++;
	  }
	}
}


spol_ansr(towhom)
	WORD		towhom;
{
	WORD		ap_msg[8];

 	ap_msg[0] = 101;
	ap_msg[1] = gl_apid;
	ap_msg[2] = 0;
	ap_msg[3] = 0;
	ap_msg[4] = 0;
	ap_msg[5] = 0;
	ap_msg[6] = 0;
	ap_msg[7] = 0;
	appl_write(towhom, 16, ADDR(&ap_msg[0]));
}


spol_draw(obj)
	WORD		obj;
{
	WORD		ap_msg[8];

	spol_setup();

	if (wh_spol)
	{
 	  ap_msg[0] = WM_REDRAW;
	  ap_msg[1] = gl_apid;
	  ap_msg[2] = 0;
	  ap_msg[3] = wh_spol;
	  objc_offset(ad_spol, obj, &ap_msg[4], &ap_msg[5]);
	  ap_msg[6] = the_spol[obj].ob_width;
	  ap_msg[7] = the_spol[obj].ob_height;
	  appl_write(gl_apid, 16, ADDR(&ap_msg[0]));
	}
}


	BYTE
*spol_alloc()
{
	WORD		i;

	for (i=0; i<NUM_SPOL; i++)
	{
	  if (*gl_splst[i] == NULL)
	    return(gl_splst[i]);
	}
	return(0);
}


spol_free(pstr)
	BYTE		*pstr;
{
	*pstr = NULL;
}

spol_bttn()
{

	if ( (gl_spnxt < NUM_SPOL) &&
	     (the_spol[ADDNAME].ob_state != NORMAL) )
	{
	  the_spol[ADDNAME].ob_state = NORMAL;
	  spol_draw(ADDNAME);
	}

	if ( (gl_spnxt == 0) &&
	     (the_spol[REMNAME].ob_state != DISABLED) )
	{
	  the_spol[REMNAME].ob_state = DISABLED;
	  spol_draw(REMNAME);
	}

	if ( (gl_spnxt == NUM_SPOL) &&
	     (the_spol[ADDNAME].ob_state != DISABLED) )
	{
	  the_spol[ADDNAME].ob_state = DISABLED;
	  spol_draw(ADDNAME);
	}

	if ( (gl_spnxt) &&
	     (the_spol[REMNAME].ob_state != NORMAL) )
	{
	  the_spol[REMNAME].ob_state = NORMAL;
	  spol_draw(REMNAME);
	}
}


spol_form()
{
	WORD		i;

	if ( gl_lastch != 0x0C )
	{
	  for (i=0; i<8; i++)
	  {
	    if ( spol_out( gl_lastch = 0x0C ) )
	      break;
	  }
	}
}



spol_remv(start)
	WORD		start;
{
	WORD		i;

	gl_spnxt--;

	if (gl_spdel[start])
	  dos_delete(ADDR(gl_sppfn[start]));

	spol_free(gl_sppfn[start]);

	if ( (start == 0) &&
	     (gl_sptab[0]) )
	  spol_form();


	for (i=start; i<(NUM_SPOL - 1); i++)
	{
	  gl_sppfn[i] = gl_sppfn[i+1];
	  gl_spdel[i] = gl_spdel[i+1];
	  gl_sptab[i] = gl_sptab[i+1];
	  gl_spcnt[i] = gl_spcnt[i+1];
	  gl_spprn[i] = gl_spprn[i+1];
	  the_spol[i + F1NAME].ob_state = the_spol[i + F1NAME + 1].ob_state;
	}
	gl_sppfn[i] = NULLPTR;
	gl_spdel[i] = FALSE;
	gl_sptab[i] = FALSE;
	gl_spcnt[i] = 0;
	gl_spprn[i] = 0;
	the_spol[i + F1NAME].ob_state = NORMAL;

	spol_bttn();
}


spol_more()
{
	WORD		i;

	gl_spcnt[0] -= 1;

	if (gl_spcnt[0])
	{
	  spol_file( gl_sppfn[0], gl_sptab[0], gl_spprn[0] );
	}
	else
	{
	  spol_remv(0);

	  if (gl_spnxt)
	    spol_file(gl_sppfn[0], gl_sptab[0], gl_spprn[0]);
	}

	spol_draw(SPOLBOX);
}


spol_chg(curr_ob, newstate)
	WORD		curr_ob;
	WORD		newstate;
{
	WORD		x, y, w, h;
	OBJECT		*pobs;

	objc_offset(ad_spol, curr_ob, &x, &y);
	w = the_spol[curr_ob].ob_width;
	h = the_spol[curr_ob].ob_height;
	objc_change(ad_spol, curr_ob, 0, x, y, w, h, newstate, TRUE);
}

	WORD
istext(pname)
	BYTE		*pname;
{
	BYTE		*pstr;
	BYTE		ptemp[4];
	WORD		i;

	pstr = pname;
	while( *pstr )
	  pstr++;
	while( (*pstr != '.') &&
	       (*pstr != '\\') &&
	       (pstr > pname) )
	  pstr--;
	if (*pstr == '.')
	{
	  pstr++;
	  for (i=0; i<3; i++)
	    ptemp[i] = toupper(*pstr++);
	  ptemp[3] = NULL;
	  if ( strcmp(&ptemp[0], "GEM") ||
	       strcmp(&ptemp[0], "IMG") ||
	       strcmp(&ptemp[0], "GMP") ||
	       strcmp(&ptemp[0], "OUT") )
	    return(FALSE);
	}
	return(TRUE);
}


hndl_spmsg(len, lpath, cnt, delit, prntr)
	WORD		len;
	LONG		lpath;	
	WORD		cnt;
	WORD		delit;
	WORD		prntr;
{
	BYTE		*pslot;
	WORD		newprn;

	pslot = spol_alloc();

	if (pslot)
	{
	  switch (prntr & 0xFF)
	  {
	    case 0:
	    case 1:
	    case 2:
	      newprn = prntr;
	      break;
	    default:
	      newprn = 0;				/* LPT1		*/
	  }
  	  LBCOPY(ADDR(pslot), lpath, len + 1);
	  gl_sppfn[gl_spnxt] = pslot;
	  gl_spdel[gl_spnxt] = delit;
	  gl_sptab[gl_spnxt] = ((prntr & 0x0100) == 0);
	  gl_spcnt[gl_spnxt] = cnt;
	  gl_spprn[gl_spnxt] = newprn;
	  if (!gl_spnxt)
	    spol_file( pslot, gl_sptab[gl_spnxt], gl_spprn[gl_spnxt] );
	  gl_spnxt++;
	}

	spol_bttn();

	spol_draw(SPOLBOX);
}


act_allchg(ex_obj, pt, chgvalue, dochg)
	WORD		ex_obj;
	GRECT		*pt;
	WORD		chgvalue, dochg;
{
	WORD		obj, newstate;
	GRECT		o;

	for(obj=F1NAME; obj!=SPOLBOX; obj=the_spol[obj].ob_next) 
	{
	  if ( (obj != ex_obj) &&
	       (gl_sppfn[obj - F1NAME] != NULLPTR)  )
	  {
	    objc_offset(ad_spol, obj, &o.g_x, &o.g_y);
	    o.g_w = the_spol[obj].ob_width;
	    o.g_h = the_spol[obj].ob_height;
	    if ( r_intersect(pt->g_x, pt->g_y, pt->g_w, pt->g_h, 
				&o.g_x, &o.g_y, &o.g_w, &o.g_h) )  
	    {
						/* make change		*/
	      newstate = the_spol[obj].ob_state;
	      if ( dochg )
		newstate |= chgvalue;
	      else
		newstate &= ~chgvalue;
	      if (newstate != the_spol[obj].ob_state)
		spol_chg(obj, newstate);
	    }
	  }
	}
}


spol_down(mx, my)
	WORD		mx, my;
{
	GRECT		m;
	WORD		curr_ob;

	curr_ob = objc_find(ad_spol, ROOT, MAX_DEPTH, mx, my);

	if ( (curr_ob == ROOT) ||
	     (curr_ob == SPOLBOX) )
	{
	  m.g_x = mx;
	  m.g_y = my;
	  graf_rubbox(mx, my, 6, 6, &m.g_w, &m.g_h);
	  act_allchg(curr_ob, &m, SELECTED, TRUE);
	}
}

	VOID
hndl_spsel(mx, my, mb, ks)
	WORD		mx, my;
	WORD		mb;
	WORD		ks;
{
	WORD		curr_ob, x, y, w, h;
	WORD		state, shifted;
	OBJECT		*pobs;
	LONG		tree;
	GRECT		full;


	tree = ad_spol;

	
	shifted = (ks & K_LSHIFT) || (ks & K_RSHIFT);
	curr_ob = objc_find(tree, ROOT, MAX_DEPTH, mx, my);
	if ( (curr_ob >= F1NAME) &&
	     (curr_ob <= F12NAME) &&
	     (gl_sppfn[curr_ob - F1NAME] != NULLPTR)  )
	{
 	  state = the_spol[curr_ob].ob_state;
	  if ( !shifted )
	  {
	    if ( !(state & SELECTED) )
	    {
	      act_allchg(curr_ob, &the_spol[0].ob_x, SELECTED, FALSE);
	      state |= SELECTED;
	    }
	  }
	  else
	  {
	    if (state & SELECTED)
	      state &= ~SELECTED;
	    else
	      state |= SELECTED;
	  }  
 	  if (state != the_spol[curr_ob].ob_state )
	    spol_chg(curr_ob, state);
	}
	else
	{
	  act_allchg(curr_ob, &the_spol[0].ob_x, SELECTED, FALSE);
	}

	graf_mkstate(&mx, &my, &mb, &ks);
	if (mb & 0x0001)
	  spol_down(mx, my);
}



splt_dir(dir_wc,dir,wc)		/* NEW ROUTINE 3/3/86 */
	BYTE *dir_wc;
	BYTE *dir, *wc;
{
	WORD length;
	BYTE *i;

	i = dir_wc + strlen(dir_wc);
	while ( ( *i != '\\') && ( *i != ':') )
	{
	    i--;
	    if( i < dir_wc)
	    {
		break;
	    }
	}
	i++;

	if(strlen(i))
	{
	    strcpy( i, wc );
	    *i = NULL;
	}
	if(strlen(dir_wc))
	{
	    strcpy( dir_wc, dir );
	}
} /* splt_dir */

	WORD
file_exists( dir, name )		/* NEW ROUTINE 3/3/86 */
	BYTE *dir;
	BYTE *name;
{
	BYTE f_name[80];

	strcpy( dir, f_name );
	strcat( f_name, name );
	return( dos_sfirst( ADDR(f_name), 0 ) );
}

	VOID
hndl_spbut(ob)
	WORD		ob;
{
	BYTE		*pstr;
	WORD		isok;
	WORD		i, j, k, remvd_1st, isgrafic;
	WORD		exists;		/* NEW LINE 3/3/86 */
	BYTE		dir[ 80 ];		/* NEW LINE 3/3/86 */
	BYTE		new_wc[ 14 ];	/* NEW LINE 3/3/86 */
	BYTE		new_file[ 14 ];	/* NEW LINE 3/3/86 */
	BYTE		dir_wc[ 80 ];	/* NEW LINE 3/3/86 */
	

	if ( ob == ADDNAME ) 
	{
	  strcpy("", &gl_fname[0]);
	  fsel_input(ADDR(&gl_fspec[0]), ADDR(&gl_fname[0]), &isok); 
	  strcpy( gl_fspec, dir_wc );		/* NEW LINE 3/3/86 */
	  strcpy( gl_fname, new_file );		/* NEW LINE 3/3/86 */
	  splt_dir( dir_wc, dir, new_wc );	/* NEW LINE 3/3/86 */		  
	  exists = file_exists( dir, new_file );	/* NEW LINE 3/3/86 */
	  if ( exists )		/* NEW LINE 3/3/86 */
	  {
	    if ( (isok) &&
	         (gl_fname[0]) &&
	         (gl_fspec[0]) )
	    {
	      strcpy(&gl_fspec[0], &gl_fspol[0]);
	      pstr = &gl_fspol[0];
	      while (*pstr)
	        pstr++;
	      pstr--;
	      while ( (*pstr) &&
		      (*pstr != '\\') &&
		      (*pstr != ':') )
	        pstr--;
	      if (*pstr)
	        pstr++;
	      strcpy(&gl_fname[0], pstr);
	      if ( istext( &gl_fname[0] ) )
	        hndl_spmsg(strlen(&gl_fspol[0]), 
				ADDR(&gl_fspol[0]), 1, FALSE, 0);
	      else
	      {
	        form_alert(1, 
ADDR("[1][The Print Spooler is only able to|\
directly print text files.  If you wish|\
to print a graphic file in background,|\
go to the OUTPUT application, and select|\
the Print in Background option.][  OK  ]") );
	      }
	    }
	  }
	  else
	  {
	    if ( isok )
	        form_alert(1, 
ADDR("[1][I am sorry, but I am unable|\
to find the file requested.][  OK  ]") );
	  }
	}
	if ( ob == REMNAME )
	{
	  j = 0;
	  remvd_1st = isgrafic = FALSE;
	  i = F1NAME;
	  while( i != SPOLBOX )
	  {
	    if (the_spol[i].ob_state & SELECTED)
	    {
	      the_spol[i].ob_state = NORMAL;
	      k = i - F1NAME;
	      if (k == 0)
	      {
		remvd_1st = TRUE;
	        isgrafic = !gl_sptab[0];
	      }
	      spol_remv(k);
	      j++;
	    }
	    else
	      i = the_spol[i].ob_next;
	  }
	  if (j)
	  {
	    if (remvd_1st)
	    {
	      spol_sts = TRUE;
	      spol_cntr = 0;
	      if ( isgrafic )
	        spol_form();
	      if (gl_spnxt)
	        spol_file(gl_sppfn[0], gl_sptab[0], gl_spprn[0]);
	    }

	    spol_draw(SPOLBOX);
	  }
	}
}


