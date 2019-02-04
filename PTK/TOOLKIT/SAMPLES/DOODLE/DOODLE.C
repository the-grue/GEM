/************************************************************************/
/*   File:   DOODLE.C                                                   */
/************************************************************************/
/*                                                                      */
/*           GGGGG        EEEEEEEE     MM      MM                       */
/*         GG             EE           MMMM  MMMM                       */
/*         GG   GGG       EEEEE        MM  MM  MM                       */
/*         GG   GG        EE           MM      MM                       */
/*           GGGGG        EEEEEEEE     MM      MM                       */
/*                                                                      */
/************************************************************************/
/*                                                                      */
/*           +--------------------------+                               */
/*           | Digital Research, Inc.   |                               */
/*           | 70 Garden Court          |                               */
/*           | Monterey, CA.     93940  |                               */
/*           +--------------------------+                               */
/*                                                                      */
/*   The  source code  contained in  this listing is a non-copyrighted  */
/*   work which  can be  freely used.  In applications of  this source  */
/*   code you  are requested to  acknowledge Digital Research, Inc. as  */
/*   the originator of this code.                                       */
/*                                                                      */
/*   Author:    Tom Rolander, Tim Oren                                  */
/*   PRODUCT:   GEM Sample Application                                  */
/*   Module:    DOODLE.C                                                */
/*   Version:   2.1                                                     */
/*   Modified:  Mitch Smith,      April 25, 1986                        */
/*   Modified:  Robert Schneider, April 20, 1989                        */
/*              Cleaned up for use with ANSI-C COMPILERS                */
/************************************************************************/

/*------------------------------*/
/*   includes                   */
/*------------------------------*/

#include "portab.h"              /* portable coding conv    */
#include "aes.h"                 /* everything for the AES  */
#include "vdi.h"                 /* everything for the VDI  */
#include "rclib.h"               /* everything for Raster   */
#include "oblib.h"               /* everything for Objects  */
#include "dosbind.h"             /* everything for the DOS  */
#include "doodle.h"              /* doodle apl resource     */

/*------------------------------*/
/*   defines                    */
/*------------------------------*/

#define  DESK           0       /* Desktop window handle */

#define  PEN_INK        BLACK
#define  PEN_ERASER     WHITE

#define  PEN_FINE       1
#define  PEN_MEDIUM     5
#define  PEN_BROAD      9

#define  N_COLORS       15L

#define  YSCALE(x)       x_mul_div(x, scrn_xsize, scrn_ysize) 

/************************************************************************
 *
 *             Data Structures
 *
 ************************************************************************/

/*------------------------------*/
/*   Local Procedures           */
/*------------------------------*/

WORD min              _(( WORD a, WORD b ));
WORD max              _(( WORD a, WORD b ));
BYTE FAR *string_addr _(( WORD which ));
VOID rast_op          _(( WORD mode, GRECT FAR *s_area, MFDB FAR *s_mfdb,
                                     GRECT FAR *d_area, MFDB FAR *d_mfdb ));
VOID vdi_fix          _(( MFDB FAR *pfd, VOID FAR *theaddr, WORD wb, WORD h ));
WORD vdi_trans        _(( WORD FAR *saddr, WORD swb,
                          WORD FAR *daddr, WORD dwb, WORD h ));
VOID trans_gimage     _(( OBJECT FAR *tree, WORD obj ));
VOID do_open          _(( WORD wh, WORD org_x, WORD org_y,
                          WORD x, WORD y, WORD w, WORD h ));
VOID do_close         _(( WORD wh, WORD org_x, WORD org_y ));
VOID set_clip         _(( WORD clip_flag, GRECT FAR *s_area ));
VOID draw_rect        _(( GRECT FAR *area ));
WORD align_x          _(( WORD x ));
VOID do_top           _(( WORD wdw_hndl ));
VOID move_do          _(( OBJECT FAR *tree, WORD inc ));
VOID redraw_do        _(( OBJECT FAR *tree, WORD obj ));
WORD xtend_do         _(( OBJECT FAR *tree, WORD obj ));
WORD hndl_dial        _(( OBJECT FAR *tree, WORD def,
                          WORD x, WORD y, WORD w, WORD h ));
WORD dr_code          _(( VOID ));
VOID set_work         _(( WORD slider_update ));
VOID save_work        _(( VOID ));
VOID restore_work     _(( VOID ));
VOID enab_menu        _(( WORD which ));
VOID disab_menu       _(( WORD which ));
WORD get_parent       _(( OBJECT FAR *tree, WORD obj ));
WORD dial_name        _(( BYTE *name ));
VOID get_path         _(( BYTE *tmp_path, BYTE *spec ));
VOID add_file_name    _(( BYTE *dname, BYTE *fname ));
WORD get_file         _(( WORD loop ));
VOID cursor           _(( WORD color ));
VOID curs_on          _(( VOID ));
VOID curs_off         _(( VOID ));
WORD hndl_menu        _(( WORD title, WORD item ));
VOID do_about         _(( VOID ));
VOID do_load          _(( WORD need_name ));
WORD do_save          _(( VOID ));
VOID do_svas          _(( VOID ));
VOID set_pen          _(( WORD pen, WORD height ));
VOID set_eraser       _(( WORD pen, WORD height, MFORM FAR *eraser ));
VOID set_color        _(( OBJECT FAR *tree, WORD color ));
WORD get_color        _(( OBJECT FAR *tree ));
VOID do_penselect     _(( VOID ));
VOID do_erase         _(( VOID ));
WORD hndl_keyboard    _(( VOID ));
WORD hndl_msg         _(( VOID ));
VOID do_redraw        _(( WORD wh, GRECT FAR *area ));
VOID do_full          _(( WORD wh ));
WORD hndl_mouse       _(( VOID ));
WORD hndl_button      _(( VOID ));
WORD draw_pencil      _(( UWORD x, UWORD y ));
VOID eraser           _(( WORD x, WORD y ));
WORD demo             _(( VOID ));
WORD demo_term        _(( WORD term_type ));
VOID pict_init        _(( VOID ));
WORD demo_init        _(( VOID ));
WORD main             _(( VOID ));

/*------------------------------*/
/*   Global Data Structures     */
/*------------------------------*/

GLOBAL WORD contrl[11];          /* control inputs             */
GLOBAL WORD intin[80];           /* max string length          */
GLOBAL WORD ptsin[256];          /* polygon fill points        */
GLOBAL WORD intout[45];          /* open workstation output    */
GLOBAL WORD ptsout[12];


/*------------------------------*/
/*   Local Data Structures      */
/*------------------------------*/

WORD gl_wchar;                   /* character width                           */
WORD gl_hchar;                   /* character height                          */
WORD gl_wbox;                    /* box (cell) width                          */
WORD gl_hbox;                    /* box (cell) height                         */
WORD gl_hspace;                  /* height of space between lines             */
WORD gem_handle;                 /* GEM vdi handle                            */
WORD vdi_handle;                 /* doodle vdi handle                         */
WORD work_out[57];               /* open virt workstation values              */
GRECT scrn_area;                 /* whole screen area                         */
GRECT work_area;                 /* drawing area of main window               */
GRECT undo_area;                 /* area equal to work_area                   */
GRECT save_area;                 /* save area for full/unfulling              */
WORD gl_rmsg[8];                 /* message buffer                            */
OBJECT FAR *gl_menu;             /* menu tree address                         */
WORD gl_apid;                    /* application ID                            */
WORD gl_xfull;                   /* full window 'x'                           */
WORD gl_yfull;                   /* full window 'y'                           */
WORD gl_wfull;                   /* full window 'w'                           */
WORD gl_hfull;                   /* full window 'h'                           */
WORD scrn_width;                 /* screen width in pixels                    */
WORD scrn_height;                /* screen height in pixels                   */
WORD scrn_planes;                /* number of color planes                    */
WORD scrn_xsize;                 /* width of one pixel                        */
WORD scrn_ysize;                 /* height of one pixel                       */
UWORD m_out = FALSE;             /* mouse in/out of window flag               */
WORD ev_which;                   /* event multi return state(s)               */
WORD mousex, mousey;             /* mouse x,y position                        */
WORD bstate, bclicks;            /* button state, & # of clicks               */
WORD kstate;                     /* key state                                 */
UWORD kreturn;                   /* keyboard char                             */
MFDB undo_mfdb;                  /* undo buffer mmry frm def blk              */
MFDB scrn_mfdb;                  /* screen memory form defn blk               */
LONG buff_size;                  /* buffer size req'd for screen              */
VOID FAR *buff_location;         /* screen buffer pointer                     */
WORD demo_whndl;                 /* demo window handle                        */
WORD demo_shade = PEN_INK;       /* demo current pen shade                    */
WORD pen_shade = PEN_INK;        /* saved pen shade                           */
WORD demo_pen = 1;               /* demo current pen width                    */
WORD demo_height = 4;            /* demo current char height                  */
WORD char_fine;                  /* character height for fine                 */
WORD char_medium;                /* character height for medium               */
WORD char_broad;                 /* character height for broad                */
WORD monumber = 5;               /* mouse form number                         */
MFORM FAR *mofaddr = 0l;         /* mouse form address                        */
WORD file_handle;                /* file handle -> pict ld/sv                 */
BYTE file_name[64] = "";         /* current pict file name                    */
BYTE name[13] = "";              /* Save As pict file name                    */
BOOLEAN key_input;               /* key inputting state                       */
WORD key_xbeg;                   /* x position for line beginning             */
WORD key_ybeg;                   /* y position for line beginning             */
WORD key_xcurr;                  /* current x position                        */
WORD key_ycurr;                  /* current y position                        */
                                 /* demo window title                         */
