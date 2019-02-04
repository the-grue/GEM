/*	CALCIF.C		10/23/84-10/23/84	Andrew Muir	*/

#include <portab.h>
#include <machine.h>
#include <taddr.h>
#include <obdefs.h>
#include <crysbind.h>
#include <cryslib.h>

typedef struct cblk
{
	LONG		cb_pcontrol;
	LONG		cb_pglobal;
	LONG		cb_pintin;
	LONG		cb_pintout;
	LONG		cb_padrin;
	LONG		cb_padrout;		
} CBLK;
						/* in STARTUP.A86	*/
EXTERN WORD		__DOS();
EXTERN WORD		crystal();

GLOBAL CBLK		c;
GLOBAL UWORD		control[C_SIZE];
GLOBAL UWORD		global[G_SIZE];
GLOBAL UWORD		int_in[I_SIZE];
GLOBAL UWORD		int_out[O_SIZE];
GLOBAL LONG		addr_in[AI_SIZE];
GLOBAL LONG		addr_out[AO_SIZE];

GLOBAL WORD		gl_apid;
GLOBAL LONG		ad_c;

GLOBAL WORD	 	gl_handle;

						/* in DOSIF.A86		*/

GLOBAL UWORD	DOS_AX;
GLOBAL UWORD	DOS_BX;
GLOBAL UWORD	DOS_CX;
GLOBAL UWORD	DOS_DX;
GLOBAL UWORD	DOS_SI;
GLOBAL UWORD	DOS_DI;
GLOBAL UWORD	DOS_ES;
GLOBAL UWORD	DOS_DS;
GLOBAL UWORD	DOS_ERR;


	VOID
dos_func(ax, lodsdx, hidsdx)
	UWORD		ax;
	UWORD		lodsdx;
	UWORD		hidsdx;
{
	DOS_AX = ax;
	DOS_DX = lodsdx;
	DOS_DS = hidsdx;

	__DOS();
}

	WORD
dos_gtime()
{
	DOS_AX = 0x2c00;
	__DOS();
	return(DOS_CX);
}

	WORD
dos_gsec()
{
	DOS_AX = 0x2c00;
	__DOS();
	return(DOS_DX);
}


	WORD
dos_gyear()
{
	DOS_AX = 0x2a00;
	__DOS();
	return(DOS_CX);
}

	WORD
dos_gdate()
{
	DOS_AX = 0x2a00;
	__DOS();
	return(DOS_DX);
}

	WORD
dos_syear(year)
	WORD		year;
{

	DOS_DX = dos_gdate();
	if (year > 83)
	  DOS_CX = year + 1900;
         else
	   DOS_CX = year + 2000;
	DOS_AX = 0x2b00;
	__DOS();	    
	DOS_AX &= 0x00ff;
	if (DOS_AX == 0)
	   return (TRUE);
	 else
	    return (FALSE); 
}


	WORD
dos_sdate(date)
	WORD		date;
{	
	WORD		tmp_date;

 	DOS_CX = dos_gyear();
	DOS_DX = dos_gdate();
	tmp_date = date;
	tmp_date &= 0x00ff;
	DOS_DX &= 0xff00;
	DOS_DX = DOS_DX | tmp_date;
	DOS_AX = 0x2b00;
	__DOS();       
	DOS_AX &= 0x00ff;
	if (DOS_AX == 0)
	  return (TRUE);
         else
	   return(FALSE);
}		


	WORD
dos_smonth(date)
	WORD		date;
{	
	WORD		tmp_date;

 	DOS_CX = dos_gyear();
	DOS_DX = dos_gdate();
	tmp_date = date;
	tmp_date <<= 8;
	DOS_DX &= 0x00ff;
	DOS_DX = DOS_DX | tmp_date;
	DOS_AX = 0x2b00;
	__DOS();       
	DOS_AX &= 0x00ff;
	if (DOS_AX == 0)
	  return (TRUE);
         else
	   return(FALSE);
}		


	WORD
