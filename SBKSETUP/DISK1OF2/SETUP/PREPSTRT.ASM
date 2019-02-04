;/*	PROSTART.A86	05/03/84 -  01/05/85	Lee Lorenzen		*/
;/*	added an SSEG with 32 words 01/25/85	Gregg Morris		*/
;/*	added check for 'GEMAES'    01/29/85	Tom Rolander		*/
;
PGROUP	GROUP	BASE,PROG

BASE	SEGMENT	WORD PUBLIC 'PROG'
BASE	ENDS

DGROUP	GROUP	DATA,STACK

DATA	SEGMENT PARA PUBLIC 'DATA'         

	PUBLIC  cmd_lin

cmd_lin	dw	80h
seg_dir dw      0
PARABEG	dw	0
PARAEND	dw	0

copyrt	db	'GEM PREP			Version 0.0'
	db	'Serial No.  XXXX-0000-654321   All Rights Reserved'
	db	'Copyright (c) 1987             Digital Research Inc.'
	db	'01/01/87'

gemaesmsg db	"GEMAES not present in memory."
	db	"$"
memerrmsg db	"Unable to free memory."
	db	"$"
terminmsg db	0dh,0ah
	db	"Execution terminated."
	db	"$"
	dw	1024 DUP (?)
stk	dw	0
DATA	ENDS

STKRSV	EQU	01000h
STACK	SEGMENT STACK 'DATA'
SBASE	DB	STKRSV DUP (?)
STACK	ENDS

PROEND	SEGMENT PARA PUBLIC 'DATA'
;
	EXTRN	PROGEND:WORD
;
PROEND	ENDS
;

PROG	SEGMENT	BYTE PUBLIC 'PROG'

	ASSUME	CS:PGROUP
	ASSUME	DS:DGROUP

GLOBAMNT	equ	0

	EXTRN	GEMAIN:NEAR
					;/*   this allows us to free	*/
				;/*   memory so the memory alloc*/
					;/*   in RSLIB.C will work	*/
;

	PUBLIC	_PREPSTRT

_PREPSTRT	PROC FAR

START :
	cli
	mov	ax,seg stk
	mov	ds,ax
	mov	ss,ax
	mov	sp,offset DGROUP:stk
	sti
;pass back any command line we find
        mov	si, offset DGROUP:seg_dir
	mov	ax,es
	mov	[si],ax
;
;	free up any memory we're not using
;		es points at our psp
;
 	mov	ax,es			;es has our psp
 	mov	PARABEG,ax
 	mov	ax,offset DGROUP:PROGEND
 	add	ax, GLOBAMNT
 	add	ax,15
 	mov	cl,4
 	shr	ax,cl
 	mov	cx,ax			; cx = our data size in paragraphs
 	mov	bx,ds
 	add	bx,cx
 	mov	PARAEND,bx
;
;
free_ex:
 	mov	ax,PARABEG
 	mov	es,ax
 	mov	bx,PARAEND
 	sub	bx,ax
	add	bx, 1024		; allow for a 16k bat file
 	mov	ah,4ah
 	int	21h
 	mov	dx,offset DGROUP:memerrmsg
 	jc	errormsg
runapplication:
	mov	ax,ds
	mov	es,ax
	call	GEMAIN
;
errormsg:
	mov	ah, 4ch
	mov 	al, 0
	int	21h

_PREPSTRT	ENDP

PROG	ENDS
;
	end START