WORD col1_curr = 0;              /* current color1 index in color_sel-array   */
BYTE FAR *wdw_title = " GEM Doodle Window ";

WORD usercolor[2] = {1, 0};
MFDB userbrush_mfdb;             /* MFDB for Prog def objects                 */
APPLBLK brushab[6];              /* 6 Programmer defined objects              */
 
LONG   color_sel[N_COLORS+1] = { /* data for scrolling                        */
                                 /*  color selector                           */
   N_COLORS,
   0x31FF1071L,
   0x32FF1072L,
   0x33FF1073L,
   0x34FF1074L,
   0x35FF1075L,
   0x36FF1076L,
   0x37FF1077L,
   0x38FF1078L,
   0x39FF1079L,
   0x41FF107AL,
   0x42FF107BL,
   0x43FF107CL,
   0x44FF107DL,
   0x45FF107EL,
   0x46FF107FL 
};

/*------------------------------*/
/*   Mouse Data Structures      */
/*------------------------------*/

/*
 * mouse form for broad eraser
 */
MFORM erase_broad =
{
   7, 7, 1, 0, 1,
   0x0000, 0x0000, 0x0000, 0x0000,     /* mask */
   0x0000, 0x1ff0, 0x1ff0, 0x1ff0,
   0x1ff0, 0x1ff0, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,     /* data */
   0x7ffc, 0x600c, 0x600c, 0x600c,
   0x600c, 0x600c, 0x7ffc, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000
};

/*
 * mouse form for medium eraser
 */
MFORM erase_medium =
{
   7, 7, 1, 0, 1,
   0x0000, 0x0000, 0x0000, 0x0000,     /* mask */
   0x0000, 0x0000, 0x07c0, 0x07c0,
   0x07c0, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,     /* data */
   0x0000, 0x1ff0, 0x1830, 0x1830,
   0x1830, 0x1ff0, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000
};

/*
 * mouse form for fine eraser
 */
MFORM erase_fine =
{
   7, 7, 1, 0, 1,
   0x0000, 0x0000, 0x0000, 0x0000,     /* mask */
   0x0000, 0x0000, 0x0000, 0x0100,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,     /* data */
   0x0000, 0x0000, 0x07c0, 0x06c0,
   0x07c0, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000
};

/************************************************************************
 *
 *             Local Procedures
 *
 ************************************************************************/

/*------------------------------*/
/*   min                        */
/*------------------------------*/
WORD min(a, b)                      /* return min of two values               */
WORD a, b;
{
   return( (a < b) ? a : b );
}

/*------------------------------*/
/*   max                        */
/*------------------------------*/
WORD max(a, b)                      /* return max of two values               */
WORD a, b;
{
   return( (a > b) ? a : b );
}

/*------------------------------*/
/*   string_addr                */
/*------------------------------*/
BYTE FAR *string_addr(which)        /* returns a tedinfo LONG string addr     */
WORD which;
{
   BYTE FAR *string;

   rsrc_gaddr(R_STRING, which, &string);
   return (string);
}

/*------------------------------*/
/*   rast_op                    */
/*------------------------------*/
VOID rast_op(mode, s_area, s_mfdb, d_area, d_mfdb)   /* bit block level trns */
WORD mode;
GRECT FAR *s_area, FAR *d_area;
MFDB FAR *s_mfdb, FAR *d_mfdb;
{
   WORD   pxy[8];

   rc_grect_to_array(s_area, pxy);
   rc_grect_to_array(d_area, &pxy[4]);
                                          /* pixel for pixel source           */
                                          /*  to destination copy             */
   vro_cpyfm(vdi_handle, mode, pxy, s_mfdb, d_mfdb);  
}

/*------------------------------*/
/*   vdi_fix                    */
/*------------------------------*/
VOID vdi_fix(pfd, theaddr, wb, h)   /* set up MFDB for transform              */
MFDB FAR *pfd;
VOID FAR *theaddr;
WORD wb, h;
{
   pfd->fww = wb >> 1;              /* # of bytes to words                    */
   pfd->fwp = wb << 3;              /* # of bytes to to pixels                */
   pfd->fh = h;                     /* height in scan lines                   */
   pfd->np = 1;                     /* number of planes                       */
   pfd->mp = theaddr;               /* memory pointer                         */
}

/*------------------------------*/
/*   vdi_trans                  */
/*------------------------------*/
WORD vdi_trans(saddr, swb, daddr, dwb, h)    /* 'on the fly' transform        */
WORD FAR *saddr;
WORD swb;
WORD FAR *daddr;
WORD dwb;
WORD h;
{
   MFDB      src, dst;                    /* local MFDB                       */

   vdi_fix((MFDB FAR *)&src, saddr, swb, h);
   src.ff = TRUE;                         /* standard format                  */

   vdi_fix((MFDB FAR *)&dst, daddr, dwb, h);
   dst.ff = FALSE;                        /* transform to device              */
                                          /* specific format                  */
   vr_trnfm(vdi_handle, &src, &dst ); 
}

/*------------------------------*/
/*   trans_gimage               */
/*------------------------------*/
VOID trans_gimage(tree, obj)        /* transform bit images and icons         */
OBJECT FAR *tree;
WORD obj;
{
   ICONBLK FAR *icblkp;
   BITBLK FAR *bitblkp;

   if (OB_TYPE(tree,obj) == G_ICON)
   {
      icblkp = (ICONBLK FAR *)OB_SPEC(tree,obj);
                                                         /* Transform Mask    */
      vdi_trans(icblkp->ib_pmask, icblkp->ib_wicon >> 3,
                icblkp->ib_pmask, icblkp->ib_wicon >> 3, icblkp->ib_hicon);

                                                         /* Transform Data    */
      vdi_trans(icblkp->ib_pdata, icblkp->ib_wicon >> 3,
                icblkp->ib_pdata, icblkp->ib_wicon >> 3, icblkp->ib_hicon);
   }
   else
   {
      bitblkp = (BITBLK FAR *)OB_SPEC(tree,obj);
                                                         /* Transform Data    */
      vdi_trans(bitblkp->bi_pdata, bitblkp->bi_wb,
                bitblkp->bi_pdata, bitblkp->bi_wb, bitblkp->bi_hl);
   }
}

/*------------------------------*/
/*   do_open                    */
/*------------------------------*/
VOID do_open(wh, org_x, org_y, x, y, w, h)   /* grow and open specified wdw   */
WORD wh;
WORD org_x, org_y;
WORD x, y, w, h;
{
   graf_growbox(org_x, org_y, 21, 21, x, y, w, h);
   wind_open(wh, x, y, w, h);
}