dos_shour(the_hour)
	WORD		the_hour;
{	
	WORD		tmp_hour;

	DOS_AX = 0x2c00;
	__DOS();
	tmp_hour = the_hour;
	tmp_hour <<= 8;
	DOS_CX &= 0x00ff;
	DOS_CX = DOS_CX | tmp_hour;
	DOS_AX = 0x2d00;
	__DOS();       
	DOS_AX &= 0x00ff;
	if (DOS_AX == 0)
	  return (TRUE);
         else
	   return(FALSE);
}		


	WORD
dos_smin(the_min)
	WORD		the_min;
{	
	WORD		tmp_min;

	DOS_AX = 0x2c00;
	__DOS();
	tmp_min = the_min;
	tmp_min &= 0x00ff;
	DOS_CX &= 0xff00;
	DOS_CX = DOS_CX | tmp_min;
	DOS_AX = 0x2d00;
	__DOS();       
	DOS_AX &= 0x00ff;
	if (DOS_AX == 0)
	  return (TRUE);
         else
	   return(FALSE);
}		



	WORD
dos_open(pname, access)
	LONG		pname;
	WORD		access;
{
	dos_func(0x3d00 + access, pname);

	return(DOS_AX);
}


	WORD
dos_close(handle)
	WORD		handle;
{
	DOS_AX = 0x3e00;
	DOS_BX = handle;

	__DOS();

	return(!DOS_ERR);
}



	WORD
dos_read(handle, cnt, pbuffer)
	WORD		handle;
	WORD		cnt;
	LONG		pbuffer;
{
	DOS_CX = cnt;
	DOS_BX = handle;
	dos_func(0x3f00, pbuffer);
	return(DOS_AX);
}




	LONG
dos_lseek(handle, smode, sofst)
	WORD		handle;
	WORD		smode;
	LONG		sofst;
{
	DOS_AX = 0x4200;
	DOS_AX += smode;
	DOS_BX = handle;
	DOS_CX = LHIWD(sofst);
	DOS_DX = LLOWD(sofst);

	__DOS();

	return(DOS_AX + HW(DOS_DX) );
}


	WORD
dos_gdrv()
{
	DOS_AX = 0x1900;

	__DOS();
	return(DOS_AX & 0x00ff);
}



	WORD
dos_delete(pname)
	LONG		pname;
{
	dos_func(0x4100, pname);
	return(DOS_AX);
}


	WORD		/* NEW ROUTINE 3/3/86 */
dos_sfirst(pspec, attr)	/* search for first matching file */
	LONG		pspec;
	WORD		attr;
{
	DOS_CX = attr; /* file attributes */

	dos_func(0x4e00, LLOWD(pspec), LHIWD(pspec));
	return(!DOS_ERR);
}


    VOID		/* NEW ROUTINE 3/3/86 */
strcat(pd, ps)
    BYTE	*pd, *ps;
{
	while(*pd)
		pd++;
	while(*pd++ = *ps++)
		;
}


/************************************************************************/
	WORD
r_intersect(x, y, w, h, px, py, pw, ph)
	WORD		x, y, w, h;
	WORD		*px, *py, *pw, *ph;
{
	WORD		tx, ty, tw, th;

	tw = min(*px + *pw, x + w);
	th = min(*py + *ph, y + h);
	tx = max(*px, x);
	ty = max(*py, y);
	*px = tx;
	*py = ty;
	*pw = tw - tx;
	*ph = th - ty;
	return( (tw > tx) && (th > ty) );
}


	WORD
objc_offset(tree, obj, poffx, poffy)
	LONG		tree;
	WORD		obj;
	WORD		*poffx, *poffy;
{
	OB_TREE = tree;
	OB_OBJ = obj;
	crys_if(OBJC_OFFSET);
	*poffx = OB_XOFF;
	*poffy = OB_YOFF;
	return( RET_CODE );
}

	UWORD
