
/*************************************************************************
**       Copyright 1999, Caldera Thin Clients, Inc.                     ** 
**       This software is licenced under the GNU Public License.        **
**       Please see LICENSE.TXT for further information.                ** 
**                                                                      ** 
**                  Historical Copyright                                ** 
**									**
**									**
**									**
**  Copyright (c) 1987, Digital Research, Inc. All Rights Reserved.	**
**  The Software Code contained in this listing is proprietary to	**
**  Digital Research, Inc., Monterey, California and is covered by U.S.	**
**  and other copyright protection.  Unauthorized copying, adaptation,	**
**  distribution, use or display is prohibited and may be subject to 	**
**  civil and criminal penalties.  Disclosure to others is prohibited.	**
**  For the terms and conditions of software code use refer to the 	**
**  appropriate Digital Research License Agreement.			**
**									**
*************************************************************************/

#include "portab.h"
#include "gsxdef.h"
#include "fontdef.h" 
#include "gsxextrn.h"

#define range(v, l, h, d) ( ( ((v < l) || (v > h))  ? d : v) )

EXTERN WORD ftmgroff;
EXTERN WORD ftmgrseg;

EXTERN WORD FLIP_Y;
EXTERN WORD XFM_MODE;
EXTERN WORD WRT_MODE;
EXTERN WORD SIZ_TAB[];
EXTERN WORD DEV_TAB[];
EXTERN WORD X1, X2, Y1, Y2;
EXTERN WORD FG_BP_1;
EXTERN WORD buff_seg, def_buf, sav_buf1, sav_buf2, txbuf1, txbuf2;

EXTERN WORD DELY;		/* width and height of character unscaled */
EXTERN UWORD *FOFF;
EXTERN WORD FWIDTH;		/* offset,segment and form with of font	*/
EXTERN UWORD *OFF_TBL;
EXTERN WORD FIR_CHR;

WORD	T_SCLSTS;
WORD	MONO_STATUS;
WORD	DESTX, DESTY;		/* upper left of destination on screen  */
WORD	ACTDELY;		/* actual height of character scaled*/
WORD	CHR_HT;			/* character top + bottom + 1*/
WORD	SPECIAL;		/* special effects flag word		*/
WORD	WEIGHT;			/* amount to thicken character		*/
WORD	R_OFF, L_OFF;		/* skew above and below baseline	*/
WORD	CHAR_DEL;		/* additional char width		*/
WORD    TEXT_BP;		/* character color			*/
UWORD	XACC_DDA;		/* accumulator for X DDA		*/
UWORD	DDA_INC;		/* the fraction to be added to the DDA  */
WORD	text_color;		/* requested color			*/
WORD	rot_case;		/* rotation case (0 - 3)		*/
WORD	h_align, v_align;	/* alignment				*/
WORD	width,height;		/* extent of string set in dqt_extent   */
GLOBAL BOOLEAN	rq_type;
GLOBAL BYTE	rq_font;
GLOBAL BYTE	rq_attr;
GLOBAL WORD	rq_size;
GLOBAL WORD	dbl;
	WORD	loaded;
static WORD ini_font_count;	/* default number of fonts		*/
#define	FHEAD struct font_head
struct font_head *cur_font,*act_font;	/* font head pointer */
struct font_head *cur_head;		/* font header copy pointer */
EXTERN struct font_head FONT_INF;	/* header for current font */
EXTERN struct font_head first;		/* font head pointer */
struct font_head *font_top = &first;