/*------------------------------*/
/*   do_close                   */
/*------------------------------*/
VOID do_close(wh, org_x, org_y)     /* close and shrink specified window      */
WORD wh;
WORD org_x, org_y;
{
   WORD   x, y, w, h;

   wind_get(wh, WF_CXYWH, &x, &y, &w, &h);
   wind_close(wh);
   graf_shrinkbox(org_x, org_y, 21, 21, x, y, w, h);
}

/*------------------------------*/
/*   set_clip                   */
/*------------------------------*/
VOID set_clip(clip_flag, s_area)    /* set clip to specified area             */
WORD clip_flag;
GRECT FAR *s_area;
{
   WORD   pxy[4];

   rc_grect_to_array(s_area, pxy);
   vs_clip(vdi_handle, clip_flag, pxy); 
}

/*------------------------------*/
/*   draw_rect                  */
/*------------------------------*/
VOID draw_rect(area)                /* used by dr_code() to draw a            */
GRECT FAR *area;                    /* rectangle around pen/eraser            */
{
   WORD   pxy[10];

   pxy[0] = area->g_x;
   pxy[1] = area->g_y;
   pxy[2] = area->g_x + area->g_w - 1;
   pxy[3] = area->g_y + area->g_h - 1;
   pxy[4] = pxy[2];
   pxy[5] = pxy[3];
   pxy[3] = pxy[1];
   pxy[6] = pxy[0];
   pxy[7] = pxy[5];
   pxy[8] = pxy[0];
   pxy[9] = pxy[1];
   v_pline(vdi_handle, 5, pxy);
}

/*------------------------------*/
/*   align_x                    */
/*------------------------------*/
WORD align_x(x)                 /* forces word alignment for column position */
WORD x;                         /* rounding to nearest word                  */
{
   return((x & 0xfff0) + ((x & 0x000c) ? 0x0010 : 0));
}   

/*------------------------------*/
/*   do_top                     */
/*------------------------------*/
VOID do_top(wdw_hndl)               /* top the window if not already active   */
WORD wdw_hndl;
{ 
   WORD active, dummy;

   wind_get(wdw_hndl, WF_TOP, &active, &dummy, &dummy, &dummy);
   if ( wdw_hndl != active )   
      wind_set(wdw_hndl, WF_TOP, 0, 0, 0, 0);  
}

/************************************************************************
 *
 *         Advanced Dialog Handling
 *
 ************************************************************************/

/*------------------------------*/
/*   move_do                    */
/*------------------------------*/
VOID move_do(tree, inc)             /* routine to scroll the color selector   */
OBJECT FAR *tree;                   /* in the Pen/Eraser Dialog               */
WORD inc;
{
   REG WORD a,b;

   b = (inc > 0) ? col1_curr + 1: col1_curr - 1;
   if (b < 0)
      b = N_COLORS - 1;

   for (a = 0; a < 4; a++)
   {
      if (b >= N_COLORS)
         b = 0;
      if (a == 0)
         col1_curr = b;
      OB_SPEC(tree,DCOL1 + a) = color_sel[b + 1];
      b++;
   }

   redraw_do(tree, DEMOPCLR);
}

/*------------------------------*/
/*   redraw_do                  */
/*------------------------------*/
VOID redraw_do(tree, obj)           /* Routine to draw a specified object     */
OBJECT FAR *tree;                   /* in a tree                              */
{
   GRECT o;

   ob_xywh(tree, obj, &o);
   o.g_x -= 3; o.g_y -= 3; o.g_w += 6; o.g_h += 6;
   objc_draw(tree, ROOT, MAX_DEPTH, o.g_x, o.g_y, o.g_w, o.g_h);
}

/*------------------------------*/
/*   xtend_do                   */
/*------------------------------*/
WORD xtend_do(tree, obj)            /* Called by hndl_dial() if extended      */
OBJECT FAR *tree;                   /* type object is the exit object         */
WORD obj;
{
   LONG obspec;

   switch (obj)
   {
      case DCOL1:                         /* selected color */
      case DCOL2:
      case DCOL3:
      case DCOL4:
         OB_SPEC(tree,DCOLAKT) = OB_SPEC(tree,obj);
         redraw_do(tree, DCOLAKT);
         break;

      case DCOLFWD:                         /* forward arrow  */
         move_do(tree, 1);
         redraw_do(tree, obj);
         break;

      case DCOLBAK:                         /* backward arrow */
         move_do(tree, -1);
         redraw_do(tree, obj);
         break;

      default:
         break;
   }

   return(FALSE);
}

/*------------------------------*/
/*   hndl_dial                  */
/*------------------------------*/
WORD hndl_dial(tree, def, x, y, w, h)  /*  General purpose dialogue           */
OBJECT FAR *tree;                      /*  handler. Provides for              */
WORD def;                              /*  extended object type               */
WORD x, y, w, h;                       /*  checking and 'local'               */
{                                      /*  processing of extended             */
                                       /*  type objects                       */
   WORD exitobj;
   WORD xtype;

   ob_draw_dialog(tree, x, y, w, h);

   FOREVER
   {
      exitobj = form_do(tree, def) & 0x7FFF;
      xtype = OB_EXTYPE(tree,exitobj);
      if (!xtype)                                     /* is not extended type */
         break;
      if (xtend_do(tree, exitobj)) 
         break;
   }

   ob_undraw_dialog(tree, x, y, w, h);

   return (exitobj);
}

/*------------------------------*/
/*   dr_code                    */
/*------------------------------*/   
WORD dr_code()                      /* called by the AES                      */
{
   PARMBLK FAR *pbp, FAR *fardr_start();
   WORD pxy[10], hl, wb;
   BITBLK FAR *bpp;

   pbp = fardr_start();

   set_clip(TRUE, (GRECT FAR *) &pbp->pb_xc);

   bpp = (BITBLK FAR *)pbp->pb_parm;      /* original obspec                  */
   userbrush_mfdb.mp = bpp->bi_pdata;     /* point to data                    */
   hl = bpp->bi_hl;                       /* height in scan lines             */
   wb = bpp->bi_wb;                       /* width in bytes                   */

   userbrush_mfdb.fwp = wb << 3;          /* Set up the MFDB                  */
   userbrush_mfdb.fww = wb >> 1;          /* in preparation                   */
   userbrush_mfdb.fh = hl;                /* for a transform                  */
   userbrush_mfdb.np = 1;                 /* monochrome assumed.              */
   userbrush_mfdb.ff = 0;                 /* Device specific format           */

   pxy[0] = pxy[1] = 0;
   pxy[2] = (wb << 3) - 1;
   pxy[3] = hl - 1;
   pxy[4] = pbp->pb_x; 
   pxy[5] = pbp->pb_y;
   pxy[6] = pxy[4] + pbp->pb_w - 1;
   pxy[7] = pxy[5] + pbp->pb_h - 1;
   
                                          /* Copy Raster Transparent          */

   vrt_cpyfm(vdi_handle, 2, pxy, &userbrush_mfdb, &scrn_mfdb, usercolor);

   if((pbp->pb_currstate != pbp->pb_prevstate) || (pbp->pb_currstate & SELECTED))
   {
      if (pbp->pb_currstate & SELECTED)
         vsl_color(vdi_handle,BLACK);
      else
         vsl_color(vdi_handle,WHITE);
      vsl_width(vdi_handle, 1);
      vsl_type(vdi_handle, FIS_SOLID);

      pbp->pb_x--;
      pbp->pb_y--;
      pbp->pb_w += 2;
      pbp->pb_h += 2;
      draw_rect((GRECT FAR *) &pbp->pb_x);
    }  
   
   set_clip(FALSE, (GRECT FAR *) &work_area);

   fardr_end(0);
}

/************************************************************************
 *
 *             Work Area Management
 *
 ************************************************************************/

