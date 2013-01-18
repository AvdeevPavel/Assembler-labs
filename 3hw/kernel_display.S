#include "kernel.h"

.code32
.globl  clear_screen

// input: al - cleaning color        
clear_screen:
	movl    $0, ABS(screen_cursor)
        
        pushl   %eax
        pushl   %ecx
        pushl   %edi

        movl    $videobuffer_size, %ecx
        xorl    %edi, %edi

1:      
        test    %ecx, %ecx
        jz      1f
        
        movb    %al,%es:(%edi)
        inc     %edi
        dec     %ecx
        jmp     1b

1:      
        popl    %edi
        popl    %ecx
        popl    %eax
        ret

