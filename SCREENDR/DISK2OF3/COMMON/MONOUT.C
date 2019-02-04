
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

/*History
Fix #	Name	Date	Fix description
1	DH	5/29/85	Polyline had edge shorten flag inverted (LSTLIN)
*/
#include "portab.h"
#include "gsxdef.h"
#include "gsxextrn.h"

/* OPEN_WORKSTATION: */
	VOID
v_opnwk()
{
	extern	WORD	dseg_seg,end_dseg;  /*Data seg and size returned at OW */
	text_init();
	init_wk();
	INIT_G();			/* go into graphics mode */

	CONTRL[7] = dseg_seg;
	CONTRL[8] = end_dseg;
}

d_opnvwk() 
{
	text_init();
	init_wk();
}


d_clsvwk()
{
} 


/* EXTENDED INQUIRE */
vq_extnd()
{
	WORD	i;
	WORD	*ipt, *ppt;

	CONTRL[2] = 6;
        CONTRL[4] = 45;
	FLIP_Y = 1;
	if ( !INTIN[0] )
	{
	    ipt = DEV_TAB;
	    ppt = SIZ_TAB;
	}
	else
	{
	    ipt = INQ_TAB;
	    ppt = INQ_PTS;
	}
	for (i = 0; i < 45; i++)
	    INTOUT[i] = *ipt++;
	for (i = 0; i < 12; i++)
	    PTSOUT[i] = *ppt++;
}
/* CLOSE_WORKSTATION: */
v_clswk()
{
	DINIT_G();
}

/* ESCAPE: */
v_escape()
{
	CHK_ESC();
}

/* POLYLINE: */
v_pline()
{
	WORD	pln_sts;
	if ( HIDE_CNT == 0 )
	{
	    pln_sts = 1;
	    HIDE_CUR();
	}
	else
	    pln_sts = 0;	
	LN_MASK = LINE_STYL[line_index];
	FG_BP_1 = line_color;

	if (line_width == 1)
	{
	  pline();
          if ( (line_beg | line_end ) & ARROWED )
	    do_arrow();
	}  /* End if:  normal polyline. */
	else
	  wline();
	if ( pln_sts == 1 )
	    DIS_CUR();
}

/* POLYMARKER: */
v_pmarker()
{
#define MARKSEGMAX 5

  EXTERN WORD m_dot[], m_plus[], m_star[], m_square[], m_cross[], m_dmnd[];

  static WORD *markhead[] =
  {
    m_dot, m_plus, m_star, m_square, m_cross, m_dmnd
  };

  WORD i, j, k, num_lines, num_vert, x_center, y_center;
  WORD sav_index, sav_color, sav_width, sav_beg, sav_end, sav_clip;
  WORD *mark_ptr;

  /* Save the current polyline attributes which will be used. */
  sav_index = line_index;
  sav_color = line_color;
  sav_width = line_width;
  sav_beg = line_beg;
  sav_end = line_end;
  sav_clip = CLIP;

  /* Set the appropriate polyline attributes. */
  line_index = 0;
  line_color = mark_color;
  line_width = 1;
  line_beg = 0;
  line_end = 0;
  CLIP = 1;

  /* Copy the PTSIN values which will be overwritten by the polyline arrays. */
  num_vert = 2*CONTRL[1];
  for (i = 0; i < 2*MARKSEGMAX; i++)
    PTSOUT[i] = PTSIN[i];

  /* Loop over the number of points. */
  for (i = 0; i < num_vert; i++)
  {
    /* Get the (x, y) position for the marker. */
    if (i < 2*MARKSEGMAX)
    {
      x_center = PTSOUT[i++];
      y_center = PTSOUT[i];
    }  /* End if:  grab from PTSOUT. */
    else
    {
      x_center = PTSIN[i++];
      y_center = PTSIN[i];
    }  /* End else:  grab from PTSIN. */

    /* Get the pointer to the appropriate marker type definition. */
    mark_ptr = markhead[mark_index];
    num_lines = *mark_ptr++;

    /* Loop over the number of polylines which define the marker. */
    for (j = 0; j < num_lines; j++)
    {
      /* How many points?  Get them. */
      CONTRL[1] = *mark_ptr++;
      for (k = 0; k < 2*CONTRL[1]; )
      {
        PTSIN[k++] = x_center + mark_scale*(*mark_ptr++);
        PTSIN[k++] = y_center + mark_scale*(*mark_ptr++);
      }  /* End for:  extract points. */

      /* Output the polyline. */
      v_pline();
    }  /* End for:  over the number of polylines defining the marker. */
  }  /* End for:  over marker points. */

  /* Restore the current polyline attributes. */
  line_index = sav_index;
  line_color = sav_color;
  line_width = sav_width;
  line_beg = sav_beg;
  line_end = sav_end;
  CLIP = sav_clip;
}  /* End "v_pmarker". */

