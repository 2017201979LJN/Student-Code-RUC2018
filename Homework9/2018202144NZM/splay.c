#include <stdlib.h>

#include "splay.h"

#ifndef SPLAY_MAX_NUM
#define SPLAY_MAX_NUM 1e9
#endif

#ifndef min
#define min(a,b) (a)<(b)?(a):(b)
#endif

#ifndef max
#define max(a,b) (a)>(b)?(a):(b)
#endif

static splay_node *root,*null;

static inline void update(splay_node *x)
{
    x->size = x->ch[0]->size + x->ch[1]->size + x->cnt;
}

static inline int getch(splay_node *x)
{
    return x == x->pre->ch[0] ? 0 : 1;
}

static inline void setch(splay_node *x, splay_node *child, int wh)
{
    x->ch[wh] = child;
    child->pre = x;
    update(x);
}

void splay_init()
{
    null = (splay_node *)malloc(sizeof(splay_node));
    null->val = 0;
    null->size = 0;
    null->cnt = 0;
    null->pre = null->ch[0] = null->ch[1] = null;
    root = null;
}
 
static splay_node *newnode(int val)
{
    splay_node *now = (splay_node *)malloc(sizeof(splay_node));
    now->pre = now->ch[0] = now->ch[1] = null;
    now->cnt = now->size = 1;
    now->val = val;
    return now;
}
 
static void rotate(splay_node *now)
{
    splay_node *father = now->pre;
    splay_node *grand = father->pre;
    int wh = getch(now); 
    setch(father, now->ch[wh ^ 1], wh);
    setch(now, father, wh ^ 1);
    if (father == root)
        root = now;
    now->pre = grand;
    if (grand != null)
        grand->ch[father == grand->ch[0] ? 0 : 1] = now;
}
 
static void splay(splay_node *now, splay_node *tar)
{
    for (;now->pre != tar; rotate(now) ) {
        if(now->pre->pre != tar) {
            rotate(getch(now) == getch(now->pre) ? now->pre : now);
        }
    }
    if (tar == null)
        root = now;  
}
 
void splay_insert(int val)
{
    splay_node *now = root,*last = null;
    int ch = 0;
    while(now != null) {
        last = now;
        if(now->val == val) {
            now->cnt++;
            now->size++;
            splay(now, null);
            return;
        }
        int wh = now->val > val ? 0 : 1;
        ch = wh;
        now = now->ch[wh];
    }
    now = newnode(val);
    if(last != null) {
        setch(last, now, ch);
        splay(now, null);
    }
    else 
        root=now;
}
 
static splay_node *find(int val)
{
    splay_node *now = root;
    while(now != null) {
        if(now->val == val) {
            splay(now, null);
            return now;
        }
        int wh = val < now->val ? 0 : 1;
        now = now->ch[wh];
    }
    return null;
}
 
void splay_erase(int val)
{
    splay_node *now = find(val);
    if(now == null)
        return;
    if(now->cnt > 1) {
        now->cnt--;
        now->size--;
        return;
    }
    if(now->ch[0] == null && now->ch[1] == null) {
        root = null;
        free(now);
        return;
    }
    if(now->ch[0] == null) {
        root = now->ch[1];
        now->ch[1]->pre = null;
        free(now);
        return;
    }
    if(now->ch[1] == null) {
        root = now->ch[0];
        now->ch[0]->pre = null;
        free(now);
        return; 
    }
    splay_node *x = now->ch[0];
    while(x->ch[1] != null) {
        x = x->ch[1];
    }
    splay(x, now);
    setch(x, now->ch[1], 1);
    x->pre = null;
    root = x;
    free(now);
}
 
int splay_kth(int k)
{
    splay_node *now = root;
    while(now != null) {
        if(now->ch[0]->size + 1 <= k && now->ch[0]->size + now->cnt >= k)
            return now->val;
        if(now->ch[0]->size + 1 > k)
            now=now->ch[0];
        else {
            k -= now->ch[0]->size + now->cnt;
            now = now->ch[1];
        }
    }
    return -1;
}
 
int splay_getrank(int val)
{
    splay_node *now = root;
    int ans = 0;
    while(now != null) {
        if(val == now->val)
            return ans + now->ch[0]->size;
        if(val < now->val)
            now = now->ch[0];
        else {
            ans += now->ch[0]->size + now->cnt;
            now = now->ch[1];
        }
    }
    return ans;
}
 
int splay_getpre(int val)
{
    splay_node *now = root;
    int ans = -1;
    while(now != null) {
        if(now->val < val) {
            ans = max(now->val, ans);
            now = now->ch[1];
        }
        else
            now = now->ch[0];
    }
    return ans;
}
 
int splay_getpost(int val)
{
    splay_node *now = root;
    int ans = SPLAY_MAX_NUM;
    while (now != null) {
        if (now->val > val) {
            ans = min(ans, now->val);
            now = now->ch[0];
        }
        else
            now = now->ch[1];
    }
    if (ans == SPLAY_MAX_NUM)
        return -1;
    return ans;
}

static void splay_free(splay_node *now)
{
    if (now == null)
        return;
    splay_free(now->ch[0]);
    splay_free(now->ch[1]);
    free(now);
}

void splay_gc()
{
    free(null);
    splay_free(root);
}
