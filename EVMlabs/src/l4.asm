.model small
.stack 400h

.data
    ; Буферы и сообщения
    prompt      db 'Введите строку: $'
    WaitMsg     db 0Dh,0Ah,'Нажмите любую клавишу для вывода результата. Нажмите Enter для выхода..$'
    resultLbl   db 0Dh,0Ah,'Результат: $'

    buffer      db 100,0,100 dup('$')    ; буфер для строки, максимальная длина 100 байт
    output      db 40 dup('$')      ; буфер для вывода, максимальная длина 40 байт
    max_seq     db 20 dup('$')      ; максимальная длина самой длинной найденной подпоследовательности

    ; Переменные
    max_len     dw 0
    curr_len    dw 0
    seq_count   dw 0
    curr_char   db 0
    exit_flag   db 0        ; Флаг для завершения программы

    ; Сохранение старого вектора INT 09h (offset,segment)
KEEP_VECTOR  dd 0

.code

; ===================== Вспомогательная процедура: Печать строки =====================
; DX - адрес строки, заканчивающейся '$'
PrintString PROC
    mov ah,09h        ; Функция DOS 09h - вывод строки
    int 21h           ; Вызов DOS
    ret
PrintString ENDP

; ===================== Преобразование строки ввода в формат с символом '$' =====================
MakeTerminated PROC
    push ax
    push bx
    push si

    lea si, buffer         ; SI указывает на начало буфера
    mov bl, [si+1]         ; BL = реальная длина введённой строки
    xor bh,bh              ; BH = 0, теперь BX = длина строки
    lea di, [si+2]         ; DI указывает на начало данных строки
    add di, bx             ; DI указывает на конец введенных данных
    mov byte ptr [di], '$' ; Добавляем символ конца строки

    pop si
    pop bx
    pop ax
    ret
MakeTerminated ENDP

; ===================== Алгоритм поиска самой длинной последовательности =====================
FindMaxSequences PROC
    push ax
    push bx
    push cx
    push dx
    push si
    push di

    ; Инициализация переменных
    mov word ptr [max_len], 0    ; Обнуляем максимальную длину
    mov word ptr [seq_count], 0  ; Обнуляем счетчик последовательностей

    lea si, buffer
    add si, 2             ; SI указывает на начало строки

; Основной цикл обработки символов строки
proc_loop:
    mov al, [si]          ; Загружаем текущий символ
    cmp al, '$'           ; Проверяем конец строки
    je fms_done           ; Если конец - завершаем

    mov [curr_char], al   ; Сохраняем текущий символ
    mov word ptr [curr_len], 1  ; Начинаем новую последовательность
    mov bx, si            ; Сохраняем начало последовательности

; Подсчет длины текущей последовательности одинаковых символов
count_seq:
    inc si                ; Переходим к следующему символу
    mov dl, [si]          ; Загружаем следующий символ
    cmp dl, '$'           ; Проверяем конец строки
    je check_seq          ; Если конец - проверяем последовательность
    cmp dl, [curr_char]   ; Сравниваем с текущим символом
    jne check_seq         ; Если другой - проверяем последовательность
    mov ax, word ptr [curr_len]  ; Увеличиваем длину последовательности
    inc ax                  ; ax = ax + 1 
    mov word ptr [curr_len], ax
    jmp count_seq         ; Продолжаем подсчет

; Проверка найденной последовательности
check_seq:
    mov ax, word ptr [curr_len]  ; AX = длина текущей последовательности
    mov cx, word ptr [max_len]   ; CX = текущая максимальная длина
    cmp ax, cx           ; Сравниваем с максимальной
    jl next_seq          ; Если меньше - переходим к следующей
    je equal_len         ; Если равна - увеличиваем счетчик

    ; Новая максимальная длина найдена
    mov word ptr [max_len], ax    ; Обновляем максимальную длину
    mov word ptr [seq_count], 1   ; Сбрасываем счетчик

    ; Сохраняем найденную последовательность
    mov di, offset max_seq        ; DI указывает на буфер для максимальной последовательности
    mov bl, [curr_char]           ; BL = символ последовательности
    mov cx, ax                    ; CX = длина последовательности
    push si
    mov al, bl                    ; AL = символ для заполнения
    rep stosb                     ; Заполняем буфер символом AL CX раз
    mov byte ptr [di], '$'        ; Добавляем терминатор
    pop si
    jmp next_seq

; Обработка последовательности равной текущей максимальной
equal_len:
    mov ax, word ptr [seq_count]  ; Увеличиваем счетчик последовательностей
    inc ax
    mov word ptr [seq_count], ax

; Переход к следующей последовательности
next_seq:
    cmp byte ptr [si], '$'        ; Проверяем конец строки
    jne proc_loop                 ; Если не конец - продолжаем

fms_done:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
FindMaxSequences ENDP

; ===================== Вывод результата на экран =====================
PrintResult PROC
    push ax
    push bx
    push cx
    push dx
    push si
    push di

    ; Печать префикса "Result: "
    lea dx, resultLbl
    call PrintString

    ; Копируем максимальную последовательность в выходной буфер
    lea si, max_seq        ; Источник - найденная последовательность
    lea di, output         ; Приемник - выходной буфер
copy_seq:
    mov al, [si]           ; Копируем символ
    cmp al, '$'            ; Проверяем конец последовательности
    je add_count           ; Если конец - добавляем счетчик
    mov [di], al           ; Копируем символ в выходной буфер
    inc si
    inc di
    jmp copy_seq

