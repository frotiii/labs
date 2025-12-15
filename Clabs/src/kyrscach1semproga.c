#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/////объявление///////
bool cmp(char* predl1, char* predl2);
void clear_text(char** text, size_t sentence_count);
////////////

// BASE BASE BASE BASE
void informations(){
    printf("Course work for option 4.4, created by Khvedynich Varvara.\n");
}

void help(){
    puts("Commands list:");
    puts("1 - Find all dates and display them on the screen in ascending order");
    puts("2 - Deleting sentences with an odd number of words");
    puts("3 - Convert all words with no numbers in them so that all letters except the last one are capitalized");
    puts("4 - print sentences without capital letters");
    puts("5 - output help about the functions");
}
// BASE BASE BASE BASE


//ВВОД И ВЫВОД ТЕКСТА, СТРУКТУРЫ И ВСЕ ФУНКЦИИ ДЛЯ НИХ, УДАЛЕНИЕ ДУБЛИКАТОВ 

char** read_text(){
    size_t sentence_count = 0;
    size_t capacity = 10; // Начальная емкость
    char** text = malloc(capacity * sizeof(char*));

    bool finish = false;
    while(!finish){
        char* sentence = malloc(1); // Выделяем 1 байт
        if (sentence == NULL) {
            puts("Error: Memory allocation error");
            return NULL;
        }
        size_t size = 0;
        bool flag = false;

        while(1){
            char c = getchar();
            if (c == EOF) { //Обработка EOF
                finish = true;
                break;
            }
            if(c == '.') {
                char* tmp = realloc(sentence, (size+2)*sizeof(char));
                if(tmp == NULL){
                    free(sentence);
                    clear_text(text, sentence_count);
                    return NULL;
                }
                sentence = tmp;
                sentence[size++] = c;
                sentence[size] = '\0';
                break;
            } 
            if(c == '\n'){
                if(flag){
                    finish = true;
                    break;
                }
                flag = true;
            } 
            else if (isspace(c) && size == 0) continue;
            else {
                flag = false;
                char* tmp = realloc(sentence, (size+1)*sizeof(char));
                if(tmp == NULL){
                    free(sentence);
                    clear_text(text, sentence_count);
                    return NULL;
                }
                sentence = tmp;
                sentence[size++] = c;
            }
        }

        if (!finish) {
            bool is_skipping = false;
            for (size_t i = 0; i < sentence_count; i++) {
                if (cmp(sentence, text[i])) {
                    free(sentence);
                    is_skipping = true;
                    break;
                }
            }
            if (is_skipping) continue;
            if (sentence_count >= capacity) { //Увеличение размера массива
                capacity *= 2;
                char** tmp = realloc(text, capacity * sizeof(char*));
                if (tmp == NULL) {
                    free(sentence);
                    clear_text(text, sentence_count);
                    return NULL;
                }
                text = tmp;
            }
            text[sentence_count++] = sentence;
        } else {
            free(sentence);
        }
    }
    return text;
}

void clear_text(char** text, size_t sentence_count){
    puts("Error: Memory allocation error");
    for (size_t i = 0; i < sentence_count; ++i) free(text[i]);
    free(text);

}

size_t line_count(char** text){
    int count = 0;
    for(int i = 0; text[i]; i++) count++;
    return count;
}

void output(char** text, size_t count){
    for(size_t i = 0; i < count; i++) puts(text[i]);

}

///////////////////////

/////// ВСПОМОГАЛКИ 

bool cmp(char* predl1, char* predl2){
    if(strlen(predl1) == strlen(predl2)){
        for(size_t i = 0; i < strlen(predl1); i++){
            if(tolower(predl1[i]) != tolower(predl2[i])) return false;
        }
        return true;
    }
    return false;
}

size_t get_word_count(char* sentence){
    char* sentence_copy = malloc((strlen(sentence) + 1)*sizeof(char)); // чтооыб не забыть \0
    strcpy(sentence_copy, sentence);
    size_t word_count = 0; 
    char* token = strtok(sentence_copy, " ,");
    while(token != NULL){
        word_count++;
        token = strtok(NULL, " ,");
    }
    free(sentence_copy);
    return word_count;
}

char** word_in_sentence(char* sentence){
    size_t count = get_word_count(sentence);
    char** word = malloc(count * sizeof(char*));
    char sentence_copy[strlen(sentence) + 1]; // чтооыб не забыть \0
    strcpy(sentence_copy, sentence);
    char* token = strtok(sentence_copy, " ,.");
    int index = 0;
    while(token != NULL) {
        char* token_copy = (char*) malloc(strlen(token) + 1 * sizeof(char));
        strcpy(token_copy, token);
        word[index++] = token_copy;
        token = strtok(NULL, " ,.");
    }
    return word;
}

////////////

/////ФУНКЦИЯ 1

int first_compare(const void* a, const void* b) {
    struct tm tm_a = *((struct tm*) a);
    struct tm tm_b = *((struct tm*) b);

    return mktime(&tm_a) - mktime(&tm_b); // mktime преобразовывает структуру в количество секунд, прошедших с 1 января 1970 года
}

