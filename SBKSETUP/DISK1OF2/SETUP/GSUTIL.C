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

/* gsutil.c	general 'C' UTILITIES for GEMSETUP.EXE.			*/

#include "portab.h"
#include "machine.h"
#include "treeaddr.h"
#include "dosbind.h"
#include "gsdefs.h"

/*----------------------------------------------------------------------*/
    WORD
min( a, b )
WORD	a, b ;
{
    return( ( a < b ) ? a : b ) ;
} /* min() */

/*--------------------------------------------------------------*/
/* converts ascii to integer.					*/
	WORD
l_atoi( s )
FBYTE	*s ;
{
	WORD    ii, nn, sign;
	BYTE	b ;

	ii = 0 ;
	b = *s ;
	while ( b == SPACE || b == '\n' || b == '\t' )
	    {
	    ++ii ;
	    b = *(s + ii) ;
	    }
	sign = ( b == '-' ) ? -1 : 1 ;
	if ( b == '+' || b == '-' )
	    ++ii ;
	nn = 0 ;
	while ( b >= '0' && b <= '9' )
	    {
	    nn = 10 * nn + b - '0';
	    ++ii ;
	    b = *(s + ii) ;
	    }
	return( sign * nn ) ;
} /* l_atoi */

/*----------------------------------------------------------------------*/
    BOOLEAN
lstr_cmp(p_str1, p_str2)
FBYTE	*p_str1, *p_str2;
{
FBYTE	*p1, *p2;
	
	p1 = p_str1;
	p2 = p_str2;
	
	while( *p1 == *p2 && *p1 != NULL )
	{
		++p1;
		++p2;
	}
	if ( *p1 != NULL )
		return(FALSE);
	return(TRUE);
} /* END lstr_cmp */

/*----------------------------------------------------------------------*/
/* return TRUE iff ps = pd up to first NULL in ps.			*/
    BOOLEAN
strcmp( ps, pd )	
BYTE	*ps, *pd ;
{
	while ( *ps == *pd && *ps )
	    {
		++ps ;
		++pd ;
	    }
	return( *ps == NULL ) ;
} /* strcmp() */

/*----------------------------------------------------------------------*/
    WORD
strlen(p1)
BYTE		*p1;
{
	WORD		len;

	len = 0;
	while( *p1++ )
		len++;

	return(len);
} /* strlen() */

/*----------------------------------------------------------------------*/
    VOID
strcpy(pd, ps)
BYTE		*pd, *ps;
{
    *pd = *ps ;
    while ( *pd )
        {
	++pd ;
	++ps ;
	*pd = *ps ;
	}	
} /* strcpy() */

/*----------------------------------------------------------------------*/
    VOID
strcat( pd, ps)
BYTE		*pd, *ps;
{
	while( *pd != NULL )
		pd++;
	*pd = *ps ;
	while( *pd )
	    {
	    ++pd ;
	    ++ps ;
	    *pd = *ps ;
	    }
} /* strcat() */

/*----------------------------------------------------------------------*/
	WORD
dos_cin()
{
	DOS_AX = 0x0800;
	__DOS(); 
	return ((WORD) DOS_AX & 0xFF00 );
} /* dos_cin() */

/*----------------------------------------------------------------------*/
	WORD
dos_getim(handle)
	WORD		handle;
{
	DOS_AX = 0x5700;
	DOS_BX = handle;

	__DOS();
	return( (WORD) DOS_CX);
} /* dos_getim() */

/*----------------------------------------------------------------------*/
    VOID
dos_exec(pcspec, segenv, pcmdln, pfcb1, pfcb2)
	LONG		pcspec;
	WORD		segenv;
	LONG		pcmdln;
	LONG		pfcb1;
	LONG		pfcb2;
{
	EXEC_BLK	exec;

	exec.eb_segenv = segenv;
	exec.eb_pcmdln = pcmdln;
	exec.eb_pfcb1 = pfcb1;
	exec.eb_pfcb2 = pfcb2;

	DOS_AX = 0x4b00;
	DOS_BX = LLOWD(ADDR(&exec));
	DOS_ES = LHIWD(ADDR(&exec));
	DOS_DX = LLOWD(pcspec);
	DOS_DS = LHIWD(pcspec);

	__EXEC();
} /* dos_exec() */


/* end of gsutil.c */
