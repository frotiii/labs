
import random
import time
from statistics import mean


def tests(matrix, start_V):

    n = len(matrix)
    steps = 0 
    
    added = [False] * n
    min_edge = [float('inf')] * n
    parent = [-1] * n
    
    min_edge[0] = 0
    
    for _ in range(n):
        v = -1
        current_min = float('inf')

        for i in range(n):
            steps += 1 
            if not added[i] and min_edge[i] < current_min:
                current_min = min_edge[i]
                v = i
        
        if v == -1:
            break
        
        added[v] = True
        steps += 1  
        

        for u in range(n):
            steps += 1  
            if (not added[u] and matrix[u][v] != -1 and matrix[u][v] < min_edge[u]):
                min_edge[u] = matrix[u][v]
                parent[u] = v
                steps += 1  
    
    tree = [[] for _ in range(n)]

    
    for i in range(n):
        steps += 1 
        if parent[i] != -1:
            cost = matrix[i][parent[i]]
            tree[i].append((parent[i], cost))
            tree[parent[i]].append((i, cost))
            steps += 2 
    
    for i in range(n):
        tree[i].sort(key=lambda x: x[1])
        steps += len(tree[i]) 
    

    visited = [False] * n
    euler_path = []

    
    def preorder(v, visited, tree, path):
        nonlocal steps
        path.append(v)
        visited[v] = True
        steps += 2  
        
        for neighbor, _ in tree[v]:
            steps += 1  
            if not visited[neighbor]:
                preorder(neighbor, visited, tree, path)
    
    preorder(start_V, visited, tree, euler_path)
    euler_path.append(start_V)
    steps += 1  
    
    final_path = []
    seen = set()
    
    for vertex in euler_path:
        steps += 1 
        if vertex not in seen:
            seen.add(vertex)
            final_path.append(vertex)
            steps += 2 
    
    if final_path[-1] != start_V:
        final_path.append(start_V)
        steps += 1  
    

    total_len = 0
    for i in range(len(final_path) - 1):
        froms = final_path[i]
        tos = final_path[i + 1]
        total_len += matrix[froms][tos]
        steps += 1 
    
    return steps




def generate_matrix(n, min_weight=1, max_weight=100, seed=None):
    if seed is not None:
        random.seed(seed)
    
    matrix = [[-1] * n for _ in range(n)]
    
    for i in range(n):
        for j in range(i + 1, n):
            weight = random.uniform(min_weight, max_weight)
            matrix[i][j] = weight
            matrix[j][i] = weight
    
    return matrix

def generate_test():
    test_sizes = [3, 4, 5, 6, 7, 8, 9, 10, 12, 15]  # 10 размеров матриц
    test_data = []
    
    for n in test_sizes:
        for i in range(10):  # 10 матриц для каждого размера
            matrix = generate_matrix(n, seed=n*100 + i)
            start_V = random.randint(0, n-1)
            test_data.append((n, start_V, matrix))
    
    return test_data


def run_tests():
    test_data = generate_test()
    
    results_by_size = {}

    
    for _, (n, start_V, matrix) in enumerate(test_data):

        start_time = time.perf_counter()
        steps = tests(matrix, start_V)
        end_time = time.perf_counter()
        
        result_time = end_time - start_time
        
        if n not in results_by_size:
            results_by_size[n] = {'times': [], 'steps': []}
        
        results_by_size[n]['times'].append(result_time)
        results_by_size[n]['steps'].append(steps)

    med_results = []
    for n in sorted(results_by_size.keys()):
        avg_time = mean(results_by_size[n]['times'])
        avg_steps = mean(results_by_size[n]['steps'])
        med_results.append((n, avg_time, avg_steps))
    
    return med_results


def print_results(results):
    print("размер =", [r[0] for r in results])
    print("время =", [r[1] for r in results])
    print("шаги =", [r[2] for r in results])


        
results = run_tests()
print_results(results)
    