d_gtext()
{		 	   
	BOOLEAN	need_extent;
	WORD	delh, delv, rdel1, rdel2;
	WORD	sav_bp;

#if EGA
	EGA_KLUG();
#endif

	if ( CONTRL[3] == 0 )
	    return;
    
	/* Take horizontal alignment into account. */

        need_extent = (CONTRL[0] != 11);
        if (h_align == 0)
	    delh = 0;
	else
	{
	    if (need_extent)
	        dqt_extent();
	    need_extent = FALSE;
	    delh = h_align == 1 ? width >> 1 : width;
	    CONTRL[2] = 0;
	}  /* End else:  center or right alignment. */
	X1 = 1;
	switch (v_align)
	{
	case 0:
            delv = 0;
            delh += L_OFF;
	    break;
	case 1:
            delv = FONT_INF.half;
	    if ( FONT_INF.top )
		X1 = FONT_INF.top;
	    delh += L_OFF + (FONT_INF.half * R_OFF) / X1;
	    break;
	case 2:
            delv = FONT_INF.ascent;
	    if( FONT_INF.top )
		X1 = FONT_INF.top;
	    delh += L_OFF + (FONT_INF.ascent * R_OFF) / X1;
	    break;
	case 3:
            delv = -FONT_INF.bottom;
	    if( FONT_INF.bottom )
		X1 = FONT_INF.bottom;
	    delh -= ((FONT_INF.bottom - FONT_INF.descent) * L_OFF) / X1;
	    break;
	case 4:
            delv = -FONT_INF.descent;
	    break;
	case 5:
            delv = FONT_INF.top;
            delh += L_OFF + R_OFF;
	    break;
	}

	rdel1 = FONT_INF.top - delv;
	rdel2 = (ACTDELY - FONT_INF.top - 1) + delv;
	switch (rot_case)
	{
	case 0:
	    DESTX = PTSIN[0] - delh;
	    DESTY = PTSIN[1] - rdel1;
	    break;
	case 1:
	    DESTX = PTSIN[0] - rdel1;
	    DESTY = PTSIN[1] + delh;
	    break;
	case 2:
	    DESTX = PTSIN[0] + delh;
	    DESTY = PTSIN[1] - rdel2;
	    break;
	case 3:
	    DESTX = PTSIN[0] - rdel2;
	    DESTY = PTSIN[1] - delh;
	    break;
	}

	if (SPECIAL != 0 || !MONO_STATUS || !MONO8XHT())
	{
	    if ( (SPECIAL & SKEW) && (WRT_MODE == 0 || WRT_MODE == 3) )
	    {
		if (need_extent)
		    dqt_extent();
	        need_extent = FALSE;
		switch (rot_case)
		{
		case 0:
		    X1 = DESTX;
		    X2 = X1 + width;
		    Y2 = Y1 = DESTY + ACTDELY - 1;
		    break;
		case 1:
		    Y2 = DESTY;
		    Y1 = Y2 - width;
		    X1 = X2 = DESTX + ACTDELY - 1;
		    break;
		case 2:
		    X2 = DESTX;
		    X1 = X2 - width;
		    Y2 = Y1 = DESTY;
		    break;
		case 3:
		    Y1 = DESTY;
		    Y2 = Y1 + width;
		    X1 = X2 = DESTX;
		    break;
	        }

		clr_skew();
#if EGA
		EGA_KLUG();
#endif
	    }  /* End if:  skewed and replace or inverse transparent mode. */
	    if (SPECIAL & UNDER)
	    {
		if (need_extent)
		    dqt_extent();
		switch (rot_case)
		{
		case 0:
		    X1 = DESTX + L_OFF;
		    X2 = X1 + width;
		    Y1 = DESTY + FONT_INF.top + 1;
		    Y2 = Y1 + FONT_INF.ul_size - 1;
		    break;
		case 1:
		    X1 = DESTX + FONT_INF.top + 1;
		    X2 = X1 + FONT_INF.ul_size - 1;
		    Y2 = DESTY - L_OFF;
		    Y1 = Y2 - width;
		    break;
		case 2:
		    X2 = DESTX - L_OFF;
		    X1 = X2 - width;
		    Y2 = DESTY + ACTDELY - FONT_INF.top - 1;
		    Y1 = Y2 - FONT_INF.ul_size + 1;
		    break;
		case 3:
		    X2 = DESTX + ACTDELY - FONT_INF.top - 1;
		    X1 = X2 - FONT_INF.ul_size + 1;
		    Y1 = DESTY + L_OFF;
		    Y2 = Y1 + width;
		    break;
	        }
	    }  /* End if:  underlined. */
	    XACC_DDA = 32767;	/* init the horizontal dda */
	    sav_bp = TEXT_BP;
	    TEXT_BLT();
	    TEXT_BP = sav_bp;
	}
} 

