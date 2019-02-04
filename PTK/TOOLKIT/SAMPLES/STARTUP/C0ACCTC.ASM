;/******************************************************************************
; *
; *    c0acctc.asm
; *
; *    Accessory runtime-startup for Turbo-C 1.5 - 2.0
; *
; *    Author   :  Robert Schneider, DR GmbH
; *    Created  :  23.05.1989
; *    Version  :  1.00
; *
; ******************************************************************************/

IFDEF __SMALL__
	EXTRN	_main	: NEAR
ELSE
	EXTRN	_main   : FAR
ENDIF

	EXTRN __emufak  : ABS

PGROUP	GROUP	_TEXT

IFDEF __SMALL__
DGROUP  GROUP   _DATA, _EMUSEG, _CVTSEG, _SCNSEG, _BSS, _BSSEND
ELSE
DGROUP  GROUP   _DATA
ENDIF

_TEXT	SEGMENT BYTE PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT PARA PUBLIC 'DATA'
_DATA	ENDS
_EMUSEG SEGMENT PARA PUBLIC 'DATA'
_EMUSEG ENDS
_CVTSEG SEGMENT PARA PUBLIC 'DATA'
_CVTSEG ENDS
_SCNSEG SEGMENT PARA PUBLIC 'DATA'
_SCNSEG ENDS
_BSS	SEGMENT WORD PUBLIC 'BSS'
_BSS	ENDS
_BSSEND SEGMENT BYTE PUBLIC 'BSSEND'
_BSSEND ENDS
_STACK  SEGMENT STACK PARA 'STACK'
_STACK  ENDS

_TEXT	SEGMENT	BYTE PUBLIC 'CODE'
	ASSUME	CS:PGROUP
        ASSUME  DS:DGROUP

IFDEF __SMALL__
BEGIN	PROC	NEAR
ELSE
BEGIN	PROC	FAR
ENDIF
	cli
	mov	ax,seg DGROUP
	mov	ds,ax
	mov	es,ax

IFDEF __SMALL__
	mov	ss,ax
	mov	sp,offset DGROUP : st_end
ELSE
	mov	ax,seg _STACK
	mov	ss,ax
	mov	sp,offset st_end
ENDIF
	sti

	mov	ah, 30h			; Get
	int	21h			; and
	mov	__version, ax		; store version of os

	call	_main

	ret			;return to tcs

BEGIN	ENDP

PUBLIC	_exit
IFDEF __SMALL__
_exit   PROC   NEAR
ELSE
_exit   PROC   FAR
ENDIF
	mov	ax,4c00h
	int	21h
_exit	 ENDP

PUBLIC	_abort
IFDEF __SMALL__
_abort   PROC   NEAR
ELSE
_abort   PROC   FAR
ENDIF
	call	_exit		; _exit(3);
_abort	 ENDP

_TEXT    ENDS
;

PUBLIC __version
PUBLIC __osmajor
PUBLIC __osminor
PUBLIC _errno
PUBLIC __8087
PUBLIC __RealCvtVector
PUBLIC __ScanTodVector

_DATA   SEGMENT
__version	label 	word
__osmajor	db	0
__osminor	db	0
_errno		dw 	0
__8087		dw	0
_DATA   ENDS

_EMUSEG SEGMENT
__emu1st	label	word
__emuLast	equ	__emu1st + 2
_EMUSEG ENDS

_CVTSEG SEGMENT
__RealCvtVector	label	word
_CVTSEG ENDS

_SCNSEG SEGMENT
__ScanTodVector	label	word
_SCNSEG ENDS

_BSS	SEGMENT
_bdata	label	byte
_BSS	ENDS

_BSSEND	SEGMENT
_edata	label	byte
_BSSEND ENDS

_STACK SEGMENT
st_begin    db 2048 dup(?)
st_end      db 0
_STACK ENDS

	END
