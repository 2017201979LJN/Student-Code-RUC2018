#include <stdio.h>

#include "splay.h"

int getint()
{
    char ch;int b = 1;
    for(ch = getchar(); ch >  '9' || ch < '0'; ch = getchar() )
        if(ch == '-')
            b = -1;
    int num = ch - '0';
    for(ch = getchar(); ch >= '0' && ch <= '9'; ch = getchar() )
        num = num * 10 + ch - '0';
    return num*b; 
} 

int main()
{
    splay_init();
    int n = getint();
    while (n--) {
        int v1 = getint(), v2 = getint();
        switch(v1)
        {
            case 0:
                splay_insert(v2);
                break;
            case 1:
                splay_erase(v2);
                break;
            case 2:
                printf("%d\n", splay_kth(v2));
                break;
            case 3:
                printf("%d\n", splay_getrank(v2));
                break;
            case 4:
                printf("%d\n", splay_getpre(v2));
                break;
            case 5:
                printf("%d\n", splay_getpost(v2));
                break;
        }
    }
    splay_gc();
    return 0;
}
