;
; this program defines 3-element array and displays sum of it.
;



SYS_EXIT    equ 1
SYS_WRITE   equ 4
STDOUT      equ 1

            segment .data
x           db      2, 3, 4  
sum         db      0

            segment .text
            global  _start
_start:
            mov     eax, 3          ; number of items in x
            mov     ebx, 0          ; store sum in ebx
            mov     ecx, x

add_item:
            add     ebx, [ecx]
            add     ecx, 1
            dec     eax
            jnz     add_item

            add     ebx, '0'
            mov     [sum], ebx

exit:
            mov     eax, SYS_WRITE
            mov     ebx, STDOUT
            mov     ecx, sum
            mov     edx, 1
            int     0x80
            mov     eax, SYS_EXIT
            xor     ebx, ebx
            int     0x80
            