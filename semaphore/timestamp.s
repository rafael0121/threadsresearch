.text
    .global getclock

getclock:
    rdtsc 
    mov 0x4(%esp), %ecx
    mov %eax, (%ecx)
    mov %edx, 0x4(%ecx)

    ret
