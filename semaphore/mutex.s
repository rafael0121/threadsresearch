.set SCHED, 0x9e

.text
    .global lockmutex
    .global unlockmutex

lockmutex:
    mov 0x4(%esp), %eax
    movl $0x1, %ecx
    xchg %ecx, (%eax) 
    cmp $0x0, %ecx
    je _ret
    mov $SCHED, %eax
    int $0x80
    jmp lockmutex

unlockmutex:
    mov 0x4(%esp), %eax
    movl $0x0, %ecx
    xchg %ecx, (%eax)
    jmp _ret

_ret:
    ret
