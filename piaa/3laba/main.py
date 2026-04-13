print("==Сначала введите особые символы ЗАМЕНЫ и их цену в ОДНУ строку. Пример 'a2 d3 b4' (символ a будет заменяться с стоимостью 2 и тд). Ввод завершается переносом строки, пары вводите через пробел")
replace_symbol = input().split()
print("==Теперь введите особые символы ДОБАВЛЕНИЯ и их цену в ОДНУ строку. Пример 'a2 d3 b4' (символ a будет добавляться с стоимостью 2 и тд). Ввод завершается переносом строки, пары вводите через пробел")
insert_symbol = input().split()
print("==Введите обычные стоимости, первую строку и втору. Формат ввода как в заданиях 4.1")

cost = list(map(int, input().split()))
#замена, вставка, удаление 
a = input().strip()
b = input().strip()
M = len(a)
N = len(b)

replace_dict = {}
for h in replace_symbol:
    replace_dict.update({h[0]:int(h[1:])})


insert_dict = {}
for h in insert_symbol:
    insert_dict.update({h[0]:int(h[1:])})

D = [[0] * (N+1) for _ in range(M+1)]

def m(a1, a2):
    if a1 == a2: return 0
    return 1
    

for j in range(1, N+1):
    if b[j-1] not in insert_dict:
        D[0][j] = D[0][j-1] + cost[1]
    else:
        D[0][j] = D[0][j-1] + insert_dict[b[j-1]]
for i in range(1, M+1):
    D[i][0] = D[i-1][0] + cost[2]

for i in range(1, M+1):
    for j in range(1, N+1):
        if m(a[i-1], b[j-1]) == 0:
            tmp = D[i-1][j-1]
        else:
            if a[i-1] not in replace_dict:
                tmp = D[i-1][j-1] + cost[0]
            else:
                tmp = D[i-1][j-1] + replace_dict[a[i-1]]
        tmp_insert = cost[1] if b[j-1] not in insert_dict else insert_dict[b[j-1]]
        D[i][j] = min(D[i][j-1] + tmp_insert, D[i-1][j] + cost[2], tmp)

print(D[M][N])