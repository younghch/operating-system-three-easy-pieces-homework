;
; this program adds up two 5-digit decimal numbers and displays the sum
;

SYS_EXIT    equ 1
SYS_WRITE   equ 4
STDIN       equ 0
STDOUT      equ 1

            segment .data

msg         db      "The sum is: ", 0xA
len         equ     $ - msg
num1        db      '23401'
num2        db      '19019'
sum         db      '     '

            segment .text
            global  _start
_start:
            mov     esi, 4              ; pointing to rightmost digit
            mov     ecx, 5              ; num of digits
            clc

add_loop:
            mov     al, [num1 + esi]
            adc     al, [num2 + esi]    ; adc: add with carry
            aaa
            pushf
            or      al, 30h
            popf

            mov     [sum + esi], al
            dec     esi
            loop    add_loop

            mov     eax, SYS_WRITE
            mov     ebx, STDOUT
            mov     ecx, msg
            mov     edx, len
            int     0x80
            mov     eax, SYS_WRITE
            mov     ebx, STDOUT
            mov     ecx, sum
            mov     edx, 5
            int     0x80

exit:
            mov     eax, SYS_EXIT
            xor     ebx, ebx
            int     0x80
            