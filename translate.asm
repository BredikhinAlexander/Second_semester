.model tiny

.code
org 100h

COLOR = 4eh


;--------------------------------------
;               CONST
;--------------------------------------
x1 = 6
x2 = 40
y1 = 6
y2 = 20
hor = 0cdh
vertical = 0bah
fon = 11110110b
;--------------------------------------


start:	mov ax, 0b800h
	mov es, ax
	mov cx, 0
	mov di, 0
	mov ax, (x2-x1)*2 ; end string
	mov dx, y1 ; start stolbick
	mov si, y2 ; end stolbick
	call strak



stolb:
	call strak
	mov cx, 0
	add dx, 1
	add di, 160d
	cmp dx, si
	je gate
	
	jmp stolb
	

gate:	mov cx, 0
	mov ax, (x2-x1+1)*2

lol:	mov bx, 160*y1 + x1-2
	cmp ax, cx
	je vert

	add bx, cx
	
	mov byte ptr es:[bx], hor
	mov byte ptr es:[bx+1], fon

	add bx, 160*(y2-y1)

	mov byte ptr es:[bx], hor
	mov byte ptr es:[bx+1], fon

	add cx, 2
	jmp lol



vert:	mov cx, 0
	mov ax, (y2-y1+1)

	mov bx, 160*(y1-1) + x1-2
kek:	cmp ax, cx
	je conor

	add bx, 160

	mov byte ptr es:[bx], vertical
	mov byte ptr es:[bx+1], fon

	add bx, 2*(x2-x1)

	mov byte ptr es:[bx], vertical
	mov byte ptr es:[bx+1], fon

	sub bx, 2*(x2-x1)

	add cx, 1
	jmp kek


conor:	mov bx, 160*(y1) + x1-2
	mov byte ptr es:[bx], 0c9h

	add bx, (x2-x1)*2
	mov byte ptr es:[bx], 0bbh

	add bx, (y2-y1)*160
	mov byte ptr es:[bx], 0bch

	sub bx, (x2-x1)*2
	mov byte ptr es:[bx], 0c8h
	jmp rstart


;outt:	mov ax, 4c00h
;	int 21h




;-----------------------------------------------------------
;fill line
;entry	di - откуда начинать заполнять 
;	ax - сколько нужно ззаполнять
;	si - чем заполнять 
;dstr	cx
;-----------------------------------------------------------

strak	proc
	mov cx, 0
aba:	mov bx, 160*(y1) + x1
	add bx, di
	cmp ax, cx
	je flgg
	add bx, cx
	mov byte ptr es:[bx], ' '
	mov byte ptr es:[bx+1], 11110011b
	add cx, 2
	jmp aba
	endp
flgg:	ret	

;-------------------------------------------------------------------





                                           





;-----------------------------------
strr	proc
	
ag:	mov ah, 9	
	mov ah, 4eh

	mov al, [bx]
	stosw 
	add bx, 1

	cmp al, ':'
	je e_f

	jmp ag

e_f:	ret
;---------------------------------------


rstart:	mov bx, 0b800h
	mov es, bx ; смещение на видеопамять

	mov di, 160*(y1+2) + x1+4
	mov bx, offset msg1
	call strr



	mov di, 160*(y1+4) + x1+4
	mov bx, offset msg2
	call strr


	mov di, 160*(y1+6) + x1+4
	mov bx, offset msg3
	call strr

	mov di, 160*(y1+2) + x1+4+18

	
	call in_line


	push ax
	mov di, 160*(y1+4) + x1+4+14


	call in_line 

	mov dx, ax
	pop ax


 

	mov bx, 160*(y1+6) + x1+6+24
	


lop:	div dl
	cmp ah, 10d
	jae bykB ; остаток >= 10
	jl zifr  ; остаток <  10

output:	mov dh, al ; запоминаем частное
	xchg ah, al
	mov ah, 4eh


	;stosw
	mov byte ptr es:[bx], al
	mov byte ptr es:[bx+1], ah
	sub bx, 2d


	mov al, dh
	mov ah, 0
	cmp al, 0
	je endd  ; если частное 0, то выходим
	jmp lop

zifr:	add ah, '0'
	jmp output

bykB:	add ah, 'A' - 10
	jmp output

endd:	mov ax, 4c00h
	int 21h


;-------------------------------------------------------
;es = 0b800h, color = 4eh
;entry:	di - текущий адресс
;	bx - начало строчки

;exit:	ax - введённое число
;destr:	di, bx, cx, dx
;------------------------------------------------------- 	
 
in_line	proc

        cld
        mov bx, di
	
in_smb:	mov ah, 00h
	int 16h
	cmp al, 0dh  ; проверка на "\r"
	je end_l

	cmp al, 08h
	je bcksp

	mov ah, 4eh
	stosw
	jmp in_smb
		
bcksp:	cmp di, bx  ; если строчка пустая
	je in_smb

	sub di, 2d
	jmp in_smb

end_l:  mov ax, 00h
	mov si, di ;конец ввода
        mov di, bx
        mov cx, 10d

symb:   mov bx, es:[di]
        

        cmp bl, 00h
        je end_n

        cmp di, si
        je end_n

        add di, 2

        mul cx
        sub bl, '0'
        mov bh, 00h
        add ax, bx
        jmp symb

end_n:	ret
	endp

.data
msg1	db 'Number:$'
msg2	db 'basis:$'
msg3	db 'answer:$'
end start
	


	