/*------------------------------*/
/*   set_work                   */
/*------------------------------*/
VOID set_work(slider_update)        /* Update undo area, clamping to page     */
BOOLEAN slider_update;              /* edges, and updt sliders if req'd       */
{
   WORD i;

   wind_get(demo_whndl, WF_WXYWH, &work_area.g_x, &work_area.g_y,
                                  &work_area.g_w, &work_area.g_h);

   undo_area.g_w = work_area.g_w;
   undo_area.g_h = work_area.g_h;
                                    /* clamp work area to page edges          */
   undo_area.g_x = align_x(undo_area.g_x);
   if ((i = undo_mfdb.fwp - (undo_area.g_x + undo_area.g_w)) < 0)
      undo_area.g_x += i;
   if ((i = undo_mfdb.fh - (undo_area.g_y + undo_area.g_h)) < 0)
      undo_area.g_y += i;

   if (slider_update)
   {
                                    /* set slider positions                   */

      wind_set(demo_whndl, WF_HSLIDE, x_mul_div(undo_area.g_x,   1000,
         undo_mfdb.fwp - undo_area.g_w), 0, 0, 0);
      wind_set(demo_whndl, WF_VSLIDE, x_mul_div(undo_area.g_y, 1000,
         undo_mfdb.fh - undo_area.g_h), 0, 0, 0);
      wind_set(demo_whndl, WF_HSLSIZE, x_mul_div(work_area.g_w, 1000,
         undo_mfdb.fwp), 0, 0, 0);
      wind_set(demo_whndl, WF_VSLSIZE, x_mul_div(work_area.g_h, 1000,
         undo_mfdb.fh), 0, 0, 0);
   }

                                   /* only use portion of work_area on screen */
   rc_intersect(&scrn_area, &work_area);
   undo_area.g_w = work_area.g_w;
   undo_area.g_h = work_area.g_h;
}

/*------------------------------*/
/*   save_work                  */
/*------------------------------*/
VOID save_work()                    /* copy work_area to undo_area buffer     */
{
   GRECT tmp_area;

   rc_copy(&work_area,&tmp_area);
   rc_intersect(&scrn_area,&tmp_area);
   graf_mouse(M_OFF, (MFORM *)0l);                          /* turn mouse off */
   rast_op(3, &tmp_area, &scrn_mfdb, &undo_area, &undo_mfdb);
   graf_mouse(M_ON, (MFORM *)0l);                           /* turn mouse on  */
}

/*------------------------------*/
/*   restore_work               */
/*------------------------------*/
VOID restore_work()                 /* restore work_area from undo_area       */
{
   GRECT tmp_area;

   rc_copy(&work_area,&tmp_area);
   rc_intersect(&scrn_area,&tmp_area);
   graf_mouse(M_OFF, (MFORM *)0l);
   rast_op(3, &undo_area, &undo_mfdb, &tmp_area, &scrn_mfdb);
   graf_mouse(M_ON, (MFORM *)0l);
}

/************************************************************************
 *
 *             Object Tree Manipulation
 *
 ************************************************************************/

/*------------------------------*/
/*   enab_menu                  */
/*------------------------------*/
VOID enab_menu(which)               /* enable specified menu item             */
WORD which;
{
   ob_undostate(gl_menu, which, DISABLED);
}

/*------------------------------*/
/*   disab_menu                 */
/*------------------------------*/
VOID disab_menu(which)              /* disable specified menu item            */
WORD which;
{
   ob_dostate(gl_menu, which, DISABLED);   
}

/*------------------------------*/
/*   get_parent                 */
/*------------------------------*/
/*
 *   Routine that will find the parent of a given object.  The
 *   idea is to walk to the end of our siblings and return
 *   our parent.  If object is the root then return NIL as parent.
 */
WORD get_parent(tree, obj)
OBJECT FAR *tree;
WORD obj;
{
   WORD pobj;

   if (obj == NIL)
      return (NIL);

   if ((pobj = OB_NEXT(tree,obj)) != NIL)
   {
      while( OB_TAIL(tree,pobj) != obj ) 
      {
         obj = pobj;
         pobj = OB_NEXT(tree,obj);
      }
   }

   return(pobj);
} 

/************************************************************************
 *
 *             File Path Name Functions
 *
 ************************************************************************/

/*------------------------------*/
/*   dial_name                  */
/*------------------------------*/
WORD dial_name ( name )             /* dialogue box input filename            */
BYTE *name;
{
   OBJECT FAR *tree;
   TEDINFO FAR *ted_addr;
   BYTE c;   
   WORD i, j;
   GRECT box;

   ob_xywh(gl_menu, DEMOFILE, &box);
   rsrc_gaddr( R_TREE, DEMOSVAD, &tree);
   ted_addr = (TEDINFO FAR *)OB_SPEC(tree,DEMONAME);/* get obspec pointer     */
   ted_addr->te_ptext = name;                   /* set obspec pointer         */
   ted_addr->te_txtlen = 9;

   name[0] = '\0';   /* null to clear edit field and position cursor          */

   if (hndl_dial(tree, DEMONAME, box.g_x, box.g_y, box.g_w, box.g_h) == DEMOSOK)
   {
      i = j =  0;
      while (TRUE)   /* parse filename string */
      {
         c = name[i++];
         if (!c) 
            break ; 
         if ( (c != ' ') && (c != '_') )
            name[j++] = c ;
      }
      if ( *name )
         strcpy( &name[j], ".DOO" ); /* add extension   */
      ob_undostate(tree, DEMOSOK, SELECTED);
      return (TRUE); 
   }
   else
   {
      ob_undostate(tree, DEMOSCNL, SELECTED);
      return (FALSE);
   }
}

/*------------------------------*/
/*   get_path                   */
/*------------------------------*/
VOID get_path(tmp_path, spec)       /* get directory path name                */
BYTE *tmp_path, *spec;
{
   WORD cur_drv;

   cur_drv = dos_gdrv();
   tmp_path[0] = cur_drv + 'A';
   tmp_path[1] = ':';
   tmp_path[2] = '\\';
   dos_gdir(cur_drv+1, &tmp_path[3]);
   if (strlen(tmp_path) > 3)
      strcat(tmp_path, "\\");
   else
      tmp_path[2] = '\0';
   strcat(tmp_path, spec);
}

/*------------------------------*/
/*   add_file_name              */
/*------------------------------*/
VOID add_file_name(dname, fname)    /* replace name at end of input file spec */
BYTE *dname, *fname;
{
   BYTE   c;
   WORD   ii;

   ii = strlen(dname);
   while (ii && (((c = dname[ii-1])  != '\\') && (c != ':')))
      ii--;
   dname[ii] = '\0';
   strcat(dname, fname);
}

/*------------------------------*/
/*   get_file                   */
/*------------------------------*/
WORD get_file(loop)                 /* use file selector to get input file    */
BOOLEAN loop;
{
   WORD fs_iexbutton;
   BYTE fs_iinsel[13];

   while (TRUE)
   {
      get_path(file_name, "*.DOO");
      fs_iinsel[0] = '\0'; 

      fsel_input(file_name, fs_iinsel, &fs_iexbutton);
      if (fs_iexbutton)
      {
         add_file_name(file_name, fs_iinsel);
         file_handle = dos_open(file_name,2);
         if (!loop || (loop && !DOS_ERR))
            return(1);
      }
      else   
      {
         disab_menu(DEMOSAVE);
         disab_menu(DEMOABAN);
         return(0);   
      }
   }
}

/************************************************************************
 *
 *             Soft Cursor Support
 *
 ************************************************************************/

/*------------------------------*/
/*   cursor                     */
/*------------------------------*/
VOID cursor(color)                  /* turn cursor on,  color = BLACK         */
WORD color;                         /* or cursor off, color = WHITE           */
{
   WORD pxy[4];

   pxy[0] = key_xcurr + 1;
   pxy[1] = key_ycurr + gl_hspace;
   pxy[2] = key_xcurr + 1;
   pxy[3] = key_ycurr - gl_hbox;

   vsl_color(vdi_handle,color);
   vswr_mode(vdi_handle,MD_REPLACE);
   vsl_type (vdi_handle,FIS_SOLID);
   vsl_width (vdi_handle,PEN_FINE);
   graf_mouse(M_OFF, (MFORM *)0l);
   set_clip(TRUE, (GRECT FAR *)&work_area);
   v_pline(vdi_handle, 2, pxy);
   set_clip(FALSE, (GRECT FAR *)&work_area);
   graf_mouse(M_ON, (MFORM *)0l);
}

