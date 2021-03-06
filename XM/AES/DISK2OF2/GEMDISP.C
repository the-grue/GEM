/*	GEMDISP.C	1/27/84 - 09/13/85	Lee Jay Lorenzen	*/

/*
*       Copyright 1999, Caldera Thin Clients, Inc.                      
*       This software is licenced under the GNU Public License.         
*       Please see LICENSE.TXT for further information.                 
*                                                                       
*                  Historical Copyright  
*	-------------------------------------------------------------
*	GEM Application Environment Services		  Version 3.0
*	Serial No.  XXXX-0000-654321		  All Rights Reserved
*	Copyright (C) 1986			Digital Research Inc.
*	-------------------------------------------------------------
*/

#include <portab.h>
#include <machine.h>
#include <struct.h>
#include <basepage.h>
#include <obdefs.h>
#include <gemlib.h>

#define KEYSTOP 0x00002b1cL			/* control backslash	*/
						/* in INPUT88.C		*/
EXTERN WORD	tchange();
EXTERN WORD	kchange();
						/* in DISPA88.A86	*/
EXTERN VOID	savestat();
EXTERN VOID	switchto();
						/* in PD88.C		*/
EXTERN VOID	insert_process();
						/* in APLIB.C		*/
EXTERN WORD		gl_play;
EXTERN WORD		gl_recd;
EXTERN WORD		gl_rlen;
EXTERN LONG		gl_rbuf;

EXTERN PD		*gl_mowner;

EXTERN WORD		kstate;

EXTERN THEGLO		D;

#if MULTIAPP
EXTERN VOID		ap_accexit();
EXTERN VOID		psetup();
GLOBAL LONG		gl_prret;
GLOBAL LONG		gl_prpid;
EXTERN WORD	 	gl_ldpid;
EXTERN SHELL		sh[];
EXTERN LONG		gl_mntree;
EXTERN PD		*gl_mnppd;
#endif


	VOID
forkq(fcode,fdata)
	WORD		(*fcode)();
	LONG		fdata;
{
	REG FPD		*f;
						/* q a fork process,	*/
						/*   enter with ints OFF*/
	if (fpcnt == 0)
	  fpt = fph = 0;

	if (fpcnt < NFORKS)
	{
	  f = &D.g_fpdx[fpt++];
						/* wrap pointer around	*/
	  if (fpt == NFORKS)
	    fpt = 0;

	  f->f_code = fcode;
	  f->f_data = fdata;

	  fpcnt++;
	  dodisp = 1;
	}
}


	VOID
disp_act(p)
	REG PD		*p;
{      
						/* process is ready,	*/
						/*   so put him on RLR	*/
	p->p_stat &= ~WAITIN;
	insert_process(p, &rlr);	
}


	VOID
mwait_act(p)
	REG PD		*p;
{	
						/* sleep on nrl if	*/
						/*   event flags are 	*/
						/*   not set		*/
	if (p->p_evwait & p->p_evflg)
	  disp_act(p);
	else
	{ 
						/* good night, Mrs.	*/
						/*   Calabash, wherever	*/
						/*   you are 		*/
	  p->p_link = nrl;
	  nrl = p;
	}
}


	VOID
