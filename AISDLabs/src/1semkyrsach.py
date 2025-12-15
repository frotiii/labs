import numpy as np 
import array as array 

#!класс самого товара
class Product:  
    def __init__(self, id: int, name: str, count: int, price: int):
        self.id = id
        self.name = name
        self.count = count 
        self.price = price
        
    def update(self,  name: str = None, count: int = None, price: int = None):
        if name is not  None: self.name = name
        if count is not  None: self.count = count
        if price is not  None: self.price = price

    def display_str(self):
        return f"Название товара: {self.name} \nАйди товара: {self.id} \nКоличество товара на складе: {self.count} \nЦена товра: {self.price} \n"
    

#!класс ноды дерева 
class AVLNode:
    def __init__(self, product: Product):
        self.product = product # в самом узле товар соотв
        self.height = 1
        self.left = None
        self.right = None 

    @property #благодаря этому метод будет типа атрибутом, а не функцией, это для удоюбства читаемости ну и просто круто, что вызвать ацдишник товраа можно будет как tovar.key, а не tovar.key()
    def key(self):
        return self.product.id 
    
    def update_height(self):
        leftH = self.left.height if self.left else 0
        rightH = self.right.height if self.right else 0
        self.height = 1 + max(leftH, rightH)

    def balance_factor(self):
        leftH = self.left.height if self.left else 0
        rightH = self.right.height if self.right else 0
        return leftH - rightH #потом главнео не забыть порядок. > 1 ЛЕВОЕ больше, < -1 ПРАВОЕ больше

