typedef struct splay_node
{
    struct splay_node *pre, *ch[2];
    int val, cnt, size;
}splay_node;

void splay_init();

void splay_insert(int val);

void splay_erase(int val);

int splay_kth(int k);

int splay_getrank(int val);

int splay_getpre(int val);

int splay_getpost(int val);

void splay_gc();
