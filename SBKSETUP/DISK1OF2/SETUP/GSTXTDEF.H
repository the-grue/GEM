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

/* gstxtdef.h	textual re-defines for GEMSETUP.EXE.			*/

#define		DEFAULT_DRV	gemroot		/* Starting drive to edit */
#define 	G_NUM_CHOICES   -1

#define		NO_SETUP_FUNC	null_procedure
#define		NO_TODO_FUNC	null_procedure

#define		GOODBYE		prompts[42]

#define		CONTINUE	choices[7]
#define		OTHER		choices[20]
#define		MORE		choices[21]

#define		CUR_SCRN	scrn_node[cur_screen-1]

#define	 	DESCRIPTION	gs_keywds[0]
#define	 	S_DESCRIPT	gs_keywds[1]
#define	 	L_DESCRIPT	gs_keywds[2]
#define	 	DRIVER_NAME	gs_keywds[3]
#define	 	FNT_WILDCRD	gs_keywds[4]
#define	 	MOUSE_ID	gs_keywds[5]
#define	 	SRC_DISK	gs_keywds[6]
#define	 	FONT_DISK	gs_keywds[7]
#define	 	SCAN_PORT	gs_keywds[8]
#define	 	SCAN_XFER	gs_keywds[9]
#define	 	SCAN_COM	gs_keywds[10]
#define	 	W_SCAN_PORT	port
#define	 	W_SCAN_XFER	mouse
#define	 	AUX_FILE	gs_keywds[11]

#define		DT_VOL		vol_floppies[0]
#define		SU_VOL		vol_floppies[1]
#define		SYS_VOL		vol_label[0]
#define		DPK_VOL		vol_label[8]

#define		DT_LABEL	label_floppies[0]
#define		SU_LABEL	label_floppies[1]
#define		SYS_LABEL       disk_label[0]
#define		DPK_LABEL       disk_label[7]

#define		W1_PROMPT	prompts[0]
#define		W1_CHOICES	choices[0]
#define		W1_NUM_CH	2
#define		W1_FOOTER       footers[0]

#define 	W2_PROMPT	prompts[58]
#define		W2_CHOICES	CONTINUE
#define		W2_NUM_CH	1
#define		W2_FOOTER	footers[2]

#define		W3_PROMPT	prompts[54]
#define		W3_E_PROMPT	prompts[58]
#define		W3_CHOICES	misc_choices[0]
#define		W3_NUM_CH	G_NUM_CHOICES
#define		W3_FOOTER	footers[2]

#define		W4_PROMPT	prompts[28]
#define		W4_CHOICES	choices[22]
#define		W4_NUM_CH	6
#define		W4_FOOTER	footers[0]

#define		W5_PROMPT	prompts[7]
#define		W5_CHOICES	choices[6]
#define		W5_NUM_CH	2
#define		W5_FOOTER	footers[0]

#define		W6_PROMPT	prompts[10]
#define		W6_CHOICES	choices[6]
#define		W6_NUM_CH	2
#define		W6_FOOTER	footers[0]

#define		W7_PROMPT	prompts[13]
#define		W7_CHOICES	CONTINUE
#define		W7_NUM_CH	1
#define		W7_FOOTER	footers[2]

#define		W8_PROMPT	prompts[16]
#define		W8_CHOICES	0
#define		W8_NUM_CH	NULLPTR
#define		W8_FOOTER	NULLPTR

#define		W9_PROMPT	prompts[16]
#define		W9_CHOICES	CONTINUE
#define		W9_NUM_CH	1
#define		W9_FOOTER	footers[2]

#define		W10_PROMPT	prompts[14]
#define		W10_CHOICES	CONTINUE
#define		W10_NUM_CH	1
#define		W10_FOOTER	footers[2]

#define		W11_PROMPT	prompts[22]
#define		W11_CHOICES	CONTINUE
#define		W11_NUM_CH	1
#define		W11_FOOTER	footers[2]

#define		W12_PROMPT	prompts[23]
#define		W12_CHOICES	misc_choices[0]
#define		W12_NUM_CH	G_NUM_CHOICES
#define		W12_FOOTER	footers[1]

#define		W13_PROMPT	prompts[23]
#define		W13_CHOICES	misc_choices[0]
#define		W13_NUM_CH	15
#define		W13_FOOTER	footers[1]

