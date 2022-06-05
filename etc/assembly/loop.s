;
; this program prints 1 to 10
; LOOP:
; LOOP instruction assumes that the ECX register contains the loop count. 
; When the loop instruction is executed, the ECX register is decremented and the control jumps to the target label,
;  until the ECX register value, i.e., the counter reaches the value zero.
;


SYS_EXIT    equ 1
SYS_WRITE   equ 4
STDOUT      equ 1

            segment .bss
num         resb    1

            segment .text
            global  _start
_start:
            mov     ecx, 10
            mov     eax, '1'

l1:
            mov     [num], eax
            mov     eax, SYS_WRITE
            mov     ebx, STDOUT
            push    ecx             ; push loop count to stack
            mov     ecx, num
            mov     edx, 1
            int     0x80

            mov     eax, [num]
            inc     eax
            pop     ecx
            loop    l1


exit:
            mov     eax, SYS_EXIT
            xor     ebx, ebx
            int     0x80
            