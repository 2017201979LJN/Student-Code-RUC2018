#include <stdio.h>
#include <stdlib.h>

#define DATA_SIZE 1000000
#define MAX_DIS 32768
#define SIZE_OF_CHAR 100000
#define SIZE_OF_ASC 256
#define INF 2e9

#define swap(a,b) ((a)^=(b)^=(a)^=(b))

struct pair
{
	int v;
	int id;
}char_cnt[SIZE_OF_CHAR], huff[SIZE_OF_ASC];

struct edge
{
	int v, next;
}e[DATA_SIZE];

unsigned char inhuff[DATA_SIZE], outhuff[DATA_SIZE] = {0};
int n, head[DATA_SIZE], edge_cnt = 0;

void insert(int v1, int v2)
{
	e[++edge_cnt].v = v2;
	e[edge_cnt].next = head[v1];
	head[v1] = edge_cnt;
}

void dfs(int x,int fa, int dep)
{
	for (int i = head[x]; i; i = e[i].next) {
		dfs(e[i].v, x, dep + 1);
	}
	if (!head[x]) {
		huff[x].v = dep;
		huff[x].id = x;
	}
}

int cmp(const void *v1, const void *v2)
{
	return ((struct pair *)v1)->v > ((struct pair *)v2)->v;	
}

int main()
{
	FILE *fp = fopen("zip.in", "r");
	n = 0;
	while (fread(inhuff + n, 1, 1, fp))
		n++;
	for (int i = 0; i < SIZE_OF_ASC; i++) {
		char_cnt[i].id = i;
		char_cnt[i].v = 0;
	}
	for (int i = 0; i < n; i++)
		char_cnt[inhuff[i]].v++;
	for (int i = 0; i < SIZE_OF_ASC; i++)
		if (char_cnt[i].v == 0)
			char_cnt[i].v = INF;
	qsort(char_cnt, SIZE_OF_ASC, sizeof(struct pair), cmp);
	int head = 0, asc_cnt = SIZE_OF_ASC;
	int asc_size = SIZE_OF_ASC;
	while (char_cnt[asc_size - 1].v == INF)
		asc_size--;
	while(head < asc_size - 1) {
		insert(asc_cnt, char_cnt[head].id);
		insert(asc_cnt, char_cnt[head + 1].id);
		char_cnt[++head].id = asc_cnt;
		char_cnt[head].v += char_cnt[head - 1].v;
		for (int j = head; j < asc_size - 1; j++) {
			if (char_cnt[j].v > char_cnt[j + 1].v) {
				swap(char_cnt[j].id, char_cnt[j + 1].id);
				swap(char_cnt[j].v, char_cnt[j + 1].v);
			}
			else 
				break;
		}
		asc_cnt++;
	}
	int root = char_cnt[asc_size - 1].id;
	dfs(root, 0, 0);
	int out_cnt = 3, out_pos = 0;
	for (int i = 0; i < n; i++) {
		int flag = 0, v = huff[inhuff[i]].v;
		for (int j = 3; j > 0; j--) {
			if (v & (1 << j))
				flag = 1;
			if (flag) {
				if (v & (1 << j))
					outhuff[out_pos] += (1 << out_cnt);
				out_cnt--;
				if (out_cnt < 0) {
					out_cnt = 3;
					out_pos++;
				}
			}	
		}
	}
	if (out_cnt != 3)
		out_pos++;
	fp = fopen("splay.zip", "w");
	fwrite(outhuff, 1, out_pos, fp);
	return 0;
}
