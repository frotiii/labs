.data
msg_formula:    .string "R = ((x + a) + (y & c)) & (z & (-b))\n"
msg_consts:     .string "Constants: a = 18, b = 8, c = 7\n"
msg_test:       .string "Input: x="
msg_y:          .string ", y="
msg_z:          .string ", z="
msg_res:        .string "Result: "
newline:        .string "\n"

.text
.globl _start

_start:
    # Загрузка констант в регистры
    li s0, 18        # a = 18
    li s1, 8         # b = 8
    li s2, 7         # c = 7

    # Вывод информации о формуле
    la a0, msg_formula
    li a7, 4
    ecall
    
    # Вывод значений констант
    la a0, msg_consts
    li a7, 4
    ecall

    # Первый тестовый случай
    li a2, 32        # x1 = 32
    li a3, 44        # y1 = 44
    li a4, 22        # z1 = 22

    # Печать входных данных первого теста
    la a0, msg_test
    li a7, 4
    ecall

    mv a0, a2        # печать x1
    li a7, 1
    ecall

    la a0, msg_y     # печать разделителя
    li a7, 4
    ecall

    mv a0, a3        # печать y1
    li a7, 1
    ecall

    la a0, msg_z     # печать разделителя
    li a7, 4
    ecall

    mv a0, a4        # печать z1
    li a7, 1
    ecall

    la a0, newline   # новая строка
    li a7, 4
    ecall

    # Первый вызов процедуры вычисления
    call calc        # вызов через call
    mv a5, a0        # сохранение первого результата

    # Печать результата первого теста
    la a0, msg_res
    li a7, 4
    ecall

    mv a0, a5        # вывод результата
    li a7, 1
    ecall

    la a0, newline   # новая строка
    li a7, 4
    ecall

    # Второй тестовый случай
    li a2, 6536      # x2 = 6536
    li a3, 200       # y2 = 200
    li a4, 300       # z2 = 300

    # Печать входных данных второго теста
    la a0, msg_test
    li a7, 4
    ecall

    mv a0, a2        # печать x2
    li a7, 1
    ecall

    la a0, msg_y     # печать разделителя
    li a7, 4
    ecall

    mv a0, a3        # печать y2
    li a7, 1
    ecall

    la a0, msg_z     # печать разделителя
    li a7, 4
    ecall

    mv a0, a4        # печать z2
    li a7, 1
    ecall

    la a0, newline   # новая строка
    li a7, 4
    ecall

    # Второй вызов процедуры вычисления
    jal ra, calc     # вызов через jal
    mv a6, a0        # сохранение второго результата

    # Печать результата второго теста
    la a0, msg_res
    li a7, 4
    ecall

    mv a0, a6        # вывод результата
    li a7, 1
    ecall

    la a0, newline   # новая строка
    li a7, 4
    ecall

    # Завершение программы
    li a7, 10
    ecall

# Процедура вычисления выражения
calc:
    add t0, a2, s0   # сложение x и a
    and t1, a3, s2   # побитовое И y и c
    neg t2, s1       # вычисление -b
    and t3, a4, t2   # побитовое И z и -b
    add t4, t0, t1   # сложение первых двух частей
    and a0, t4, t3   # финальное побитовое И
    ret              #