/*------------------------------*/
/*   curs_on                    */
/*------------------------------*/
VOID curs_on()                      /* turn 'soft' cursor 'on'                */
{
   cursor(pen_shade);
}

/*------------------------------*/
/*   curs_off                   */
/*------------------------------*/
VOID curs_off()                     /* turn 'soft' cursor 'off'               */
{
   cursor(PEN_ERASER);
}

/************************************************************************
 *
 *             Menu Handling
 *
 ************************************************************************/

/*------------------------------*/
/*   hndl_menu                  */
/*------------------------------*/
WORD hndl_menu(title, item)
WORD title, item;
{
   WORD done;

   graf_mouse(ARROW, (MFORM *)0l);
   done = FALSE;

   switch (item)
   {
      case DEMOINFO: /* Desk menu */
         do_about();   
         break;
      case DEMOLOAD: /* Load File    */
         do_load(TRUE);
         break;
      case DEMOSAVE: /* Save File    */
         do_save();
         break;
      case DEMOSVAS: /* Save File As */
         do_svas();
         break;
      case DEMOABAN: /* Abandon File */
         file_handle = dos_open(file_name, 2);
         do_load(FALSE);
         break;
      case DEMOQUIT: /* Quit - Exit back to Desktop   */
         done = TRUE;
         break;
      case DEMOPENS: /* Pen/Eraser Selection    */
         do_penselect();
         break;
      case DEMOERAP: /* Erase Picture    */
         do_top(demo_whndl);
         do_erase();
         break;
   }

   menu_tnormal(gl_menu, title, TRUE);
   graf_mouse(monumber, mofaddr); 

   return (done);   
}

/*------------------------------*/
/*   do_about                   */
/*------------------------------*/
VOID do_about()                     /* display Demo Info...                   */
{
   OBJECT FAR *tree;
   GRECT box;

   ob_xywh(gl_menu, DEMODESK, &box);          /* DESK menu title xywh       */
   rsrc_gaddr(R_TREE, DEMOINFD, &tree);         /* address of DEMOINFD        */
   hndl_dial(tree, 0, box.g_x, box.g_y, box.g_w, box.g_h);
   ob_undostate(tree, DEMOOK, SELECTED);        /* deselect OK button         */
}

/*------------------------------*/
/*   do_load                    */
/*------------------------------*/
VOID do_load(need_name)             /* load demo picture file                 */
BOOLEAN need_name;
{
   if (!need_name || get_file(TRUE))
   {
      if (!DOS_ERR)
      {
         dos_read(file_handle, buff_size,buff_location);
         dos_close(file_handle);
         enab_menu(DEMOSAVE);
         enab_menu(DEMOABAN);
         restore_work();
      }
   }
}

/*------------------------------*/
/*   do_save                    */
/*------------------------------*/
WORD do_save()                      /* save current named demo picture        */
{
   if (*file_name)
   {
      file_handle = dos_open(file_name,2);
      if (!DOS_ERR)
      {    /* File already exists    */
         if (form_alert(1, string_addr(DEMOOVWR)) == 2)
         {
            dos_close(file_handle);
            return(FALSE);                      /* Cancel - dont't overwrite  */
         }
      }

      if(DOS_ERR) 
         file_handle = dos_create(file_name,0); 
      if(DOS_ERR)
      { 	/* disable Save and Abandon	*/ 
         disab_menu(DEMOSAVE);
         disab_menu(DEMOABAN);
         return(FALSE);
      }

      dos_write(file_handle, buff_size, buff_location);   
      dos_close(file_handle);
      enab_menu(DEMOSAVE);
      enab_menu(DEMOABAN);
      return(TRUE); 
   }
}

/*------------------------------*/
/*   do_save_as                 */
/*------------------------------*/
VOID do_svas()                      /* save demo picture as named             */
{
   if (dial_name(name))
   { 
      if (name[0] != '\0')
      {
         add_file_name(file_name, name);
         do_save(); 
      }
   }
}

/*------------------------------*/
/*   set_pen                    */
/*------------------------------*/
VOID set_pen(pen, height)           /* set pen width and height               */
WORD pen, height;
{
   demo_pen = pen;
   demo_height = height;
   monumber = 5;   /* thin cross hair */
   mofaddr = 0x0L;
}

/*------------------------------*/
/*   set_eraser                 */
/*------------------------------*/
VOID set_eraser(pen, height, eraser)   /* set mouse form to eraser            */
WORD pen, height;
MFORM FAR *eraser;
{
   demo_pen = pen;
   demo_height = height;
   demo_shade = PEN_ERASER;
   monumber = 255;
   mofaddr = eraser;
}      

/*------------------------------*/
/*   set_color                  */
/*------------------------------*/
VOID set_color(tree, color)         /*  Set Pen Color Selection               */
OBJECT FAR *tree;
WORD color;
{
   OB_SPEC(tree,DCOLAKT) = color_sel[color];
}

/*------------------------------*/
/*   get_color                  */
/*------------------------------*/
WORD get_color(tree)                /* Get Pen Color Selection                */
OBJECT FAR *tree;
{
   return ((WORD)(OB_SPEC(tree,DCOLAKT) & 0xf));
}

/*------------------------------*/
/*   do_penselect               */
/*------------------------------*/
VOID do_penselect()                 /* Use dialogue box to input selection    */
{                                   /* of specified pen/eraser                */
   WORD exit_obj, psel_obj, color;
   OBJECT FAR *tree;
   LONG bind[2];
   GRECT box;

   ob_xywh(gl_menu, DEMOPENS, &box);
   rsrc_gaddr(R_TREE, DEMOPEND, &tree);
                                    /* first setup current selection state    */
   switch (demo_pen)
   {
      case PEN_FINE:
         ob_dostate(tree, (demo_shade != PEN_ERASER) ? DEMOPFIN : DEMOEFIN, SELECTED);
         break;
      case PEN_MEDIUM:
         ob_dostate(tree, (demo_shade != PEN_ERASER) ? DEMOPMED : DEMOEMED, SELECTED);
         break;
      case PEN_BROAD:
         ob_dostate(tree, (demo_shade != PEN_ERASER) ? DEMOPBRD : DEMOEBRD,
                                                                      SELECTED);
         break;
   }

   set_color(tree, pen_shade);

                                    /* get dialogue box input                 */

   exit_obj = hndl_dial(tree, 0, box.g_x, box.g_y, box.g_w, box.g_h);
   ob_undostate(tree, exit_obj, SELECTED);

   for (psel_obj = DEMOPFIN; psel_obj <= DEMOEBRD; psel_obj++)
      if (OB_STATE(tree,psel_obj) & SELECTED)
      {
         ob_undostate(tree, psel_obj, SELECTED);
         break;
      }
   color = get_color(tree);

   if (exit_obj == DEMOPSOK)
   {
      switch (psel_obj)
      {
         case DEMOPFIN:
            set_pen(PEN_FINE, char_fine);
            demo_shade = color;
            break;
         case DEMOPMED:
            set_pen(PEN_MEDIUM, char_medium);
            demo_shade = color;
            break;
         case DEMOPBRD:
            set_pen(PEN_BROAD, char_broad);
            demo_shade = color;
            break;
         case DEMOEFIN:
            set_eraser(PEN_FINE, char_fine, &erase_fine);
            break;
         case DEMOEMED:
            set_eraser(PEN_MEDIUM, char_medium, &erase_medium);
            break;
         case DEMOEBRD:
            set_eraser(PEN_BROAD, char_broad, &erase_broad);
            break;
      }
      pen_shade = color;
   }
}

/*------------------------------*/
/*   do_erase                   */
/*------------------------------*/
VOID do_erase()                     /* clear the screen and the undo buffer   */
{ 
   rast_op(0, &scrn_area, &scrn_mfdb, &scrn_area, &undo_mfdb);
   restore_work();
}

/************************************************************************
 *
 *             Keyboard Handling
 *
 ************************************************************************/

