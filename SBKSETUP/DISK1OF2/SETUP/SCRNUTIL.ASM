; SCRNUTIL.ASM	misc screen utilities for GEMPREP.
;		written by Cregg D. Lund  9/87
;
PGROUP	GROUP	PROG
DGROUP	GROUP	DATA

DATA	SEGMENT PARA PUBLIC 'DATA'

; The following definitions are bits used to redefine characters into the
; Gem logo and the box and check used in the gemsetup userface.
bits	db	01111111B
	db	10000000B
	db	10000000B
	db	10011110B
	db	10011110B
	db	10011110B
	db	10011110B
	db	10011110B
	db	10011110B
	db	10011110B
	db	10011110B
	db	10000000B
	db	10000000B
	db	01111111B

	db	11111110B
	db	00000001B
	db	00000001B
	db	01111001B
	db	01001001B
	db	01001001B
	db	01001001B
	db	01001001B
	db	01001001B
	db	01001001B
	db	01111001B
	db	00000001B
	db	00000001B
	db	11111110B

check	db	00000000B
	db	00000001B
	db	00000001B
	db	00000011B
	db	00000010B
	db	00000110B
	db	11000100B
	db	01001100B
	db	01101000B
	db	00111000B
	db	00110000B
	db	00010000B
	db	00000000B
	db	00000000B

	db	11111111B
	db	11111111B
	db	11000011B
	db	11000011B
	db	11000011B
	db	11000011B
	db	11000011B
	db	11000011B
	db	11000011B
	db	11000011B
	db	11000011B
	db	11111111B
	db	11111111B
	db	00000000B


DATA	ENDS

PROG	SEGMENT	BYTE PUBLIC 'PROG'
	ASSUME	CS:PGROUP
	ASSUME	DS:DGROUP

	public  cls, printn, printa, pos_cur, cur_off, cur_on, get_row, page_s
        public  cur_set, prn_logo, get_key, b_stuff, scr_up, scr_dn, ibm_type
        public  read_chr, beeper, egacard

;************************************************************************
; This routine determines if a monochrome card is in control of the 
; video.  If it is then character redefinition won't work.
egacard:
		push 	es
		mov	ax,0b000h
		mov	es,ax
		mov	bx,0
		mov	ax,es:[bx]
		pop	es
		ret

;this code never gets executed
		push 	es		;save es (just in case)
		mov	ax,40h		;BIOS data
		mov	es,ax
		mov	al,es:[87h]	;EGA?
		cmp	al,0		;al = 0 then mono or color card 
		je	mono		;no ega
		test	al,00001000B	;if bit 3 = 1 then ega not active
		jnz	mono		;assume mono
		pop	es
		mov	ax,1		;TRUE (no mono)
		ret
mono:		pop	es
		mov	ax,0		;FALSE (mono)
		ret
		
;************************************************************************
; This routine beeps the speaker.
beeper:
cycles		equ	50
frequ1		equ	300
frequ2		equ	1000
pb		equ 	61h
		cli
		mov	dx,cycles
		in	al,pb		;port b
		and	al,11111110B	;?
next_c:		or	al,00000010B
		out	pb,al
		mov	cx,frequ1
first_h:	loop	first_h		;delay
		and	al,11111101B
		out	61h,al
		mov	cx,frequ2
second_h:	loop 	second_h
		dec	dx
		jnz	next_c
		sti
		ret
	
;************************************************************************
; This routine returns the char and attribute of row 0, col 0. 
read_chr:
		mov	ah,2	;function #
		mov	dh,0	;row
		mov	dl,0	;column
		mov	bh,0	;page
		int	10h	;position cursor
		mov	ah,8	;function #
		mov	bh,0	;page
		int 	10h	;ah <- attribute, al <- char, (03C9)
		ret
                    
;************************************************************************
; This routine reutrns a "machine-type" code where:
;		FF	PC
;		FE	XT
;		FC	AT
;		FD	PCjr
ibm_type:
		mov	ax,0F000H
		mov	es,ax			;point ES at ROMs
		mov	al,es:[0fffeh]		;get byte
		mov	ah,0
		ret

;************************************************************************
; This routine scrolls logo upward.
scr_up:
	mov	ah,6
	mov	al,1
	mov	ch,0
	mov	cl,0
	mov	dh,24
	mov	dl,79
	mov	bh,7
	int	10h
	ret
scr_dn:
	mov	ah,7
	mov	al,1
	mov	ch,0
	mov	cl,0
	mov	dh,24
	mov	dl,79
	mov	bh,7
	int	10h
	ret
;************************************************************************
; This routine clears the character display between row1,col1 and row2,col2.
;
; C-CODE SYNTAX: cls(row1, col1, row2, col2)

cls:
	push	bp
	mov	bp,sp
	mov	ax,4[bp]	;row 1 (start)
	mov 	ch,al
	mov	ax,6[bp]	;col 1
	mov	cl,al
	mov	ax,8[bp]	;row 2 (stop)
	mov	dh,al
	mov	ax,10[bp]	;col 2
	mov	dl,al
	mov	ah,6		;scroll routine
	mov	al,0		;number of rows to scroll (0=clear rows)
	mov	bh,7		;attribute byte for fill (black)
	int	10h		;BIOS call
	pop 	bp
	ret
;	END 	cls
;************************************************************************
; This routine displays the GEMPREP logo at the top of the screen.  Because
;function 9 writes a char with an attribute but without bumping the cursor,
;each char is written twice. Once to set the attribute, and once to bumb the
;cursor. 

