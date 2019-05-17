extern CSum

global mysum:

section .text

mysum:
        push rbp
        mov rbp, rsp

        call CSum

        pop rbp

        ret