; Добавление счетчика последовательностей к результату
add_count:
    mov ax, word ptr [seq_count]  ; Загружаем счетчик
    cmp ax, 9             ; Проверяем, больше ли 9
    ja  add_multidigit    ; Если да - используем специальный символ
    add al, '0'           ; Преобразуем число в символ
    mov [di], al          ; Добавляем к результату
    inc di
    jmp finish_out

; Обработка чисел больше 9
add_multidigit:
    ; Преобразуем двухзначное число
    mov bx, 10
    div bl              ; AL = десятки, AH = единицы
    add al, '0'         ; Преобразуем десятки в символ
    mov [di], al
    inc di
    mov al, ah          ; Берем единицы
    add al, '0'         ; Преобразуем единицы в символ  
    mov [di], al
    inc di

; Завершение формирования выходной строки
finish_out:
    mov byte ptr [di], '$' ; Добавляем симол конца строки 
    lea dx, output         ; Выводим результат
    call PrintString

    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
PrintResult ENDP

; ===================== Обработчик прерывания клавиатуры INT 09h =====================
INT09_HANDLER PROC FAR
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push bp
    push ds
    push es

    mov ax, @data         ; Загружаем сегмент данных
    mov ds, ax
    mov es, ax

    in  al, 60h           ; Читаем скан-код из порта клавиатуры
    test al, 80h          ; Проверяем бит 7 (0=нажатие, 1=отпускание)
    jnz  call_old_handler ; Если отпускание - передаем старому обработчику

    ; Обработка нажатия клавиши
    cmp al, 1Ch           ; Проверяем скан-код Enter
    je  do_enter          ; Если Enter - особая обработка

    ; Обработка обычной клавиши
    call FindMaxSequences ; Находим максимальные последовательности
    call PrintResult      ; Выводим результат
    jmp call_old_handler  ; Передаем управление старому обработчику

; Обработка нажатия Enter
do_enter:
    call FindMaxSequences ; Находим максимальные последовательности
    call PrintResult      ; Выводим результат
    mov exit_flag, 1      ; Устанавливаем флаг завершения программы

; Переход к оригинальному обработчику прерывания
call_old_handler:
    pop es
    pop ds
    pop bp
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    jmp dword ptr [KEEP_VECTOR]  ; Переход к старому обработчику (ТЗ)
INT09_HANDLER ENDP

; ===================== ГЛАВНАЯ ПРОГРАММА =====================
start:
    mov ax, @data         ; Инициализация сегмента данных
    mov ds, ax
    mov es, ax

    ; 1. Ввод исходной строки для обработки
    lea dx, prompt        ; Вывод приглашения
    call PrintString

    lea dx, buffer        ; Ввод строки средствами DOS
    mov ah, 0Ah           ; Функция DOS 0Ah - буферизованный ввод
    int 21h
    call MakeTerminated   ; Добавляем символ конца строки 

    ; 2. Сохранение оригинального вектора прерывания 09h
    mov ah, 35h           ; Функция DOS 35h - получить вектор прерывания
    mov al, 09h           ; Номер прерывания - 09h (клавиатура)
    int 21h
    mov word ptr KEEP_VECTOR, bx      ; Сохраняем смещение
    mov word ptr KEEP_VECTOR+2, es    ; Сохраняем сегмент

    ; 3. ЗАДЕРЖКА ПЕРЕД ЗАМЕНОЙ 09h (6 ТЗ)
    mov cx, 0Eh           ; Время задержки - старшее слово
    mov dx, 0FFFFh        ; Время задержки - младшее слово
    mov ah, 86h           ; Функция DOS 86h - задержка в микросекундах
    int 15h               ; Вызов прерывания BIOS

    ; 4. Установка нового обработчика прерывания 09h
    push ds
    mov dx, offset INT09_HANDLER  ; Смещение нового обработчика
    mov ax, seg INT09_HANDLER     ; Сегмент нового обработчика
    mov ds, ax
    mov ah, 25h           ; Функция DOS 25h - установить вектор прерывания
    mov al, 09h           ; Номер прерывания - 09h
    int 21h
    pop ds

    ; 5. ВЫВОД СООБЩЕНИЯ ПЕРЕД ОЖИДАНИЕМ (5 ТЗ)
    lea dx, WaitMsg       ; Сообщение о ожидании нажатий
    call PrintString

    ; 6. Цикл ожидания флага завершения
wait_loop:
    cmp exit_flag, 1      ; Проверка флага завершения
    jne wait_loop         ; Продолжаем ожидание, если флаг не установлен

    ; 7. ВОССТАНОВЛЕНИЕ ОРИГИНАЛЬНОГО ВЕКТОРА ПЕРЕД ЗАВЕРШЕНИЕМ (7 ТЗ)
    push ds
    mov dx, word ptr KEEP_VECTOR      ; Восстанавливаем смещение
    mov ax, word ptr KEEP_VECTOR+2    ; Восстанавливаем сегмент
    mov ds, ax
    mov ah, 25h           ; Функция установки вектора прерывания
    mov al, 09h           ; Номер прерывания
    int 21h
    pop ds
    mov ax, 0C00h        
    int 21h

    ; 8. Завершение программы
    mov ax, 4C00h         ; Функция DOS 4Ch - завершение программы
    int 21h               ; Код возврата 0

end start
