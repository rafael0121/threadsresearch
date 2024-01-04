.set SCHED, 0x9e

.text
    .global lockmutex
    .global unlockmutex

lockmutex:
    movl $0x1, %ecx
    xchg %ecx, 0x4(%esp) 
    cmp $0x0, %ecx
    je _ret
    mov $SCHED, %eax
    int $0x80
    jmp lockmutex

unlockmutex:
    movl $0x0, %ecx
    xchg %ecx, 0x4(%esp)
    jmp _ret

_ret:
    ret