text_init()
{
    WORD new_width;
    WORD id_save;
    WORD temp_max, temp_top;
    struct font_head *def_save;		/* font head pointer */
    union
    {
      WORD wd[2];
      WORD *pt;
    } scratch;

    cur_head = &FONT_INF;

    /* Set the default text scratch buffer addresses. */
    scratch.pt = &def_buf;
    v_nop();  /* Don't remove this -- Lattice optimization will kill you! */
    buff_seg = scratch.wd[1];
    txbuf1 = sav_buf1;
    txbuf2 = sav_buf2;

    SIZ_TAB[0] = SIZ_TAB[1] = 32767;
    SIZ_TAB[2] = SIZ_TAB[3] = 0;
	
    cur_font = font_top;
    id_save = cur_font->font_id;

    for ( DEV_TAB[5] = 0, ini_font_count = 1 ; TRUE ;
          cur_font = cur_font->next_font )
    {
        if ( cur_font->flags & DEFAULT )
            def_save = cur_font;
	if ( cur_font->font_id != id_save )
        {   
	    ini_font_count++;
	    id_save = cur_font->font_id;
	}
	if (cur_font->font_id == 1)
	{
	    temp_max = cur_font->max_char_width;
	    temp_top = cur_font->top + 1;
	    if ( SIZ_TAB[0] > temp_max )
	        SIZ_TAB[0] = temp_max;
	    if ( SIZ_TAB[1] > temp_top )
		SIZ_TAB[1] = temp_top;
	    if ( SIZ_TAB[2] < temp_max )
		SIZ_TAB[2] = temp_max; 
	    if ( SIZ_TAB[3] < temp_top )
		SIZ_TAB[3] = temp_top;
	    DEV_TAB[5]++;    		/* number of sizes */
 	}

	if (cur_font->next_font == 0)
	    break;
    }

    DEV_TAB[10] = ini_font_count;	/* number of faces */

    act_font = cur_font = def_save;
    cpy_head();

    TEXT_BP = MAP_COL[text_color = range(INTIN[6], 0, MAX_COLOR - 1, 1)];
    SPECIAL = rot_case = h_align = v_align = 0;
    in_rot();
    in_doub();	

    /* rq_size was set to 1 in the original source from Ventura.
       GEM WRITE chose the 8 point system font.  
       Sel_Size searched for a system font 1 pixel high as it's
       default.
       rq_size is now set for a 10 point system font as the default
       jn 9-15-87 */

    rq_font = 1;				/* jn 9-15-87 */
    rq_size = 10 ;				/* jn 9-15-87 */
    rq_attr = 0;
    dbl = 0;
    rq_type = TRUE;
    loaded = FALSE;
    CHAR_DEL = WEIGHT = R_OFF = L_OFF = MONO_STATUS = 0;	

    CHR_HT = FONT_INF.top + FONT_INF.bottom + 1;
    ACTDELY = FONT_INF.form_height;
}


dst_height()
{
    if (XFM_MODE == 0)
        PTSIN[1] = (DEV_TAB[1] + 1) - PTSIN[1];
    rq_size = PTSIN[1];
    if( rq_size == 0 )
	rq_size = 1;		/* it must be at least one pel high */
    rq_type = FALSE;	
    sel_font();	

    PTSOUT[0] = FONT_INF.max_char_width;
    PTSOUT[1] = FONT_INF.top+1;
    PTSOUT[2] = FONT_INF.max_cell_width;
    PTSOUT[3] = CHR_HT;	
    inc_lfu();
    CONTRL[2] = 2;
    FLIP_Y = 1;
} 