/* FILLED_AREA: */
v_fillarea()
{
	plygn();
}

/*  GDP: */
v_gdp()
{
    WORD	i,ltmp_end,rtmp_end;
    i = CONTRL[5];
    if (( i > 0 ) && ( i < 11 )) 
    {  
	i--;
	switch ( i )
	{
	    case 0:	/* GDP BAR */
	      dr_recfl();
	      if ( fill_per == TRUE )
	      {
		LN_MASK = 0xffff;
		PTSIN[5] = PTSIN[7] = PTSIN[3];
		PTSIN[3] = PTSIN[9] = PTSIN[1];
		PTSIN[4] = PTSIN[2];
		PTSIN[6] = PTSIN[8] = PTSIN[0];
		CONTRL[1] = 5;
		pline();
	      }
	      break;	

	    case 1: /* GDP ARC */
	      gdp_arc();
	      break;

	    case 2: /* GDP PIE */
	      gdp_arc();
	      break;
	 	
	    case 3: /* GDP CIRCLE */
	      xc = PTSIN[0]; yc = PTSIN[1];   
	      xrad = PTSIN[4];   
	      yrad = SMUL_DIV (xrad, xsize, ysize ); 
	      del_ang = 3600;
	      beg_ang = 0;
	      end_ang = 3600;
	      clc_nsteps();
	      clc_arc();
	      break;

	    case 4: /* GDP ELLIPSE */
	      xc = PTSIN[0]; yc = PTSIN[1];   
	      xrad = PTSIN[2];
	      yrad = PTSIN[3];
	      if ( xfm_mode < 2 ) /* if xform != raster then flip */
	        yrad = yres - yrad;	
	      del_ang = 3600;
	      beg_ang = 0;
	      end_ang = 0;
	      clc_nsteps();
	      clc_arc();
	      break;
		
	    case 5: /* GDP ELLIPTICAL ARC */
	      gdp_ell();
	      break;
		
	    case 6: /* GDP ELLIPTICAL PIE */
	      gdp_ell();
	      break;

	    case 7: /* GDP Rounded Box */
	      ltmp_end = line_beg;
	      line_beg = SQUARED;
	      rtmp_end = line_end;
	      line_end = SQUARED;
	      gdp_rbox();
	      line_beg = ltmp_end;
	      line_end = rtmp_end; 	
	      break;

	    case 8: /* GDP Rounded Filled Box */
	      gdp_rbox();
              break;

	    case 9: /* GDP Justified Text */
	      d_justified();
              break;
	    }
	}
}    	

/* INQUIRE CURRENT POLYLINE ATTRIBUTES */
vql_attr()
{
	INTOUT[0] = line_qi;
	INTOUT[1] = line_qc;
	INTOUT[2] = write_qm;
	INTOUT[3] = line_beg;
	INTOUT[4] = line_end;
	PTSOUT[0] = line_qw;
	PTSOUT[1] = 0;
	CONTRL[2] = 1;
	CONTRL[4] = 5;
}

/* INQUIRE CURRENT Polymarker ATTRIBUTES */
vqm_attr()
{
	INTOUT[0] = mark_qi;
	INTOUT[1] = mark_qc;
	INTOUT[2] = write_qm;
	CONTRL[4] = 3;
  	PTSOUT[0] = mark_scale*DEF_MKWD;
  	PTSOUT[1] = mark_scale*DEF_MKHT;
	CONTRL[2] = 1;
	FLIP_Y = 1;
}

/* INQUIRE CURRENT Fill Area ATTRIBUTES */
vqf_attr()
{
	INTOUT[0] = fill_style;
	INTOUT[1] = fill_qc;
	INTOUT[2] = fill_qi;
	INTOUT[3] = write_qm;
	INTOUT[4] = fill_per;
	CONTRL[4] = 5;
}	

