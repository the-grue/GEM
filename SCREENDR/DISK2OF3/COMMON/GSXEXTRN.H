extern  WORD	NEXT_PAT;
extern	WORD	line_index,line_color, line_qi,line_qc,line_width,line_qw;
extern	WORD	num_qc_lines, q_circle[];
extern	WORD	line_beg, line_end;
extern	WORD	mark_height,mark_scale,mark_index,mark_color,mark_qi,mark_qc;
extern	WORD	fill_style,fill_index,fill_color,fill_per,fill_qi;
extern	WORD	fill_qc,fill_qp;
extern	WORD	val_mode,chc_mode,loc_mode,str_mode;
extern	WORD	write_qm;
extern	WORD	xfm_mode;

/* filled area variables */

extern	WORD	y,odeltay,deltay,deltay1,deltay2;
extern	WORD	fill_miny,fill_maxy;
extern	WORD	fill_intersect;
extern	WORD	fill_buffer[];
extern	WORD	*patptr;
extern	WORD	patmsk;

/* gdp area variables */
extern	WORD	xc, yc, xrad, yrad, del_ang, beg_ang, end_ang;
extern	WORD	start, angle, n_steps;

/* attribute environment save variables */
extern	WORD	s_fill_per, *s_patptr, s_patmsk, s_nxtpat;
extern	WORD	s_begsty, s_endsty;

extern  WORD	CLIP, XMN_CLIP, XMX_CLIP, YMN_CLIP, YMX_CLIP;
extern	WORD	LINE_STYL[];
extern	WORD 	DITHER[];
extern	WORD	HATCH0[],HATCH1[],OEMPAT[];
extern	WORD	UD_PATRN;
extern	WORD	SOLID;
extern	WORD	HOLLOW;
extern	WORD	HAT_0_MSK,HAT_1_MSK;
extern	WORD	DITHRMSK,OEMMSKPAT;

extern 	WORD	DEV_TAB[];		/* initial intout array for open workstation*/
extern 	WORD	SIZ_TAB[];		/* initial ptsout array for open workstation*/
extern	WORD	INQ_TAB[];		/* extended inquire values */
extern	WORD	INQ_PTS[];		/* extended inquire points */

extern 	WORD	CONTRL[],INTIN[],PTSIN[],INTOUT[],PTSOUT[];

extern	WORD	FG_BP_1,FG_BP_2,FG_BP_3,FG_BP_4;
extern	WORD	BG_BP_1,BG_BP_2,BG_BP_3,BG_BP_4;
extern	WORD	LN_MASK,LSTLIN;
extern	WORD	HIDE_CNT;
extern	WORD	MOUSE_BT;
extern	WORD	FLIP_Y;
extern	WORD	WRT_MODE;
extern	WORD	REAL_COL[3][MAX_COLOR];
extern	WORD	REQ_COL[3][MAX_COLOR];
extern	WORD	MAP_COL[];
extern	WORD	X1,Y1,X2,Y2;
extern	WORD	GCURX,GCURY,TERM_CH;


/* Assembly Language Support Routines */
extern	VOID	ABLINE(),HABLINE(),CLEARMEM();
extern	VOID	CHK_ESC(),INIT_G(),DINIT_G();
extern	VOID	CLC_FLIT();
extern	WORD	SMUL_DIV();
extern	WORD	HIDE_CUR(), DIS_CUR();

/* C Support routines */
extern	WORD 	isin();
extern	WORD 	icos();
extern  WORD	text_init();
extern	WORD	arb_corner();
