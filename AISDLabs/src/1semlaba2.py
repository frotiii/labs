def min_run(n):
    r = 0
    while n >= 16:
        r |= n & 1
        n >>= 1
    min_r = n + r
    return min_r

def insertion_sort(arr):
    result = array('i')
    if len(arr) == 0:
        return result
    result.append(arr[0])

    for i in range(1, len(arr)):
        item = arr[i]
        flag = False

        for j in range(len(result)):
            if abs(item) > abs(result[j]):
                tmp = array('i')
                for k in range(j):
                    tmp.append(result[k])
                tmp.append(item)
                for k in range(j, len(result)):
                    tmp.append(result[k])
                result = tmp
                flag = True
                break
        if not flag:
            result.append(item)

    return result

def galop(arr, start, end, item): #галоп чере бинарку. поиск, куда можно вставить переданный айтем
    left, right = start, end
    pos = right 

    while left <= right:
        mid = left + (right - left) // 2
        if abs(arr[mid]) < abs(item):
            pos = mid - 1
            right = mid - 1
        else:
            left = mid + 1
    return pos

def merge_no_main(a, b, id): #сливает конкретные два массива галопом 
    result = array('i')
    i = j = 0

    count_a = count_b = 0
    count_galop = 0

    while i < len(a) and j < len(b):
        if abs(a[i]) >= abs(b[j]):
            result.append(a[i])
            i += 1
            count_a += 1
            count_b = 0

            if count_a >= 3:
                pos = galop(a, i, len(a) - 1, b[j])
                count_galop += 1
                while i <= pos and i < len(a):
                    result.append(a[i])
                    i += 1
                count_a = 0

        else:
            result.append(b[j])
            j += 1
            count_b += 1
            count_a = 0

            if count_b >= 3:
                pos = galop(b, j, len(b) - 1, a[i])
                count_galop += 1
                while j <= pos and j < len(b):
                    result.append(b[j])
                    j += 1
                count_b = 0

    while i < len(a):
        result.append(a[i])
        i += 1
    
    while j < len(b):
        result.append(b[j])
        j += 1

    print(f"Gallops {id}: {count_galop}")
    print(f"Merge {id}:", ' '.join(str(x) for x in result))

    return result