dst_point()
{
    rq_size = INTIN[0];
    rq_type = TRUE;
    sel_font();
    CONTRL[4] = 1;
    CONTRL[2] = 2;
    INTOUT[0] = FONT_INF.point;
    PTSOUT[0] = FONT_INF.max_char_width;
    PTSOUT[1] = FONT_INF.top+1;
    PTSOUT[2] = FONT_INF.max_cell_width;
    PTSOUT[3] = CHR_HT;	
    inc_lfu();
} 

make_header()
{
    WORD	i;
    cpy_head();
    FONT_INF.point <<= 1;
    i = ACT_SIZ( FONT_INF.top );
    if( DDA_INC == 0xffff )
	i++;
    else
    {
	if( i == ACT_SIZ( FONT_INF.top + 1 ) )
	    i--;	
    }		
    FONT_INF.top = i;
    FONT_INF.ascent = ACT_SIZ( FONT_INF.ascent ) + 1;
    FONT_INF.half = ACT_SIZ( FONT_INF.half ) + 1;
    FONT_INF.descent = ACT_SIZ(FONT_INF.descent );
    FONT_INF.bottom = ACT_SIZ( FONT_INF.bottom );
    FONT_INF.max_char_width = ACT_SIZ( FONT_INF.max_char_width );
    FONT_INF.max_cell_width = ACT_SIZ( FONT_INF.max_cell_width );
    FONT_INF.left_offset = ACT_SIZ( FONT_INF.left_offset );
    FONT_INF.right_offset = ACT_SIZ( FONT_INF.right_offset );
    FONT_INF.thicken = ACT_SIZ( FONT_INF.thicken );
    FONT_INF.ul_size = ACT_SIZ( FONT_INF.ul_size );
    ACTDELY = ACT_SIZ( FONT_INF.form_height );

    SPECIAL |= SCALE;
    cur_font = cur_head;
}

dst_style()
{
    rq_attr = INTIN[0] & 0x000f;	
    sel_font();	
    CONTRL[4]=1;
    INTOUT[0] = rq_attr;	
} 


dst_aligmnent()
{
    h_align = INTOUT[0] = INTIN[0];
    v_align = INTOUT[1] = INTIN[1];
    CONTRL[4] = 2;
} 

dst_rotation()
{
    while (INTIN[0] < 0)
	    INTIN[0] += 3600;
    while (INTIN[0] >= 3600)
	    INTIN[0] -= 3600;
    INTOUT[0] = (rot_case = (INTIN[0] + 450) / 900) * 900;
    SPECIAL = (SPECIAL & ~ROTATE) | (rot_case << 6);
    CONTRL[4] = 1;
}

dst_font()
{
    rq_font = INTIN[0];
    sel_font();
    CONTRL[4] = 1;

    INTOUT[0] = FONT_INF.font_id & 0x00ff;
}
	FHEAD
