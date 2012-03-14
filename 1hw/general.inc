; es:di - long number
; cx    - length in words
fill_zeros:
		push	ax
		push	cx
		push	di
		xor	ax, ax
		rep	stosw
		pop	di
		pop	cx
		pop	ax
		ret

; ds:di - long number
; cx    - length in words
; bx    - summand
add_long:
		push	ax
		push	bx
		push	cx
		push	dx
		push	di
@@:
		or	cx, cx
		jz	@f

		xor	dx, dx
		add	[di], bx
		adc	dx, 0
		mov	bx, dx
		add	di, 2
		dec	cx
		jmp	@b
@@:
		pop	di
		pop	dx
		pop	cx
		pop	bx
		pop	ax
		ret

; ds:si - number 1
; ds:di - number 2
; cx    - length in words
; result is stored in ax
; 2 if [si] > [di], 1 if equal, 0 - other
compare_2long:
		push	di
		push	si
		push	cx
		mov 	ax, 1
cmp1:
		or	cx, cx
		jz	cmp3

		cmpsw
		jz	cmp2
		mov	ax, 2
		jae	cmp2
		mov 	ax, 0
cmp2:
		dec	cx
		jmp	cmp1
cmp3:
		pop	cx
		pop	si
		pop	di
		ret

; ds:di - long number
; cx    - length in words
; bx    - multiplier
mul_long:
		push	ax
		push	bx
		push	cx
		push	dx
		push	di
		push	si

		xor	si, si

@@:
		or	cx, cx
		jz	@f

		mov	ax, [di]
		mul	bx
		add	ax, si
		adc	dx, 0
		mov	[di], ax
		add	di, 2
		mov	si, dx

		dec	cx
		jmp	@b

@@:
		pop	si
		pop	di
		pop	dx
		pop	cx
		pop	bx
		pop	ax
		ret


; ds:di - long number
; cx    - length in words
; bx    - divisor
; returns remainder in dx
div_long:
		push	ax
		push	cx
		push	di

		add	di, cx
		add	di, cx
		xor	dx, dx

@@:
		or	cx, cx
		jz	@f

		sub	di, 2
		mov	ax, [di]
		div	bx
		mov	[di], ax

		dec	cx
		jmp	@b
@@:
		pop	di
		pop	cx
		pop	ax
		ret

; ds:di - long number
; cx    - length in words
; returns result in cf
is_zero:
		push	cx
		push	di
@@:
		or	cx, cx
		jz	@f
		mov	ax, [di]
		add	di, 2
		dec	cx
		or	ax, ax
		jz	@b
		clc
		jmp	is_zero_quit
@@:
		stc
is_zero_quit:
		pop	di
		pop	cx
		ret

; ds:di - long number
; cx    - length in words
print_long:
		mov	si, print_long_buf + print_long_buf_size
		dec	si
		mov	byte [si], '$'
		dec	si
		mov	byte [si], 10
		dec	si
		mov	byte [si], 13
		mov	bx, 10

@@:
		call	div_long
		add	dl, '0'
		dec	si
		mov	[si], dl

		call	is_zero
		jnz	@b

		mov	ah, 9
		mov	dx, si
		int	21h

		ret

; ds:di - long number
; cx    - length in words
read_long:
		push	es
		mov	ax, ds
		mov	es, ax
		call	fill_zeros
		pop	es

read_long_read_again:
		mov	ah, 8
		int	21h

		cmp	al, 13
		jz	read_long_quit

		cmp	al, '0'
		jb	read_long_read_again

		cmp	al, '9'
		ja	read_long_read_again

		mov	dl, al
		push	dx
		mov	ah, 2
		int	21h
		pop	dx

		mov	bx, 10
		call	mul_long

		sub	dl, '0'

		xor	bx, bx
		mov	bl, dl
		call	add_long

		jmp	read_long_read_again

read_long_quit:
		mov	ah, 9
		mov	dx, crlf
		int	21h

		ret


