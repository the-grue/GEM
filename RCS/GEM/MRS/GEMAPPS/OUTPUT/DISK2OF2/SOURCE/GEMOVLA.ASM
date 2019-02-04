; GEMOVL_A.ASM	misc overlay tools for use with PLINK86 and GEM.
; 06-14-85	Susan Bancroft
;
PGROUP	GROUP	PROG
DGROUP	GROUP	DATA

DATA	SEGMENT PARA PUBLIC 'DATA'

	EXTRN	_VER:BYTE

DATA	ENDS

PROG	SEGMENT	BYTE PUBLIC 'PROG'
	ASSUME	CS:PGROUP
	ASSUME	DS:DGROUP

	extrn	_ovlerr:near

	public  OvlErr

;***************************************************************************
;
; Disply dialog "can't find overlay... please insert disk yuk yuk.."
;
;***************************************************************************
OvlErr	proc	far
	push	bp		;save the callers bp
	mov	bp, sp
	push	ds
	push	es
	mov	ax, seg dgroup:_VER
	mov	ds, ax
	mov	es, ax
	call	_ovlerr
	pop	es
	pop	ds
	pop	bp
	ret
OvlErr	endp

;************************************************************************
;
PROG	ENDS


	end