void function1(char** text, size_t count){
    struct tm* dates = malloc(sizeof(struct tm));
    size_t dates_count = 0;
    time_t t = time(NULL); //получение времени в секундах с 1 января 70х
    for (size_t i = 0; i < count; i++) {
        int day, month, year;
        char* sentence = text[i];
        for (int k = 0; sentence[k]; k++) {
            if(sscanf(&sentence[k], "d%dm%dy%d", &day, &month, &year) != 3) continue; // должны получить 3 числа , если нет, то пропускаем
            
            struct tm* tm_gmt = gmtime(&t); // тут и ниже блок заполнения членов структуры в формате  GMT
            tm_gmt->tm_mday = day;
            tm_gmt->tm_mon = month - 1;
            tm_gmt->tm_year = year - 1900;
            dates = realloc(dates, (dates_count + 1) * sizeof(struct tm));
            memcpy(&dates[dates_count], tm_gmt, sizeof(struct tm)); // копирование структуры в массив dates
            dates_count++;
        }
    }

    qsort(dates, dates_count, sizeof(struct tm), first_compare);

    for (size_t i = 0; i < dates_count; i++) {
        char date[2000];
        if(!strftime(date, 2000 * sizeof(char), "%d:%m:%Y", &dates[i])) continue; // strftime -- извлечение из структуры значений даты в массив date по маске. последнее - указатель на саму дату в структуре 
        printf("%s\n", date);
    }
    free(dates);
}

////////////

/////ФУНКЦИЯ 2

void function2(char** text, size_t count){
    for(size_t i = 0; i < count; i++){
        if(get_word_count(text[i])%2 == 0) puts(text[i]);
        free(text[i]);	
    }
}

/////////////

/////ФУНКЦИЯ 3

bool is_digit(char* word){
    size_t len = strlen(word);
    bool flag = false;
    for(size_t i = 0; i < len; i++){
        if(isdigit(word[i])){
            flag = true;
            break;
        }
    }
    return flag;
}

void function3(char** text, size_t count){
    for(size_t i = 0; i < count; i++){
        char* sentence = text[i];

        char** word = word_in_sentence(sentence);
        size_t count_word = get_word_count(sentence);

        for(size_t k = 0; k < count_word; k++){
            char* this_word = word[k];
            if(!is_digit(word[k])){
                size_t len_word = strlen(this_word);
                for(size_t l = 0; l < len_word-1; l++){
                    this_word[l] = toupper(this_word[l]);
                }
                this_word[len_word - 1] = tolower(this_word[len_word - 1]);
            }   
            
            if(k == count_word - 1) {
                printf("%s.\n", this_word);
                continue;
            }
            printf("%s ", this_word);
            free(this_word);
        }
        free(word);
    }
}


/////////////

/////ФУНКЦИЯ 4

bool is_upper(char* sentence){
    size_t len = strlen(sentence);
    bool flag = false;
    for(size_t i = 0; i < len; i++){
        if(isupper(sentence[i])){
            flag = true;
            break;
        }
    }
    return flag;
}

void function4(char** text, size_t count){
    for(size_t i = 0; i < count; i++){
        if(!is_upper(text[i])) puts(text[i]);
    }
}

/////////////

void fun9(){

}


void fun8(char** text, size_t count, char* sent){
    if(count%2 == 0){
        for(size_t i = 0; i < count/2; i++){
            char* sentence1 = text[i];
            char* sentence2 = text[count - i - 1];
            size_t len1 = strlen(sentence1);
            char new_sent1[len1-2];
            for(size_t k = 0; k < len1 - 1; k++){
                new_sent1[k] = sentence1[k];
            }
            printf("%s %s %s\n", new_sent1, sent,  sentence2);
        }

    } else{
        for(size_t i = 0; i < (count/2) + 1; i++){
            char* sentence1 = text[i];
            char* sentence2 = text[count - i - 1];
            size_t len1 = strlen(sentence1);
            char new_sent1[len1-2];
            for(size_t k = 0; k < len1 - 1; k++){
                new_sent1[k] = sentence1[k];
            }
            printf("%s %s %s\n", new_sent1, sent,  sentence2);


        }
    }
}

////////////


int main(){
    informations();
    int command;
    if(scanf("%d", &command) != 1) puts("Error: bad function number"); 
    char c;
    while ((c = getchar()) != '\n') continue; //очистка буфера ввода 

    if(command == 5){
        help();
        return 0;
    } 
    if(command == 8){
        char buf[101];
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;
    }
    char buf[101];
    char** text = read_text(); 
    size_t count = line_count(text);


    switch(command){
        case 0:
            output(text, count);
            break;
        case 1:
            function1(text, count);
            break;
        case 2:
            function2(text, count);
            break;
        case 3:
            function3(text, count);
            break;
        case 4:
            function4(text, count);
            break;
        case 8:
            fun8(text, count, buf);
            break;
        case 9:
            fun9();
            break;
        case 5:
            help();
            break;
        default:
            puts("Error: bad function number");

    }

}

/////////
