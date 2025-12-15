#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>


typedef struct{ //структура для хранения массива из строк. стоки - сами директории
  char** data; // сам массив , который хранит пути к файлам разрешения txt
  size_t size;
  size_t capacity;
}Array;

void init_array(Array* arr, size_t start_capacity);
void push_to_arr(Array* arr, const char* str);
void free_array(Array* arr);
char *pathcat(const char* path1, const char* path2);
void list_dir(const char* dir_name, Array* arr);
void find_match(const Array* arr, const char* word, FILE* output);
bool is_valid_filemane(const char* filename);

void init_array(Array* arr, size_t start_capacity){
  arr->data = malloc(start_capacity*sizeof(char*));
  arr->size = 0;
  arr->capacity = start_capacity;
}

void push_to_arr(Array* arr, const char* str){
  if(arr->size >= arr->capacity){
    arr->capacity *= 2;
    arr->data = realloc(arr->data, arr->capacity*sizeof(char*));
  }
  arr->data[arr->size] = strdup(str);
  arr->size++;
}

void free_array(Array* arr){
  for(int i = 0; i < arr->size; i++) free(arr->data[i]);
  free(arr->data);
  arr->size = 0;
  arr->capacity = 0;
}

char *pathcat(const char* path1, const char* path2){
  if(!path1 || !path2) return NULL;
  char* result = malloc(strlen(path2) + strlen(path1) + 2);
  snprintf(result, strlen(path2) + strlen(path1) + 2, "%s/%s", path1, path2);
  return result;
}

void list_dir(const char* dir_name, Array* arr){
  DIR *dir = opendir(dir_name);
  if(!dir){
    printf("Failed to open %s directory\n", dir_name);
    return;
  }

  struct dirent *de;
  while((de = readdir(dir)) != NULL){
    if(de->d_type == DT_REG && is_valid_filemane(de->d_name)){
      char* full = pathcat(dir_name, de->d_name);
      push_to_arr(arr, full);
      free(full);
    }else if(de->d_type == DT_DIR && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0){
      char* subdir = pathcat(dir_name, de->d_name);
      list_dir(subdir, arr);
      free(subdir);
    }

  }
  closedir(dir);
}

void find_match(const Array* arr, const char* word, FILE* output) {
    bool used_files[arr->size];
    memset(used_files, 0, sizeof(used_files));

    for(size_t i = 0; word[i] != '\0'; i++) {
        bool found = false;
        
        for(size_t j = 0; j < arr->size; j++) {
            if(used_files[j]) continue;
            
            const char* path = arr->data[j];
            const char* filename = strrchr(path, '/') + 1;
            
            if(filename[0] == word[i]) {
                fprintf(output, "./%s\n", path + 4);  
                used_files[j] = true;
                found = true;
                break;
            }
        }
        
        if(!found) {
            return;
        }
    }
}

bool is_valid_filemane(const char* filename){
  const char* dot = strrchr(filename, '.');
  if(!dot || strcmp(dot, ".txt") != 0) return false;

  size_t len = dot - filename;
  if(len != 1) return false;

  return isalpha(filename[0]);
}

int main(){
  char word[100];
  scanf("%99s", word);
  Array files;
  init_array(&files, 10);
  list_dir("tmp", &files);

  FILE* output = fopen("result.txt", "w");

  find_match(&files, word, output);

  fclose(output);


  free_array(&files);
  return 0;
}