prn_logo:
	push 	bp
	mov	bp,offset DGROUP:bits	;redefine ascii 30, 31
	mov	ax,seg bits
	mov	es,ax
	mov	cx,2		;the number of chars to redefine
	mov	dx,30		;first ascii char to redefine
	mov	bl,0
	mov	bh,14
	mov	al,0
	mov	ah,11h
	int	10h
	mov	bp,offset DGROUP:check	;redefine ascii 16, 17 (check)
	mov	ax,seg check
	mov	es,ax
	mov	cx,2		;the number of chars to redefine
	mov	dx,16		;first ascii char to redefine
	mov	bl,0
	mov	bh,14
	mov	al,0
	mov	ah,11h
	int	10h
	pop	bp
	ret

;this routine emulates the C printf routine
printn:
	push	bp
	mov	bp, sp
	mov	si, 4[bp]	;get the string pointer
printn_lp:
	mov	al, [si]
	and	al, al		;is the the end of the string
	jz	printn_done
	cmp	al, 0ah		; is this an lf
	jz	printn_crlf
printn_ok:
	push	si
	mov	ah, 14
	mov	bx, 7
	int	10h
	pop	si
	inc	si
	jmp	printn_lp
printn_done:
	pop	bp
	ret
printn_crlf:
	inc	si
	push	si
	mov	ah, 14
	mov	al, 0ah
	mov	bx,7
	int	10h
	mov	ah, 14
	mov	al, 0dh
	mov	bx,7
	int	10h
	pop	si
	jmp	printn_lp
;
;
;this routine emulates the C printf routine only this routine
;prints the character in reverse video.
printa:
	push	bp
	mov	bp, sp
	mov	si, 6[bp]	;get the string pointer
printa_lp:
	mov	al, [si]
	and	al, al		;is the the end of the string
	jz	printa_done
	cmp	al, 0ah		; is this an lf
	jz	printa_crlf
printa_ok:
	push	si
; SET CHARACTER ATTRIBUTE
	mov	ah, 9
	mov	bx, 4[bp]	; Get attribute
	mov	bh,0		; Write to page 0
	mov	cx, 1
	int	10h
;PRINT CHARACTER WITH ATTRIBUTE
        mov 	ah, 14
	mov	bx, 7
	int	10h
	pop	si
	inc	si
	jmp	printa_lp
printa_done:
	pop	bp
	ret
printa_crlf:
	inc	si
	push	si
	mov	ah, 14
	mov	al, 0ah
	mov	bx,7
	int	10h
	mov	ah, 14
	mov	al, 0dh
	mov	bx,7
	int	10h
	pop	si
	jmp	printa_lp

;************************************************************************
; This routine positions the cursor at absolute x and y.
;
pos_cur:
	push	bp
	mov	bp,sp
	mov	ax,4[bp]	;get row
	mov	dh,al
	mov	ax,6[bp]	;get column
	mov	dl,al
	mov	ah,2
	mov	bh,0
	int	10h
	pop	bp
	ret
;************************************************************************
cur_set:
	push	bp
	mov	bp,sp
	mov	ax,4[bp]	;get start line
	mov	ch,al
	mov	ax,6[bp]	;get stop line
	mov	cl,al
	mov	ah,1
	int	10h
	pop	bp
	ret
;************************************************************************
cur_off:
	mov	dx,3b4h		;port for 6845 address reg
	mov	al,10		;reg 10
	out 	dx,al		;select request
	inc	dx		;next port
	mov	al,32		;bit 6 = 1, off
	out	dx,al		;turn off cursor
	ret
cur_on:
	mov	dx,3b4h		;port for 6845 address reg
	mov	al,10		;reg 10
	out 	dx,al		;select request
	inc	dx
	mov	al,11
	out	dx,al
	ret
;
get_row:
	mov	ah,3
	mov	bh,0
	int	10h
	mov 	ah,0
	mov	al,dh
	ret
;************************************************************************
page_s:
	push	bp
	mov 	bp,sp
	mov	ax,4[bp]
	mov	ah,5
	int	10h
	cmp	al,0
	jz	exit_s
	mov	ah,2				;set cursor position 0,0
	xor	dx, dx
	mov	bh,1 
	int	10h
	mov	ah, 9
	mov	bh,1
	mov	cx, 2000
	mov	al, 20h
	mov	bl, 7
	int	10h
exit_s:
	pop	bp
	ret

;************************************************************************
;
get_key:
; The following comment coded clears the input buffer by pointing the
;head_ptr and tail_ptr at the same address.  Absolute addresses are used
;though, which is why the code is commented out. C.Lund
;	cli			;disable interrupts
;	sub	ax,ax		;clear ax
;	push	es		;save es
;	mov	es,ax		;set es to bottom of memory
;	mov	al,es:[41ah]	;al points to buffer head
;	mov	es:[41ch],al	;head points to tail (i.e. buffer empty)
;	pop	es
;	sti			;enable interrupts
	mov	ah,0ch		;clear input buffer
	mov	al,6		;Don't wait for keystroke
	int	21h
;	mov	ah,2		;BEEP generator
;	mov	dl,7
;	int	21h
	mov	ah,0		;wait for keystroke
	int	16h		;get keystroke
	ret
;************************************************************************
;	b_stuff(pointer, count, value);					*
;		pointer:	ADDR	4[bp]				*
;		count:		WORD	8[bp]				*
;		value:		BYTE	10[bp]				*
;************************************************************************
b_stuff:
		push	bp
		mov	bp, sp

; Set the requested number of bytes to the requested value.
		les	di, 4[bp]		; es:di = destination address
		mov	cx, 8[bp]		; cx = count
		mov	al, 10[bp]		; al = value to set
	rep	stosb				; stuff

; Restore and exit.
		pop	bp
		ret

PROG	ENDS


	end