#define		W14_PROMPT	prompts[25]
#define		W14_CHOICES	misc_choices[0]
#define		W14_NUM_CH	G_NUM_CHOICES
#define		W14_FOOTER	footers[1]

#define		W15_PROMPT	prompts[26]
#define		W15_CHOICES	choices[11]
#define		W15_NUM_CH	2
#define		W15_FOOTER	footers[0]

#define		W16_PROMPT	prompts[49]
#define		W16_CHOICES	choices[4]
#define		W16_NUM_CH	2
#define		W16_FOOTER	footers[0]

#define		W17_PROMPT	misc_choices[0]
#define		W17_CHOICES	choices[13]
#define		W17_NUM_CH	2
#define		W17_FOOTER	footers[0]

#define		W18_PROMPT	prompts[29]
#define		W18_CHOICES	misc_choices[0]
#define		W18_NUM_CH	2
#define		W18_FOOTER	footers[0]

#define		W19_PROMPT	prompts[30]
#define		W19_CHOICES	misc_choices[0]
#define		W19_NUM_CH	2
#define		W19_FOOTER	footers[0]

#define		W20_PROMPT	prompts[31]
#define		W20_CHOICES	misc_choices[0]
#define		W20_NUM_CH	5
#define		W20_FOOTER	footers[0]

#define		W21_PROMPT	prompts[32]
#define		W21_CHOICES	misc_choices[0]
#define		W21_NUM_CH	G_NUM_CHOICES
#define		W21_FOOTER	footers[1]

#define		W22_PROMPT	prompts[33]
#define		W22_CHOICES	choices[8]
#define		W22_NUM_CH	5
#define		W22_FOOTER	footers[0]

#define		W23_PROMPT	prompts[34]
#define		W23_CHOICES	misc_choices[0]
#define		W23_NUM_CH	G_NUM_CHOICES
#define		W23_FOOTER	footers[1]

#define		W24_PROMPT	prompts[35]
#define		W24_CHOICES	choices[11]
#define		W24_NUM_CH	2
#define		W24_FOOTER	footers[0]

#define		W25_PROMPT	prompts[36]
#define		W25_CHOICES	misc_choices[0]
#define		W25_NUM_CH	G_NUM_CHOICES
#define		W25_FOOTER	footers[1]

#define		W26_PROMPT	prompts[37]
#define		W26_CHOICES	choices[11]
#define		W26_NUM_CH	2
#define		W26_FOOTER	footers[0]

#define		W27_PROMPT	prompts[38]
#define		W27_CHOICES	misc_choices[0]
#define		W27_NUM_CH	G_NUM_CHOICES
#define		W27_FOOTER	footers[1]

#define		W28_PROMPT	prompts[45]
#define		W28_CHOICES	misc_choices[0]
#define		W28_NUM_CH	G_NUM_CHOICES
#define		W28_FOOTER	footers[0]

#define		W29_PROMPT	prompts[46]
#define		W29_CHOICES	misc_choices[0]
#define		W29_NUM_CH	G_NUM_CHOICES
#define		W29_FOOTER	footers[0]

#define		W30_PROMPT	prompts[52]
#define		W30_CHOICES	CONTINUE
#define		W30_NUM_CH	1
#define		W30_FOOTER	footers[2] 

#define		W31_PROMPT	misc_choices[0]
#define		W31_CHOICES	0
#define		W31_NUM_CH	NULLPTR
#define		W31_FOOTER	NULLPTR

#define		W32_PROMPT	prompts[39]
#define		W32_CHOICES	CONTINUE
#define		W32_NUM_CH	1
#define		W32_FOOTER	footers[0]

#define		W33_PROMPT	prompts[47]
#define		W33_CHOICES	CONTINUE
#define		W33_NUM_CH	1
#define		W33_FOOTER	footers[0]

#define		W34_PROMPT	prompts[4]
#define		W34_CHOICES	CONTINUE
#define		W34_NUM_CH	1
#define		W34_FOOTER	footers[0]

#define		W35_PROMPT	prompts[60]
#define		W35_CHOICES	choices[6]
#define		W35_NUM_CH	2
#define		W35_FOOTER	footers[0]

#define		W36_PROMPT	misc_choices[0]
#define		W36_CHOICES	CONTINUE
#define		W36_NUM_CH	1
#define		W36_FOOTER	footers[2]

