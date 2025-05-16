.text
.globl _start
_start:
    mov     $13,%rdx
    leaq    msg,%rsi
    movq $1, %rdi
    movl $0x01, %eax
    syscall

    xorl    %edi, %edi 
    mov     $0x3c, %eax
    syscall

.section .rodata
msg: .asciz "Hello world!\n"

