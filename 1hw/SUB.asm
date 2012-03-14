	org	100h

	mov	di, arg1
	mov	cx, numsize
	call	read_long

	mov	di, arg2
	mov	cx, numsize
	call	read_long

	mov	si, arg1
	mov	cx, numsize
	call	sub_2long

	call	print_long

	mov	ax, 4c00h
	int	21h

include 'general.inc'