;/*	GEMASM.ASM	5/03/84 -  01/05/85	Lee Lorenzen		*/
;/*	modified for MASM	    01/31/85	Tom Rolander		*/
;
; Copyright 1999 by Caldera Thin Clients, Inc.
; This software is licenced under the GNU Public License
; Please see LICENSE.TXT for further information.
PGROUP	GROUP	CODE

CODE	SEGMENT	BYTE PUBLIC 'CODE'
	ASSUME	CS:PGROUP
;
GEMENTRY 	equ	0efh
;
	PUBLIC	gem
gem:	push	bp
	mov	bp,sp
	mov	cx,200
	mov	dx,0
	mov	bx,4[bp]
	mov	ax,6[bp]
	mov	es,ax
	int	GEMENTRY
	pop	bp
	ret
;
CODE	ENDS

	end