/*------------------------------*/
/*   hndl_keyboard              */
/*------------------------------*/
WORD hndl_keyboard()
{
   WORD i;
   BYTE str[2];
   GRECT lttr, test;

   if ((str[0] = kreturn & 0xff) == 0x03)   /* Ctrl C */
      return(TRUE); 

   graf_mouse(M_OFF, (MFORM *)0l);
   if (!key_input)
   {
      vswr_mode(vdi_handle, MD_REPLACE);
      vst_color(vdi_handle, pen_shade); 
         
      /* set text height, then calculate space between lines    */
      vst_height(vdi_handle, demo_height, &gl_wchar,&gl_hchar,&gl_wbox,
                                                                  &gl_hbox);
      gl_hspace = gl_hbox - gl_hchar; 
   
      /* set text alignment to left justification, bottom   */
      vst_alignment(vdi_handle, 0, 3, &i, &i);
      /* get current mouse location, button and keybd state   */
      graf_mkstate(&key_xbeg, &key_ybeg, &i, &i);

      key_xcurr = ++key_xbeg;
      key_ycurr = --key_ybeg;
   }
   else
      curs_off();
   str[1] = '\0'; 
   if (str[0] == 0x1A)   /* Ctrl Z  */
   {
      save_work();   /* update undo area from work area   */
      graf_mouse(M_ON, (MFORM *)0l);
      return(key_input = FALSE);
   }
   else if (str[0] == 0x0D)   /* carriage return  */
   {   
      /* adjust x,y */
      key_ycurr += gl_hbox + gl_hspace;
      key_xcurr = key_xbeg;
   }
   else if (str[0] == 0x08)   /* backspace  */
   {
      if (key_input && (key_xcurr != key_xbeg))
      { /* 'back up' */
         for (i = 0; i < gl_wbox; i++)
         {
            key_xcurr--;
            curs_off();
         }
      }
   }
   else if ((str[0] >= ' ') && (str[0] <= 'z'))
   {  /* output character so long as it   */
      /* fits in the work area            */

      lttr.g_x = key_xcurr;
      lttr.g_y = key_ycurr - gl_hbox;
      lttr.g_w = gl_wbox;
      lttr.g_h = gl_hbox;
   
      rc_copy(&lttr, &test);
      rc_intersect(&work_area, &test);
      if (!rc_equal(&lttr, &test))
      {
         graf_mouse(M_ON, (MFORM *)0l);
         return (FALSE);
      }
      set_clip(TRUE, (GRECT FAR *)&work_area);
      v_gtext(vdi_handle, key_xcurr, key_ycurr, str);
      set_clip(FALSE, (GRECT FAR *)&work_area); 

      /* update x position */
      key_xcurr += gl_wbox;
   }

   if (!key_input)
   {
      key_input = TRUE;
   }
   curs_on();
   graf_mouse(M_ON, (MFORM *)0l);

   return(FALSE);   
}

/************************************************************************
 *
 *            Message Handling
 *
 ************************************************************************/

/*------------------------------*/
/*   hndl_msg                   */
/*------------------------------*/
MLOCAL BOOLEAN hndl_msg()
{
   BOOLEAN done; 
   WORD wdw_hndl;
   GRECT work;

   done = FALSE;
   wdw_hndl = gl_rmsg[3]; 
   switch( gl_rmsg[0] )
   {
      case MN_SELECTED:
         done = hndl_menu(wdw_hndl, gl_rmsg[4]);         /* Title, Item       */
         break;
      case WM_REDRAW:
         do_redraw(wdw_hndl, (GRECT *) &gl_rmsg[4]);     /* x,y,w,h           */
         break;
      case WM_TOPPED:
         if(wdw_hndl == demo_whndl)             /* make sure it's my window   */
            wind_set(wdw_hndl, WF_TOP, 0, 0, 0, 0);
         break;
      case WM_CLOSED:
         done = TRUE;               /* terminate, exit back to DESKTOP.APP    */
         break;
      case WM_FULLED:
         do_full(wdw_hndl);         /* toggle between full and previous       */
         break;

      case WM_ARROWED:              /* calculate new undo_area x,y            */
         switch(gl_rmsg[4])         /* requested action                       */
         {
            case WA_UPPAGE:         /* page up   */
            undo_area.g_y = max(undo_area.g_y - undo_area.g_h, 0);
            break;
         case WA_DNPAGE:            /* page down   */
            undo_area.g_y += undo_area.g_h;
            break;
         case WA_UPLINE:            /* row up   */
            undo_area.g_y = max(undo_area.g_y - YSCALE(16), 0);
            break;
         case WA_DNLINE:            /* row down   */
            undo_area.g_y += YSCALE(16);
            break;
         case WA_LFPAGE:            /* page left   */
            undo_area.g_x = max(undo_area.g_x - undo_area.g_w, 0);
            break;
         case WA_RTPAGE:            /* page right   */
            undo_area.g_x += undo_area.g_w; 
            break;
         case WA_LFLINE:            /* column left   */
            undo_area.g_x = max(undo_area.g_x - 16, 0);
            break;
         case WA_RTLINE:            /* column right   */
            undo_area.g_x += 16;
            break;
         }
         set_work(TRUE);            /* update slider positions                */
         restore_work();            /* update screen from undo_area           */
         break;

      case WM_HSLID:                /* horizontal slider                      */
         undo_area.g_x = align_x(x_mul_div(undo_mfdb.fwp - undo_area.g_w, 
         gl_rmsg[4], 1000)); 
         set_work(TRUE);
         restore_work();
         break;

      case WM_VSLID:                /* vertical slider                        */
         undo_area.g_y = x_mul_div(undo_mfdb.fh - undo_area.g_h,
         gl_rmsg[4],1000);
         set_work(TRUE);
         restore_work();
         break;

      case WM_SIZED:                /* new window size requested              */
                                    /* get work area x,y,w,h                  */
         wind_calc(1, 0x0fef, gl_rmsg[4], gl_rmsg[5], gl_rmsg[6],
            gl_rmsg[7], &work.g_x, &work.g_y, &work.g_w, &work.g_h); 

         work.g_x = align_x(work.g_x);    /* WORD alignment                   */
         work.g_w = align_x(work.g_w);    /*  for performance                 */

                                          /* get border area x, y, w, h       */
         wind_calc(0, 0x0fef, work.g_x, work.g_y, work.g_w, work.g_h,
                      &gl_rmsg[4], &gl_rmsg[5], &gl_rmsg[6], &gl_rmsg[7]); 

                        /* set current x,y,w,h - borders, title bar, (info) */

         wind_set(wdw_hndl, WF_CXYWH, gl_rmsg[4], gl_rmsg[5], gl_rmsg[6],
                        gl_rmsg[7]);
         set_work(TRUE);                  /* update slider positions          */
         break;

      case WM_MOVED:                      /* user moved the window            */
         gl_rmsg[4] = align_x(gl_rmsg[4]);
         wind_set(wdw_hndl, WF_CXYWH, align_x(gl_rmsg[4]) - 1,
         gl_rmsg[5], gl_rmsg[6], gl_rmsg[7]);
         set_work(FALSE);                 /* NO slider update                 */
         break;
   }

   return(done);
}

/*------------------------------*/
/*   do_redraw                  */
/*------------------------------*/
VOID do_redraw(wh, area)            /* redraw specified area from undo bffr   */
WORD wh;
GRECT FAR *area;
{
   GRECT box;
   GRECT dirty_source, dirty_dest;

   graf_mouse(M_OFF, (MFORM *)0l);           /* turn mouse off      */

/*
 * get the coordinates of the first 
 * rectangle in the window's rectangle list
 */

   wind_get(wh, WF_FIRSTXYWH, &box.g_x, &box.g_y, &box.g_w, &box.g_h);
   while ( box.g_w && box.g_h ) 
   {
/*
 * AES returns zero width and height when no more rectangles
 */ 
      if (rc_intersect(area, &box))
      {  
         if (wh == demo_whndl)
         {
/*
 * copy rectangle list x,y,w,h to dirty_dest
 */
            rc_copy(&box, &dirty_dest);

            if (rc_intersect(&work_area, &dirty_dest))
            {  
/*
 * calculate dirty_source x and y
 */
          dirty_source.g_x = (dirty_dest.g_x - work_area.g_x) + undo_area.g_x;
          dirty_source.g_y = (dirty_dest.g_y - work_area.g_y) + undo_area.g_y;  

/*
 * window rectangle w and h to dirty_source
 */
               dirty_source.g_w = dirty_dest.g_w;
               dirty_source.g_h = dirty_dest.g_h; 

/*
 * pixel for pixel source to dest copy
 */
               rast_op(3, &dirty_source, &undo_mfdb, &dirty_dest, &scrn_mfdb);
            } /* end if (rc_intersec ... */
         } /* end if (wh == demo ...  */
      } /* end if( rc_intersect ... */
/*
 * get next rectangle in window's rectangle list
 */
      wind_get(wh, WF_NEXTXYWH, &box.g_x, &box.g_y, &box.g_w, &box.g_h);
   } /* end while */
/*
 * done walking the rectangle list - turn mouse back on
 */
   graf_mouse(M_ON, (MFORM *)0l);
}