def merge_block(blocks):

    all_blocks = array('i')

    block_bounds = array('i')
 
    for block in blocks:
        start = len(all_blocks)
        all_blocks.extend(block)
        end = len(all_blocks) - 1
        block_bounds.append(start)
        block_bounds.append(end)

    stack = array('i')
    count = 0

    for i in range(0, len(block_bounds), 2):
        stack.append(i)

        while len(stack) >= 2:
            if len(stack) == 2:
                z_idx = stack.pop()  
                y_idx = stack.pop()  
                
                y_start, y_end = block_bounds[y_idx], block_bounds[y_idx+1]
                z_start, z_end = block_bounds[z_idx], block_bounds[z_idx+1]
                y_len = y_end - y_start + 1
                z_len = z_end - z_start + 1
                
                if y_len <= z_len:
                    y_block = all_blocks[y_start:y_end+1]
                    z_block = all_blocks[z_start:z_end+1]
                    merged = merge_no_main(y_block, z_block, count)
                    count += 1

                    new_start = len(all_blocks)
                    all_blocks.extend(merged)
                    new_end = len(all_blocks) - 1
                    block_bounds.append(new_start)
                    block_bounds.append(new_end)
                    stack.append(len(block_bounds) - 2)
                else:
                    stack.append(y_idx)
                    stack.append(z_idx)
                    break
            
            if len(stack) >= 3:
                z_idx = stack.pop()
                y_idx = stack.pop()
                x_idx = stack.pop()
                
                x_start, x_end = block_bounds[x_idx], block_bounds[x_idx+1]
                y_start, y_end = block_bounds[y_idx], block_bounds[y_idx+1]
                z_start, z_end = block_bounds[z_idx], block_bounds[z_idx+1]
                
                x_len = x_end - x_start + 1
                y_len = y_end - y_start + 1
                z_len = z_end - z_start + 1
                
                if x_len <= y_len + z_len or y_len <= z_len:
                    if x_len >= z_len:

                        y_block = all_blocks[y_start:y_end+1]
                        z_block = all_blocks[z_start:z_end+1]
                        merged = merge_no_main(y_block, z_block, count)
                        count += 1
                        
                        new_start = len(all_blocks)
                        all_blocks.extend(merged)
                        new_end = len(all_blocks) - 1
                        
                        block_bounds.append(new_start)
                        block_bounds.append(new_end)
                        stack.append(x_idx)
                        stack.append(len(block_bounds) - 2)
                    else:

                        x_block = all_blocks[x_start:x_end+1]
                        y_block = all_blocks[y_start:y_end+1]
                        merged = merge_no_main(x_block, y_block, count)
                        count += 1
                        
                        new_start = len(all_blocks)
                        all_blocks.extend(merged)
                        new_end = len(all_blocks) - 1
                        
                        block_bounds.append(new_start)
                        block_bounds.append(new_end)
                        stack.append(len(block_bounds) - 2)
                        stack.append(z_idx)
                else:
                    stack.append(x_idx)
                    stack.append(y_idx)
                    stack.append(z_idx)
                    break

    while len(stack) > 1:
        if len(stack) == 2:
            z_idx = stack.pop()
            y_idx = stack.pop()
            
            y_block = all_blocks[block_bounds[y_idx]:block_bounds[y_idx+1]+1]
            z_block = all_blocks[block_bounds[z_idx]:block_bounds[z_idx+1]+1]
            merged = merge_no_main(y_block, z_block, count)
            count += 1
            
            new_start = len(all_blocks)
            all_blocks.extend(merged)
            new_end = len(all_blocks) - 1
            
            block_bounds.append(new_start)
            block_bounds.append(new_end)
            stack.append(len(block_bounds) - 2)
        
        if len(stack) >= 3:
            z_idx = stack.pop()
            y_idx = stack.pop()
            x_idx = stack.pop()
            
            x_start, x_end = block_bounds[x_idx], block_bounds[x_idx+1]
            z_start, z_end = block_bounds[z_idx], block_bounds[z_idx+1]
            x_len = x_end - x_start + 1
            z_len = z_end - z_start + 1
            
            if x_len >= z_len:
                y_block = all_blocks[block_bounds[y_idx]:block_bounds[y_idx+1]+1]
                z_block = all_blocks[block_bounds[z_idx]:block_bounds[z_idx+1]+1]
                merged = merge_no_main(y_block, z_block, count)
                count += 1
                
                new_start = len(all_blocks)
                all_blocks.extend(merged)
                new_end = len(all_blocks) - 1
                
                block_bounds.append(new_start)
                block_bounds.append(new_end)
                stack.append(x_idx)
                stack.append(len(block_bounds) - 2)
            else:
                x_block = all_blocks[block_bounds[x_idx]:block_bounds[x_idx+1]+1]
                y_block = all_blocks[block_bounds[y_idx]:block_bounds[y_idx+1]+1]
                merged = merge_no_main(x_block, y_block, count)
                count += 1
                
                new_start = len(all_blocks)
                all_blocks.extend(merged)
                new_end = len(all_blocks) - 1
                
                block_bounds.append(new_start)
                block_bounds.append(new_end)
                stack.append(len(block_bounds) - 2)
                stack.append(z_idx)
 
    if len(stack) == 1:
        result_idx = stack[0]
        return all_blocks[block_bounds[result_idx]:block_bounds[result_idx+1]+1]
    else:
        return array('i')

def reverse(arr): 
    result = array('i')
    for i in range(len(arr) -1, -1, -1):
        result.append(arr[i])
    return result

if __name__ == "__main__":
    #тип мейн
    n = int(input())
    arr_input = input().split()

    arr = array('i')
    for num_str in arr_input:
        arr.append(int(num_str))

    minrun = min_run(n)
    blocks = []
    id = 0

    i = 0
    while i < n:
        cur = array('i')
        flag_high = True
        flag_low = True

        while i < n - 1 and abs(arr[i]) < abs(arr[i + 1]) and flag_high:
            cur.append(arr[i])
            flag_low = False
            i += 1

        while i < n - 1 and abs(arr[i]) >= abs(arr[i + 1]) and flag_low:
            cur.append(arr[i])
            flag_high = False
            i += 1

        cur.append(arr[i])

        if flag_high and len(cur) > 1:
            cur = reverse(cur)
        
        while i < n - 1 and len(cur) < minrun:
            i += 1
            cur.append(arr[i])

        sort_block = insertion_sort(cur)
        blocks.append(sort_block)
        print(f"Part {id}:", ' '.join(str(x) for x in sort_block))

        i += 1
        id += 1

    if len(blocks) > 1:
        result = merge_block(blocks)
    elif len(blocks) == 1:
        result = blocks[0]
    else:
        result = array('i')

    print("Answer:", ' '.join(str(x) for x in result))