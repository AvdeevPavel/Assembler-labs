#include "kernel.h"
        .code32

        .globl  count_interrupts
        .globl  scan_codes_buffer

        .globl  redirect_IRQ
        .globl  int8_handler
        .globl  int9_handler
        .globl  exGP_handler
        
count_interrupts:
        .long   0

scan_codes_buffer:
        .space  scan_codes_buffer_size, 0

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
        incl    ABS(count_interrupts)
        call    update_display
        jmp     int_EOI
        
// keyboard handler
int9_handler:        
        pushl   %eax
        pushl   %edi
        pushl   %ecx
        xorl    %eax, %eax

        // request scan-code
        inb     $0x60, %al
        decb    %al
    
        // only touch-down
        movb    %al, %ah
        andb    $0x80, %ah
        jnz     __clear_request
        
        pushw   %ax
    
        // shift scan codes
        movl    $ABS(scan_codes_buffer+scan_codes_buffer_size-1), %edi
        movl    $(scan_codes_buffer_size-1), %ecx
1:      
        orl     %ecx, %ecx
        jz      1f

        movb    -1(%edi), %al
        movb    %al, (%edi)

        dec     %edi
        dec     %ecx
        jmp     1b
1:
    
        popw    %ax
        movb    %al, ABS(scan_codes_buffer)
        
        // send confirmation to keyboard
        inb     $0x61, %al
        orb     $0x80, %al
        outb    %al, $0x61
        xorb    $0x80, %al
        outb    %al, $0x61

__clear_request:
        popl    %ecx
        popl    %edi
        popl    %eax
        call    update_display
        jmp     int_EOI
        
// end of interrupt
int_EOI:
        pushw   %ax
        movb    $0x20, %al
        outb    %al, $master_interrupt_service_register
        outb    %al, $slave_interrupt_service_register
        popw    %ax
        iret