inside(x, y, tx, ty, tw, th)
	UWORD		x, y;
	UWORD		tx, ty, tw, th;
{
	if ( (x >= tx) && (y >= ty) &&
	     (x < tx + tw) && (y < ty + th) )
	  return(TRUE);
	else
	  return(FALSE);
} /* inside */


  	WORD
min(a,b)
	WORD		a,b;
{
	return((a < b) ? a:b);
}

  	WORD
max(a,b)
	WORD		a,b;
{
	return((a > b) ? a:b);
}



	WORD
strcpy(ps, pd)
	BYTE		*ps, *pd;
{
	while(*pd++ = *ps++)
	  ;
}


	WORD
crys_if(opcode)
	WORD		opcode;
{
	WORD		i;

	i = opcode * 5;
	c.cb_pcontrol = ADDR( &ctrl_cnts[i] );
	crystal(ad_c);
	return( RET_CODE );
}


	WORD
menu_register(pid,pstr)
	WORD		pid;
	LONG		pstr;
{
	MM_PID = pid;
	MM_PSTR = pstr;
	return(crys_if(MENU_REGISTER));
}


	UWORD
appl_init()
{
	WORD		i;

	c.cb_pcontrol = ADDR(&control[0]); 
	c.cb_pglobal = ADDR(&global[0]);
	c.cb_pintin = ADDR(&int_in[0]);
	c.cb_pintout = ADDR(&int_out[0]);
	c.cb_padrin = ADDR(&addr_in[0]);
	c.cb_padrout = ADDR(&addr_out[0]);
	ad_c = ADDR(&c);
	crys_if(APPL_INIT);
	gl_apid = RET_CODE;
	return( global[10] );
} /* appl_init */


	WORD
appl_write(rwid, length, pbuff)
	WORD		rwid;
	WORD		length;
	LONG		pbuff;
{
	AP_RWID = rwid;
	AP_LENGTH = length;
	AP_PBUFF = pbuff;
	return( crys_if(APPL_WRITE) );
}



	WORD
evnt_timer(locnt, hicnt)
	UWORD		locnt, hicnt;
{
	T_LOCOUNT = locnt;
	T_HICOUNT = hicnt;
	return( crys_if(EVNT_TIMER) );
}


	WORD
evnt_multi(flags, bclk, bmsk, bst, m1flags, m1x, m1y, m1w, m1h, 
		m2flags, m2x, m2y, m2w, m2h, mepbuff,
		tlc, thc, pmx, pmy, pmb, pks, pkr, pbr)
	UWORD		flags, bclk, bmsk, bst;
	UWORD		m1flags, m1x, m1y, m1w, m1h;
	UWORD		m2flags, m2x, m2y, m2w, m2h;
	LONG		mepbuff;
	UWORD		tlc, thc;
	UWORD		*pmx, *pmy, *pmb, *pks, *pkr, *pbr;
{
	MU_FLAGS = flags;

	MB_CLICKS = bclk;
	MB_MASK = bmsk;
	MB_STATE = bst;

	MMO1_FLAGS = m1flags;
	MMO1_X = m1x;
	MMO1_Y = m1y;
	MMO1_WIDTH = m1w;
	MMO1_HEIGHT = m1h;

	MMO2_FLAGS = m2flags;
	MMO2_X = m2x;
	MMO2_Y = m2y;
	MMO2_WIDTH = m2w;
	MMO2_HEIGHT = m2h;

	MME_PBUFF = mepbuff;

	MT_LOCOUNT = tlc;
	MT_HICOUNT = thc;

	crys_if(EVNT_MULTI);

	*pmx = EV_MX;
	*pmy = EV_MY;
	*pmb = EV_MB;
	*pks = EV_KS;
	*pkr = EV_KRET;
	*pbr = EV_BRET;

	return( RET_CODE );
}


	WORD