pline()
{
	WORD	i,j;

	j = 0;
	LSTLIN = FALSE;
	for (i = (CONTRL[1]-1 );i > 0;i--)
	{
	    if (i == 1)	      
	        LSTLIN = TRUE;
	    X1 = PTSIN[j++];
	    Y1 = PTSIN[j++];	
            X2 = PTSIN[j];
	    Y2 = PTSIN[j+1];
	    if ( CLIP )
	    {   
	        if ( clip_line() )
		    ABLINE();
	    }
	    else	
	        ABLINE();
	}				
}

	WORD
clip_line()
{
	WORD	deltax, deltay, x1y1_clip_flag, x2y2_clip_flag, line_clip_flag;
	WORD	*x,*y;

	while (( x1y1_clip_flag = code( X1, Y1 )) | 
		( x2y2_clip_flag = code( X2, Y2)))
	{
	    if ( ( x1y1_clip_flag & x2y2_clip_flag ))
		return( FALSE );
	    if ( x1y1_clip_flag )
	    {
		line_clip_flag = x1y1_clip_flag;
		x = &X1; y = &Y1;
	    }
  	    else
	    {
		line_clip_flag = x2y2_clip_flag;
		x = &X2; y = &Y2;
	    }
	    deltax = X2 - X1; deltay = Y2 - Y1;
	    if ( line_clip_flag & 1 )	/* left ? */
	    {
		*y = Y1 + SMUL_DIV( deltay, (XMN_CLIP-X1), deltax );
		*x = XMN_CLIP;
	    }
	    else if ( line_clip_flag & 2 )	/* right ? */
	    {
		*y = Y1 + SMUL_DIV( deltay, (XMX_CLIP-X1), deltax );
		*x = XMX_CLIP;
	    }
	    else if ( line_clip_flag & 4 )	/* top ? */
	    {
		*x = X1 + SMUL_DIV( deltax, (YMN_CLIP-Y1), deltay );
		*y = YMN_CLIP;
	    }
	    else if ( line_clip_flag & 8 )	/* bottom ? */
	    {
		*x = X1 + SMUL_DIV( deltax, (YMX_CLIP-Y1), deltay );
		*y = YMX_CLIP;
	    }
	}
	return( TRUE );		/* segment now cliped  */
}

	WORD
code( x, y )
	WORD	x,y;
{
	WORD	clip_flag;
	clip_flag = 0;
	if ( x < XMN_CLIP )
	    clip_flag = 1;
	else if ( x > XMX_CLIP )
	    clip_flag = 2;
	if ( y < YMN_CLIP )
	    clip_flag +=4;
	else if ( y > YMX_CLIP )
	    clip_flag +=8;
	return ( clip_flag );
}

plygn()	
{
	WORD	i,j,k;
	FG_BP_1 = fill_color;
	LSTLIN  = FALSE;
	fill_miny = fill_maxy = PTSIN[1];
	j = 3;
	for (i = (CONTRL[1] );i > 0;i--)
	{
	  k = PTSIN [j++];
	  j++;
	  if ( k < fill_miny )
	    fill_miny = k;
	  else if ( k > fill_maxy )
	    fill_maxy = k;
	}
	k = fill_miny; 
	if ( CLIP )
	{
	    if ( fill_miny < YMN_CLIP )		
	    {
	  	if ( fill_maxy >= YMN_CLIP )    	/* plygon starts before clip */
		    fill_miny = YMN_CLIP;      	/* plygon partial overlap */
		else
		    return;		       	/* plygon entirely before clip */
	    }
	    if ( fill_maxy > YMX_CLIP )
	    {
		if ( fill_miny <= YMX_CLIP )	/* plygon ends after clip */
		    fill_maxy = YMX_CLIP;	/* plygon partial overlap */
		else
		    return;			/* plygon entirely after clip */ 
	    }
	    if ( fill_miny != k )		/* plygon fills n -1 scans */
		--fill_miny;
        }
	j = CONTRL[1] * 2;
	PTSIN[j] = PTSIN[0];
	PTSIN[j+1] = PTSIN [1];
	for (Y1 = fill_maxy; Y1 > fill_miny; Y1--)
	{
	    fill_intersect = 0;
	    CLC_FLIT();
	}
	if ( fill_per == TRUE )
	{
	    LN_MASK = 0xffff;
	    CONTRL[1]++;
	    pline();
	}
}

	VOID
