#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// for IO
#define INPUT         "data.in"
#define OUTPUT_MY_ANS "my_qsort.out"
#define OUTPUT_ANS    "qsort.out"

#define READ_ONLY  "r"
#define WRITE_ONLY "w"

#define SMALLER -1
#define LARGER   1
#define EQUAL    0

FILE* input;
FILE* output_my_qsort;
FILE* output_qsort;

struct my_struct
{
	int a;
	char b;
	long long c;
	char d;
	double e;
};

void init(void);
void process(void);
void terminate(void);

void input_my_struct(struct my_struct* ptr);
void output_my_struct(FILE* file, struct my_struct* ptr);

void my_qsort(void* base, size_t num, size_t size,
			  int (*cmp)(const void*, const void*));

int gen_rand_int_limit(int L, int R);

void* calc_addr(void* base, size_t size, int index);

void swap(void* a, void* b, size_t size);

int cmp_char(const void* a, const void* b);
int cmp_int(const void* a, const void* b);
int cmp_double(const void* a, const void* b);
int cmp_my_struct(const void* a, const void* b);

int main(void)
{
	init();
	process();
	terminate();
}

void init(void)
{
	srand(time(NULL));
	input = fopen(INPUT, READ_ONLY);
	output_my_qsort = fopen(OUTPUT_MY_ANS, WRITE_ONLY);
	output_qsort = fopen(OUTPUT_ANS, WRITE_ONLY);
}

void process(void)
{

	int num_of_elements;
	fscanf(input, "%d", &num_of_elements);
	
	void* block_char_my_qsort      = malloc(num_of_elements * sizeof(char));
	void* block_int_my_qsort       = malloc(num_of_elements * sizeof(int));
	void* block_double_my_qsort    = malloc(num_of_elements * sizeof(double));
	void* block_my_struct_my_qsort = malloc(num_of_elements * sizeof(struct my_struct));
	void* block_char_qsort         = malloc(num_of_elements * sizeof(char));
	void* block_int_qsort          = malloc(num_of_elements * sizeof(int));
	void* block_double_qsort       = malloc(num_of_elements * sizeof(double));
	void* block_my_struct_qsort    = malloc(num_of_elements * sizeof(struct my_struct));

	for (int i = 0; i < num_of_elements; ++i)
	{
		int val;
		fscanf(input, "%d", &val);
		*((char*)(block_char_my_qsort + i * sizeof(char))) = (char)val;
		*((char*)(block_char_qsort + i * sizeof(char))) = (char)val;
	}
	for (int i = 0; i < num_of_elements; ++i)
	{
		fscanf(input, "%d", (int*)(block_int_my_qsort + i * sizeof(int)));
		*((int*)(block_int_qsort + i * sizeof(int))) =
			*((int*)(block_int_my_qsort + i * sizeof(int)));
	}
	for (int i = 0; i < num_of_elements; ++i)
	{
		fscanf(input, "%lf", (double*)(block_double_my_qsort + i * sizeof(double)));
		*((double*)(block_double_qsort + i * sizeof(double))) =
			*((double*)(block_double_my_qsort + i * sizeof(double)));
	}
	for (int i = 0; i < num_of_elements; ++i)
	{
		input_my_struct((struct my_struct*)(block_my_struct_my_qsort + i * sizeof(struct my_struct)));
		for (int j = 0; j < sizeof(struct my_struct); ++j)
		{
			*((char*)(block_my_struct_qsort + i * sizeof(struct my_struct) + j)) =
				*((char*)(block_my_struct_my_qsort + i * sizeof(struct my_struct) + j));
		}
	}

	my_qsort(block_char_my_qsort, num_of_elements, sizeof(char), cmp_char);
	my_qsort(block_int_my_qsort, num_of_elements, sizeof(int), cmp_int);
	my_qsort(block_double_my_qsort, num_of_elements, sizeof(double), cmp_double);
	my_qsort(block_my_struct_my_qsort, num_of_elements, sizeof(struct my_struct), cmp_my_struct);
	qsort(block_char_qsort, num_of_elements, sizeof(char), cmp_char);
	qsort(block_int_qsort, num_of_elements, sizeof(int), cmp_int);
	qsort(block_double_qsort, num_of_elements, sizeof(double), cmp_double);
	qsort(block_my_struct_qsort, num_of_elements, sizeof(struct my_struct), cmp_my_struct);

	for (int i = 0; i < num_of_elements; ++i)
	{
		fprintf(output_my_qsort, "%d%c",
				(int)(*((char*)(block_char_my_qsort + i * sizeof(char)))),
				" \n"[i == num_of_elements - 1]);
		fprintf(output_qsort, "%d%c",
				(int)(*((char*)(block_char_qsort + i * sizeof(char)))),
				" \n"[i == num_of_elements - 1]);
	}
	for (int i = 0; i < num_of_elements; ++i)
	{
		fprintf(output_my_qsort, "%d%c",
				*((int*)(block_int_my_qsort + i * sizeof(int))),
				" \n"[i == num_of_elements - 1]);
		fprintf(output_qsort, "%d%c",
				*((int*)(block_int_qsort + i * sizeof(int))),
				" \n"[i == num_of_elements - 1]);
	}
	for (int i = 0; i < num_of_elements; ++i)
	{
		fprintf(output_my_qsort, "%f%c",
				*((double*)(block_double_my_qsort + i * sizeof(double))),
				" \n"[i == num_of_elements - 1]);
		fprintf(output_qsort, "%f%c",
				*((double*)(block_double_qsort + i * sizeof(double))),
				" \n"[i == num_of_elements - 1]);
	}
	for (int i = 0; i < num_of_elements; ++i)
	{
		output_my_struct(output_my_qsort, (struct my_struct*)(block_my_struct_my_qsort + i * sizeof(struct my_struct)));
		output_my_struct(output_qsort, (struct my_struct*)(block_my_struct_qsort + i * sizeof(struct my_struct)));
	}

	free(block_char_my_qsort);
	free(block_int_my_qsort);
	free(block_double_my_qsort);
	free(block_my_struct_my_qsort);
	free(block_char_qsort);
	free(block_int_qsort);
	free(block_double_qsort);
	free(block_my_struct_qsort);
	
}

