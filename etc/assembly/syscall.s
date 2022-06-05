;
;Practicing system calls in assembly
;
; how to call system call in linux:
; - Put the system call number in the EAX register.
; - Store the arguments to the system call in the registers EBX, ECX, etc.
; - Call the relevant interrupt (80h).
; - The result is usually returned in the EAX register.
;
; %eax	Name	    %ebx	        %ecx	        %edx	%esx	%edi
; 1	    sys_exit	int	            -	            -	    -	    -
; 2	    sys_fork	struct pt_regs	-	            -	    -	    -
; 3	    sys_read	unsigned int	char *	        size_t	-	    -
; 4	    sys_write	unsigned int	const char *	size_t	-	    -
; 5	    sys_open	const char *	int	            int	    -	    -
; 6	    sys_close	unsigned int	-	            -	    -	    -
;
            segment .data
userMsg     db      "Please enter a number: "
lenUserMsg  equ     $ - userMsg
dispMsg     db      "You have entered: "
lenDispMsg  equ     $ - dispMsg

            segment .bss
number      resb    5

            segment .text
            global  _start
_start:
            ; show please enter message
            mov     eax, 4
            mov     ebx, 1
            mov     ecx, userMsg
            mov     edx, lenUserMsg
            int     80h

            ; read from user
            mov     eax, 3
            mov     ebx, 2
            mov     ecx, number
            mov     edx, 5
            int     80h

            ; show message
            mov     eax, 4
            mov     ebx, 1
            mov     ecx, dispMsg
            mov     edx, lenDispMsg
            int     80h
            mov     eax, 4
            mov     ebx, 1
            mov     ecx, number
            mov     edx, 5
            int     80h
            
            ; exit
            mov     eax, 1
            mov     ebx, 0
            int     80h

