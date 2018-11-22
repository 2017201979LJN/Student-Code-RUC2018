#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_OF_STUDENT 1000
#define NUM_OF_NAME 100


struct dlink{
    char *name;
    int score;
    struct dlink *pre, *suc;
};


// create a name in dlink
// use ad dlink.name = new_name(name_src);
char* new_name(char *p){
    int num = strlen(p);
    char *ret = malloc(num),
         *q = ret;
    while(*p != '\0'){
        *q = *p;
        p++;
        q++;
    }
    return ret;
}

// create dlink of n without name and score
void dlink_build(struct dlink *p, int n){
    int i;
    for(i = 0; i < n-1; i++){
        (p + i)->suc = p + i + 1;
    }
    (p+ n - 1)->suc = p;

    for(i = 1; i < n; i++){
        (p + i)->pre = p + i - 1;
    }
    p->pre = p + n - 1;
}

// evaluate name and score
void dlink_init(struct dlink *p, char *name, int score){
    p->name = new_name(name);
    p->score = score;
}

// output name and score of p
void dlink_output(struct dlink *p){
    printf("%10s %3d\n", p->name, p->score);
}


// delete one dlink
void dlink_delete(struct dlink *p){
    p->pre->suc = p->suc;
    p->suc->pre = p->pre;
    dlink_output(p);
    free(p->name);
}

// compare 2 dlink
int dlink_cmp(struct dlink *p, struct dlink *q){
    if(p->score == q->score) 
        return strcmp(p->name, q->name) < 0;
    return p->score < q->score;
}

// swap 2 dlink
void dlink_swap(struct dlink *p, struct dlink *q){
    char *tmp = p->name;
    p->name = q->name;
    q->name = tmp;
    int t = p->score;
    p->score = q->score;
    q->score = t;
}

// output the whole dlink  (begin with *head)
void output(struct dlink *head){
    struct dlink *t = head;
    do{
        dlink_output(t);
        t = t->suc;
    }while(t != head);
}


// sort the dlink
void bubble_sort(struct dlink *head, struct dlink *tail, int num){
    if(head == tail) 
        return; 
    struct dlink *p, *q;
    int i;
    for(i = 0; i < num; i++){
        for(p = head, q = p->suc; p != tail; p = q, q = q->suc){
            if(dlink_cmp(p, q) == 0)
                dlink_swap(p, q);
        }
    }
}



int main()
{
    int n;
    scanf("%d", &n);
    
    struct dlink pool[NUM_OF_STUDENT],
                 *head = pool,
                 *tmp = pool;

    dlink_build(pool, n);
    int i;
    for(i = 0; i < n; i++){
        char c[NUM_OF_NAME];
        int score;
        scanf("%s%d", c, &score);
        dlink_init(tmp, c, score);
        tmp++;
    }    
    
    bubble_sort(head, head + n - 1, n);
    
    output(head);

}