void terminate(void)
{
	fclose(input);
	fclose(output_my_qsort);
	fclose(output_qsort);
}

void input_my_struct(struct my_struct* ptr)
{
	int val_b, val_d;
	fscanf(input, "%d%d%lld%d%lf", &(ptr->a), &val_b, &(ptr->c), &val_d, &(ptr->e));
	ptr->b = (char)val_b;
	ptr->d = (char)val_d;
}

void output_my_struct(FILE* file, struct my_struct* ptr)
{
	fprintf(file, "%d ", ptr->a);
	fprintf(file, "%d ", (int)ptr->b);
	fprintf(file, "%lld ", ptr->c);
	fprintf(file, "%d ", (int)ptr->d);
	fprintf(file, "%f\n", ptr->e);
}

void my_qsort(void* base, size_t num, size_t size,
			  int (*cmp)(const void*, const void*))
{
	if (num <= 1) return;
	void* top = calc_addr(base, size, (int)num - 1);
	int key_index = gen_rand_int_limit(0, (int)num - 1);
	void* key_ptr = calc_addr(base, size, key_index);
	swap(key_ptr, top, size);
	void* last = base;
	for (void* ptr = base; ptr < top; ptr += size)
	{
		if ((*cmp)(ptr, top) != LARGER)
		{
			swap(last, ptr, size);
			last += size;
		}
	}
	swap(last, top, size);
	int num_of_left_elements = (last - base) / size,
		num_of_right_elements = num - 1 - num_of_left_elements;
	my_qsort(base, num_of_left_elements, size, cmp);
	my_qsort(last + size, num_of_right_elements, size, cmp);
}

int gen_rand_int_limit(int L, int R)
{
	return L + rand() % (R - L + 1);
}

void* calc_addr(void* base, size_t size, int index)
{
	return base + size * index;
}

void swap(void* a, void* b, size_t size)
{
	if (a == b) return;
	for (int i = 0; i < (int)size; ++i, ++a, ++b)
	{
		char t = *((char*)a);
		*((char*)a) = *((char*)b);
		*((char*)b) = t;
	}
}

int cmp_char(const void* a, const void* b)
{
	if (*((char*)a) < *((char*)b)) return SMALLER;
	if (*((char*)a) > *((char*)b)) return LARGER;
	return EQUAL;
}

int cmp_int(const void* a, const void* b)
{
	if (*((int*)a) < *((int*)b)) return SMALLER;
	if (*((int*)a) > *((int*)b)) return LARGER;
	return EQUAL;
}

int cmp_double(const void* a, const void* b)
{
	if (*((double*)a) < *((double*)b)) return SMALLER;
	if (*((double*)a) > *((double*)b)) return LARGER;
	return EQUAL;
}

int cmp_my_struct(const void* a, const void* b)
{
	for (int i = 0; i < sizeof(struct my_struct); ++i, ++a, ++b)
	{
		int cmp_res = cmp_char(a, b);
		if (cmp_res != EQUAL) return cmp_res;
	}
	return EQUAL;
}

