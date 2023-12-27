.text
    .globl lockmutex, unlockmutex
    .type unlockmutex, @function
    .type lockmutex, @function

lockmutex:
    
    mov 0x4(%esp), %eax
    mov $1, %ebx
    xchg (%eax), %ebx
    cmp $0, %ebx
    je _ret
    mov $0x9e, %eax
    int $0x80
    jmp lockmutex

unlockmutex:
    mov 0x4(%esp), %eax
    mov $0x0, %ebx
    xchg (%eax), %ebx
    jmp _ret

_ret:
    ret
