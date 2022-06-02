;
; Displays nine starts on the screen, for IA-32 architecture
;
        segment .text
        global  _start
_start:         
        mov     edx, len
        mov     ecx, msg    ;message to write
        mov     ebx, 1
        mov     eax, 4      ;syscall - write
        int     0x80

        mov     edx, 9      ;length of stars
        mov     ecx, s2
        mov     ebx, 1
        mov     eax, 4
        int     0x80

        mov     eax, 1      ;syscall - exit
        int     0x80

        segment .data
msg     db      'Displaying 9 stars', 0xa
len     equ     $ - msg
s2      times   9 db '*'

