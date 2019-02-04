;/*	DOSASM.ASM	4/18/84 - 10/24/84	Lee Jay Lorenzen	*/
;/*	modified for MASM	    01/31/85	Tom Rolander		*/
;
;
PGROUP	GROUP	PROG

DGROUP	GROUP	DATA

DATA	SEGMENT PARA PUBLIC 'DATA'
	EXTRN	DOS_AX:WORD, DOS_BX:WORD, DOS_CX:WORD, DOS_DX:WORD
	EXTRN	DOS_DS:WORD, DOS_ES:WORD, DOS_SI:WORD, DOS_DI:WORD
	EXTRN	DOS_ERR:WORD
;	PUBLIC	err_code

;err_code dw 	0     ; equal to contents of DI reg after critical error

DATA	ENDS

PROG	SEGMENT	BYTE PUBLIC 'PROG'
	ASSUME	CS:PGROUP
	ASSUME	DS:DGROUP
;
;	extrn	d_err_c:near	;part of a new critical error handler
;
	PUBLIC	__DOS
	PUBLIC	__EXEC
	PUBLIC  N_FDRV
;	PUBLIC  gt_int24
;	PUBLIC  d_err_a
;
savax	dw	0
savds	dw	0
savss	dw	0
savsp	dw	0

__DOS:
	push	bp
	mov	bp,sp
	push	es
	push	si
	push	di
	mov	ax,DOS_DI
	mov	di,ax
	mov	ax,DOS_SI
	mov	si,ax
	mov	ax,DOS_ES
	mov	es,ax
	mov	ax,DOS_AX
	mov	bx,DOS_BX
	mov	cx,DOS_CX
	mov	dx,DOS_DX
	mov	savax,ax
	push	ds
	mov	ax,DOS_DS
	mov	ds,ax
	mov	ax,savax
	int	21h
	mov	savax,ax
	mov	ax,ds
	mov	savds,ax
	pop	ds
	mov	ax,savax
	jc	d_error
	mov	DOS_ERR,0
	jmp	d_exit
d_error:
	mov	DOS_ERR,1
d_exit:
	mov	DOS_AX,ax
	mov	DOS_BX,bx
	mov	DOS_CX,cx
	mov	DOS_DX,dx
	mov	ax,savds
	mov	DOS_DS,ax
	mov	ax,es
	mov	DOS_ES,ax
	mov	ax,si
	mov	DOS_SI,ax
	mov	ax,di
	mov	DOS_DI,ax
	pop	di
	pop	si
	pop	es
	pop	bp
	ret
;
;
;
__EXEC:
	push	bp
	mov	bp,sp
	push	es
	push	ds
					; update ss,sp in uda in case this
					;   is execing a crystal application
;	mov	bx,rlr			; bx = rlr
;	mov	bx,4[bx]		; bx = uda address
;	mov	ax,sp
;	mov	2[bx],ax		; new saved sp = sp
;	mov	ax,ss
;	mov	4[bx],ax		; new saved ss = ss
	mov	ax,DOS_ES
	mov	es,ax
	mov	ax,DOS_AX
	mov	bx,DOS_BX
	mov	cx,DOS_CX
	mov	dx,DOS_DX
	mov	savax,ax
	mov	ax,DOS_DS
	mov	ds,ax
	mov	ax,ss
	mov	savss,ax
	mov	ax,sp
	mov	savsp,ax
	mov	ax,savax
	int	21h
	cli
	mov	savax,ax
	mov	ax,savss
	mov	ss,ax
	mov	ax,savsp
	mov	sp,ax
	mov	ax,savax
	sti
	pop	ds
	jc	de_error
	mov	DOS_ERR,0
	jmp	de_exit
de_error:
	mov	DOS_ERR,1
de_exit:
	mov	DOS_AX,ax
	mov	DOS_BX,bx
	mov	DOS_CX,cx
	mov	DOS_DX,dx
	mov	ax,ds
	mov	DOS_DS,ax
	mov	ax,es
	mov	DOS_ES,ax
	pop	es
	pop	bp
	ret

;This routine is used to take over the hardware interrupt #24 which is 
;the critical error interrupt.  Vector 24h will point to the routine below
;gt_int24:
;	push	ds	
;	mov	dx,offset d_err_a
;	mov 	ax,seg d_err_a
;	mov	ds,ax
;	mov	ah,25h
;	mov	al,24h			;interupt to take over
;	int	21h
;	pop	ds
;	ret


;This is the new interrupt service routine for int 24h.	
;d_err_a proc	far
;	push	bp		;save the callers bp
;	mov	bp, sp
;	push	ds
;	push	ax
;	mov	ax, ss	
;	mov	ds, ax
;	pop	ax
;	mov	err_code, di	; global var in case c code wants it later
;	call	d_err_c
;	pop	ds
;	pop	bp
;	mov	al, 1	; 0=ignore; 1=retry; 2=term; 3=fail (dos 3 only)
;	iret
;d_err_a endp

N_FDRV  PROC    NEAR

        push    bp
        mov     bp, sp
        int     11h
        test    al, 1
        jz      no_drives
        and     ax, 00C0h
        mov     cx, 6
        shr     ax, cl
        inc     ax
        jmp      drv_exit
no_drives:
        mov     ax, 0FFFFh
drv_exit:
        pop     bp
        ret
N_FDRV  ENDP

PROG	ENDS

	end
