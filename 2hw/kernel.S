#include "kernel.h"
        
        .text   
        .code16
.globl _start
_start:

        movw    $videobuffer_segment_RM, %ax
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
        .short code_segm_selector
        
// Protected Mode
.code32
protected_entry:
        
        // data and stack segment
        movw    $data_segm_selector, %ax
        movw    %ax, %ds
        movw    %ax, %ss
        movl    $0xFFFF, %esp

        // video segment
        movw    $video_segm_selector, %ax
        movw    %ax, %es

        
        movw    $0xFFFF, %dx
        movw    $0x2820, %bx
        call    redirect_IRQ
        
        
        // turn on interrupts
        inb     $CMOS_index_port, %al
        xorb    $NMI_bit, %al
        outb    %al, $CMOS_index_port
        sti                     // turn on maskable interrupts
        

        // wait for interrupts
        jmp     .
