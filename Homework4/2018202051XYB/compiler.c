#include <stdlib.h>
#include <stdio.h>

char MY_REGISTER[15][5] = {
    "%rdi", "%rsi", "%rdx", "%rcx", "%r8",
    "%r9", "%r10", "%r11", "%r12", "%r13", "%r14",
    "%rbx"
};

void read_name(){
    char c = getchar();
    while(c != '('){
        c = getchar();
    }
    return ;
}

int read_parm(){
    int ret = 0,
        flag = 0;
    char c = getchar();
    while(c != '{'){
        if(flag == 0 && c >= 'a' && c <= 'z') 
            flag = 1;
        if(c == ',') 
            ret++;
        c = getchar();
    }
    return ret + flag;
}

void set(char c, int* x1, int* x2, int* x3){
    if(*x1 == -1)  *x1 = c - 'a';
    else if(*x2 == -1) *x2 = c - 'a';
    else *x3 = c - 'a';
}

long long read_num(char c){
    long long ret = 0ll,
              sgn = 1;
    if(c == '-') sgn = -1;
    else ret = c - '0';
    
    c = getchar();
    while(c >= '0' && c <= '9'){
        ret = ret * 10 + c - '0';
        c = getchar();
    }
    return ret * sgn;
} 

void output_add(int has_num, int val, int x1, int x2, int x3){
   // printf("output_add %d %d   %c = %c + %c\n",
   //             has_num, val, 'a'+x1, 'a'+x2, 'a'+x3);

    char *c1 = MY_REGISTER[x1],
         *c2 = MY_REGISTER[x2],
         *c3 = MY_REGISTER[x3];
    
    if(x1 == x2 && x1 == x3){
        // a = a + a
        printf("\t\tsalq\t%s\n",c1);
        return ;
    }

    if(x1 == x2 || x1 == x3){
        if(x1 == x3) {
            char *cx = c2;
            c2 = c3;
            c3 = cx;
        }
        if(has_num)
            // a = a + 1
            printf("\t\taddq\t$%d, %s\n", val, c1);
        else 
            // a = a + b
            printf("\t\taddq\t%s, %s\n", c3, c1);
        return ;
    }
    
    if(x2 == x3){
        // a = b + b
        printf("\t\tmovq\t%s, %s\n", c2, c1);
        printf("\t\tsalq\t%s\n",c1);
        return ;
    }

    if(has_num) {
        // a = b + 1
        printf("\t\tmovq\t%s, %s\n", c2, c1);
        printf("\t\taddq\t$%d, %s\n",val, c1);
        return ;
    }
    
    printf("\t\tmovq\t%s, %s\n", c2, c1);
    printf("\t\taddq\t%s, %s\n", c3, c1);
    return ;
}


void output_mul(int has_num, int val, int x1, int x2, int x3){
  //   printf("output_mul %d %d   %c = %c * %c\n",
   //             has_num, val, 'a'+x1, 'a'+x2, 'a'+x3);

    char *c1 = MY_REGISTER[x1],
         *c2 = MY_REGISTER[x2],
         *c3 = MY_REGISTER[x3];
    
    if(x1 == x2 && x1 == x3){
        // a = a * a
        printf("\t\tmovq\t%s, %%rax\n", c1);
        printf("\t\timulq   %s, %%rax\n", c1);
        printf("\t\tmovq\t%%rax, %s\n", c1);
        return ;
    }

    if(x1 == x2 || x1 == x3){
        if(x1 == x3) {
            char *cx = c2;
            c2 = c3;
            c3 = cx;
        }
        if(has_num){
            if(val == 1) 
                return;
            // a = a * 1
            printf("\t\timulq   $%d, %s\n", val, c1);
        }
        else 
            // a = a * b
            printf("\t\timulq   %s, %s\n", c3, c1);
        return ;
    }
    
    if(has_num) {
        // a = b * 1
        printf("\t\tmovq\t%s, %s\n", c2, c1);
        printf("\t\timulq   $%d, %s\n",val, c1);
        return ;
    }
    
    printf("\t\tmovq\t%s, %s\n", c2, c1);
    printf("\t\timulq   %s, %s\n", c3, c1);
    return ;
}


void read_expression(){
    char c = getchar();
    int val = 0, x1 = -1, x2 = -1, x3 = -1, opt = -1; 
    int has_num = 0;

    while(c != 'r'){
        if(c >= 'a' && c <= 'e'){
            set(c, &x1, &x2, &x3);
        }
        if(c == '+') opt = 1;
        if(c == '*') opt = 2;
        
        if(c == '-' || (c >= '0' && c <='9')){
            has_num = 1;
            val = read_num(c);
        }
        
        if(c == ';'){
            if(opt == 2) output_mul(has_num, val, x1, x2, x3);
            if(opt == 1) output_add(has_num, val, x1, x2, x3);
            val = 0;
            x1 = x2 = x3 = -1;
            opt = -1;
            has_num = 0;
        }
      //  if(c == '=' ) printf("=================== read\n");
        c = getchar();
    }

    while(c != 'n') c = getchar();
    while(c < 'a' || c > 'e') c = getchar();
    printf("\t\tmovq\t%s, %%rax\n", MY_REGISTER[c-'a']);

}

int main(){
    freopen("data.c","r",stdin);
    read_name();
    int parm_num = read_parm();
    read_expression();

}                
