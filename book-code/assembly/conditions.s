;
; this program displays largest of three variables. Prints '*' for ascii 42.
;
; Flag registers:
; Zero flag     : set when the result of an operation is zero.
; Carry flag    : set when the result of unsigned arithmetic is too large for the destination operand 
;                 or when subtraction requires a borrow.
; Sign flag     : set when the high bit of the destination operand is set indicating a negative result.
; Overflow flag : set when signed arithmetic generates a result which ifs out of range.
;
; 
; Instruction	Description	                            Flags tested
; JE/JZ	        Jump Equal or Jump Zero	                ZF
; JNE/JNZ	    Jump not Equal or Jump Not Zero	        ZF
; JG/JNLE	    Jump Greater or Jump Not Less/Equal	    OF, SF, ZF
; JGE/JNL	    Jump Greater/Equal or Jump Not Less	    OF, SF
; JL/JNGE	    Jump Less or Jump Not Greater/Equal	    OF, SF
; JLE/JNG	    Jump Less/Equal or Jump Not Greater	    OF, SF, ZF
;
; result of compare
; CMP                  ZF CF    Flags
; destination > source 0  0     SF = OF
; destination < source 0  1     SF != OF
; destination = source 1  0     ZF = 1


SYS_EXIT    equ 1
SYS_WRITE   equ 4
STDIN       equ 0
STDOUT      equ 1

            segment .data

msg         db      "The largest digit is: ", 0xA
len         equ     $ - msg
num1        db      42
num2        db      24
num3        db      33


            segment .bss
largest     resb    2

            segment .text
            global  _start
_start:
            mov     ecx, [num1]
            cmp     ecx, [num2]
            jg      compare_third
            mov     ecx, [num2]

            
compare_third:
            cmp     ecx, [num3]
            jg      exit
            mov     ecx, [num3]


exit:
            mov     [largest], ecx
            mov     eax, SYS_WRITE
            mov     ebx, STDOUT
            mov     ecx, msg
            mov     edx, len
            int     0x80
            mov     eax, SYS_WRITE
            mov     ebx, STDOUT
            mov     ecx, largest
            mov     edx, 2
            int     0x80
            mov     eax, SYS_EXIT
            xor     ebx, ebx
            int     0x80
            