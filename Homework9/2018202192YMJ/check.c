#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

struct data
{
	long long id;
	int time;
	char type;
};

int my_cmp(const void *a, const void *b)
{
	struct data *A = (struct data *)a;
	struct data *B = (struct data *)b;
	if (A -> id != B -> id) return A -> id < B -> id;
	else return A -> time < B -> time;
}

int type_value(int x)
{
	if (x == 0) return 1;
	else return -1;
}

struct data d[1001000];

int main()
{
	int num = 0;
	long long id;
	int time;
	char type, str[10];
	while (scanf("%llx %s", &id, str) != EOF)
	{
		d[num].id = id;
		d[num].time = num;
		d[num].type = (str[0] == 'm') ? 1 : 0;
		num ++;
	}
	qsort(d, num, sizeof(struct data), my_cmp);
	int used = 0;
	for(int i = 0; i <= num; i++)
	{
		if (used < 0)
		{
			printf("%llx has too many free!\n", d[i - 1].id);
			return 0;
		}
		if( (i != 0 && d[i].id != d[i - 1].id) || i == num)
		{
			if(used != 0)
			{
				if (used > 0)
					printf("%llx hasn't enough free!\n", d[i - 1].id);
				else
					printf("%llx has too many free!\n", d[i - 1].id);
				return 0;
			}
			if (i == num) break;
		}
		used += type_value(d[i].type);
	}
	printf("All malloc and free are in pair!\n");
	return 0;
}