; ds:di - long number 1
; ds:si - long number 2
; cx    - length in words
; result is stored in ds:di
add_2long:
		push	ax
		push	bx
		push	cx
		push	di
		push	si

		xor	ax, ax
@@:
		or	cx, cx
		jz	@f

		xor	bx, bx
		add	ax, [di]
		adc	bx, 0
		add	ax, [si]
		adc	bx, 0
		mov	[di], ax
		mov	ax, bx

		add	di, 2
		add	si, 2
		dec	cx
		jmp	@b

@@:
		pop	si
		pop	di
		pop	cx
		pop	bx
		pop	ax
		ret


; ds:si - long number 1
; ds:di - long number 2
; cx    - length in words
; result is stored in ds:di
sub_2long:
		pusha
		xor	dx, dx
@@:
		or	cx, cx
		jz	@f

		xor	bx, bx

		mov	ax, [si]
		sbb	ax, [di]
		adc	bx, 0
		sbb	ax, dx
		adc	bx, 0
		mov dx, bx
		mov	[di], ax

		add	di, 2
		add	si, 2
		dec	cx
		jmp	@b
@@:
		popa
		ret

;ds:si - long number 1
;ds:di - long number 2
;cx    - length in words
;result is stored in res2
mul_2long:
		pusha
		push di
		mov	di, res
		call fill_zeros
		pop di

		xor	dx, dx
mul1:
		or	cx, cx
		jz	mul3

		push cx
		push si
		push di

		mov	ax, [di] 
		or	ax, ax
		jz	mul2

		mov	di, temp
		mov	cx, numsize

		call fill_zeros
		call add_2long
		mov	bx, 100h
		mov	cx, dx
@@:
		or cx, cx
		jz @f
		push cx
		mov	cx, numsize
		call mul_long
		call mul_long	
		pop	cx

		dec	cx
		jmp	@b
@@:
		mov	bx, ax
		mov	cx, numsize
		call mul_long

		mov	di, res
		mov	si, temp
		call add_2long
mul2:
		inc	dx
		pop di
		pop si
		pop	cx

		add	di, 2
		dec	cx
		jmp	mul1
mul3:
		popa
		ret

; ds:si - long number 1
; ds:di - long number 2
; cx    - length in words
; result is stored in mid
div_2long:
		pusha

		mov	dx, si
		mov	bx, di

		mov	di, left	
		call fill_zeros

		mov	di, right	
		call fill_zeros

		call add_2long
div1:
		push bx
		push dx

		mov	di, mid
		call fill_zeros
	
		mov si, left
		call add_2long
	
		mov si, right
		call add_2long
	
		mov	bx, 2
		call div_long
	
		pop	dx
		pop	bx

		mov	si, left
		call compare_2long
		cmp	ax, 1
		jz div1_2

		mov	si, right
		call compare_2long
		cmp	ax, 1
		jz	div1_2

		mov	si, bx	
		call mul_2long
		
		mov	si, res
		mov di, dx
		call compare_2long
		cmp	ax, 1
		jz div2
		ja div1_1

		mov	di, left
		call fill_zeros
		
		mov	si, mid
		call add_2long
		jmp	div1
div1_1:
		mov	di, right
		call fill_zeros
		
		mov	si, mid
		call add_2long
		jmp	div1
div1_2:
		mov	si, right
		mov	di, dx
		call mul_2long
		
		mov	si, res
		call compare_2long
		cmp	ax, 2
		jz div2
		
		mov	di, mid
		call fill_zeros
		call add_2long
div2:
		popa
		ret


crlf:		db	13,10,'$'

numsize = 1024
arg1:		rw	numsize
arg2:		rw	numsize
temp:		rw	numsize
res:		rw	numsize
left:		rw	numsize
mid:		rw	numsize
right:		rw	numsize

print_long_buf_size = 8192

print_long_buf: rb	print_long_buf_size

