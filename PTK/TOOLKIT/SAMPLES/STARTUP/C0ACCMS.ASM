;/******************************************************************************
; *
; *    c0accms.asm
; *
; *    Accessory runtime-startup for Microsoft-C 5.X
; *
; *    Author   :  Robert Schneider, DR GmbH
; *    Created  :  05.06.1989
; *    Version  :  1.00
; *
; *****************************************************************************/

IFDEF __SMALL__
EXTRN	_main:	NEAR
ELSE
EXTRN	_main:	FAR
ENDIF

PGROUP	GROUP	_TEXT

IFDEF __SMALL__
DGROUP  GROUP   _DATA, _BSS, _STACK
ELSE
DGROUP  GROUP   _DATA
ENDIF

_TEXT	SEGMENT BYTE PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT PARA PUBLIC 'DATA'
_DATA	ENDS
_BSS	SEGMENT WORD PUBLIC 'BSS'
_BSS	ENDS
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

	call	_main

	ret			;return to tcs

BEGIN	ENDP

_TEXT    ENDS

_DATA   SEGMENT
_DATA   ENDS

_BSS	SEGMENT
_BSS	ENDS

_BSSEND	SEGMENT
_BSSEND ENDS

_STACK SEGMENT
st_begin    db 4096 dup(?)
st_end      db 0
_STACK ENDS

	END
