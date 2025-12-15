.data
arr:        .word 0,0,0,0,0,0,0,0,0,0 #иниц массива нулями 

.text
.globl _start

_start:
    li s0, 18        # a = 18
    li s1, 8         # b = 8  
    li s2, 7         # c = 7

    # arr[0] = a + b + c
    # arr[i+1] = arr[i] + a + b - c

    la t0, arr        # t0 -- arr[0]
    add t1, s0, s1    # t1 = a + b
    add t1, t1, s2    # t1 = a + b + c
    sw t1, 0(t0)      # arr[0] = t1

    li t2, 0          # для границ
    li t6, 9        

inic:
    bge t2, t6, my_task       # когда цикл завершен выход

    slli t3, t2, 2             # сдвиг битовый для правильной индексации ( 0,4,8..)
    add t3, t3, t0             # для получения тончого адреса в памяти
    
    lw t4, 0(t3)               # текущий элемент в т4
    
    # arr[i] + a + b - c
    add t4, t4, s0
    add t4, t4, s1
    sub t4, t4, s2
    
    # Сохраняем в arr[i+1]
    sw t4, 4(t3)               # +4 бита получили след элемент, туда сохранили

    addi t2, t2, 1
    j inic


my_task:

    # если (arr[6] + arr[7] + arr[5] < threshold) 
    # то (res1 = arr[8] & arr[9]) → t7
    # иначе (res2 = arr[1] & c) → a3
    
    # threshold -> s1
    # res1 -> t7  
    # res2 -> a3

    # использование базовой адресации для считывания элементов массива
    lw t1, 24(t0)    # 6*4 = 24
    lw t2, 28(t0)    # 7*4 = 28 
    lw t3, 20(t0)    # 5*4 = 20

    # использование регистровой адресации для сложения
    add t4, t1, t2       
    add t4, t4, t3   

    # использование непосредственной адресации для установки порога
    li s1, 500           

    # относительная к счетчику команд , переход
    blt t4, s1, do_true  # если меньше то тру
    
do_false:
    lw t1, 4(t0)         # arr[1]
    and a3, t1, s2    
    j exit  
    
do_true:
    lw t1, 32(t0)        # это arr[8]
    lw t2, 36(t0)        # это arr[9]
    and t6, t1, t2    
     
 
exit:
    li a7, 10
    ecall