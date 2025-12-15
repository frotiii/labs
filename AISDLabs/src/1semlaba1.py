class CircularDeque:
    def __init__(self, n: int, push_force: bool):
        self.buffer = [None]*n
        self.push_force = push_force
        self.capacity = n 
        self.get_front = -1
        self.get_back = -1
        self.count = 0

    def push_front(self, x: int):
        if self.full():
            if self.push_force:
                self.get_front = (self.get_front - 1) % self.capacity
                self.buffer[self.get_front] = x
                self.get_back = (self.get_back - 1) % self.capacity
            else:
                raise Exception("Error")
        else:
            if self.empty():
                self.get_front = 0
                self.get_back = 0
            else:
                self.get_front = (self.get_front - 1) % self.capacity
            self.buffer[self.get_front] = x
            self.count += 1


    def push_back(self, x: int):
        if self.full():
            if self.push_force:
                self.get_back = (self.get_back + 1) % self.capacity
                self.buffer[self.get_back] = x
                self.get_front = (self.get_front + 1) % self.capacity
            else:
                raise Exception("Error")
        else:
            if self.empty():
                self.get_front = 0
                self.get_back = 0
            else:
                self.get_back = (self.get_back + 1) % self.capacity
            self.buffer[self.get_back] = x
            self.count += 1



    def pop_front(self) -> int:
        if self.empty():
            raise Exception("Erorr")
        else:
            value = self.buffer[self.get_front]
            if self.size() == 1:
                self.get_front = -1
                self.get_back = -1
            else:
                self.get_front = (self.get_front + 1) % self.capacity
            self.count -= 1
            return value


    def pop_back(self) -> int:
        if self.empty():
            raise Exception("Erorr")
        else:
            value = self.buffer[self.get_back]
            if self.size() == 1:
                self.get_front = -1
                self.get_back = -1
            else:
                self.get_back = (self.get_back - 1) % self.capacity
            self.count -= 1
            return value

    def front(self) -> int:
        return self.buffer[self.get_front]

    def back(self) -> int:
        return self.buffer[self.get_back]

    def size(self) -> int:
        return self.count
    
    def empty(self) -> bool:
        return self.count == 0

    def full(self):
        return self.count == self.capacity

    def resize(self, new_cap: int):
        new_buffer = [None] * new_cap
        ind = self.get_front
        for i in range(min(self.count, new_cap)):
            new_buffer[i] = self.buffer[ind]
            ind = (ind + 1) % self.capacity
        self.buffer = new_buffer
        self.capacity = new_cap

        if self.count > 0:
            self.get_front = 0
            self.get_back = min(self.capacity - 1, self.count -1)
            self.count = min(self.capacity, self.count)


# добавление в конец -- добавление вместо самого старого жлемента
# в начало -- добавление вместо самого свежего 
#  bool push_force, отражающее, будут ли при переполнении буфера элементы перезаписывать старые. 
