
; nasm -f elf64 -l 1-nasm.lst 1-nasm.s  ;  ld -s -o 1-nasm 1-nasm.o

section .text

global _start                  

_start:         
                push '!'
                push 100d
                push 3802d
                push Msg_love
                push '%'
                push 3802d
                push Msg2                
                push 's'
                push 'i'              
                push Msg1

                call my_printf

                jmp end_prog
                

                ;;;;;;mov rbp, rsp





;-------------------------------------------------------
;                       my_printf
;-------------------------------------------------------
;input: arguments of printf
;output: string in terminal
;-------------------------------------------------------
my_printf:
                push rbp
                mov rbp, rsp
                add rbp, 16 ; теперь rbp указывает на самый "левый" аргумент printf
     
           
                mov r8, [rbp]

snova:          push r8
                push '%'

                call strchr ; в rbx адресс %
                mov rcx, rbx ; return value

                mov r14, rcx ; в r14 не изменится а во всём остальном уничтожится нафиг
                
                cmp r8, rcx
                je next
                ;;;;jne print_str     




print_str:  
                                       
                mov rax, 0x01      
                mov rdi, 1         
                mov rsi, r8

                sub rcx, r8

                mov rdx, rcx
                syscall


next:           add r14, 1

                mov rbx, r14
                mov dl, [rbx]

                cmp dl, 'c'
                je chrr


                cmp dl, '%'
                je procent

                
                cmp dl, 's'
                je stringg

            
                cmp dl, 'd'
                je dec

                cmp dl, 'b'
                je binn

                cmp dl, 'x'
                je hexx




;========================================
chrr:           add rbp, 8
                
                mov rax, 0x01
                mov rdi, 1
                mov rsi, rbp
                mov rdx, 1
                syscall

                add r14, 1

                jmp zicle

;========================================

procent:        
                mov rax, 0x01
                mov rdi, 1
                mov rsi, r14
                mov rdx, 1
                syscall

                add r14, 1

                jmp zicle                      

;========================================
stringg:        

                add rbp, 8

                mov rsi, [rbp]

                push rsi
                push 0x0a
                call strchr  ; return rbx

                mov rax, 0x01
                mov rdi, 1
                ;mov rsi, [rbp]
                
                sub rbx, [rbp]

                mov rdx, rbx
                syscall

                inc r14 

                jmp zicle
                
                
;========================================


dec:
                mov rbx, data_end
                push rbx
                push 10
                call memset  ;in rbx


                mov r9, rbx
                mov r10, rbx ;не уничтожаем 
        
                mov dl, 10d
                add rbp, 8
                mov rax, [rbp]              


looop:          div dl
                mov dh, al ; запоминаем частное 
                xchg ah, al

                add al, 48d
                mov [r9], al ; кладём в память остаток
                sub r9, 1

                mov al, dh  
                mov ah, 0
                cmp al, 0
                je end_dec

                jmp looop

end_dec:        
                mov rax, 0x01
                mov rdi, 1
                mov rsi, r9
                sub r10, r9
                inc r10 ; вот потому что)
                mov rdx, r10
                syscall
                
                inc r14
                jmp zicle
                                           
;========================================

binn:
                mov rbx, data_end
                push rbx
                push 10
                call memset  ;in rbx


                mov r9, rbx
                mov r10, rbx ;не уничтожаем 
        
                mov dl, 2
                add rbp, 8
                mov rax, [rbp]              


looop_b:        div dl
                mov dh, al ; запоминаем частное 
                xchg ah, al

                add al, 48d
                mov [r9], al ; кладём в память остаток
                sub r9, 1

                mov al, dh  
                mov ah, 0
                cmp al, 0
                je end_bin

                jmp looop_b

end_bin:        
                mov rax, 0x01
                mov rdi, 1
                mov rsi, r9
                sub r10, r9
                inc r10 ; вот потому что)
                mov rdx, r10
                syscall
                
                inc r14
                jmp zicle
                    
;========================================

hexx:
                mov rbx, data_end
                push rbx
                push 10
                call memset  ;in rbx


                mov r9, rbx
                mov r10, rbx ;не уничтожаем 
        
                mov dl, 16d
                add rbp, 8
                mov rax, [rbp]              


looop_h:        div dl

                cmp ah, 10d
                jae bykB ; остаток >= 10
                jl  zifr ; остаток <  10

outputt:        mov dh, al ; запоминаем частное 
                xchg ah, al

            
                mov [r9], al ; кладём в память остаток
                sub r9, 1

                mov al, dh  
                mov ah, 0
                cmp al, 0
                je end_hex

                jmp looop_h


zifr:           add ah, '0'
                jmp outputt

bykB:           add ah, 'A' - 10
                jmp outputt


end_hex:        
                mov rax, 0x01
                mov rdi, 1
                mov rsi, r9
                sub r10, r9
                inc r10 ; вот потому что)
                mov rdx, r10
                syscall
                
                inc r14
                jmp zicle
;========================================



zicle:          
                mov rax, r14
                mov r8, r14 ; для цикла                              
                mov cl, [rax]
                cmp cl, 0x0a
                je end_my_printf

                jmp snova
            



end_my_printf:  pop rbp
                ret 




;--------------------------------------------------
;                   strchr
;--------------------------------------------------
;input: pointer to start find    push reg
;       what find                push '%' or smth else
;out:   pointer on find element - rbx
;destr: rax,rbx,dl
;--------------------------------------------------

strchr:         
                push rbp
                mov rbp, rsp
                add rbp, 8

                mov al, [rbp+8]
                mov rbx, [rbp+16]

again:          mov dl, [rbx]
          
                cmp al, dl
                je end_strchr

                cmp dl, 0x0a
                je end_strchr

                add rbx, 1

                jmp again

                
end_strchr:     
                pop rbp
                ret 16




                                              


end_prog:       mov rax, 0x3C
                xor rdi, rdi
                syscall





;----------------------------------------------------
;                       memset
;----------------------------------------------------
;input: pointer on start mem    push reg
;       len str                 push len
;out:   pointer on end of str   in rbx
;destr: rax,rbx, rdx, rcx
;----------------------------------------------------

memset:
                push rbp
                mov rbp, rsp
                add rbp, 8

                mov al, [rbp+8]
                mov rbx, [rbp+16]
                
                xor dl, dl

rest:           xor rcx, rcx
                mov [rbx], rcx
                
                cmp dl, al
                je end_memset

                inc dl
                inc rbx

                jmp rest

end_memset:     pop rbp
                ret 16                                

            
            
section     .data
            
Msg1:           db "Sanya %c%%%c %s %x%c, and I %s %x %d %% %c", 0x0a
Msg2:           db "lalka and love", 0x0a
Msg_love:       db "love", 0x0a


data_end:



