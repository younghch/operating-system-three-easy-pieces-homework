;
; this program prints printable ascii character set.
;
; characteristic of stack:
; - Only words or doublewords could be saved into the stack, not a byte.
; - The stack grows in the reverse direction, i.e., toward the lower memory address
; - The top of the stack points to the last item inserted in the stack; it points to the lower byte of the last word inserted.
;

SYS_EXIT    equ 1
SYS_WRITE   equ 4
STDOUT      equ 1

            segment .data
achar       db      32

            segment .text
            global  _start
_start:
            call    display
            mov     eax, SYS_EXIT
            int     0x80

display:
            mov     ecx, 94
next:
            push    ecx
            mov     eax, SYS_WRITE
            mov     ebx, STDOUT
            mov     ecx, achar
            mov     edx, 1
            int     0x80

            pop     ecx
            mov     dx, [achar]
            inc     byte [achar]
            loop    next
            ret
            