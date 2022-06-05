;
; Hello world program for IA-32 architecture
;
                segment .text
                global  _start
_start:         mov     edx, 13
                mov     ecx, msg
                mov     ebx, 1          ; stdout
                mov     eax, 4          ; syscall - write
                int     0x80            ; call kernel

                mov     eax, 1          ; syscall - exit
                int     0x80


                segment .data
msg             db      "Hello, World", 10
