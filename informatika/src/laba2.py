"""реализация односвязного списка"""


class Node:
    """класс для хранения узла, который содержит значение узла и ссылку на следующий узел"""

    def __init__(self, data, next=None):
        """в конструкторе инициализируются значения узла. Записываются данные узла и ссылка на следующий узел ( по умолчнаию стоит None ).
        на вход принимает значение data и next , который по умолчанию None
        Сложность метода O(1), так как время выполнения не зависит от вводимых данных"""

        self.data = data
        self.next = next

    def get_data(self):
        """метод возвращает значение узла, на вход ничего не подается.
        Сложность метода O(1), так как время выполнения не зависит от вводимых данных ( вводимых данных и нет )"""

        return self.data

    def change_data(self, new_data):
        """метод изменяет текущие данные на новые.
        на вход принимает new_data
        Сложность O(1), так как время выполнения не зависит от вводимых данных"""

        self.data = new_data

    def __str__(self):
        """метод, который выводит данные узла в нужном формате data: <node_data>, next: <node_next>
        на вход ничего не принимает
        Сложность O(1), так как время выполнения не зависит от вводимых данных
        ( вводимые данные в данном случае отсутствуют )"""

        node_data = self.data
        if self.next is not None:
            node_next = self.next.data
        else:
            node_next = None
        return f"data: {node_data}, next: {node_next}"


class LinkedList:
    """класс для создания спискка и работы с ним"""

    def __init__(self, head = None):
        """конструктор создает список. Если значение head == None , то создается пустой список,
        в ином случае создается список из одного элемента
        вводимые данные head.
        Сложность O(1), так как время выпаолнения не зависит от вводимых данных"""

        if head is None:
            self.head = None
            self.length = 0
        else:
            self.head = Node(head)
            self.length = 1

    def __len__(self):
        """метод возвращает длину списка
        на вход ничего не принимает
        сложность O(1), так как время выпаолнения не зависит от вводимых данных"""

        return self.length

    def append(self, element):
        """метод добавляет элемент в конец списка
        на вход подается значение для будущего узла
        метод создает объект класса Node, если head == None, то обновляет head, в этом случае сложность O(1)
        иначе циклом доходит до конца списка и обновляет указатель. После увеличивает значение длины списка
        в этом случае сложность O(n)"""

        new_node = Node(element)
        if self.head is None:
            self.head = new_node
        else:
            tmp = self.head
            while tmp.next:
                tmp = tmp.next
            tmp.next = new_node
        self.length += 1

    def __str__(self):
        """метод выводит значения списка в нужном формате. на вход ничего не принимает
        если список пуст, то возвращает result, сложность O(1)
        иначе проходит по всем элементам списка, записывая их в перемнную value
        после цикла обновляет данные value, result и возвращает последнее. сложность O(n)"""

        result = "LinkedList[]"
        if self.head is not None:
            value = ""
            tmp = self.head
            while tmp.next:
                value += f"data: {tmp.data}, next: {tmp.next.data}; "
                tmp = tmp.next
            value += f"data: {tmp.data}, next: None"
            result = f"LinkedList[length = {self.length}, [{value}]]"
        return result

    def pop(self):
        """метод удаляет последий элемент списка, на вход ничего не принимает
        циклом доходит до предпоследнего элемента и обновляет указатель на последний жлемент ( None ), уменьшает списка длину на 1
        сложность метода O(n)"""

        if self.head is None:
            raise IndexError("LinkedList is empty!")
        if self.length == 1:
            self.head = None
            self.length = 0
        else:
            tmp = self.head
            while tmp.next.next is not None:
                tmp = tmp.next
            tmp.next = None
            self.length -= 1

    def clear(self):
        """метод очищает список, на вход ничего не принимает
        записывает в head None, а длину списка инициализирует нулем
        Сложность O(1)"""

        self.length = 0
        self.head = None

    def change_on_start(self, n, new_data):
        """метод изменяет значение конкретного элемента на новое значение.
        на вход поступает номер элемента ( начиная с 1 ) и новое знаение
        если значение n больше длины списка или меньше 1, то возвращается сообщение об ошибке
        в ином случае цикл доходит до нужного узла и обновляет значение на новое
        сложность O(n)"""

        if n > self.length or n < 1:
            raise KeyError("Element doesn't exist!")
        tmp = self.head
        count = 0
        while count != (n-1):
            tmp = tmp.next
            count += 1
        tmp.data = new_data