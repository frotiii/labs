//Создаёт новый список и возврщает на него указатель. Список должен быть пустым
struct ListStruct *list_init() { 
    struct ListStruct* list = (struct ListStruct*)malloc(sizeof(struct ListStruct));
    if(list == NULL){
        printf("memory allocation error for list");
        exit(1);
    }
    list->head = NULL;
    return list;
}

//Очищает список list и всего его элементы
void list_destroy(struct ListStruct *list) {

    if(list_is_empty(list) == false){
        struct ListNode* new = list->head;
        struct ListNode* tmp;
        do{
            tmp = new->next;
            free(new);
            new = tmp;
        }while(list->head != new);
    }else return;
    free(list);
}

//Добавляет новый элемент в начало списка
void list_push_front(struct ListStruct *list, int data) {
    struct ListNode* new_list = (struct ListNode*)malloc(sizeof(struct ListNode));
    if(new_list == NULL){
        printf("memory allocation error for list");
        exit(1);
    }
    new_list->data = data;

    if(list_is_empty(list) == true){
        new_list->next = new_list;
        new_list->prev = new_list;
        list->head = new_list;  
    } else{
        new_list->next = list->head;
        new_list->prev = list->head->prev;
        list->head->prev->next = new_list;
        list->head->prev = new_list;
        list->head = new_list;
    }
}

//Добавляет новый элемент в конец списка
void list_push_back(struct ListStruct *list, int data) {
    struct ListNode* new_list = (struct ListNode*)malloc(sizeof(struct ListNode));
    if(new_list == NULL){
        printf("memory allocation error for list");
        exit(1);
    }
    new_list->data = data;

    if(list_is_empty(list) == true){
        new_list->next = new_list;
        new_list->prev = new_list;
        list->head = new_list;  
    } else{
        new_list->next = list->head;
        new_list->prev = list->head->prev;
        list->head->prev->next = new_list;
        list->head->prev = new_list;
    }
    
}

//Добавляет новый элемент после переданного
void list_push(struct ListStruct *list, struct ListNode *prev_node, int data) {
    if(prev_node == NULL) list_push_back(list, data);
    else{
        struct ListNode* tmp = (struct ListNode*)malloc(sizeof(struct ListNode));
        if(tmp == NULL){
            printf("memory allocation error for list");
            exit(1);
        }
        tmp->data = data;
        tmp->next = prev_node->next;
        tmp->prev = prev_node;
        prev_node->next->prev = tmp;
        prev_node->next = tmp;
    }
}

//Удаляет элемент из списка
void list_pop(struct ListStruct *list, struct ListNode *node_to_remove) {

    if(node_to_remove != NULL && list_is_empty(list) == false){

        if(node_to_remove->next == node_to_remove) list->head = NULL;
        else{
            node_to_remove->prev->next = node_to_remove->next;
            node_to_remove->next->prev = node_to_remove->prev;

            if(node_to_remove == list->head) list->head = node_to_remove->next;
            
        }

    }else return;

}

//Подсчитывает количество элементов в списке
int list_count(struct ListStruct *list) {

    if(list_is_empty(list) == true) return 0;
    

    int count = 0;
    struct ListNode* item = list->head;
    
    do{
        count++;
        item = item->next;
    } while(item != list->head);
    
    return count;
}

//Возвращает указатель на элемент под индексом index 
struct ListNode *list_get(struct ListStruct *list, int index) {
    if(list_is_empty(list) == false && index < list_count(list) && index >= 0){
        struct ListNode* tmp = list->head;
        int i = 0;
        while(i < index){
            tmp = tmp->next;
            i++;
        }
        return tmp;
    }else return NULL; 
}

//Возвращает указатель на первый элемент в списке
struct ListNode *list_get_head(struct ListStruct *list) {
    if(list_is_empty(list) == true) return NULL;
    else return list->head;
}

//Возвращает указатель на последний элемент в списке
struct ListNode *list_get_tail(struct ListStruct *list) {
    if(list_is_empty(list) == true) return NULL;
    else return list->head->prev;
}

//Проверяет пуст ли  список или нет
bool list_is_empty(struct ListStruct *list) {
    return list->head == NULL;
}

//Сортирует список по возрастанию значения поля data в элементах
void list_sort(struct ListStruct *list) {
    if(list_is_empty(list) == false){

        int swap;
        do{
            swap = 0;
            struct ListNode* tmp = list->head;

            do{
                struct ListNode* new = tmp->next;

                if(tmp->data > new->data){
                    struct ListNode *tmp_prev = tmp->prev;
                    struct ListNode *new_next = new->next;

                    tmp->next = new_next;
                    tmp->prev = new;
                    new->next = tmp;
                    new->prev = tmp_prev;

                    tmp_prev->next = new;
                    new_next->prev = tmp;

                    if(tmp == list->head) list->head = new;
                    swap = 1;

                } else tmp = new;

            }while(tmp->next != list->head);

        }while(swap);

    }else return;
    
}