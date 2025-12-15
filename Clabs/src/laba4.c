#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int cmp(const void* a, const void* b){
    return abs(*(int*)b) - abs(*(int*)a);
}


int main(){
    int arr[1000];
    for(int i = 0; i < 1000; i++) scanf("%d ", &arr[i]);
    
    clock_t start = clock();
    qsort(arr, 1000, sizeof(int), cmp);
    clock_t end = clock();
    
    for(int i = 0; i < 1000; i++) printf("%d ", arr[i]);
    printf("\n%ld", end - start);
    
}