objc_draw(tree, drawob, depth, xc, yc, wc, hc)
	LONG		tree;
	WORD		drawob, depth;
	WORD		xc, yc, wc, hc;
{
	OB_TREE = tree;
	OB_DRAWOB = drawob;
	OB_DEPTH = depth;
	OB_XCLIP = xc;
	OB_YCLIP = yc;
	OB_WCLIP = wc;
	OB_HCLIP = hc;
	return( crys_if( OBJC_DRAW ) );
}


	WORD
objc_find(tree, startob, depth, mx, my)
	LONG		tree;
	WORD		startob, depth, mx, my;
{
	OB_TREE = tree;
	OB_STARTOB = startob;
	OB_DEPTH = depth;
	OB_MX = mx;
	OB_MY = my;
	return( crys_if( OBJC_FIND ) );
}



	WORD
objc_change(tree, drawob, depth, xc, yc, wc, hc, newstate, redraw)
	LONG		tree;
	WORD		drawob, depth;
	WORD		xc, yc, wc, hc;
	WORD		newstate, redraw;
{
	OB_TREE = tree;
	OB_DRAWOB = drawob;
	OB_DEPTH = depth;
	OB_XCLIP = xc;
	OB_YCLIP = yc;
	OB_WCLIP = wc;
	OB_HCLIP = hc;
	OB_NEWSTATE = newstate;
	OB_REDRAW = redraw;
	return( crys_if( OBJC_CHANGE ) );
}


	WORD
form_alert(defbut, astring)
	WORD		defbut;
	LONG		astring;
{
	FM_DEFBUT = defbut;
	FM_ASTRING = astring;
	return( crys_if( FORM_ALERT ) );
}


	WORD
graf_mouse(m_number, m_addr)
	WORD		m_number;
	LONG		m_addr;
{
	GR_MNUMBER = m_number;
	GR_MADDR = m_addr;
	return(crys_if(GRAF_MOUSE));
}


	WORD
graf_mkstate(pmx, pmy, pmstate, pkstate)
	WORD		*pmx, *pmy, *pmstate, *pkstate;
{
	crys_if( GRAF_MKSTATE );
	*pmx = GR_MX;
	*pmy = GR_MY;
	*pmstate = GR_MSTATE;
	*pkstate = GR_KSTATE;
}


	WORD
graf_handle(pwchar, phchar, pwbox, phbox)
	WORD		*pwchar, *phchar;
	WORD		*pwbox, *phbox;
{
	crys_if(GRAF_HANDLE);
	*pwchar = GR_WCHAR ;
	*phchar = GR_HCHAR;
	*pwbox = GR_WBOX;
	*phbox = GR_HBOX;
	return(RET_CODE);
}


	VOID
graf_growbox(orgx, orgy, orgw, orgh, x, y, w, h)
	WORD		orgx, orgy, orgw, orgh;
	WORD		x, y, w, h;
{
	GR_I1 = orgx;
	GR_I2 = orgy;
	GR_I3 = orgw;
	GR_I4 = orgh;
	GR_I5 = x;
	GR_I6 = y;
	GR_I7 = w;
	GR_I8 = h;
	return( crys_if( GRAF_GROWBOX ) );
} /* graf_growbox */


	VOID
graf_shrinkbox(orgx, orgy, orgw, orgh, x, y, w, h)
	WORD		orgx, orgy, orgw, orgh;
	WORD		x, y, w, h;
{
	GR_I1 = orgx;
	GR_I2 = orgy;
	GR_I3 = orgw;
	GR_I4 = orgh;
	GR_I5 = x;
	GR_I6 = y;
	GR_I7 = w;
	GR_I8 = h;
	return( crys_if( GRAF_SHRINKBOX ) );
} /* graf_shrinkbox */


	VOID
graf_watchbox(tree, obj, instate, outstate)
	LONG		tree;
	WORD		obj;
	UWORD		instate, outstate;
{
	GR_TREE = tree;
	GR_OBJ = obj;
	GR_INSTATE = instate;
	GR_OUTSTATE = outstate;
	return( crys_if( GRAF_WATCHBOX ) );
} /* graf_watchbox */


	VOID