*sel_effect(f_ptr)
	FHEAD		*f_ptr;
{
	WORD		size;
	BYTE		eff;
	BYTE		find;
	BYTE		font;
	FHEAD		*bold;
	FHEAD		*italic;
	FHEAD		*normal;
	FHEAD		*ptr;

	/* Determine what is being searched for. */
	font = f_ptr->font_id;
	size = f_ptr->point;
	find = rq_attr & (SKEW | THICKEN);
	normal = italic = bold = (FHEAD *)NULLPTR;
	SPECIAL = SPECIAL & 0xfff0;
	SPECIAL = SPECIAL | rq_attr;
	/* Scan for a font matching the effect exactly. */
	for (ptr = f_ptr; ptr && ((ptr->font_id & 0xff) == font) &&
			(ptr->point == size); ptr = ptr->next_font) {
		eff = (BYTE)((ptr->font_id >> 8) & 0x00ff);
		if (eff == find) {
			SPECIAL &= (~find);
			return(ptr);
		}  /* End if:  effect found. */

		if (eff == 0x0)
			normal = ptr;
		else if (eff == THICKEN)
			bold = ptr;
		else if (eff == SKEW)
			italic = ptr;
	}  /* End for:  over fonts. */

	/* No exact match was found.  If the effect is bold-italic, */
	/* return italic (if possible) or bold (if possible).       */
	if (find == (SKEW | THICKEN)) {
		if (italic) {
			SPECIAL &= (~SKEW);
			return(italic);
		}  /* End if:  returning italic. */
		else if (bold) {
			SPECIAL &= (~THICKEN);
			return(bold);
		}  /* End elseif:  returning bold. */
	}  /* End if:  bold-italic. */

	/* Running out of alternatives.  Set the style and try to */
	/* return normal.                                         */
	if (normal)
		return(normal);
	else
		return(f_ptr);
}  /* End "sel_effect". */


	FHEAD
*sel_size(f_ptr)
	FHEAD		*f_ptr;
{
	WORD		close_size;
	WORD		this_size;
	BYTE		font;
	WORD		ptsize;
	WORD		curptsz;
	FHEAD		*ptszptr;
	FHEAD		*close_ptr;
	FHEAD		*ptr;

	/* Look for a font whose size matches exactly. */
	font = f_ptr->font_id;
	ptsize = f_ptr->point;
	ptszptr = f_ptr;
	for (ptr = f_ptr; ptr && ((ptr->font_id & 0xff) == font);
			ptr = ptr->next_font) {
		curptsz = ptr->point;
		if (rq_type)
			this_size = curptsz;
		else
			this_size = ptr->top + 1;
		if( ptsize != curptsz )	/* return head of size chain */
		{
		    ptsize = curptsz;
		    ptszptr = ptr;
		}
		if (this_size == rq_size)
			return(ptszptr);
		else if (this_size > rq_size)
			break;
	}  /* End for:  over fonts. */

	/* No exact match.  Look for a doubling match. */
	ptsize = f_ptr->point;
	ptszptr = f_ptr;
	for (ptr = f_ptr; ptr && ((ptr->font_id & 0xff) == font);
			ptr = ptr->next_font) {
		curptsz = ptr->point;
		if (rq_type)
			this_size = curptsz;
		else
			this_size = ptr->top + 1;
		if( ptsize != curptsz )	/* return head of size chain */
		{
		    ptsize = curptsz;
		    ptszptr = ptr;
		}
		this_size <<= 1;
		if (this_size == rq_size) {
			dbl = TRUE;
			return(ptszptr);
		}  /* End if:  exact doubling match. */
		else if (this_size > rq_size)
			break;
	}  /* End for:  over fonts. */

	/* No doubling match.  Return the next size lower (this */
	if (rq_type)
		close_size = f_ptr->point;
	else
		close_size = f_ptr->top + 1;
	close_ptr = f_ptr;
	ptsize = f_ptr->point;
	ptszptr = f_ptr;
	for (ptr = f_ptr; ptr && ((ptr->font_id & 0xff) == font);
			ptr = ptr->next_font) {
		curptsz = ptr->point;
		if (rq_type)
			this_size = curptsz;
		else
			this_size = ptr->top + 1;
		if( ptsize != curptsz )	/* return head of size chain */
		{
		    ptsize = curptsz;
		    ptszptr = ptr;
		}
		if (this_size < rq_size) {
/* DJH 3/23/87 */
/* fixed potential doubling instead of scaling bug < to <= */
			if (rq_size - this_size <= rq_size - close_size) {
/* DJH */
				close_ptr = ptszptr;
				close_size = this_size;
				dbl = FALSE;
			}  /* End if:  smaller found. */
		}  /* End if:  normal candidate. */

/*----------------------------------------------------------------
* jn 11-10-87
* Put doubling back in
* the following comment was Dons.
* DJH 3/23/87 removed doubling to make sure largest size is used 
* He used his initials to close the comment 
*-----------------------------------------------------------------*/
#if 1

		if ((this_size <<= 1) < rq_size) {
			if (rq_size - this_size < rq_size - close_size) {
				close_ptr = ptr;
				close_size = this_size;
				dbl = TRUE;
			}  /* End if:  smaller found. */
			dbl = TRUE;
		}  /* End if:  double candidate. */

#endif
	}  /* End for:  over fonts. */
	if( (this_size << 1) < rq_size )
	    dbl = TRUE;
	return(close_ptr);
}  /* End "sel_size". */

	VOID
