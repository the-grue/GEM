;/*	TCS.ASM		01/30/85 -  01/30/85	Tom Rolander		*/
;/*	Tiny CS								*/
;
PGROUP	GROUP	BASE,PROG

BASE	SEGMENT	WORD PUBLIC 'PROG'
BASE	ENDS

DGROUP	GROUP	DATA,STACK

DATA	SEGMENT PARA PUBLIC 'DATA'
	PUBLIC	_PSP
_PSP	dw	0
	dw	0
	PUBLIC	_VER
_VER	db	"xxxxxxxxxx1.0"
	db	1024 dup(?)
stk	dw	0
DATA	ENDS

STKRSV	EQU	1024
STACK	SEGMENT STACK 'DATA'
SBASE	DB	STKRSV DUP (?)
STACK	ENDS

PROG	SEGMENT	BYTE PUBLIC 'PROG'

	ASSUME	CS:PGROUP
	ASSUME	DS:DGROUP
;
;
	EXTRN	PROMAIN:NEAR
;
;
	PUBLIC	_TCS
_TCS	PROC 	FAR
START:
	cli
	mov	ax,dgroup
	mov	ds,ax
	mov	ss,ax
	mov	sp,offset dgroup:stk
	sti
;
	mov	ax,es			;es has our psp
	mov	_PSP+2,ax		;save for use by prostart
					;when it free's memory
	call	PROMAIN

	mov	ax,04c00h
	int	21h			;terminate

_TCS	ENDP

PROG	ENDS
;
	END START