forker()
{
	REG FPD		*f;
	REG PD		*oldrl;
	REG LONG	amt;
	FPD		g;

	infork = 1;
	oldrl = rlr;
	rlr = (PD *) -1;
	while(fpcnt)
	{
/* critical area	*/
	  cli();
	  fpcnt--;
	  f = &D.g_fpdx[fph++];
  					/* copy FPD so an interrupt	*/
					/*  doesn't overwrite it.	*/
	  LBCOPY(ADDR(&g), ADDR(f), sizeof(FPD) );
	  if (fph == NFORKS) 
	    fph = 0;
	  sti();
/* */
						/* see if recording	*/
	  if (gl_recd)
	  {
						/* check for stop key	*/
	    if ( (g.f_code == &kchange) &&
	         ((g.f_data & 0x0000ffffL) == KEYSTOP) )
	      gl_recd = FALSE;
						/* if still recording	*/
						/*   then handle event	*/
	    if (gl_recd)
	    {
						/* if its a time event &*/
						/*   previously recorded*/
						/*   was a time event	*/
						/*   then coalesce them	*/ 
						/*   else record the	*/
						/*   event		*/
	      if ( (g.f_code == &tchange) &&
#if MC68K
	           (LLGET(gl_rbuf - sizeof(FPD)) == &tchange) )
#endif
#if I8086
	           (LWGET(gl_rbuf - sizeof(FPD)) == &tchange) )
#endif
	      {
	        amt = g.f_data + LLGET(gl_rbuf-sizeof(LONG));
	        LLSET(gl_rbuf - sizeof(LONG), amt);	      
	      }
	      else
	      {
	        LBCOPY(gl_rbuf, ADDR(f), sizeof(FPD));
	        gl_rbuf += sizeof(FPD);
	        gl_rlen--;
		gl_recd = gl_rlen;
	      }
	    }
	  }
	  (*g.f_code)(g.f_data);
	}
	rlr = oldrl;
	infork = 0;
}

	VOID
chkkbd()
{
	REG WORD	achar, kstat;
						/* poll keybd 		*/
	if (!gl_play)
	{
	  kstat = gsx_kstate();
	  achar = (gl_mowner->p_cda->c_q.c_cnt < KBD_SIZE) ? gsx_char() : 0x0;
	  if ( (achar) ||
	     (kstat != kstate) )
	  {
	    cli();
	    forkq(kchange, achar, kstat);
	    sti();
	  }
	}
}


	VOID
schedule()
{
	REG PD		*p;

						/* run through lists	*/
						/*   until someone is	*/
						/*   on the rlr or the	*/
						/*   fork list		*/
	do
	{
						/* poll the keyboard	*/
	  chkkbd();
						/* now move drl		*/
						/*   processes to rlr	*/
	  while ( drl )
	  {
	    drl = (p = drl) -> p_link;
	    disp_act(p);
	  }
						/* check if there is	*/
						/*   something to run	*/
	} while ( !rlr && !fpcnt );
}


/************************************************************************/
/*									*/
/* dispatcher maintains all flags/regs so it looks like an rte to	*/
/*   the caller.							*/
/*   dispatch() = rte							*/
/*   rlr -> p_stat determines the action to perform on the process that	*/
/*		was in context						*/
/*   rlr -> p_uda -> dparam is used by the action routines		*/
/*									*/
/************************************************************************/

	VOID
disp()
{
	REG PD		*p;
/* savestate() is a machine (& compiler) dependent routine which:
 *	1) saves any flags that will be trashed by the TAS
 *	2) if (indisp) restore flags, return to dsptch caller
 *	3) otherwise 
 *		save machine state, 
 *		return to dsptch here 
 */
 	savestat(rlr->p_uda);
						/* take the process p	*/
						/*   off the ready list	*/
						/*   root		*/
	rlr = (p=rlr) -> p_link;
						/* based on the state	*/
						/*   of the process p	*/
						/*   do something	*/
	if (p->p_stat & WAITIN) 
	  mwait_act(p);
	else
	  disp_act(p);
						/* run through and 	*/
						/*   execute all the	*/
						/*   fork processes	*/
	do 
	{
	  if (fpcnt)
	    forker();
	  schedule();
	} while (fpcnt);


/* switchto() is a machine dependent routine which:
 *	1) restores machine state
 *	2) clear "indisp" semaphore
 *	3) returns to appropriate address
 *		so we'll never return from this
 */
#if MULTIAPP

	if (rlr->p_pid == 1)
	{
	  if (gl_mntree)
	    pr_load(gl_mnppd->p_pid);	
	}
	else
	  pr_load(rlr->p_pid);

/*
	if (sh[rlr->p_pid].sh_state & ABORT)
	{


	  psetup(rlr, &ap_accexit);		/* force a terminate	*/
	}
*/
#endif
	switchto(rlr->p_uda);
}