/*------------------------------*/
/*   do_full                    */
/*------------------------------*/
VOID do_full(wh)     /* depending on current window state, either make window */
WORD wh;             /* full size -or- return to previous shrunken size       */
{
   GRECT prev;
   GRECT curr;
   GRECT full;

   graf_mouse(M_OFF,(MFORM *)0l);
   wind_get(wh, WF_CXYWH, &curr.g_x, &curr.g_y, &curr.g_w, &curr.g_h);
   wind_get(wh, WF_PXYWH, &prev.g_x, &prev.g_y, &prev.g_w, &prev.g_h);
   wind_get(wh, WF_FXYWH, &full.g_x, &full.g_y, &full.g_w, &full.g_h);
   if ( rc_equal(&curr, &full) )
   {
/*
 * is full now so change to previous
 */
      graf_shrinkbox(prev.g_x, prev.g_y, prev.g_w, prev.g_h,
                     full.g_x, full.g_y, full.g_w, full.g_h);
      wind_set(wh, WF_CXYWH, prev.g_x, prev.g_y, prev.g_w, prev.g_h);
      rc_copy(&save_area, &undo_area);
   }
   else
   {
/*
 * is not full so make it full
 */
      rc_copy(&save_area, &undo_area);
      graf_growbox(curr.g_x, curr.g_y, curr.g_w, curr.g_h,
                   full.g_x, full.g_y, full.g_w, full.g_h);  
      wind_set(wh, WF_CXYWH, full.g_x, full.g_y, full.g_w, full.g_h);
   } 

   set_work(TRUE);
   graf_mouse(M_ON, (MFORM *)0l);
}

/************************************************************************
 *
 *             Mouse Handling
 *
 ************************************************************************/

/*------------------------------*/
/*   hndl_mouse                 */
/*------------------------------*/
WORD hndl_mouse()                   /* change mouse form depending on         */
{                                   /* whether it's in or out of window       */
   if (m_out)
      graf_mouse(ARROW, (MFORM *)0l);
   else
      graf_mouse(monumber, mofaddr);

   m_out = !m_out;                  /* change MU_M1 entry/exit flag           */

   return(FALSE);
}

/************************************************************************
 *
 *             Button Handling
 *
 ************************************************************************/

/*------------------------------*/
/*   hndl_button                */
/*------------------------------*/
WORD hndl_button()
{
   if (rc_inside(mousex, mousey, &work_area))
      draw_pencil(mousex, mousey);

   return(FALSE);
}

/*------------------------------*/
/*   draw_pencil                */
/*------------------------------*/
WORD draw_pencil(x, y)
UWORD x, y;
{
   UWORD pxy[4];
   WORD done;
   UWORD mflags;
   UWORD locount, hicount;
   UWORD ev_which, kreturn;
   WORD  bbutton, kstate, breturn;

   set_clip(TRUE, (GRECT FAR *)&work_area);
   pxy[0] = x;
   pxy[1] = y;      

   vsl_color(vdi_handle,demo_shade);         /* set line color                */
   vswr_mode(vdi_handle,MD_REPLACE);         /* replace writing mode          */
   vsl_type (vdi_handle,FIS_SOLID);          /* solid line type               */

   if (demo_shade != PEN_ERASER)
   {
      vsl_width (vdi_handle,demo_pen);       /* set line width                */
      vsl_ends(vdi_handle, 2, 2);            /* rounded end style             */
      hicount = 0;                           /* MU_TIMER high                 */
      locount = 125;                         /*  and low count                */
      mflags = MU_BUTTON | MU_M1 | MU_TIMER;
      graf_mouse(M_OFF, (MFORM *)0l);        /* turn mouse 'off'              */
   }
   else
   {
      vsf_interior(vdi_handle, 1);           /* solid interior fill           */
      vsf_color(vdi_handle, WHITE);          /* fill color                    */
      mflags = MU_BUTTON | MU_M1;
   }

   done = FALSE;
   while (!done)
   {
      ev_which = evnt_multi(mflags,
      0x01, 0x01, 0x00,                      /* 1 click, 1 button, button up  */
      1, (UWORD)pxy[0], (UWORD)pxy[1], (UWORD)1, (UWORD)1,
      0, 0, 0, 0, 0,
      gl_rmsg, locount, hicount,
      (WORD *)&pxy[2], (WORD *)&pxy[3], &bbutton, &kstate,
      &kreturn, &breturn);

      if (ev_which & MU_BUTTON)
      {
         if (!(mflags & MU_TIMER))
            graf_mouse(M_OFF, (MFORM *)0l);
         if (demo_shade != PEN_ERASER)
            v_pline(vdi_handle, 2, (WORD *) pxy);
         else
            eraser((WORD) pxy[2], (WORD) pxy[3]);
         graf_mouse(M_ON, (MFORM *)0l);
         done = TRUE;
      }
      else
         if (ev_which & MU_TIMER)
         {
            graf_mouse(M_ON, (MFORM *)0l);
            mflags = MU_BUTTON | MU_M1;
         }
         else
         {
            if (!(mflags & MU_TIMER))
               graf_mouse(M_OFF, (MFORM *)0l);
            if (demo_shade != PEN_ERASER)
            {
               v_pline(vdi_handle, 2, (WORD *) pxy);
               mflags = MU_BUTTON | MU_M1 | MU_TIMER;
            }
            else
            {
               eraser((WORD) pxy[2], (WORD) pxy[3]);
               graf_mouse(M_ON,(MFORM *)0l);
            }
            pxy[0] = pxy[2];
            pxy[1] = pxy[3];
         }
   } /* while */

   set_clip(FALSE, (GRECT FAR *)&work_area);
   save_work();
}

/*------------------------------*/
/*   eraser                     */
/*------------------------------*/
VOID eraser(x, y)                   /* erase rectangle of eraser size at x,y  */
WORD x, y;
{
   WORD erase_xy[4];

   if (demo_pen == PEN_FINE)      /* 5 x 3 */
   {
      erase_xy[0] = x - 2;
      erase_xy[1] = y - 1;
      erase_xy[2] = x + 2;
      erase_xy[3] = y + 1;
   }
   else if (demo_pen == PEN_MEDIUM)   /* 9 x 5 */
   {
      erase_xy[0] = x - 4;
      erase_xy[1] = y - 2;
      erase_xy[2] = x + 4;
      erase_xy[3] = y + 2;
   }
   else            /* 13 x 7 */
   {
      erase_xy[0] = x - 6;
      erase_xy[1] = y - 3;
      erase_xy[2] = x + 6;
      erase_xy[3] = y + 3;
   }

   vr_recfl(vdi_handle, erase_xy);
}

/************************************************************************
 *
 *             Demo Event Handler
 *
 ************************************************************************/

