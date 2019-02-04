
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

#include    "portab.h"
#include    "jmptbl.h"

extern   WORD    CONTRL[];

WORD	(*jmptb1[])() =
{
	v_nop,
	v_opnwk,
	dinit_g,
	CLEARMEM,
	v_nop,
	chk_esc,
	v_pline,
	v_pmarker,
	d_gtext,
	plygn,
	v_nop,
	v_gdp,
	dst_height,
	dst_rotation,
	s_colmap,
	vsl_type,
	vsl_width,
	vsl_color,
	vsm_type,
	vsm_height,
	vsm_color,
	dst_font,
	dst_color,
	vsf_interior,
	vsf_style,
	vsf_color,
	vq_color,
	v_nop,
	v_locator,
	v_nop,
	v_choice,
	v_string, 
	vswr_mode,
	vsin_mode,
	v_nop,
	vql_attr,
	vqm_attr,
	vqf_attr,
	dqt_attributes,
	dst_alignment
};

WORD	(*jmptb2[])() =
{
	d_opnvwk,
	v_nop,
	vq_extnd,
	v_nop,
	vsf_perimeter,
	v_nop,
	dst_style,
	dst_point,
	vsl_ends,
	dro_cpyfm,
	TRAN_FM,
	XFM_CRFM,
	XFM_UDFL,
	vsl_udsty,
	dr_recfl,
	vqi_mode,
	dqt_extent,
	dqt_width,
	EX_TIMV,
	dt_loadfont,
	dt_unloadfont,
	drt_cpyfm,
	v_show_c,
	HIDE_CUR,
	vq_mouse_status,
	VEX_BUTV,
	VEX_MOTV,
	VEX_CURV,
	vq_key_s,
	s_clip,
	dqt_name,
	dqt_fontinfo,
	dqt_just
};


SCREEN() 
{
	CONTRL[2] = CONTRL[4] = 0;
	if (CONTRL[0] <= 39)
	    (*jmptb1[CONTRL[0]])();
	else if (CONTRL[0] >= 100 && CONTRL[0] <= 132 )
	    (*jmptb2[CONTRL[0] - 100])();
}