sel_font()
{
	FHEAD		*ptr;

	/* Find a matching face identifier.  If there is no matching */
	/* face, use the first non-dummy face.                       */
	for (ptr = font_top; ptr && ((ptr->font_id & 0xff) != rq_font);
		ptr = ptr->next_font)
		;
	if (!ptr)
		ptr = font_top;

	/* Find the first font in the face which is the closest in size */
	/* (note:  it may be a doubled font -- dbl will be set).        */
	dbl = FALSE;
	ptr = sel_size(ptr);

	/* Find the closest matching attribute. */
	act_font = cur_font = sel_effect(ptr);

	/* If the font needs to be doubled, build a header. */
        SPECIAL &= ~SCALE;
        cpy_head();
        ACTDELY = FONT_INF.form_height;
	if ( (dbl) || (FONT_INF.top + 1 != rq_size ) )
	{
	    if( dbl )
	    {
	        DDA_INC = 0xFFFF;
	        T_SCLSTS = 1;
		make_header();
	    }
	    else
            {
		if( !rq_type )
		{
	            DDA_INC = CLC_DDA(FONT_INF.top + 1, rq_size);
	            make_header();
		}
            }
	}
	CHR_HT = FONT_INF.top + 1 + FONT_INF.bottom;
    	CHAR_DEL = WEIGHT = R_OFF = L_OFF = 0;	
    	if (SPECIAL & THICKEN)
    	    CHAR_DEL = WEIGHT = FONT_INF.thicken;
    	if (SPECIAL & SKEW)
    	{
	    L_OFF = FONT_INF.left_offset;
   	    R_OFF = FONT_INF.right_offset;
	    CHAR_DEL += (L_OFF + R_OFF);
        }
        MONO_STATUS =
    	FONT_INF.max_cell_width == 8 ? MONOSPACE & FONT_INF.flags : FALSE;

}  /* End "sel_font". */


dst_color()
{
    CONTRL[4]=1;
    INTOUT[0] = text_color = range(INTIN[0], 0, MAX_COLOR - 1, 1);
    TEXT_BP = MAP_COL[text_color];
}

dqt_attributes()
{
    INTOUT[0] = FONT_INF.font_id;
    INTOUT[1] = text_color;
    INTOUT[2] = 900*( (SPECIAL & ROTATE) >> 6 );
    INTOUT[3] = h_align;
    INTOUT[4] = v_align;
    INTOUT[5] = WRT_MODE;

    PTSOUT[0] = FONT_INF.max_char_width;
    PTSOUT[1] = FONT_INF.top + 1;
    PTSOUT[2] = FONT_INF.max_cell_width;
    PTSOUT[3] = PTSOUT[1] + FONT_INF.bottom;

    CONTRL[2] = 2;
    CONTRL[4] = 6;
    FLIP_Y = 1;	
}

