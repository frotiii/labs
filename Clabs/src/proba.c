#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUF1 10
#define BUF 5

char* read_text_odn(){
	char* input_text = calloc(BUF1, sizeof(char)); // создали массив размера 10 с элементами инициализированными нулями
	char c; // создали переменную в которой будем хранить текущий считанный символ
	size_t tmp_size = 0; // временный размер
	size_t size = BUF1; // постоянный, конечный размер, котрый офк увеличится, мы просто стартуем с 10

	while( (c = (char)getchar() ) != EOF){
		if(tmp_size >= size){ //если текущий размер памяти больше или равен постоянному ( так скажем ), то мы преевыделяем память и увеличиваем ее 
			size += BUF1;
			input_text = (char*)realloc(input_text, size*sizeof(char));
		}
		input_text[tmp_size] = c;
		tmp_size++;
	}
	
	return input_text;


}

char** read_text_dv(char* text, const char* delims, int* n){ //тоесть мсы сначала считаем цельным куском весь текст с помощью 1 функции, после передадим его в текущую функцию и уже в ней будем его расщиплять , вторая перемнная, которую я подаю, это массив РАЗДЕЛИТЕЛЕЙ, по которым мы и бдуем расщиплять 
	char** sents = calloc(BUF, sizeof(char*));
	size_t i = 0;
	size_t size = BUF;
	char* tok = strtok(text, delims);
	while(tok){
		if(i >= size){ 
                        size += BUF;
                        sents = (char**)realloc(sents, size*sizeof(char*));
                }
		sents[i] = calloc(strlen(tok), sizeof(char));
		strncpy(sents[i], tok, strlen(tok));
		i++;
		tok = strtok(NULL, delims);
	}
	*n = i;

	return sents;

}



int main(){
	char* text = read_text_odn(); //считывание ожномерным массивом 	
	int n = 0; //на выходе в n будет лежать колво строк в двумерном массиве
	char** sents = read_text_dv(text, ".;?!", &n); //считывание в ДВУМЕРНЫЙ массив
	//printf("[%s]\n", text);
	//вывод по предложениям отдельно:
	for(size_t i = 0; i < n; i++){
		printf("[%s]\n", sents[i]);
	}

	return 0;
}
