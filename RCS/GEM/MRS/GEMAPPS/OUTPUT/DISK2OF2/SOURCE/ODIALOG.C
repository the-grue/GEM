/************************************************************************/
/*	Copyright ( c ) 1985 Digital Research Inc.		        */
/*    The software contained in this listing is proprietary to       	*/
/*    Digital Research Inc., Pacific Grove, California and is        	*/
/*    covered by U.S. and other copyright protection.  Unauthorized  	*/
/*    copying, adaptation, distribution, use or display is prohibited	*/
/*    and may be subject to civil and criminal penalties.  Disclosure	*/
/*    to others is prohibited.  For the terms and conditions of soft-	*/
/*    ware code use refer to the appropriate Digital Research        	*/
/*    license agreement.						*/
/************************************************************************/

#include "portab.h"
#include "machine.h"
#include "obdefs.h"
#include "formlib.h"
#include "rsrclib.h"
#include "gembind.h"
#include "dosbind.h"
#include "xgembind.h"

#include "output.h"
#include "odefs.h"
#include "ovar.h"

EXTERN WORD	form_center() ;			/* GEMBIND.LIB */

/*--------------------------------------------------------------*/
	VOID
do_miss( dir, fname )
	FBYTE *dir, *fname ;
{
	LONG tree ;
	WORD	ok ;

	rsrc_gaddr( GAD_TREE, FILEMISS, &tree ) ;
	set_spec( tree, MISSDIR, dir ) ;
	set_spec( tree, MISSITEM, fname ) ;
	tree = do_dialog( FILEMISS ) ;
    	ok = okcan( tree, MISSCONT, MISSCONT ) ;
} /* do_miss */

/*--------------------------------------------------------------*/
	VOID
do_info( dial_num, ok_num )
WORD	dial_num, ok_num ;
{
	LONG tree ;
	UWORD state ;

	tree = do_dialog( dial_num ) ;
	state = get_state( tree, ok_num ) ;
	if ( state & SELECTED )
	{
	  state &= ~( SELECTED ) ;
	  set_state( tree, ok_num, state ) ;
	}
} /* do_info */

/*--------------------------------------------------------------*/
    VOID
do_shrtinfo()
{
LONG 	tree ;
WORD	xc, yc, xd, yd, wd, hd ;

    rsrc_gaddr( GAD_TREE, SHRTCUT, &tree ) ;
    xc = gl_desk.g_w / 2 ;
    yc = gl_desk.g_h / 2 ;
    form_center( tree, &xd, &yd, &wd, &hd ) ;
    form_dial( FMD_START, xc, yc, 0, 0, xd, yd, wd, hd);
    form_dial( FMD_GROW,  xc, yc, 0, 0, xd, yd, wd, hd);
    objc_draw( tree, ROOT, MAX_DEPTH, xd, yd, wd, hd ) ;
    form_do( tree, 0 ) ;
    if ( OK == okcan( tree, SHRTCONT, SHRTCANC ) )
	{
    	rsrc_gaddr( GAD_TREE, SHRTCUT2, &tree ) ;
	form_center( tree, &xd, &yd, &wd, &hd ) ;
    	objc_draw( tree, ROOT, MAX_DEPTH, xd, yd, wd, hd ) ;
    	form_do( tree, 0 ) ;
	okcan( tree, SHRTOK, SHRTOK ) ;	/* sets button state back to NORMAL */
	}
    form_dial(FMD_SHRINK, xc, yc, 0, 0, xd, yd, wd, hd);
    form_dial(FMD_FINISH, xc, yc, 0, 0,	xd, yd, wd, hd);
} /* do_shrtinfo */


/* end of outdial.c */