/*------------------------------*/
/*   demo                       */
/*------------------------------*/
demo()                              /* Main event multi-loop              */
{
   key_input = FALSE;

   FOREVER
   {
/* 
 * wait for Button, Message, 1 Mouse,keyboard
 */
      ev_which = evnt_multi(MU_BUTTON | MU_MESAG | MU_M1 | MU_KEYBD,
                     0x02, 0x01, 0x01, /* 2 clicks, 1 button, button down */
      /* entry , work_area x,y,w,h */
                     m_out,
                     (UWORD)work_area.g_x, (UWORD)work_area.g_y,
                     (UWORD)work_area.g_w, (UWORD)work_area.g_h, 
      /* mouse rect 2 flags , x,y,w,h */
                     0, 0, 0, 0, 0, 
      /* Message buffer, timer low count , high count    */
                     gl_rmsg, 0, 0,  
      /* mouse posit ,  btn state, r & lshift, Ctrl, Alt    */
                     &mousex, &mousey, &bstate, &kstate, 
      /* keybd key,# btn clicks */
                     &kreturn, &bclicks);

      wind_update(BEG_UPDATE);

      if (!(ev_which & MU_KEYBD))   /* not KEYBD event */
      {
         if (key_input)      /* key_input TRUE? */
         {
            curs_off();   /* turn cursor off */
            key_input = FALSE;   
            save_work();   
         }
      }

      if (ev_which & MU_MESAG)   
         if (hndl_msg()) 
            break;  

      if (ev_which & MU_BUTTON)
         if (hndl_button())
            break;

      if (ev_which & MU_M1)
         if (hndl_mouse())
            break;

      if (ev_which & MU_KEYBD)
         if (hndl_keyboard())
            break;

      wind_update(END_UPDATE);
   }
}

/************************************************************************
 *
 *             Termination
 *
 ************************************************************************/


/*------------------------------*/
/*   demo_term                  */
/*------------------------------*/
demo_term(term_type)
WORD term_type;
{
   switch (term_type)   /* NOTE: all cases fall through      */
   {
      case 0:                                         /* normal termination   */
         do_close(demo_whndl, gl_wfull/2, gl_hfull/2);
         wind_delete(demo_whndl);
      case 1:                                /* no .RSC file or no virt wksta */
         wind_update(END_UPDATE);
         appl_exit();
      case 2:                                         /* not enough memory    */
         graf_mouse(ARROW, 0x0L);
         v_clsvwk( vdi_handle );
      case 3:                                         /* no window available  */
         menu_bar(0x0L, FALSE);
         dos_free(undo_mfdb.mp);
      case 4:                                         /* appl_init() failed   */
         break;
   }
}

/************************************************************************
 *
 *             Initialization
 *
 ************************************************************************/

/*------------------------------*/
/*   pict_init                  */
/*------------------------------*/
VOID  pict_init()                   /* transform IMAGES and ICONS             */
{                                   /* set up Programmer Defined objects      */
   OBJECT FAR *tree;
   WORD tr_obj, nobj;

   rsrc_gaddr(R_TREE, DEMOINFD, &tree);
   trans_gimage(tree, DEMOIMG); 

   rsrc_gaddr(R_TREE, DEMOPEND, &tree);
   for (tr_obj = DEMOPFIN; tr_obj <= DEMOEBRD; tr_obj++)
   {     
/*
 * Loop through Pen/Eraser  Dialog objects transforming
 * them, setting type to G_PROGDEF, establishing the
 * address of the drawing code and setting the obspec
 * pointer for each.
 */
      trans_gimage(tree, tr_obj);
      tree[tr_obj].ob_type = G_PROGDEF;
      nobj = tr_obj - DEMOPFIN;
#if MS_C && M_I86SM
      brushab[nobj].ab_code = MKFP(fardr_cs(), dr_code);
#else
      brushab[nobj].ab_code = (WORD (FAR *)())dr_code;
#endif
      brushab[nobj].ab_parm = OB_SPEC(tree,tr_obj);
      OB_SPEC(tree,tr_obj) = (LONG)&brushab[nobj];
   }
}

/*------------------------------*/
/*   demo_init                  */
/*------------------------------*/
WORD demo_init()
{
   WORD work_in[11];
   WORD i;

/*
 * init application
 */
   gl_apid = appl_init();
   if (gl_apid == -1)
      return(4); 

   wind_update(BEG_UPDATE);
   graf_mouse(HOURGLASS, (MFORM *)0l);
 
/*
 * Load Resource File
 */
   if (!rsrc_load( "DOODLE.RSC" ))
   {
/*
 * No Resource File
 */
      graf_mouse(ARROW, (MFORM *)0l);
      form_alert(1,"[3][Fatal Error !|DOODLE.RSC|File Not Found][ Abort ]");
      return(1);
   }
/*
 * open virtual workstation
 */
   for (i = 0; i < 10; i++)
      work_in[i] = 1;         /* initial defaults: line style,color,etc */

   work_in[10]=2;             /* raster coordinates */

/*
 * Get the VDI handle for GEM AES screen workstation
 */
   gem_handle = graf_handle(&gl_wchar,&gl_hchar,&gl_wbox,&gl_hbox);
   vdi_handle = gem_handle;
   v_opnvwk(work_in,&vdi_handle,work_out);

   if (vdi_handle == 0)
      return(1);

   scrn_width = work_out[0] + 1;             /* # of pixels wide              */
   scrn_height = work_out[1] + 1;            /* # of pixels high              */
   scrn_xsize = work_out[3];                 /* pixel width (microns)         */
   scrn_ysize = work_out[4];                 /* pixel height(microns)         */

   char_fine = work_out[46];                 /* minimum char height           */
   char_medium = work_out[48];               /* maximum char height           */
   char_broad = char_medium * 2;       

   vq_extnd(vdi_handle, 1, work_out);        /* extended inquire              */
   scrn_planes = work_out[4];                /* # of planes                   */

   undo_mfdb.fwp = scrn_width;               /* width in pixels               */
   undo_mfdb.fww = undo_mfdb.fwp >> 4;       /* width in words                */
   undo_mfdb.fh = scrn_height;               /* form height                   */
   undo_mfdb.np = scrn_planes;               /* # of planes                   */
   undo_mfdb.ff = 0;
   
   buff_size = (LONG)(undo_mfdb.fwp>>3) *    /* # of bytes                    */
       (LONG)undo_mfdb.fh *                  /* form height                   */
       (LONG)undo_mfdb.np;                   /* # of planes                   */

   buff_location = undo_mfdb.mp  = dos_alloc(buff_size);
   if (undo_mfdb.mp == 0)
      return(2);                             /* not enough memory             */
   
   scrn_area.g_x = 0;
   scrn_area.g_y = 0;
   scrn_area.g_w = scrn_width;
   scrn_area.g_h = scrn_height;
   scrn_mfdb.mp = 0l;

   rc_copy(&scrn_area, &undo_area);
   rast_op(0, &undo_area, &scrn_mfdb, &undo_area, &undo_mfdb);

/*
 * transforms & programmer defined objects
 */
   pict_init();

/*
 * Get Desktop work area
 */
   wind_get(DESK, WF_WXYWH, &gl_xfull, &gl_yfull, &gl_wfull, &gl_hfull);

/*
 * initialize menu
 */
   rsrc_gaddr(R_TREE, DEMOMENU, &gl_menu);

/*
 * show menu
 */
   menu_bar(gl_menu, TRUE);   
   
/*
 * create window with all components except info line
 */
   demo_whndl = wind_create(0x0fef, gl_xfull - 1, gl_yfull, gl_wfull, gl_hfull);
   if (demo_whndl == -1)
   { 
/*
 * No Window Available
 */
      form_alert(1, string_addr(DEMONWDW));
      return(3);
   }

   wind_set(demo_whndl, WF_NAME, FPOFF(wdw_title), FPSEG(wdw_title), 0, 0); 

   gl_xfull = align_x(gl_xfull);
   do_open(demo_whndl, gl_wfull/2, gl_hfull/2, align_x(gl_xfull)-1,
                       gl_yfull, gl_wfull, gl_hfull);

/*
 * get work area of window
 */
   wind_get(demo_whndl, WF_WXYWH, &work_area.g_x, &work_area.g_y,
                                  &work_area.g_w, &work_area.g_h);

/*
 * initial slider positions
 */
   set_work(TRUE);

/*
 * save_area used by do_full()
 */
   rc_copy (&undo_area, &save_area);

   graf_mouse(ARROW, (MFORM *)0l);
   wind_update(END_UPDATE);

   return(0);   /* successful initialization */
}

/************************************************************************
 *
 *             Main Program
 *
 ************************************************************************/

/*------------------------------*/
/*   main                       */
/*------------------------------*/
main()
{
   WORD   term_type;

   if ((term_type = demo_init()) == 0)
      demo();
   demo_term(term_type);
}
