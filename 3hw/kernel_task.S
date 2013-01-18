#include "kernel.h"

.code32
.globl  task_switcher
.globl  task1_worker
.globl  task2_worker

task_switcher:
	cli
	inb $0x70, %al
	orb $0x80, %al 
	out %al, $0x70

	push %ax

	movw $task1_descr + 40, %ax
	test %ax, 0b00000010
	jne to2

to1:
	andb 0b11111101, %ax
	movw %ax, (task2_descr + 40)	
	pop %ax
	jmp task1_worker

to2:
	andb 0b11111101, %ax
	movw %ax, (task1_descr + 40)	
	pop %ax
	jmp task2_worker
	        
task1_worker:
	inb $0x70, %al
	orb $0x7F, %al 
	out %al, $0x70
        sti
        call    clear_screen
1:
        movb    $0b01010101, %al
        call    clear_screen
        hlt

        jmp     1b

task2_worker:
	inb $0x70, %al
	orb $0x7F, %al 
	out %al, $0x70
        sti
        call    clear_screen
1:      
        movb    $0b11111111, %al
        call    clear_screen
        hlt
        
        jmp     1b
        

