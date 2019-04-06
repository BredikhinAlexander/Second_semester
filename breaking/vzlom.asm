.model tiny

.code
org 100h




start:
                ;вводим имя пользователя

                mov dx, offset usr_name_pls 
		call my_printf

		mov di, offset Username
		call input

		mov dx, offset check_name
		call my_printf
		mov dx, offset Username
		call my_printf

		jmp proverka

		;вводим пароль

pas:		mov dx, offset paswrd_pls 
		call my_printf

		mov di, offset Password
		call input

		mov dx, offset check_paswrd
		call my_printf
		mov dx, offset Password
		call my_printf

		jmp lalka

		

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	 
Username	db 10 dup('$')
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

adminuser_name	db 'bred'
adminnamelen	equ $ - adminuser_name

adminpswrd	db 'MATAN'
universal	db 'kotic'



		


proverka:			
                mov si, offset adminuser_name
                mov di, offset Username
                mov dx, 0
                mov bx, 0

next:           
		

		mov ax, [si]
		;add ax, 2
		mov cx, [di]

		add bx, cx

		cmp ax, cx
                jne other_user

		inc dx
		cmp dx, 3d
		je second_pr


                inc si
                inc di

                jmp next


other_user:
		jmp pas

lalka:          mov si, offset adminpswrd
                mov di, offset Password 
                mov dx, 0

snova:          mov ax, [si]
                add ax, 3d
                mov cx, [di]
                add cx, 5d
                
                cmp ax, cx
                jne ne_ok 

                inc dx
                cmp dx, 5d
                je password_ok

                jmp snova


second_pr:	
		mov dx, offset strok
		call my_printf

		cmp bx, 3C39h
		jne success

		jmp other_user






success:  	mov dx, offset msg
		call my_printf



		mov dx, offset paswrd_pls 
		call my_printf

		mov di, offset Password
		call input

		mov dx, offset check_paswrd
		call my_printf
		mov dx, offset Password
		call my_printf


                mov si, offset universal
                mov di, offset Password 
                mov dx, 0

zicle:          mov ax, [si]
                mov cx, [di]
                
                cmp ax, cx
                jne ne_ok 

                inc dx
                cmp dx, 5d
                je password_ok

                jmp zicle


	        ;jmp password_ok




ne_ok:
		mov dx, offset fail
		call my_printf

		jmp end_prog

		

password_ok:	
		mov dx, offset all_ok
		call my_printf

		jmp end_prog




;====================================================
;	   	    input
;----------------------------------------------------
;entr: di - start buf
;exit: di - end buf
;destr: ax, di
;====================================================		

input		proc

		cld

again:		mov ah, 01h
		int 21h

		cmp al, 0dh
		je stopinput

		stosb

		jmp again


stopinput:	
		ret
		endp
;------------------------------------------------------





;======================================================
;		     my_printf
;------------------------------------------------------
;entry: dx - string to print
;destr: ax
;======================================================

my_printf	proc


		mov ah, 9
		int 21h

		ret
		endp
;------------------------------------------------------





end_prog:	mov ax, 4c00h
		int 21h


;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	 
Password	db 8 dup('$')
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


.data

usr_name_pls	db 0dh, 0ah, 'Enter your username, please:  $'
paswrd_pls	db 0dh, 0ah, 'Enter your password, please:  $'

check_name	db 0dh, 0ah, 'your username is:  $'
check_paswrd	db 0dh, 0ah, 'your password is:  $' 

all_ok		db 0dh, 0ah, 'hi selfish Dasha, you have admin rights!!!$'
fail		db 0dh, 0ah, 'lalka pozornay$'

msg		db 0dh, 0ah, 'krasavchic, automatic authorization, enter universal password$'

strok		db 0dh, 0ah, 'want another solution$'
end start



