#include <stdio.h>
#include <stdlib.h>

void my_swap(void *a, void *b, size_t size){
    char *pa = (char *)a,
         *pb = (char *)b,
         temp;
    while(size--){
        temp = *pa;
        *pa = *pb;
        *pb = temp;
        pa++;
        pb++;
    }
}

void my_qsort(void* base,
             size_t num,
             size_t size,
             int (*compar)(const void*, const void*)){

    if(num == 1 || num == 0) return;

    int key = rand() % num;
    my_swap(base, base + key * size, size);


    int l = 0,
        r = num - 1;
    while(l < r){

        while(l < r && compar(base + l * size, base + r * size) < 0)
            r--;
        if(l < r){
            my_swap(base + l * size, base + r * size, size);
            l++;
        } 
        
        while(l < r && compar(base + l * size, base + r * size) < 0)
            l++;
        if(l < r){
            my_swap(base + l * size, base + r * size, size);
            r--;
        }
    }
    
    // [0 ~ l-1] (l=r) [r+1 ~ num-1] 
    my_qsort(base, l, size, compar);
    my_qsort(base + (r + 1) * size, num - r - 1, size, compar);
    return ;
}


int my_int_compare(const void *a, const void *b){
    return *(int *)a - *(int *)b;
}

int main(){
    int n;
    scanf("%d", &n);

    int a[n], i;
    for(i = 0; i < n; i++)
        scanf("%d", a + i);

    my_qsort(a, n, sizeof(int), my_int_compare);

    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");

/*
    //check answer
    int flag = 1;
    for(i = 1; i < n; i++)
        if(a[i-1] > a[i]) flag = 0;
    printf(flag?"Correct\n":"Wrong Answer\n");
    
*/

}
