#include "kernel.h"

        .code32

        .globl  update_display
        
cursor:
        .long   0

        
        
clear_screen:
        movl    $0, ABS(cursor)
        
        pushl   %eax
        pushl   %ecx
        pushl   %edi

        xorl    %ecx, %ecx
        movw    $videobuffer_size, %cx
        xorl    %edi, %edi
        movb    $space_char, %al
        movb    $0x07, %ah
        rep stosw

        popl     %edi
        popl     %ecx
        popl     %eax
        ret


// eax - printed number
// cursor - current cursor
print_long:
        pushl   %ebp
        movl    %esp, %ebp
#define buffer_size 20
        subl    $buffer_size, %esp // buffer

        pushl   %edi
        pushl   %esi
        pushl   %ebx
        pushl   %edx
        pushl   %ecx

        // setting buffer
        xorl    %esi,%esi
        xorl    %edx,%edx
        movl    ABS(cursor), %edi
        movl    $10, %ebx
        
1:
        divl    %ebx
        movb    $0x07, %dh
        addb    $0x30, %dl
        movw    %dx, -buffer_size(%ebp,%esi)
        xorl    %edx,%edx
        
        addl    $2, %esi
        
        orl     %eax, %eax
        jnz     1b

        movl    %esi, %ecx
        shrl    $1, %ecx        
        subl    $2, %esi
        // printing
        
1:
        movw    -buffer_size(%ebp,%esi), %dx
        movw    %dx, %es:(%edi)

        addl    $2,%edi
        subl    $2,%esi

        dec     %ecx
        jnz     1b

        movl    %edi, ABS(cursor)
        
        popl    %ecx
        popl    %edx
        popl    %ebx
        popl    %esi
        popl    %edi

        movl    %ebp,%esp
        popl    %ebp
        ret

newline:
        pushl   %eax
        pushl   %ebx
        pushl   %edx
        
        movl    ABS(cursor), %eax
        movl    $160, %ebx
        xorl    %edx, %edx        
        divl    %ebx
        xorl    %edx, %edx        
        mull    %ebx
        addl    %ebx, %eax
        movl    %eax, ABS(cursor)

        popl    %edx
        popl    %ebx
        popl    %eax
        ret

update_display:
        pushl   %esi
        pushl   %edi
        pushl   %eax
        
        call    clear_screen
        
        movl    $scan_codes_buffer_size, %ecx
        movl    $ABS(scan_codes_buffer), %esi
        xorl    %eax,%eax
1:      
        orl     %ecx, %ecx
        jz      1f


        movb    (%esi), %al
        call    print_long

        addl    $2,ABS(cursor)
        dec     %ecx
        inc     %esi
        jmp     1b
1:      
        call    newline

        movl    ABS(count_interrupts), %eax
        call    print_long

        popl    %eax
        popl    %edi
        popl    %esi
        ret