graf_rubbox(xorigin, yorigin, wmin, hmin, pwend, phend)
	WORD		xorigin, yorigin;
	WORD		wmin, hmin;
	WORD		*pwend, *phend;
{
	GR_I1 = xorigin;
	GR_I2 = yorigin;
	GR_I3 = wmin;
	GR_I4 = hmin;
	crys_if( GRAF_RUBBOX );
	*pwend = GR_O1;
	*phend = GR_O2;
	return( RET_CODE );
}


	WORD
fsel_input(pipath, pisel, pbutton)
	LONG		pipath, pisel;
	WORD		*pbutton;
{
	FS_IPATH = pipath;
	FS_ISEL = pisel;
	crys_if( FSEL_INPUT );
	*pbutton = FS_BUTTON;
	return( RET_CODE );
}


					/* Window Manager		*/
	WORD
wind_create(kind, wx, wy, ww, wh)
	UWORD		kind;
	WORD		wx, wy, ww, wh;
{
	WM_KIND = kind;
	WM_WX = wx;
	WM_WY = wy;
	WM_WW = ww;
	WM_WH = wh;
	return( crys_if( WIND_CREATE ) );
}


	WORD
wind_open(handle, wx, wy, ww, wh)
	WORD		handle;
	WORD		wx, wy, ww, wh;
{
	WM_HANDLE = handle;
	WM_WX = wx;
	WM_WY = wy;
	WM_WW = ww;
	WM_WH = wh;
	return( crys_if( WIND_OPEN ) );
}


	WORD
wind_close(handle)
	WORD		handle;
{
	WM_HANDLE = handle;
	return( crys_if( WIND_CLOSE ) );
}


	WORD
wind_delete(handle)
	WORD		handle;
{
	WM_HANDLE = handle;
	return( crys_if( WIND_DELETE ) );
}


	WORD
wind_get(w_handle, w_field, pw1, pw2, pw3, pw4)
	WORD		w_handle;
	WORD		w_field;
	WORD		*pw1, *pw2, *pw3, *pw4;
{
	WM_HANDLE = w_handle;
	WM_WFIELD = w_field;
	crys_if( WIND_GET );
	*pw1 = WM_OX;
	*pw2 = WM_OY;
	*pw3 = WM_OW;
	*pw4 = WM_OH;
	return( RET_CODE );
}



	WORD
wind_set(w_handle, w_field, w2, w3, w4, w5)
	WORD		w_handle;	
	WORD		w_field;
	WORD		w2, w3, w4, w5;
{
	WM_HANDLE = w_handle;
	WM_WFIELD = w_field;
	WM_IX = w2;
	WM_IY = w3;
	WM_IW = w4;
	WM_IH = w5;
	return( crys_if( WIND_SET ) );
}


	WORD
wind_find(mx, my)
	WORD		mx, my;
{
	WM_MX = mx;
	WM_MY = my;
	return( crys_if( WIND_FIND ) );
}


	WORD
wind_update(beg_update)
	WORD		beg_update;
{
	WM_BEGUP = beg_update;
	return( crys_if( WIND_UPDATE ) );
}

	WORD
wind_calc(wctype, kind, x, y, w, h, px, py, pw, ph)
	WORD		wctype;
	UWORD		kind;
	WORD		x, y, w, h;
	WORD		*px, *py, *pw, *ph;
{
	WM_WCTYPE = wctype;
	WM_WCKIND = kind;
	WM_WCIX = x;
	WM_WCIY = y;
	WM_WCIW = w;
	WM_WCIH = h;
	crys_if( WIND_CALC );
	*px = WM_WCOX;
	*py = WM_WCOY;
	*pw = WM_WCOW;
	*ph = WM_WCOH;
	return( RET_CODE );
}

	WORD
rsrc_obfix(tree, obj)
	LONG		tree;
	WORD		obj;
{
	RS_TREE = tree;
	RS_OBJ = obj;
	return( crys_if(RSRC_OBFIX) );
}