gdp_rbox()
{
	WORD	i,j, rdeltax, rdeltay;
        arb_corner(PTSIN, LLUR);
	X1 = PTSIN[0]; Y1 = PTSIN[1];
	X2 = PTSIN[2]; Y2 = PTSIN[3];
	rdeltax = (X2 - X1)/2; rdeltay = (Y1 - Y2)/2;
	xrad = xres >> 6;
	if ( xrad > rdeltax )
	    xrad = rdeltax;
	yrad = SMUL_DIV( xrad, xsize, ysize );
	if ( yrad > rdeltay )
	    yrad = rdeltay;
	PTSIN[0] = 0; PTSIN[1] = yrad;
	PTSIN[2] = SMUL_DIV ( Icos(675), xrad, 32767 ) ;
	PTSIN[3] = SMUL_DIV ( Isin(675), yrad, 32767 ) ;
	PTSIN[4] = SMUL_DIV ( Icos(450), xrad, 32767 ) ;
	PTSIN[5] = SMUL_DIV ( Isin(450), yrad, 32767 ) ;
	PTSIN[6] = SMUL_DIV ( Icos(225), xrad, 32767 ) ;
	PTSIN[7] = SMUL_DIV ( Isin(225), yrad, 32767 ) ;
	PTSIN[8] = xrad; PTSIN[9] = 0;
	xc = X2 - xrad; yc = Y1 - yrad;
	j = 10;
	for ( i = 9; i >= 0 ; i-- )
	{ 
	    PTSIN[j+1] = yc + PTSIN[i--];
	    PTSIN[j] = xc + PTSIN[i];
	    j += 2;
	}
	xc = X1 + xrad; 
	j = 20;
	for ( i = 0; i < 10; i++ )
	{ 
	    PTSIN[j++] = xc - PTSIN[i++];
	    PTSIN[j++] = yc + PTSIN[i];
	}
	yc = Y2 + yrad;
	j = 30;
	for ( i = 9; i >= 0; i-- )
	{ 
	    PTSIN[j+1] = yc - PTSIN[i--];
	    PTSIN[j] = xc - PTSIN[i];
	    j += 2;
	}
	xc = X2 - xrad;
	j = 0;
	for ( i = 0; i < 10; i++ )
	{ 
	    PTSIN[j++] = xc + PTSIN[i++];
	    PTSIN[j++] = yc - PTSIN[i];
	}
	PTSIN[40] = PTSIN[0];
	PTSIN[41] = PTSIN[1]; 
	iptscnt = 21;
    	if (gdp_code == 8) 
	{
	  LN_MASK = LINE_STYL[line_index];
	  FG_BP_1 = line_color;

	  if (line_width == 1)
	  {
	      pline();
	  }  /* End if:  normal polyline. */
	  else
	      wline();
	}
    	else
	  plygn();

	return;
}
	
	VOID
gdp_arc()
{
	beg_ang = INTIN[0];
	end_ang = INTIN[1];
	del_ang = end_ang - beg_ang;
	if ( del_ang < 0 )
	    del_ang += 3600; 
	xrad = PTSIN[6];   
	yrad = SMUL_DIV ( xrad, xsize, ysize );
	clc_nsteps();
	n_steps = SMUL_DIV ( del_ang, n_steps, 3600 );
	if ( n_steps == 0 )
	  return;
	xc = PTSIN[0]; yc = PTSIN[1];   
	clc_arc();
	return;
}

clc_nsteps()
{
	if ( xrad > yrad )
	    n_steps = xrad;
	else
	    n_steps = yrad;
	n_steps = n_steps >> 2;
	if ( n_steps < 16 )
	    n_steps = 16;
	else
	{
	    if ( n_steps > MAX_ARC_CT )
	        n_steps = MAX_ARC_CT;
	}
	return;
}

gdp_ell()
{
	beg_ang = INTIN[0];
	end_ang = INTIN[1];
	del_ang = end_ang - beg_ang;
	if ( del_ang < 0 )
	    del_ang += 3600;
	xrad = PTSIN[2];   
	yrad = PTSIN[3];
	if ( xfm_mode < 2 ) /* if xform != raster then flip */
	  yrad = yres - yrad;	
	clc_nsteps();	
	n_steps = SMUL_DIV ( del_ang, n_steps, 3600 );
	if ( n_steps == 0 )
	  return;
	xc = PTSIN[0]; yc = PTSIN[1];   
	clc_arc();
	return;
}

	VOID
