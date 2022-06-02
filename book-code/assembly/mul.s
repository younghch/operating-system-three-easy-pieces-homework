;
; A multiplies 3 with 2
;
;  difference between eax,ax,al,ah:
;  - EAX is the full 32-bit value
;  - AX is the lower 16-bits
;  - AL is the lower 8 bits
;  - AH is the bits 8 through 15 (zero-based)
;

SYS_EXIT    equ 1
SYS_WRITE   equ 4
STDIN       equ 0
STDOUT      equ 1

            segment .data

msg         db      "The result is: ", 0xA
len         equ     $ - msg

            segment .bss
res         resb    1

            segment .text
            global  _start
_start:
            mov     al, '3'
            sub     al, '0'
            mov     bl, '2'
            sub     bl, '0'
            mul     bl
            add     al, '0'

            mov     [res], al
            
            mov     eax, SYS_WRITE
            mov     ebx, STDOUT
            mov     ecx, msg
            mov     edx, len
            int     0x80
            mov     eax, SYS_WRITE
            mov     ebx, STDOUT
            mov     ecx, res
            mov     edx, 1
            int     0x80

exit:
            mov     eax, SYS_EXIT
            xor     ebx, ebx
            int     0x80
            