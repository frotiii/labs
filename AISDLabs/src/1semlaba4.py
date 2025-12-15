class SegmentTree:
    def __init__(self, n):
        self.n = n
        self.size = 1
        while self.size < n:
            self.size *= 2
        self.t = array('q', [0] * (4 * n))
    
    def update(self, v, tl, tr, l, r, add):
        if l > r:
            return
        if l == tl and tr == r:
            self.t[v] += add
        else:
            tm = (tl + tr) // 2
            self.update(v*2, tl, tm, l, min(r, tm), add)
            self.update(v*2+1, tm+1, tr, max(l, tm+1), r, add)
    
    def get(self, v, tl, tr, pos):
        if tl == tr:
            return self.t[v]
        tm = (tl + tr) // 2
        if pos <= tm:
            return self.t[v] + self.get(v*2, tl, tm, pos)
        else:
            return self.t[v] + self.get(v*2+1, tm+1, tr, pos)

n, m = map(int, input().split())
seg_tree = SegmentTree(n)

for _ in range(m):
    data = input().split()
    if data[0] == '1':
        l, r, v = int(data[1]), int(data[2]), int(data[3])
        seg_tree.update(1, 0, n-1, l, r-1, v) 
    else:
        i = int(data[1])
        print(seg_tree.get(1, 0, n-1, i))