clc_arc()
{
	WORD	i,j;
	if ( CLIP )
	{
	    if ( (( xc + xrad ) < XMN_CLIP) || ((xc - xrad) > XMX_CLIP) ||
			((yc + yrad ) < YMN_CLIP) || ((yc - yrad) > YMX_CLIP))
		return;
	}
	start = angle = beg_ang ;	
    	i = j = 0 ;
    	Calc_pts(j);
	for ( i = 1; i < n_steps; i++ )
    	{
	  j += 2;
	  angle = SMUL_DIV( del_ang, i , n_steps ) + start;
          Calc_pts(j);
        }
	j += 2;
    	i = n_steps ;
    	angle = end_ang ;
    	Calc_pts(j);

/*----------------------------------------------------------------------*/
/* If pie wedge	draw to center and then close. If arc or circle, do 	*/
/* nothing because loop should close circle.				*/

        iptscnt = n_steps + 1 ;	/* since loop in Clc_arc starts at 0 */
	if ((gdp_code == 3)||(gdp_code == 7)) /* pie wedge */
	{
	  n_steps++;
	  j +=2;
	  PTSIN[ j ] = xc; PTSIN[ j + 1 ] = yc;
	  iptscnt = n_steps+1;
	}
    	if ((gdp_code == 2) || ( gdp_code == 6 )) /* open arc */
          v_pline();
    	else
	  plygn();
}

	VOID
Calc_pts(j) 
	WORD	j;
{
	WORD	k;
	k = SMUL_DIV ( Icos(angle), xrad, 32767 ) + xc ;
    	PTSIN[ j ] = k;
/*	if ( xfm_mode < 2 )
*/	    k = yc - SMUL_DIV ( Isin( angle), yrad, 32767 ) ;
/*	else
	    k = yc + SMUL_DIV ( Isin( angle), yrad, 32767 ) ;
*/	
    	PTSIN[ j + 1 ] = k;
}

st_fl_ptr()
{
	patmsk = 0;
	switch ( fill_style )
	{
		case 0:
			patptr = &HOLLOW;
			break;

		case 1:
			patptr = &SOLID;
			break;

		case 2:
			if ( fill_index < 8 )
			{
			    patmsk = DITHRMSK;
			    patptr = &DITHER[ fill_index * (patmsk+1) ];
			}
			else
			{
			    patmsk = OEMMSKPAT;
			    patptr = &OEMPAT[ (fill_index - 8) * (patmsk+1) ]; 
			}
			break;
		case 3:
			if ( fill_index < 6 )
			{
			    patmsk = HAT_0_MSK;
			    patptr = &HATCH0[ fill_index * (patmsk+1) ];
			}
			else
			{
			    patmsk = HAT_1_MSK;
			    patptr = &HATCH1[ (fill_index - 6) * (patmsk+1) ];
			}
			break;
		case 4:
			patmsk = 0x000f;
			patptr = &UD_PATRN;
			break;
	}
}

#define ABS(x) ((x) >= 0 ? (x) : -(x))

wline()
{
  WORD i, j, k;
  WORD numpts, wx1, wy1, wx2, wy2, vx, vy, tinv;

  /* Don't attempt wide lining on a degenerate polyline. */
  if ( (numpts = CONTRL[1]) < 2)
    return;

  /* If the ends are arrowed, output them. */
  if ( (line_beg | line_end) & ARROWED)
    do_arrow();

  /* Set up the attribute environment.  Save attribute globals which need */
  /* to be saved.                                                         */
  s_fa_attr();

  /* Since the PTSIN array has to be overwritten for the call to plygn, */
  /* save the values stored in the elements which will be overwritten.  */
  for (i = 2; i < 10; i++)
    PTSOUT[i] = PTSIN[i];

  /* Initialize the starting point for the loop. */
  j = 0;
  wx1 = PTSIN[j++];
  wy1 = PTSIN[j++];

  /* If the end style for the first point is not squared, output a circle. */
  if (s_begsty != SQUARED)
    do_circ(wx1, wy1);

  /* Loop over the number of points passed in. */
  for (i = 1; i < numpts; i++)
  {
    /* Get the ending point for the line segment and the vector from the */
    /* start to the end of the segment.                                  */
    if (j < 10)
    {
      wx2 = PTSOUT[j++];
      wy2 = PTSOUT[j++];   
    }  /* End if:  copy from saved elements. */
    else
    {
      wx2 = PTSIN[j++];
      wy2 = PTSIN[j++];
    }  /* End else:  copy from PTSIN array. */

    vx = wx2 - wx1;
    vy = wy2 - wy1;

    /* Ignore lines of zero length. */
    if ( (vx == 0) && (vy == 0) )
      continue;

    /* Calculate offsets to fatten the line.  If the line segment is */
    /* horizontal or vertical, do it the simple way.                 */
    if (vx == 0)
    {
      vx = q_circle[0];
      vy = 0;
    }  /* End if:  vertical. */

    else if (vy == 0)
    {
      vx = 0;
      vy = num_qc_lines - 1;
    }  /* End else if:  horizontal. */

    else
    {
      /* Find the offsets in x and y for a point perpendicular to the line */
      /* segment at the appropriate distance.                              */
      k = SMUL_DIV(-vy, ysize, xsize);
      vy = SMUL_DIV(vx, xsize, ysize);
      vx = k;
      perp_off(&vx, &vy);
    }  /* End else:  neither horizontal nor vertical. */

    /* Prepare the control and points parameters for the polygon call. */
    CONTRL[1] = 4;
    PTSIN[0] = wx1 + vx;
    PTSIN[1] = wy1 + vy;
    PTSIN[2] = wx1 - vx;
    PTSIN[3] = wy1 - vy;
    PTSIN[4] = wx2 - vx;
    PTSIN[5] = wy2 - vy;
    PTSIN[6] = wx2 + vx;
    PTSIN[7] = wy2 + vy;
    plygn();

    /* If the terminal point of the line segment is an internal joint, */
    /* or the end style is not squared, output a filled circle.        */
    if ( (s_endsty != SQUARED) || (i < numpts - 1) )
      do_circ(wx2, wy2);

    /* The line segment end point becomes the starting point for the next */
    /* line segment.                                                      */
    wx1 = wx2;
    wy1 = wy2;
  }  /* End for:  over number of points. */

  /* Restore the attribute environment. */
  r_fa_attr();
}  /* End "wline". */


