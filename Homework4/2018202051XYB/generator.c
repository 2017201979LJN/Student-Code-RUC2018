#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int rand_number(int l, int r){
    return rand() % (r - l + 1) + l;
}

int main(int argc, char** argv)
{
    freopen("data.c", "w", stdout);
    srand(time(NULL));
       
    int n = rand_number(1, 5),
        m = rand_number(n, 5 * n),
        i;

    char c[10] = {' ', 'a', 'b', 'c', 'd', 'e', '+', '*'};
    
    printf("long long cal(");
    for(i = 1; i < n; i++)
        printf("long long %c, ", c[i]);
    printf("long long %c){\n", c[n]);
    
    for(i = 1; i < m; i++){
        char c1 = c[rand_number(1, n)],
             c2 = c[rand_number(1, n)],
             c3 = c[rand_number(1, n)],
             opt = c[rand_number(6, 7)];
        int num = rand_number(1, 50),
            mode = rand_number(1, 3);

        if(mode == 1){
            printf("\t%c = %c %c %c ;\n", c1, c2, opt, c3);
        }
        if(mode == 2){
            printf("\t%c = %c %c %d ;\n", c1, c2, opt, num);
        }
        if(mode == 3){
            printf("\t%c = %d %c %c ;\n", c1, num, opt, c2);
        }

    }
    
    char ret = c[rand_number(1, n)];
    printf("\treturn %c ;\n}", ret);
}
