class Cell:
    def __init__(self, val=None):
        self.next = None
        self.prev = None
        self.val = val

class ChainList:
    def __init__(self):
        self.head = None
        self.tail = None
        self.size = 0

    def append(self, val):
        node = Cell(val)
        if self.size == 0:
            self.head = node
            self.tail = node
        else:
            self.tail.next = node
            node.prev = self.tail
            self.tail = node
        self.size += 1

    def __iter__(self):
        cur = self.head
        while cur:
            yield cur.val
            cur = cur.next

class RopeLeaf:
    def __init__(self, left=None, right=None, chars=None):
        self.left = left
        self.right = right
        self.height = 1
        if chars is None:
            self.chars = None
        else:
            self.chars = array('u', chars) if not isinstance(chars, array) else chars
        self.weight = len(self.chars) if self.is_leaf() else 0

    def is_leaf(self):
        return self.left is None and self.right is None and self.chars is not None

class RopeTree:
    MAX_SEG = 8

    def __init__(self, text=""):
        self.root = self.build_tree(text)

    def build_tree(self, text):
        if not text:
            return None
        if len(text) <= self.MAX_SEG:
            return RopeLeaf(chars=text)
        half = len(text) // 2
        left_part = self.build_tree(text[:half])
        right_part = self.build_tree(text[half:])
        return self.join_nodes(left_part, right_part)

    def total_len(self, node):
        if node is None:
            return 0
        if node.is_leaf():
            return len(node.chars)
        return self.total_len(node.left) + self.total_len(node.right)

    def node_height(self, node):
        return 0 if node is None else node.height

    def refresh(self, node):
        if node is None:
            return None
        lh = self.node_height(node.left)
        rh = self.node_height(node.right)
        node.height = 1 + max(lh, rh)
        if not node.is_leaf():
            node.weight = self.total_len(node.left)
        return node

    def balance_factor(self, node):
        if node is None:
            return 0
        return self.node_height(node.left) - self.node_height(node.right)

    def rotate_left(self, node):
        temp = node.right
        node.right = temp.left
        temp.left = node
        self.refresh(node)
        self.refresh(temp)
        return temp

    def rotate_right(self, node):
        temp = node.left
        node.left = temp.right
        temp.right = node
        self.refresh(node)
        self.refresh(temp)
        return temp

    def rebalance_node(self, node):
        if node is None:
            return None
        self.refresh(node)
        diff = self.balance_factor(node)
        if diff > 1:
            if self.balance_factor(node.left) < 0:
                node.left = self.rotate_left(node.left)
            return self.rotate_right(node)
        if diff < -1:
            if self.balance_factor(node.right) > 0:
                node.right = self.rotate_right(node.right)
            return self.rotate_left(node)
        return node

    def join_nodes(self, left, right):
        if left is None: return right
        if right is None: return left
        combined = RopeLeaf(left=left, right=right)
        combined.weight = self.total_len(left)
        return self.rebalance_node(combined)

    def split_node(self, node, idx):
        if node is None:
            return None, None
        if node.is_leaf():
            s = node.chars.tounicode()
            left_s, right_s = s[:idx], s[idx:]
            left_node = RopeLeaf(chars=left_s) if left_s else None
            right_node = RopeLeaf(chars=right_s) if right_s else None
            return left_node, right_node
        if idx < node.weight:
            left_part, right_part = self.split_node(node.left, idx)
            joined_right = self.join_nodes(right_part, node.right)
            return left_part, self.rebalance_node(joined_right)
        else:
            left_part, right_part = self.split_node(node.right, idx - node.weight)
            joined_left = self.join_nodes(node.left, left_part)
            return self.rebalance_node(joined_left), right_part

    def insert_text(self, node, pos, txt):
        if node is None:
            return self.build_tree(txt)
        left_piece, right_piece = self.split_node(node, pos)
        mid = self.build_tree(txt)
        return self.rebalance_node(self.join_nodes(self.join_nodes(left_piece, mid), right_piece))

    def remove_range(self, node, start, end):
        if node is None:
            return None
        left_part, tmp = self.split_node(node, start)
        _, right_part = self.split_node(tmp, end - start)
        return self.rebalance_node(self.join_nodes(left_part, right_part))

    def to_string(self, node=None):
        node = node or self.root
        if node is None:
            return ""
        if node.is_leaf():
            return node.chars.tounicode()
        return self.to_string(node.left) + self.to_string(node.right)

    def collect_nodes(self, node, leafs, internals):
        if node is None:
            return
        if node.is_leaf():
            leafs.append(node.chars.tounicode())
        else:
            internals.append(node.weight)
            self.collect_nodes(node.left, leafs, internals)
            self.collect_nodes(node.right, leafs, internals)

    def display(self):
        leaves = ChainList()
        nodes = ChainList()
        leaf_list = []
        node_list = []
        self.collect_nodes(self.root, leaf_list, node_list)
        for v in leaf_list:
            leaves.append(v)
        for w in node_list:
            nodes.append(w)
        i = 1
        for v in leaves:
            print(f"Leaf {i}: {v}")
            i += 1
        j = 1
        for w in nodes:
            print(f"Node {j}: {w}")
            j += 1
        print(self.to_string())

text = input().strip()
ins = input().split(" ", 1)
dele = input().split(" ", 1)

rope = RopeTree(text)
rope.display()
if ins and ins[0] != "":
    rope.root = rope.insert_text(rope.root, int(ins[0]), ins[1])
    rope.display()
if dele and dele[0] != "":
    rope.root = rope.remove_range(rope.root, int(dele[0]), int(dele[0]) + int(dele[1]))
    rope.display()
    