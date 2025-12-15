#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define SIZE 1000


int main(){

    const char* pattern = "(\\w+)@[a-zA-Z0-9_-]+: ?~ ?# (.+)";
    regex_t rg;
    regcomp(&rg, pattern, REG_EXTENDED);
    regmatch_t pmatch[3];
    
    while(1){
        char line[SIZE];
        fgets(line, SIZE, stdin);
        if(strcmp(line, "Fin.")) {
            int i = regexec(&rg, line, 3, pmatch, 0);
            if(i == 0){
                size_t len_us = pmatch[1].rm_eo - pmatch[1].rm_so; // eo - индекс последнего символа в метче 1, а метч 1 это юзер нейм . so - индекс первого символа. аналогично с ком
                size_t len_co = pmatch[2].rm_eo - pmatch[2].rm_so;
                // вот так была получена длина 

                char *user = malloc(len_us+1); //для еще символа окончания строки
                char *command = malloc(len_co+1);

                memcpy(user, line+pmatch[1].rm_so, len_us);
                user[len_us] = '\0';
                memcpy(command, line+pmatch[2].rm_so, len_co);
                command[len_co] = '\0';

                printf("%s - %s", user, command);
                
                free(user);
                free(command);
                
            }
            
        }
        
        else break;
    }
    
    regfree(&rg);
    return 0;
    
}