perp_off(vx, vy)
int *vx, *vy;
{
  int i, x, y, u, v, quad, magnitude, min_val, x_val, y_val;

  /* Mirror transform the vector so that it is in the first quadrant. */
  if (*vx >= 0)
    quad = (*vy >= 0) ? 1 : 4;
  else
    quad = (*vy >= 0) ? 2 : 3;

  quad_xform(quad, *vx, *vy, &x, &y);

  /* Traverse the circle in a dda-like manner and find the coordinate pair   */
  /* (u, v) such that the magnitude of (u*y - v*x) is minimized.  In case of */
  /* a tie, choose the value which causes (u - v) to be minimized.  If not   */
  /* possible, do something.                                                 */
  min_val = 32767;
  u = q_circle[0];
  v = 0;
  FOREVER
  {
    /* Check for new minimum, same minimum, or finished. */
    if ( ( (magnitude = ABS(u*y - v*x)) < min_val ) ||
         ( (magnitude == min_val) && (ABS(x_val - y_val) > ABS(u - v) ) ) )
    {
      min_val = magnitude;
      x_val = u;
      y_val = v;
    }  /* End if:  new minimum. */

    else
      break;

    /* Step to the next pixel. */
    if (v == num_qc_lines - 1)
    {
      if (u == 1)
        break;
      else
        u--;
    }  /* End if:  doing top row. */

    else
    {
      if (q_circle[v + 1] >= u - 1)
      {
        v++;
        u = q_circle[v];
      }  /* End if:  do next row up. */
      else
      {
        u--;
      }  /* End else:  continue on row. */
    }  /* End else:  other than top row. */
  }  /* End FOREVER loop. */

  /* Transform the solution according to the quadrant. */
  quad_xform(quad, x_val, y_val, vx, vy);
}  /* End "perp_off". */


quad_xform(quad, x, y, tx, ty)
int quad, x, y, *tx, *ty;
{
  switch (quad)
  {
    case 1:
    case 4:
      *tx = x;
      break;

    case 2:
    case 3:
      *tx = -x;
      break;
  }  /* End switch. */

  switch (quad)
  {
    case 1:
    case 2:
      *ty = y;
      break;

    case 3:
    case 4:
      *ty = -y;
      *ty = -y;
      break;
  }  /* End switch. */
}  /* End "quad_xform". */