dqt_extent()
{
    WORD i, j, horsts, ix;

    if ( CONTRL[3] == 0 )
	return;

    width = 0;	

    for (i = 0 ; i < CONTRL[3] ; i++)
    {	
	if ((j = chk_ade(INTIN[i])) == 1) {	/* jn 10-27-87 */
	  chk_fnt() ;
	}
	else if (j == 2) {			/* jn 11-22-87 */
	  INTIN[i] = ' ' ;
	  if (chk_ade(INTIN[i]) == 1) {
	    chk_fnt() ;
	  }
	}
        horsts = FONT_INF.flags & HORZ_OFF;
	ix = INTIN[i] - FIR_CHR;
	width += FONT_INF.off_table[ix + 1] - FONT_INF.off_table[ix];
	if( horsts )
	{
	    ix <<= 1;
	    width -= FONT_INF.hor_table[ix] + FONT_INF.hor_table[ix + 1];
	}
    }	


    if ( (SPECIAL & SCALE) != 0 )
        width = ACT_SIZ(width);

    if ((SPECIAL & THICKEN) && !(FONT_INF.flags & MONOSPACE))
	width += CONTRL[3] * WEIGHT;
    height = CHR_HT;
    CONTRL[2] = 4;

    for (i = 0 ; i < 8 ; i++)
	PTSOUT[i] = 0;

    switch (rot_case)
    {
    case 0:
	    PTSOUT[2] = PTSOUT[4] = width;
	    PTSOUT[5] = PTSOUT[7] = height;
	    break;
    case 1:
	    PTSOUT[0] = PTSOUT[2] = height;
	    PTSOUT[3] = PTSOUT[5] = width;
	    break;
    case 2:
	    PTSOUT[0] = PTSOUT[6] = width;
	    PTSOUT[1] = PTSOUT[3] = height;
	    break;
    case 3:
	    PTSOUT[4] = PTSOUT[6] = height;
	    PTSOUT[1] = PTSOUT[7] = width;
	    break;
    }  /* End switch. */
    FLIP_Y = 1;	
}

dqt_width()
{
    WORD  k;
	
    k = INTIN[0];
    
    if (chk_ade(k) == 1) {	/* jn 1-5-88, make sure font is in ram */
        chk_fnt();		
    }

    if (k < FONT_INF.first_ade || k > FONT_INF.last_ade)
	INTOUT[0] = -1;
    else
    {
    	INTOUT[0] = k;
	k -= FONT_INF.first_ade;
 	PTSOUT[0] = FONT_INF.off_table[k+1] - FONT_INF.off_table[k];
 	if ( (SPECIAL & SCALE) != 0 )
	    PTSOUT[0] = ACT_SIZ(PTSOUT[0]);

        if ( FONT_INF.flags & HORZ_OFF )
        {
	    PTSOUT[2] = FONT_INF.hor_table[k << 1];
	    PTSOUT[4] = FONT_INF.hor_table[(k << 1) + 1];
        }
        else
	    PTSOUT[2] = PTSOUT[4] = 0;
    }
    CONTRL[2] = 3;
    CONTRL[4] = 1;
    FLIP_Y = 1;	
}

dqt_name()
{
    WORD i;
    BYTE *name;
    struct font_head *tmp_font;

    tmp_font = font_top;
    for (i = 1 ; i < INTIN[0] ; i++, tmp_font = tmp_font->next_font)
      while ((tmp_font->font_id & 0xff)==(tmp_font->next_font->font_id & 0xff))
	    tmp_font = tmp_font->next_font;
    INTOUT[0] = (tmp_font->font_id & 0xff) ;
    for (i = 1, name = tmp_font->name ; INTOUT[i++] = *name++ ;)
	;
    while(i < (CONTRL[4] = 33))
	INTOUT[i++] = 0;
} 

