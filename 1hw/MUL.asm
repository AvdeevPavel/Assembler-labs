	org	100h

	mov		di, arg1
	mov		cx, numsize
	call	read_long

	mov		di, arg2
	mov		cx, numsize
	call	read_long

   	mov    	si, arg1
	mov     di, arg2  
	mov     cx, numsize
	call    mul_2long
    
	mov		di, res
	mov     cx, 2*numsize
	call	print_long

	mov	ax, 4c00h
	int	21h

include 'general.inc'