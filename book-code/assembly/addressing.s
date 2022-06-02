;
;Practicing addressing in assembly
;

        segment .data
name    db      'wrong cho', 10

        segment .text
        global  _start
_start:
        ; write name
        mov     eax, 4
        mov     ebx, 1
        mov     ecx, name
        mov     edx, 10
        int     80h

        ; change name
        mov     [name], dword 'youn'

        ; write name
        mov     eax, 4
        mov     ebx, 1
        mov     ecx, name
        mov     edx, 10
        int     80h
        
        ;exit
        mov     eax, 1
        int     80h

