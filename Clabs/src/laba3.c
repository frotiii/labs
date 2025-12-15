#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h" // для isspace(проверяет символы табуляции и пробела) и isupper(колво заглавных букв)

int is_fin(char ch) { //функция для проверки на конец предложения
     if (ch == '.'  ch == '?'  ch == ';'  ch == '!') {
        return 1;
    }
    return 0;
}

char* get_sentence() { //считать и занести в массивчик
    char *sentence = malloc(0);
    int size = 0;

    while(1) {
        char ch = getchar();

        if (isspace(ch) && size == 0){//isspace--проверка на символ пробела
            continue;
        }

        if (ch == '\0'  ch == '\n'){
            continue;
        }

        if (is_fin(ch)) {
            char *tmp;

            tmp = realloc(sentence, (size+2)*sizeof(char));
            size+=2;

            if (tmp == NULL) {//првоерка
                free(sentence);
                return NULL;
            }

            sentence = tmp;

            sentence[size-2] = ch;
            sentence[size-1] = '\0';

            return sentence;
        }
        else {
            char *tmp;

            tmp = realloc(sentence, (size+1)*sizeof(char));
            ++size; 

            if (tmp == NULL) {//првоерка
                free(sentence);
                return NULL;
            }

             sentence = tmp;

            sentence[size-1] = ch;
        }
    }
}

int to_kill(char *str) { //для полдсчета колва букв заглавных в строкаах
    int count = 0;
    
    char *ch = str;

    while (*ch != '\0') {
        if (isupper(*ch)){//isupper -- возвр 1 елси буква загл
            ++count;
        }

        ++ch;
    }

    if(count>1){
        return 1;
    }else{
        return 0;
    };
}

int main() {
    char **lines = malloc(0);
    int size = 0;
    int true_size = 0;

    while (1) {
        char* sen = get_sentence();

        if (sen == NULL) {//првоерка
            for (int i = 0; i < size; ++i) free(lines[i]);
            free(lines);

        }

        if (to_kill(sen)) {
            free(sen);
            ++true_size;
            continue;
        }

        char **tmp = realloc(lines, (size+1)*sizeof(char*));
        ++size;
        ++true_size;

        if (tmp == NULL) {//првоерка
            for (int i = 0; i < size; ++i) free(lines[i]);
            free(lines);
            free(sen);
        }

        lines = tmp;

        lines[size-1] = sen;

        if (strcmp(sen, "Dragon flew away!") == 0) {
            break;
        }
    }

    for (int i = 0; i < size; ++i) {
        puts(lines[i]);
    }

    printf("Количество предложений до %d и количество предложений после %d\n", true_size-1, size-1);

    return 0;
}
