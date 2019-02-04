
; Program..: LHGEM 1.1
; Author...: Ken Mauro
; Date.....: July 4,1999
; Notice...: (C)1994-1999 Ken Mauro, all rights reserved.
;                         Free for non-commercial use.
; Notes....: INT21h 4bh=DOS EXEC
; Purpose..: GEM AES UMB loader for MSDOS 5/6


stdin	equ	0
stdout	equ	1
stderr	equ	2

cr	equ	0dh
lf	equ	0ah
blank	equ	20h


cseg	segment para public 'CODE'	;Program Segment

	assume cs:cseg, ds:data, ss:stack

shell	proc far

	mov ax,data
	mov ds,ax
	mov ax,es:[002ch]		; get segment of environment block
	mov env_seg,ax			; store it

	mov bx,3072/16			; shrink un-needed memory (in decimal)
	mov ah,4ah			; dos modify memory block
	int 21h				; call dos, execute it.
	jnc dcode1			; jump if succesful
	mov dx,offset msg1		; else load error message
	mov cx,msg1_length
	jmp error			; error exit


dcode1: mov bx, offset inp_key
	mov dx,[bx]
	mov bx, offset inp_buf_len	; len of filename in patch buffer
	mov cx,[bx]			; stored in low, high = always 0
	mov bx, offset inp_buf		; start of our patch buffer
dloop1:	mov al,[bx]			; read byte
	sub al,dl			; subtraction factor (key) 40h
	mov byte ptr [bx],al		; store it back
	inc bx
	dec cx
	cmp cx,0
	jne dloop1

dcode2:	mov bx,offset msg0_key
	mov dx,[bx]
	mov bx,offset msg0_len		;len of filename in patch buffer
	mov cx,[bx]			; stored in low, high = always 0
	mov bx, offset msg0		; start of our patch buffer
dloop2:	mov al,[bx]			; read byte
	sub al,dl			; subtraction factor (key) 40h
	mov byte ptr [bx],al		; store it back
	inc bx
	dec cx
	cmp cx,0
	jne dloop2

	sub dx,dx			; checksum routine
	mov bx,offset msg0_len		; length of msg0  buffer
	mov cx,[bx]			; stored in low, high counter
	mov bx, offset msg0		; start of msg0 buffer
	mov ah,0
dloop3:	mov al,[bx]			; read byte
	add dx,ax			; add to tally register
	inc bx
	dec cx
	cmp cx,0
	jne dloop3
	mov al,[bx]
	cmp dl,al
	jne error			; low byte failed
	inc bx
	mov al,[bx]
	cmp dh,al
	jne error			; high byte failed

print1:	mov dx,offset msg0		; point to title message
	mov cx,msg0_length
	mov bx,stdout			; print it
	mov ah,40h
	int 21h

	call time_delay			; press any key, or time delay

	call get_comspec		; find where our command.com is located
	jnc setvct
	mov dx,offset msg3
	mov cx,16
	jmp error			; error exit
setvct:	mov dx,offset exec		;set to our ctrl-c vector (int23h)
	mov ax,cs
	mov ds,ax
	mov ax,2523h
	int 21h
	mov ax,data
	mov ds,ax
	mov es,ax
exec:					;setup and execute an extrinsic command
	mov al,cr
	mov cx,cmd_tail_length
	mov di,offset cmd_tail+1
	cld
	repnz scasb
	mov ax,di
	sub ax,offset cmd_tail+2
	mov cmd_tail,al
	mov par_cmd,offset cmd_tail
	push ds				; start of exec
	push es
	mov cs:stk_seg,ss		; save stack segment
	mov cs:stk_ptr,sp		; save stack pointer
	mov dx,offset com_spec		; get command.com location
	mov bx,offset par_blk		; get parameter block
	mov al,0			; load and execute a program
	mov ah,4bh			; INT 21h 4Bh=EXEC
	int 21h				; call dos and do it.
	mov ss,cs:stk_seg		; restore stack segment
	mov sp,cs:stk_ptr		; restore stack pointer
	pop es
	pop ds
	jnc exit
error:
	mov bx,stderr
	mov ah,40h
	int 21h
	mov ax,4c01h			; set error return code 1
	int 21h				; exit to DOS
exit:	mov ax,4c00h			; set good return code 0
	int 21h				; exit to DOS
shell	ENDP

	stk_seg	dw	0		; these must be in cseg space
	stk_ptr	dw	0


time_delay	PROC near


time1:	mov ah,2ch			; Title message delay
	int 21h
	mov bh,dh
	add bh,1			; set delay in seconds
	cmp bh,59
	jge time1
time2:	mov ah,2ch
	int 21h
	cmp dh,bh
	jne time2
;	mov ah,8			; press any key to continue.
;	int 21h				; only for unregistered version.
	ret

time_delay	ENDP



get_comspec	PROC near

	mov si,offset com_var
       call get_env
	 jc gcsp2
	mov si,offset com_spec
gcsp1:	mov al,es:[di]
	mov [si],al
	inc si
	inc di
	 or al,al
	jnz gcsp1
gcsp2:	ret

get_comspec	ENDP


get_env		PROC near
	mov es,env_seg
	xor di,di
genv1:	mov bx,si
	cmp byte ptr es:[di],0
	jne genv2
	stc
	ret
genv2:	mov al,[bx]
	or  al,al
	 jz genv3
	cmp al,es:[di]
	jne genv4
	inc bx
	inc di
	jmp genv2
genv3:	ret
genv4:	xor al,al
	mov cx,-1
	cld
	repnz scasb
	jmp genv1

get_env ENDP
cseg	ENDS					; end of cseg (code segment)


stack	segment para stack 'STACK'		; declare stack segment
	dw	64 dup (?)
stack	ends

data	segment para public 'DATA'		; declare data segment

msg1	db	cr,lf,'LHgem unable to free memory.',cr,lf
msg1_length	equ $-msg1
msg2	db	cr,lf,'LoadHigh function failed.',cr,lf
msg2_length	equ $-msg2
msg3	db	cr,lf,'No COMSPEC= in environment.',cr,lf
msg3_length	equ $-msg3

com_var		db	'COMSPEC=',0		; environment variable to match
com_spec	db	64 dup (0)		; MAXLEN of comspec variable

null_tail	db	0,cr
cmd_tail	db	0,' /C '

inp_buf		db	64 dup (0)
inp_buf_length	equ 	$-inp_buf
inp_buf_len	dw	inp_buf_length
inp_key		dw	0

cmd_tail_length	equ 	$-cmd_tail-1

env_seg		dw	0
par_blk		equ	$
		dw	0
par_cmd		dw	offset cmd_tail
		dw	seg cmd_tail
		dd	-1
		dd	-1


msg0	db	'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿',cr,lf
	db	'³   LHgem 1.1 - GEM AES  UMB HiLoader (MS-DOS)   ³',cr,lf
	db	'³                                                ³',cr,lf
	db	'³   (C)1994-1999 Ken Mauro, all rights reserved. ³',cr,lf
	db	'³                Free for non commercial use.    ³',cr,lf
	db	'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ',cr,lf
msg0_length	equ $-msg0
checksum	dw	0
msg0_key	dw	0
msg0_len	dw	msg0_length	;must be last word for config program.

data		ENDS			;end of data segment

		END	shell