do_circ(cx, cy)
WORD cx, cy;
{
  WORD k;

  /* Only perform the act if the circle has radius. */
  if (num_qc_lines > 0)
  {
    /* Do the horizontal line through the center of the circle. */
    X1 = cx - q_circle[0];
    X2 = cx + q_circle[0];
    Y1 = Y2 = cy;
    if (clip_line() )
      ABLINE();

    /* Do the upper and lower semi-circles. */
    for (k = 1; k < num_qc_lines; k++)
    {
      /* Upper semi-circle. */
      X1 = cx - q_circle[k];
      X2 = cx + q_circle[k];
      Y1 = Y2 = cy - k;
      if (clip_line() )
        ABLINE();

      /* Lower semi-circle. */
      X1 = cx - q_circle[k];
      X2 = cx + q_circle[k];
      Y1 = Y2 = cy + k;
      if (clip_line() )
        ABLINE();
    }  /* End for. */
  }  /* End if:  circle has positive radius. */
}  /* End "do_circ". */


s_fa_attr()
{
  /* Set up the fill area attribute environment. */
  LN_MASK = LINE_STYL[0];
  fill_color = line_color;
  s_fill_per = fill_per;
  fill_per = TRUE;
  s_patptr = patptr;
  s_nxtpat = NEXT_PAT;
  NEXT_PAT = 0;	
  patptr = &SOLID;
  s_patmsk = patmsk;
  patmsk = 0;
  s_begsty = line_beg;
  s_endsty = line_end;
  line_beg = line_end = SQUARED;
}  /* End "s_fa_attr". */


r_fa_attr()
{
  /* Restore the fill area attribute environment. */
  NEXT_PAT = s_nxtpat;
  fill_color = MAP_COL[fill_qc];
  fill_per = s_fill_per;
  patptr = s_patptr;
  patmsk = s_patmsk;
  line_beg = s_begsty;
  line_end = s_endsty;
}  /* End "r_fa_attr". */


do_arrow()
{
  WORD x_start, y_start, new_x_start, new_y_start;

  /* Set up the attribute environment. */
  s_fa_attr();

  /* Function "arrow" will alter the end of the line segment.  Save the */
  /* starting point of the polyline in case two calls to "arrow" are    */
  /* necessary.                                                         */
  new_x_start = x_start = PTSIN[0];
  new_y_start = y_start = PTSIN[1];

  if (s_begsty & ARROWED)
  {
    arrow(&PTSIN[0], 2);
    new_x_start = PTSIN[0];
    new_y_start = PTSIN[1];
  }  /* End if:  beginning point is arrowed. */

  if (s_endsty & ARROWED)
  {
    PTSIN[0] = x_start;
    PTSIN[1] = y_start;
    arrow(&PTSIN[2*CONTRL[1] - 2], -2);
    PTSIN[0] = new_x_start;
    PTSIN[1] = new_y_start;
  }  /* End if:  ending point is arrowed. */

  /* Restore the attribute environment. */
  r_fa_attr();
}  /* End "do_arrow". */


arrow(xy, inc)
WORD *xy, inc;
{
  WORD i, arrow_len, arrow_wid, line_len;
  WORD *xybeg, sav_contrl, triangle[6];
  WORD dx, dy;
  WORD base_x, base_y, ht_x, ht_y;

  /* Set up the arrow-head length and width as a function of line width. */
  arrow_wid = (arrow_len = (line_width == 1) ? 8 : 3*line_width - 1)/2;

  /* Initialize the beginning pointer. */
  xybeg = xy;

  /* Find the first point which is not so close to the end point that it */
  /* will be obscured by the arrowhead.                                  */
  for (i = 1; i < CONTRL[1]; i++)
  {
    /* Find the deltas between the next point and the end point.  Transform */
    /* to a space such that the aspect ratio is uniform and the x axis      */
    /* distance is preserved.                                               */
    xybeg += inc;
    dx = *xy - *xybeg;
    dy = SMUL_DIV(*(xy + 1) - *(xybeg + 1), ysize, xsize);

    /* Get the length of the vector connecting the point with the end point. */
    /* If the vector is of sufficient length, the search is over.            */
    if ( (line_len = vec_len(ABS(dx), ABS(dy))) >= arrow_len)
      break;
  }  /* End for:  over i. */

  /* If the longest vector is insufficiently long, don't draw an arrow. */
  if (line_len < arrow_len)
    return;

  /* Rotate the arrow-head height and base vectors.  Perform calculations */
  /* in 1000x space.                                                      */
  ht_x = SMUL_DIV(arrow_len, SMUL_DIV(dx, 1000, line_len), 1000);
  ht_y = SMUL_DIV(arrow_len, SMUL_DIV(dy, 1000, line_len), 1000);
  base_x = SMUL_DIV(arrow_wid, SMUL_DIV(dy, -1000, line_len), 1000);
  base_y = SMUL_DIV(arrow_wid, SMUL_DIV(dx, 1000, line_len), 1000);

  /* Transform the y offsets back to the correct aspect ratio space. */
  ht_y = SMUL_DIV(ht_y, xsize, ysize);
  base_y = SMUL_DIV(base_y, xsize, ysize);

  /* Save the parameter arrays we're about to squash.  Note that four */
  /* vertices must be saved because plygn will stomp on the fourth.   */
  sav_contrl = CONTRL[1];
  for (i = 0; i < 8; i++)
    PTSOUT[i] = PTSIN[i];

  /* Build a polygon to send to plygn.  Build into a local array first since */
  /* xy will probably be pointing to the PTSIN array.                        */
  CONTRL[1] = 3;
  triangle[0] = *xy + base_x - ht_x;
  triangle[1] = *(xy + 1) + base_y - ht_y;
  triangle[2] = *xy - base_x - ht_x;
  triangle[3] = *(xy + 1) - base_y - ht_y;
  triangle[4] = *xy;
  triangle[5] = *(xy + 1);
  for (i = 0; i < 6; i++)
    PTSIN[i] = triangle[i];
  plygn();

  /* Restore the squashed parameter arrays. */
  CONTRL[1] = sav_contrl;
  for (i = 0; i < 8; i++)
    PTSIN[i] = PTSOUT[i];

  /* Adjust the end point and all points skipped. */
  *xy -= ht_x;
  *(xy + 1) -= ht_y;
  while ( (xybeg -= inc) != xy)
  {
    *xybeg = *xy;
    *(xybeg + 1) = *(xy + 1);
  }  /* End while. */
}  /* End "arrow". */


