SSEG SEGMENT STACK
          DW 12 DUP(?)
SSEG ENDS

DATA SEGMENT

a dw 3        ; вводимые данные 
b dw 2
i dw 3
k dw 2

i1 dw 4        ; промежуточные результаты
i2 dw 6
res dw ?  

DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE, DS:DATA, SS:SSEG

Main PROC FAR

    push DS
    sub AX,AX
    push AX
    mov AX,DATA
    mov DS,AX

;----------ГЛАВНАЯ ВЫЧИСЛИТЕЛЬНАЯ ЧАСТЬ---------
    mov ax,i		; ax = i
    shl ax,1		; ax = 2i
    add ax,i		; ax = 3i
    
    mov bx,b		; ax = a
    cmp a, bx       ; сравнение a и b.zf=1 a=b;sf=1 a<b; cf=1 a>b 
    jg a_gr_b       ; условный переход при a > b

;----------Ветка fn1 и 2, если a <= b---------
; i1 = (i << 1) + i + 6 

    add i1, 2		; i1 = 6
    add i1, ax		; il = 3i + 6

; i2 = 6 -(i << 2) - (i << 1) 
	shl ax, 1		; ax = 6*i
	sub i2, ax		; i2 = 6 - 6*i

    jmp fn3_calc    ; переход к fn3_calc

;----------Ветка fn1 и 2, если a > b---------
a_gr_b:
    ; i1 = 4 - (i << 2) - (i << 1)
    shl ax, 1		; ax = 6i
    sub i1, ax		; i1 = 4 - 6i
    
    ; i2 = 20 - 4i
    sub ax, i		; ax = 5i
    sub ax, i		; ax = 4i
	add i2, 14		; i2 = 20
	sub i2, ax		; i2 = 20 - 4i

fn3_calc:
	mov ax, i1		; ax = i1
	mov bx, i2		; bx = i2
mod_i1:
	neg ax			; ax = -i1
	js mod_i1		; если ax отрицательное возвращение mod_i1

mod_i2:
	neg bx			; bx = -i2
	js mod_i2		; если bx отрицательное возвращение mod_i2

    cmp k, 0		; Сравнение k и 0.zf=1 k=0;sf=1 k<0; cf=1 k>0 
    jl k_negative   ; если k < 0, переход k_negative 

;----------Ветка fn3, если k >= 0---------
; res = max(6, |i1|)
	cmp ax, 6		; равнение a и 6.zf=1 a=6;sf=1 a<6; cf=1 a>6 
	jl if_6			; переход if_6	 если 6 больше ax
	mov res, ax		; res = ax ( res = |i1| )
	jmp return		; переход return
if_6:
	mov res, 6		; res = 6
	jmp return		; переход return
	
k_negative:			
;----------Ветка fn3, если k < 0---------
; res = |i1| + |i2|
	mov res, ax		; res = ax ( res = |i1| )
	add res, bx		; res = ax + bx ( res = |i1| + |i2| )
;------КОНЕЦ ГЛАВНОЙ ВЫЧИСЛИТЕЛЬНАЯ ЧАСТЬ---------
;колво строк 32
return:				
    ret 

Main ENDP
CODE ENDS

END Main
