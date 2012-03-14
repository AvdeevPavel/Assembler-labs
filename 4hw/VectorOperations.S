#include "VectorOperations.h"
#define SSE3_BIT 0x1
#define SSE4_BIT 0x80000
.data
temp_buffer:
	.space 4 

.text
.globl add
.type add, @function
// x - in eax
// y - in ebx
// size - in esi
//output - in x  
add: 
	pushl %eax
	pushl %ebx
	pushl %esi
	movl 16(%esp), %eax
	movl 20(%esp), %ebx
	movl 24(%esp),%esi 
	
	xorps %xmm1, %xmm1
	xorps %xmm2, %xmm2
a_1: 
	cmpl $4, %esi

	jb ea_1

	movups (%eax), %xmm1
	movups (%ebx), %xmm2
	addps %xmm2, %xmm1
	movups %xmm1, (%eax) 
	
	addl $16, %eax
	addl $16, %ebx 
	subl $4, %esi	
	jmp a_1
ea_1:
	test %esi, %esi
	jz endAdd
	movss (%eax), %xmm1
	movss (%ebx), %xmm2
	addss %xmm2, %xmm1
	movss %xmm1, (%eax) 
	addl $4, %eax
	addl $4, %ebx 
	decl %esi	
	jmp ea_1	
endAdd:
	pop %esi
	pop %ebx
	pop %esi
	ret
				
.globl sub
.type sub, @function
// x - in eax
// y - in ebx
// size - in esi
//output - in x  
sub: 
	pushl %eax
	pushl %ebx
	pushl %esi
	
	movl 16(%esp), %eax
	movl 20(%esp), %ebx
	movl 24(%esp),%esi

	xorps %xmm1, %xmm1
	xorps %xmm2, %xmm2
s_1: 
	cmpl $4, %esi

	jb es_1

	movups (%eax), %xmm1
	movups (%ebx), %xmm2
	subps %xmm2, %xmm1
	movups %xmm1, (%eax) 
	
	addl $16, %eax
	addl $16, %ebx 
	subl $4, %esi	
	jmp s_1

es_1:
	test %esi, %esi
	jz endSub
	movss (%eax), %xmm1
	movss (%ebx), %xmm2
	subss %xmm2, %xmm1
	movss %xmm1, (%eax) 
	addl $4, %eax
	addl $4, %ebx 
	decl %esi	
	jmp es_1	
endSub:
	pop %esi
	pop %ebx
	pop %esi
	ret

.globl whatSse
.type whatSse, @function
//output - in eax
whatSse:
	pushl %ecx
	pushl %edx
	pushl %ebx

	movl $1, %eax
	cpuid
1: 
	test $SSE3_BIT, %ecx 
	jnz 1f
	movl $NONE_SUPPORTED, %eax
	jmp endSse
1: 	
	test $SSE4_BIT, %ecx	
	jnz 1f
	movl $SSE3_SUPPORTED, %eax
	jmp endSse	
1:
	movl $SSE41_SUPPORTED, %eax
endSse: 
	popl %ebx
	popl %edx
	popl %ecx
	ret

.globl mulSSE3
.type mulSSE3, @function
// x - in eax
// y - in ebx
// size - in esi
//output - in st(0)  
mulSSE3:
	pushl %eax
	pushl %edi
	pushl %edx
	movl 16(%esp), %eax
	movl 20(%esp), %edi
	movl 24(%esp), %edx
 
	xorps %xmm1, %xmm1
	xorps %xmm2, %xmm2
	xorps %xmm0, %xmm0	
m3_1: 
	test %edx, %edx
	jz em3_1

	movups (%eax), %xmm1
	movups (%edi), %xmm2

	mulps %xmm2, %xmm1
	haddps %xmm1, %xmm1 // a0 + a1 | a3 + a2 | a0 + a1 | a2 + a3 | 
	haddps %xmm1, %xmm1 // a0 + a1 + a3 + a2| ..  | .. | .. |
	addps %xmm1, %xmm0	

	addl $16, %eax
	addl $16, %edi 
	subl $4, %edx	
	jmp m3_1
em3_1:  
	movss %xmm0, (temp_buffer)
	fld (temp_buffer)
	popl %edx
	popl %edi
	popl %eax
	ret

.globl mulSSE41
.type mulSSE41, @function
// x - in eax
// y - in ebx
// size - in esi
//output - in st(0)  
mulSSE41: 
	xorps %xmm1, %xmm1
	xorps %xmm2, %xmm2
	xorps %xmm0, %xmm0	
m41_1: 
	test %edx, %edx
	jz em41_1

	movups (%eax), %xmm1
	movups (%edi), %xmm2

	dpps $0b11110001, %xmm2, %xmm1
	addps %xmm1, %xmm0	

	addl $16, %eax
	addl $16, %edi 
	subl $4, %edx	
	jmp m41_1
em41_1: 
	movss %xmm0, (temp_buffer)
	fld (temp_buffer)
	ret


