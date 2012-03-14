#include "kernel.h"

// Data
        .globl  GDTR
        .globl  IDTR
        
        .p2align 3
GDT:
        .quad   0 // empty descriptor
//             [ LIMIT    |      BASE    |  PDLSTYPE    GD0ALIMT | BASE ]
code_descr:
        .byte   0xFF,0xFF, 0x00,0x00,0x00,0b10011010, 0b11001111,  0x00
data_descr:
        .byte   0xFF,0xFF, 0x00,0x00,0x00,0b10010010, 0b11001111,  0x00
video_descriptor:
        .byte   0xFF,0xFF, 0x00,0x80,0x0B,0b10010010, 0b01000000,  0x00

        .set    GDT_size, . - GDT
        
GDTR:
        .short  GDT_size-1
        .long   ABS(GDT)

        .p2align 3
IDT:
        .space  32*8,0 // 0-32
//           [  Offset                |  Segment          |   PDL0D110000-----| Offset ]        
        .short  int8_handler,           code_segm_selector, 0b1000111000000000, 1   // IRQ 0 - system timer
	.short  int9_handler,           code_segm_selector, 0b1000111000000000, 1   // IRQ 1 - keyboard
        
        .set    IDT_size, . - IDT
IDTR:
        .short  IDT_size-1
	.quad   ABS(IDT)
        
