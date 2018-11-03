#include <stdio.h>
#include <string.h>

#define REG_LEN 5
#define ORDER_LEN 10
#define EXP_LEN 20
#define SIZE_OF_CHAR 260
#define MAX_NUM 101
#define STR_LEN 1000
#define BUF_SIZE 100
#define WORD_LEN 20
#define SIZE_OF_BYTES 8

#define swap(a,b) (a) ^= (b) ^= (a) ^= (b)

enum {
    ADD = '+',
    MULTIPLY = '*',
    ENTER = '\n',
    EQUAL = '='
};

struct trie_node {
    int ch[SIZE_OF_CHAR], index;
};

char RBP[REG_LEN] = "%rbp", RSP[REG_LEN] = "%rsp", RAX[REG_LEN] = "%rax";
char PUSH[ORDER_LEN] = "pushq   ", POP[ORDER_LEN] = "popq    ", MOV[ORDER_LEN] = "movq    ", ADDQ[ORDER_LEN] = "addq    ", IMUL[ORDER_LEN] = "imulq   ";

void push_stack()
{
    printf ("%s%s\n", PUSH, RBP);
}

void pop_stack()
{
    printf ("%s%s\n", POP, RBP);
}

void init_reg()
{
    push_stack();
    printf ("%s%s, %s\n", MOV, RSP, RBP);
}

char buf[BUF_SIZE];
int buf_top=0;

char get_ch()
{
    return (buf_top > 0) ? buf[--buf_top] : getchar();
}

void push_ch (char ch)
{
    buf[buf_top++] = ch;
}

struct trie_node trie[STR_LEN];
int node_num = -1, word_num = 0;

int newnode()
{
    node_num++;
    for (int i = 0; i < SIZE_OF_CHAR; i++) {
        trie[node_num].ch[i] = 0;
    }
    trie[node_num].index = 0;
    return node_num;
}

void init_trie()
{
    newnode();
}

int get_index (char *str, int cur_node, int cur_char, int is_num, int len)
{
    if (cur_char == len) {
        if (is_num) {
            int v = 0;
            for (int i = 0; i < len; i++)
                v = v * 10 + str[i] - '0';
            return v;
        }
        if (!trie[cur_node].index)
            trie[cur_node].index = ++word_num;
        return trie[cur_node].index + MAX_NUM;
    }
    char ch = str[cur_char];
    if (ch < '0' || ch > '9')
        is_num = 0;
    if (!trie[cur_node].ch[ch])
        trie[cur_node].ch[ch] = newnode();
    return get_index (str, trie[cur_node].ch[ch], cur_char + 1, is_num, len);
}

void trans_express (char *express)
{
    char ch, str[WORD_LEN];
    int str_len = 0, oper, var = 0, var1 = 0, var2 = 0;
    memset(str, 0 ,sizeof(str) );
    for (int i = 0; i < strlen (express); i++) {
        ch = express[i];
        if (ch == EQUAL) {
            var = get_index (str, 0, 0, 1, str_len) - MAX_NUM;
            memset (str, 0, sizeof(str) );
            str_len = 0;
        }
        else if (ch == ADD || ch == MULTIPLY || ch == ENTER) {
            if (ch == ADD)
                oper = 1;
            else if (ch == MULTIPLY)
                oper = 2;
            if (!var1)
                var1 = get_index (str, 0, 0, 1, str_len);
            else
                var2 = get_index (str, 0, 0, 1, str_len);
            memset (str, 0, sizeof(str) );
            str_len = 0;
            if (ch == ENTER)
                break;
        }
        else if (ch != ' ' && ch != '\t')
            str[str_len++] = ch;
    }
    if (!var2) {
        if (var1 < MAX_NUM)
            printf ("%s$%d, -%d(%s)\n", MOV, var1, SIZE_OF_BYTES * var, RBP);
        else
            printf ("%s-%d(%s), -%d(%s)\n", MOV, SIZE_OF_BYTES * (var1 - MAX_NUM), RBP, SIZE_OF_BYTES * var, RBP);
        return;
    }
    if (var1 - MAX_NUM == var)
        swap(var1, var2);
    if (var2 - MAX_NUM == var) {
        if (var1 < MAX_NUM)
            printf ("$%d, -%d(%s)\n", oper == 1 ? ADDQ : IMUL, var1, SIZE_OF_BYTES * var, RBP);
        else
            printf ("%s-%d(%s), -%d(%s)\n", oper == 1 ? ADDQ : IMUL, SIZE_OF_BYTES * (var1 - MAX_NUM), RBP, SIZE_OF_BYTES * var, RBP);
        return;
    }
    if (var2 < MAX_NUM)
        swap(var1, var2);
    var2 -= MAX_NUM;
    printf ("%s-%d(%s), %s\n", MOV, SIZE_OF_BYTES * var2, RBP, RAX);
    if (var1 > MAX_NUM) {
        var1 -= MAX_NUM;
        printf ("%s-%d(%s), %s\n", oper == 1 ? ADDQ : IMUL, SIZE_OF_BYTES * var1, RBP, RAX);
    }
    else
        printf ("%s$%d, %s\n", oper == 1 ? ADDQ : IMUL, var1, RAX);
    printf ("%s%s, -%d(%s)\n", MOV, RAX, SIZE_OF_BYTES * var, RBP);
}

int main()
{
    init_reg();
    char express[EXP_LEN];
    init_trie();
    while (~scanf ("%s", express) ) {
        express[strlen(express)] = '\n';
        trans_express (express);
        memset (express, 0, sizeof(express) );
    }
    pop_stack();
    return 0;
}