dqt_fontinfo()
{
#if 0
    INTOUT[0] = FIR_CHR;
    INTOUT[1] = FONT_INF.last_ade;
#else
    INTOUT[0] = 32;
    INTOUT[1] = 255 ;
#endif
    PTSOUT[0] = FONT_INF.max_cell_width;
    PTSOUT[1] = FONT_INF.bottom;
    PTSOUT[3] = FONT_INF.descent;
    PTSOUT[5] = FONT_INF.half;
    PTSOUT[7] = FONT_INF.ascent;
    PTSOUT[9] = FONT_INF.top;
    if ( !(FONT_INF.flags & MONOSPACE) )
        PTSOUT[2] = WEIGHT;
    else
	PTSOUT[2] = 0;
    PTSOUT[6] = R_OFF;
    PTSOUT[4] = L_OFF;

    CONTRL[2] = 5;
    CONTRL[4] = 2;
    FLIP_Y = 1;	
} 

dt_loadfont()
{
  WORD id, i;
  union
  {
    WORD w_ptr[2];
    struct font_head *f_ptr;
  } fontptr;
  struct font_head *this_font;

  /* The scratch text buffer segment is passed in CONTRL[8].  The size of */
  /* the buffer is passed in CONTRL[9].  Use them to set the text scratch */
  /* buffer addresses.                                                    */
  buff_seg = CONTRL[8];
  txbuf1 = 0;
  txbuf2 = CONTRL[9] >> 1;
  ftmgroff = CONTRL[10];
  ftmgrseg = CONTRL[5];

  /* The font segment is passed in CONTRL[7].  Plug it into a pointer. */
  fontptr.w_ptr[0] = 0;
  fontptr.w_ptr[1] = CONTRL[7];
  v_nop();  /* Don't remove this -- Lattice optimization may bite you! */

  /* If the font offset and segment are not already linked into the */
  /* font chain, link them in.                                      */
  for (this_font = font_top;
    (this_font->next_font != 0) && (this_font->next_font != fontptr.f_ptr);
    this_font = this_font->next_font);

  CONTRL[4] = 1;
  INTOUT[0] = 0;

  if (this_font->next_font == 0)
  {
    /* Patch the last link to point to the first new font. */
    this_font->next_font = fontptr.f_ptr;
    id = this_font->font_id & 0xff ;

    /* Find out how many distinct font id numbers have just been linked in. */
    while (this_font = this_font->next_font)
    {
      /* Update the count of font id numbers, if necessary. */
      if ((this_font->font_id & 0xff) != id)
      {
        id = this_font->font_id & 0xff ;
        INTOUT[0]++;
      }  /* End if:  new font id found. */
    }  /* End while:  over fonts. */
  }  /* End if:  must link in. */
  /* Update the device table count of faces. */
  DEV_TAB[10] += INTOUT[0];
  loaded = TRUE;
}  /* End "dt_loadfont". */

dt_unloadfont()
{
  union
  {
    WORD w_ptr[2];
    struct font_head *f_ptr;
  } fontptr;
  union
  {
    WORD wd[2];
    WORD *pt;
  } scratch;
  struct font_head *this_font;

  /* The font segment chain to be unloaded is passed in CONTRL[7].  Plug */
  /* it into a pointer.  If it is zero, bail out.                        */
  if (!CONTRL[7])
    return;
  fontptr.w_ptr[0] = 0;
  fontptr.w_ptr[1] = CONTRL[7];
  v_nop();  /* Don't remove this -- Lattice optimization may bite you! */

  /* Find a pointer to the font segment and offset in the chain and */
  /* break the link.                                                */
  for (this_font = font_top;
    (this_font->next_font != 0) && (this_font->next_font != fontptr.f_ptr);
    this_font = this_font->next_font);
  if (this_font->next_font)
    this_font->next_font = 0;

  /* Reset the default text font and scratch buffer addresses. */
  scratch.pt = &def_buf;
  v_nop();  /* Don't remove this -- Lattice optimization may bite you! */
  buff_seg = scratch.wd[1];
  txbuf1 = sav_buf1;
  txbuf2 = sav_buf2;
  cur_font = font_top;
  cpy_head();

  /* Re-initialize the number of fonts indicated in the device table. */
  DEV_TAB[10] = ini_font_count;
  loaded = FALSE;
}  /* End "dt_unloadfont". */