class AVLTree:
    def __init__(self):
        self.root = None

    def is_empty(self):
        return self.root is None 
        
    # так кароче тут балансировки 4; малые сначада; сделала левую корнем правый поворот; 
    #     p               q
    #   /   \            /  \
    #  q      c  <=>    a    p
    # / \                   / \
    # a  b                 b    c
    
    def small_right_rotate(self, p: AVLNode): # новый корень левый ребеноек => поворот правый
        q = p.left
        b = q.right 

        p.left = b 
        q.right = p

        p.update_height()
        q.update_height()

        return q
    
    def small_left_rotate(self, q: AVLNode): # новый корень правый ребеноек => поворот левый
        p = q.right
        b = p.left

        q.right = b
        p.left = q

        p.update_height()
        q.update_height()

        return p
    
    def big_right_rotate(self, a: AVLNode):
        a.left = self.small_left_rotate(a.left)
        return self.small_right_rotate(a)
    
    def big_left_rotate(self, a: AVLNode):
        a.right = self.small_right_rotate(a.right)
        return self.small_left_rotate(a)

    # БАЛАНСИРОВКИ -- ВСЕ!!!!
    
    def _balance(self, node: AVLNode):
        if not node: return None
        node.update_height()
        balance = node.balance_factor()

        # определить именно поворот 
        if balance > 1 and node.left.balance_factor() >= 0:
            return self.small_right_rotate(node)
        
        if balance < -1 and node.right.balance_factor() <= 0:
            return self.small_left_rotate(node)

        if balance > 1 and node.left.balance_factor() < 0:
            return self.big_right_rotate(node)

        if balance < -1 and node.right.balance_factor() > 0:
            return self.big_left_rotate(node)

        return node 
    

    def search(self, product_id):
        def _search(node, target_id):
            if not node:
                return None
            if target_id == node.key:
                return node.product
            elif target_id < node.key:
                return _search(node.left, target_id)
            else:
                return _search(node.right, target_id)
        
        return _search(self.root, product_id)
        
    def contains(self, product_id):
        return self.search(product_id) is not None 
    
    def insert(self, product: Product):
        def _insert(node: AVLNode, new_product: Product):
            if not node:
                return AVLNode(new_product)

            if new_product.id < node.key:
                node.left = _insert(node.left, new_product)
            elif new_product.id > node.key:
                node.right = _insert(node.right, new_product)
            else:
                print(f"Товар с ID {new_product.id} уже существует!")
                return node  
            
            return self._balance(node)
        
        self.root = _insert(self.root, product)
        return True
    
    
    def _find_min(self, node: AVLNode):
        curr = node
        while curr and curr.left: 
            curr = curr.left
        return curr
    
    def delete(self, product_id):
        def _delete(node: AVLNode, target_id):
            if not node:
                return None 
            
            if target_id < node.key:
                node.left =  _delete(node.left, target_id)
            elif target_id > node.key:
                node.right = _delete(node.right, target_id)
            else:
                if not node.left: return node.right 
                elif not node.right: return node.left

                tmp = self._find_min(node.right)
                node.product = tmp.product 
                node.right = _delete(node.right, tmp.key)
            return self._balance(node)
        self.root = _delete(self.root, product_id)
        return True
    
    def inorder(self):
        def _count_nodes(node):
            if not node:
                return 0
            return 1 + _count_nodes(node.left) + _count_nodes(node.right)
        
        total_nodes = _count_nodes(self.root)
    
        ids = array.array('i')
        counts = array.array('i')
        prices = array.array('i')
        names = np.empty(total_nodes, dtype='U50') if total_nodes > 0 else np.array([], dtype='U50')

        index = 0

        def _inorder(node: AVLNode):
            nonlocal index
            if node: 
                _inorder(node.left)
                ids.append(node.product.id)
                counts.append(node.product.count)
                prices.append(node.product.price)
                names[index] = node.product.name
                index += 1
                _inorder(node.right)

        _inorder(self.root)
        return ids, names, counts, prices
    
    def preorder(self):
        def _count_nodes(node):
            if not node:
                return 0
            return 1 + _count_nodes(node.left) + _count_nodes(node.right)
        
        total_nodes = _count_nodes(self.root)

        ids = array.array('i')
        counts = array.array('i')
        prices = array.array('i')
        names = np.empty(total_nodes, dtype='U50') if total_nodes > 0 else np.array([], dtype='U50')

        index = 0

        def _preorder(node: AVLNode):
            nonlocal index
            if node:
                ids.append(node.product.id)
                counts.append(node.product.count)
                prices.append(node.product.price)
                if total_nodes > 0:
                    names[index] = node.product.name
                    index += 1
                _preorder(node.left)
                _preorder(node.right)
        
        _preorder(self.root)
        return ids, names, counts, prices

    
    def print_all_products(self):
        ids, names, counts, prices = self.inorder()
        
        print("\n★ Все товары, что находятся в продаже")
        if len(ids) == 0:
            print("Товаров нет")
        else:
            for i in range(len(ids)):
                print(f"id: {ids[i]}, Название: '{names[i]}', Кол-во: {counts[i]}, Цена: {prices[i]}")
            print(f"Всего товаров: {len(ids)}")

    def size(self):
        ids, _, _, _ = self.inorder()
        return len(ids)
    

    def visualize_tree(self):
        def _visualize(node, prefix="", is_left=True):
            if not node:
                return ""
            
            result = ""
            result += _visualize(node.right, prefix + ("│   " if is_left else "    "), False)
            
            connector = "└ " if is_left else "┌ "
            result += prefix + connector + f"id:{node.key}\n"
            
            result += _visualize(node.left, prefix + ("    " if is_left else "│   "), True)
            return result
        
        if self.is_empty():
            visualization = "Дерево пустое\n"
        else:
            visualization = "AVL-дерево:\n"
            visualization += _visualize(self.root, "", False)
        
        print(visualization)
        return visualization
    

    
# !Класс самого магазина, который все склеивает 
class Store:
    def __init__(self):
        self.store = AVLTree()

    #? добавлять товар на склад
    def add_product(self, id: int, name:  str, price: int): 
        product = self.store.search(id)
        if product: 
            product.count += 1
            print(f"Товар '{product.name}' [id: {id}]; Количество увеличено до {product.count}")
            return True
        else: 
            product = Product(id, name, 1, price)
            flag = self.store.insert(product)
            if flag: 
                print(f"Новый товар '{name}' [id: {id}] добавлен на склад")
            return flag 

    #? удалять товар по ID 
    def delete_product(self, id: int):
        if not self.store.contains(id): 
            print(f"Товра с id {id} не найден")
            return False
        
        flag = self.store.delete(id)
        if flag: 
            print("Товар успешно удален")
        return flag
    
    #? обновлять информацию о товаре
    def update_product(self, id: int, name: str = None, count: int = None, price: int = None):
        product = self.store.search(id)
        if not product: 
            print(f"Товра с id {id} не найден")
            return False
        
        product.update(name = name, count = count, price = price)
        print("Товар обновлен ")
        return True 
    
    #? искать товар по его ID и получать всю информацию о нем
    def find_product(self, id):
        product = self.store.search(id)
        if not product: 
            print(f"Товра с id {id} не найден")
        else: print(product.display_str())
    
    #? выводить все товары в порядке возрастания ID
    def display_products(self):
        self.store.print_all_products()

