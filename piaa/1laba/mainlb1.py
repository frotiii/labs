N = int(input())

flag_for2 = False
flag_for3 = False
flag_for5 = False
    
if N % 2 == 0 and N % 3 != 0:
    flag_for2 = True
elif N % 3 == 0 and N % 2 != 0:
    flag_for3 = True
elif N % 3 == 0 and N % 2 == 0:
    flag_for2 = True
    flag_for3 = False
elif N%5 == 0 and N % 3 != 0:
    flag_for5 = True
elif  N%5 == 0 and N % 3 == 0:
    flag_for5 = False
    flag_for3 = True


min_count = float('inf') #для мин колва квадратиков 
solution_for_min_count = [] #тут должны юбыть координаты левого + размиер квад

heights = [0] * N 


def find_first_zero(h):
    min_h = min(heights)
    if min_h == N: return None
    return h.index(min_h), min_h

def first_big(h, tmp_solution, minus):
    global min_count, solution_for_min_count
    
    for one in range(N//2 + 1, N - minus):
        two = N - one
        three = two

        for i in range(one):
            h[i] = one
        tmp_solution.append((1, 1, one))

        for i in range(one, N):
            h[i] = two
        tmp_solution.append((1, 1 + one, two))

        for i in range(two):
            h[i] += two
        tmp_solution.append((1 +one, 1, three))

        main_func(h, tmp_solution)

        tmp_solution.pop()
        tmp_solution.pop()
        tmp_solution.pop()

        for i in range(N):
            h[i] = 0

def main_func(h, tmp_solution):
    global min_count, solution_for_min_count

    first = find_first_zero(h)
    if first is None: #все заполнено => нужно сравнить с пред и если чо обноить данные 
        if len(tmp_solution) < min_count:
            min_count = len(tmp_solution)
            solution_for_min_count = tmp_solution.copy()
            return 
    if len(tmp_solution) + 1 >= min_count:
        return 
    x, y = first
    
    max_size = 0
    while ( x + max_size < N ) and (h[x + max_size] == y): max_size += 1
    maximal = min(max_size, N - y)
    for size in range(maximal, 0, -1):

        for i in range(x, x + size):
            h[i] += size

        tmp_solution.append((x+1, y+1, size))
        main_func(h, tmp_solution)

        tmp_solution.pop()
        for i in range(x, x + size):
            h[i] -= size

if flag_for2: 
    print(4)
    print(f'1 1 {N//2}')
    print(f'{1 + N//2} 1 {N//2}')
    print(f'1 {1 + N//2} {N//2}')
    print(f'{1 + N//2} {1 + N//2} {N//2}')
elif flag_for3:
    print(6)
    size = N // 3
    print(f'1 1 {size*2}')
    print(f'1 {1 + size*2} {size}')
    print(f'{1 + size*2} 1 {size}')
    print(f'{1 + size} {1 + size*2} {size}')
    print(f'{1 + size*2} {1 + size} {size}')
    print(f'{1 + size*2} {1 + size*2} {size}')
elif flag_for5:
    print(8)
    size = N // 5
    print(f'1 1 {size*3}')
    print(f'{1 + size*3} 1 {size*2}')
    print(f'1 {1 + size*3} {size*2}')
    print(f'{1 + size*2} {1 + size*3} {size*2}')

    print(f'{1 + size*3} {1 + size*2} {size}')
    print(f'{1 + size*4} {1 + size*2} {size}')
    print(f'{1 + size*4} {1 + size*3} {size}')
    print(f'{1 + size*4} {1 + size*4} {size}')
else:
    if N < 7: 
        main_func(heights, [])
    if N < 29:
        first_big(heights, [], 0)
    if N < 37:
        first_big(heights, [], 13)
    else:
        first_big(heights, [], 16)
    
    print(min_count)
    for i in range(len(solution_for_min_count)):
        print(*solution_for_min_count[i])

