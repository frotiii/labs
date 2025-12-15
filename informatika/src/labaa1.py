"""модуль для работы с книгами и газетами"""


class Edition:
    """класс Edition. Это базовый класс, от которого будут наследоваться другие классы."""

    def __init__(self, name, price, age_limit, style):
        """создание объектов класа Edition и проверка на корректность ввода данных для объектов"""

        if not(isinstance(name, str)) or not(isinstance(price, int)) or price <= 0 \
        or not(isinstance(age_limit, int)) or  age_limit <= 0 or style not in ("c", "b"):
            raise ValueError('Invalid value')
        self.name = name
        self.price = price
        self.age_limit = age_limit
        self.style = style


class Book(Edition):
    """класс Book, который наследуется от класса Edition. \
    Класс содержит объекты author, hardcover, pages и те, которые были унаследованы от класса Edition"""

    def __init__(self, name, price, age_limit, style, author, hardcover, pages):
        """создание объектов класса Book и проверка на корректность ввода данных для объектов"""

        if not(isinstance(author, str)) or not(isinstance(hardcover, bool)) \
        or not(isinstance(pages, int) and pages > 0):
            raise ValueError('Invalid value')
        super().__init__(name, price, age_limit, style)
        self.author = author
        self.hardcover = hardcover
        self.pages = pages

    def __str__(self):
        """метод str, который на вход принимает объект и возвращает строку.\
        Вывод всей информации о книге"""

        return f"Book: название {self.name}, цена {self.price}, возрастное ограничение \
{self.age_limit}, стиль {self.style}, автор {self.author}, твердый переплет {self.hardcover}, количество страниц {self.pages}."

    def __eq__(self, other):
        """метод eq, который принимает на вход два объекта и сранивает их по названию и автору, возвращая булевое значение: True или False"""

        return self.name == other.name and self.author == other.author


class Newspaper(Edition):
    """класс Newspaper, который наследуется от Edition. \
    Класс содержит объекты online_edition, country, frequency и те, которые были унаследованы от класса Edition"""

    def __init__(self, name, price, age_limit, style, online_edition, country, frequency):
        """создание объектов класса Newspaper и проверка на корректность ввода данных для объектов"""

        if not(isinstance(online_edition, bool)) or not(isinstance(country, str)) or not(isinstance(frequency, int) and frequency > 0):
            raise ValueError('Invalid value')
        super().__init__(name, price, age_limit, style)
        self.online_edition = online_edition
        self.country = country
        self.frequency = frequency

    def __str__(self):
        """метод str, который принимает на вход объект, а возвращает строку.\
        Вывод всей информации о газете"""

        return f"Newspaper: название {self.name}, цена {self.price}, возрастное ограничение {self.age_limit}, \
стиль {self.style}, интернет издание {self.online_edition}, страна {self.country}, периодичность {self.frequency}."

    def __eq__(self, other):
        """метод eq, который принимает на вход два объекта, а воозвращает булевое значение. Происходит сравнение газет по нащваниям и стране"""

        return self.name == other.name and self.country == other.country


class BookList(list):
    """класс BookList, который наследуется от list"""

    def __init__(self, name):
        """создание объектов класса BookList"""

        super().__init__()
        self.name = name

    def append(self, p_object):
        """метод append, который получает на вход объект и ничего не возвращает (None). \
        сначала происходит проверка  на принадлежность объекта к классу Book, а после доюавление объекта в список\
        в случае не принадлежности объекта к классу Book выбрасывается ошибка"""

        if isinstance(p_object, Book):
            super().append(p_object)
        else:
            raise TypeError(f'Invalid type {type(p_object)}')

    def total_pages(self):
        """метод total_pages, который на вход принимает объект, а возвращает значение типа int. \
        происходит суммирование страниц всех книг и возвращение этого значения"""

        pages = [self[books].pages for books in range(len(self))]
        return sum(pages)

    def print_count(self):
        """метод print_count, который на вход принимает объект и ничгео не возвращает (None). \
        Вывод количество книг"""

        print(len(self))


class NewspaperList(list):
    """класс NewspaperList, который наследуется от list"""

    def __init__(self, name):
        """создание объектов класса NewspaperList"""

        super().__init__()
        self.name = name

    def extend(self, p_objects):
        """метод extend, который на вход принимает объект и ничего не возвращает (None). \
        происходит проверка на принадлежность объекта к типу list, после добавление в конец списка. \
        в случае, если на вход подан не список, то выбрасывается ошибка"""

        if isinstance(p_objects, list):
            objects = [item for item in p_objects if isinstance(item, Newspaper)]
            super().extend(objects)
        else:
            raise TypeError(f'Invalid type {type(p_objects)}')

    def print_age(self):
        """метод print_age, который принимает на вход объект и ничего не возвращает (None). \
        находиит минимальное возрастное ограничение и выводит его"""

        ages = [self[newspaper].age_limit for newspaper in range(len(self))]
        print(min(ages))

    def print_total_price(self):
        """метод print_total_price, который принимает на вход объект и ничего не возвращает (None). \
        происходит суммирование всех значений price и вывод их на экран"""

        prices = [self[newspaper].price for newspaper in range(len(self))]
        print(sum(prices))