#include "kernel.h"

.globl GDTR
.globl IDTR   
.globl screen_cursor
.globl task1_descr
.globl task2_descr
.globl gp_error_message
.globl gp_error_message_size
  
.p2align 3
GDT:
        .quad   0 // empty descriptor
//             [ LIMIT    |      BASE    |  PDLSTYPE    GD0ALIMT | BASE ]
code_descr:
        .byte   0xFF,0xFF, 0x00,0x00,0x00, 0b10011010, 0b11001111,  0x00
data_descr:
        .byte   0xFF,0xFF, 0x00,0x00,0x00, 0b10010010, 0b11001111,  0x00
video_descriptor:
        .byte   0xFF,0xFF, 0x00,0x80,0x0B, 0b10010010, 0b01000011,  0x00
       
task0_descr:
        .byte   0x67,0x00
        .short             task0_tss  
        .byte                        0x01,  0b10001001, 0b01000000,  0x00 // note 0x01
        
task1_descr:
        .byte   0x67,0x00
        .short             task1_tss  
        .byte                        0x01,  0b10001001, 0b01000000,  0x00 // note 0x01
        
task2_descr:
        .byte   0x67,0x00
        .short             task2_tss  
        .byte                        0x01,  0b10001001, 0b01000000,  0x00 // note 0x01

task0_gate_descr:
        .byte   0x00,0x00
        .short             task0_selector
        .byte                        0x00,  0b10000101, 0b11001111,  0x00


task1_gate_descr:
        .byte   0x00,0x00
        .short             task1_selector
        .byte                        0x00,  0b10000101, 0b11001111,  0x00

task2_gate_descr:
        .byte   0x00,0x00
        .short             task2_selector
        .byte                        0x00,  0b10000101, 0b11001111,  0x00

.set    GDT_size, . - GDT
GDTR:
        .short  GDT_size - 1
        .long   ABS(GDT)

.p2align 3
IDT:
//           [  Offset                |  Segment           |   PDL0D110000-----| Offset ]        
	.space  13*8,0 // 0-12
        .short  exGP_handler,         code_segment_selector, 0b1000111000000000, 1   // 13  #GP
        .space  18*8, 0 // 14-31
        .short  int8_handler,         code_segment_selector, 0b1000111000000000, 1   // IRQ 0 - system timer
        .short  int_EOI,              code_segment_selector, 0b1000111000000000, 1   // IRQ 1 - keyboard
        
.set    IDT_size, . - IDT
IDTR:
        .short  IDT_size-1
	.quad   ABS(IDT)

TSS:
task0_tss:    
        .space  tss_size, 0
task1_tss:
//            task_link esp0    ss0     esp1    ss1     esp2    ss2
        .long   0,      0,      0,      0,      0,      0,      0
//              cr3     eip                     eflags  eax     ecx     edx     ebx     esp     ebp     esi     edi
        .long   0,      ABS(task1_worker),      0,      0,      0,      0,      0,      0x7fff, 0,      0,      0	//functions task worker
//              es                            cs,                    ss,                    ds,                   fs,     gs,     LDT     iomap
        .long   videobuffer_segment_selector, code_segment_selector, data_segment_selector, data_segment_selector, 0,     0,      0,      0

task2_tss:
//            task_link esp0    ss0     esp1    ss1     esp2    ss2
        .long   0,      0,      0,      0,      0,      0,      0
//              cr3     eip                     eflags  eax     ecx     edx     ebx     esp     ebp     esi     edi
        .long   0,      ABS(task2_worker),      0,      0,      0,      0,      0,      0xFFFF, 0,      0,      0	//functions task worker
//              es                            cs                     ss,                    ds                    fs,     gs      LDT     iomap
        .long   videobuffer_segment_selector, code_segment_selector, data_segment_selector, data_segment_selector, 0,     0,      0,      0

screen_cursor:
        .long   0

gp_error_message:
        .ascii  "PROTECTION FAULT"
        .set    gp_error_message_size, . - gp_error_message
        
