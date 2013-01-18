#include "kernel.h"
        
.text   
.code16
.globl _start

_start:
	// set CGA 320x200 mode
        xorw    %ax, %ax
        movb    $0x04, %al
        int     $0x10

        movw    $videobuffer_segment_real, %ax
        movw    %ax, %es

        movw    %cs, %ax
        movw    %ax, %ds        
        
        xorw    %ax, %ax
        movw    %ax, %ss
        xorw    %sp, %sp
        
        // open A20-line
        inb     $0x92, %al
        orb     $2, %al
        out     %al, $0x92

        // turn off interrupts
        cli
        in      $CMOS_index_port, %al
        or      $NMI_bit, %al
        out     %al, $CMOS_index_port

        lgdt    (GDTR)
        lidt    (IDTR)

        // to PM
        movl    %cr0, %eax
        orb     $PM_bit, %al
        movl    %eax, %cr0

        // jump to 32bit segm
        .byte 0x66                      // prefix of 32 command
        .byte 0xEA                      // opcode of ljmp
        .long ABS(protected_entry)      // offset of entry
        .short code_segment_selector     

//Protected mode
.code32
.globl error
protected_entry:
        // setting data and stack segment
        movw    $data_segment_selector, %ax
        movw    %ax, %ds
        movw    %ax, %ss
        movl    $0xFFFF, %esp

        // setting videobuffer segment
        movw    $videobuffer_segment_selector, %ax
        movw    %ax, %es

        movw    $0xFFFF, %dx
        movw    $0x2820, %bx
        call    redirect_IRQ

        // load task0 descr        
        movw    $task0_selector, %ax
        ltr     %ax

        // turn on NMI
        inb     $CMOS_index_port, %al
        xorb    $NMI_bit, %al
        outb    %al, $CMOS_index_port
	sti
        
        jmp     task_switcher

error:
        call    clear_screen
        jmp     halt_system
        
halt_system:
        hlt
        jmp     halt_system

