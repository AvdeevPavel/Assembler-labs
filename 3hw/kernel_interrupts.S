#include "kernel.h"
        
.code32
.globl  redirect_IRQ
.globl  int8_handler
.globl  exGP_handler
.globl  int_EOI

redirect_IRQ:
	// bx - { bl - beginning interrupt descr for IRQ 0..7, bl - same for IRQ 8..15 }
	// dx - IRQ interrupt mask (dl for 0..7, dh for 8..15)
        pushl   %ecx
        pushl   %eax
        
        
        // APIC Off
        movl    $0x1b, %ecx
        rdmsr
        orb     $0b1000, %ah
        wrmsr

        movb    $0x11, %al
        outb    %al, $slave_interrupt_service_register
        outb    %al, $master_interrupt_service_register
        
        movb    %bh, %al
        outb    %al, $0xa1
        movb    %bl, %al
        outb    %al, $0x21
        
        movb    $2, %al
        outb    %al, $0xa1
        movb    $4, %al
        outb    %al, $0x21

        movb    $1, %al
        outb    %al, $0xa1
        outb    %al, $0x21

        movb    %dh, %al
        outb    %al, $0xa1
        movb    %dl, %al
        outb    %al, $0x21

        // APIC On
        movl    $0x1b, %ecx
        rdmsr
        andb     0b11110111, %ah
        wrmsr

        popl    %eax
        popl    %ecx
        ret

        
// timer handler
int8_handler:
        pushw   %ax
        movb    $0x20, %al
        outb    %al, $master_interrupt_service_register
        outb    %al, $slave_interrupt_service_register
        popw    %ax
       
	jmp task_switcher
        
// end of interrupt
int_EOI:
        pushw   %ax
        movb    $0x20, %al
        outb    %al, $master_interrupt_service_register
        outb    %al, $slave_interrupt_service_register
        popw    %ax
        iret

exGP_handler:
        pop     %eax // error code
        movl    $(gp_error_message+addr_read_kernel), %esi 
        movl    $gp_error_message_size, %eax
        int     $0x80
        jmp     error
