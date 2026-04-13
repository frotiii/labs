import sys

full_input = sys.stdin.read().strip().split('\n')
start_V = int(full_input[0].strip())
matrix = []

for v in full_input[1:]:
    row = list(map(float, v.split()))
    matrix.append(row)

n = len(matrix)

#!построение графа пор приму 
added = [False] * n           
min_edge = [float('inf')] * n 
parent = [-1] * n            

min_edge[0] = 0  

for _ in range(n):
    v = -1
    current_min = float('inf')
    for i in range(n):
        if not added[i] and min_edge[i] < current_min:
            current_min = min_edge[i]
            v = i
    
    if v == -1:
        break
    added[v] = True

    for u in range(n):
        if (not added[u] and matrix[u][v] != -1 and matrix[u][v] < min_edge[u]):
            min_edge[u] = matrix[u][v]
            parent[u] = v

#! нахождение эйлерова цикла 
tree = [[] for _ in range(n)]
for i in range(n):
    if parent[i] != -1:
        cost = matrix[i][parent[i]]
        tree[i].append((parent[i], cost))
        tree[parent[i]].append((i, cost))

for i in range(n):
    tree[i].sort(key=lambda x: x[1])

def preorder(v, visited, tree, path):

    path.append(v)
    visited[v] = True
    
    for neighbor, _ in tree[v]:
        if not visited[neighbor]:
            preorder(neighbor, visited, tree, path)

visited = [False] * n
euler_path = []
preorder(start_V, visited, tree, euler_path)

euler_path.append(start_V)


#!удаление поыторок 

final_path = []
seen = set()

for vertex in euler_path:
    if vertex not in seen:
        seen.add(vertex)
        final_path.append(vertex)

if final_path[-1] != start_V:
    final_path.append(start_V)

#тут уже рассчеты для выовда 
total_len = 0
for i in range(len(final_path) - 1):
    froms = final_path[i]
    tos = final_path[i + 1]
    total_len += matrix[froms][tos]

print(total_len)
print(*final_path)