init_wk()
{
	WORD	i,j,k;
	line_qi = INTIN[1];
	if ((line_qi > MAX_LINE_STYLE) || (line_qi < 1))
		line_qi = 1; 
	line_index = ( line_qi - 1);

	line_qc = INTIN[2];
	line_color = line_qc;
	if ((line_color >= MAX_COLOR) || (line_color < 0))
		line_color = 1; 
		line_color = MAP_COL[ line_color ];

	mark_qi = INTIN[3];
	if ((mark_qi > MAX_MARK_INDEX) || (mark_qi < 1))
		mark_qi = 3;
	mark_index = mark_qi - 1;

	mark_color = INTIN[4];
	if ((mark_color >= MAX_COLOR) || (mark_color < 0))
		mark_color = 1; 
	mark_qc = mark_color;
	mark_color = MAP_COL[ mark_color ];

	mark_height = DEF_MKHT;
	mark_scale = 1;

	fill_style = INTIN[7];
	if ((fill_style > MX_FIL_STYLE) || (fill_style < 0))
		fill_style = 0; 

	NEXT_PAT = 0;
	fill_qi = INTIN[8];
	if (fill_style == 2 )
	{
	    if ((fill_qi > MX_FIL_PAT_INDEX) || (fill_qi < 1))
	  	fill_qi = 1; 
	}
	else 
	{
	    if ((fill_qi > MX_FIL_HAT_INDEX) || (fill_qi < 1))
	  	fill_qi = 1; 
	}
	fill_index = fill_qi - 1;

	fill_color = INTIN[9];	    
	if ((fill_color >= MAX_COLOR) || (fill_color < 0))
		fill_color = 1; 
	fill_qc = fill_color;
	fill_color = MAP_COL[ fill_color ];

	xfm_mode = INTIN[10];

        st_fl_ptr();
	WRT_MODE  = 0;	 		/* default is replace mode	*/
	write_qm = 1;
	line_qw = line_width = DEF_LWID;
	line_beg = line_end = 0;	/* default to squared ends	*/
	loc_mode = 0; 			/* default is request mode	*/
	val_mode = 0;       		/* default is request mode	*/
	chc_mode = 0; 			/* default is request mode	*/
	str_mode = 0; 			/* default is request mode	*/
	fill_per = TRUE;		/* set fill perimeter on */
	HIDE_CNT = 1;			/* turn the cursor off as default */
	XMN_CLIP = 0;
	YMN_CLIP = 0;
	XMX_CLIP = xres;
	YMX_CLIP = yres;
	CLIP = 0;
	FLIP_Y = 1;

	CONTRL[2] = 6;
	CONTRL[4] = 45;
	for (i=0;i<45;i++)
		INTOUT[i] = DEV_TAB[i];
	for (i=0;i<12;i++)
		PTSOUT[i] = SIZ_TAB[i];
	
}
