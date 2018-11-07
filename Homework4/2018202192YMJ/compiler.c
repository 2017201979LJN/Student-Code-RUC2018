#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

// boolean values
#define TRUE  1
#define FALSE 0

// number of bytes
#define NUM_OF_BYTES_OF_REG 8

// base
#define BASE_OF_DECIMAL 10

// limits
#define NUM_OF_VARIABLES_MAX 50
#define LEN_OF_STRING_MAX    100

// for IO
#define INPUT "calculate.c"
#define OUTPUT "my_asm"
#define READ_ONLY "r"
#define WRITE_ONLY "w"

// characters & strings
#define SPACE ' '
#define TAB '\t'
#define NEWLINE '\n'
#define COMMA ','
#define COLON ':'
#define SEMICOLON ';'
#define LEFT_BRACKET '('
#define RIGHT_BRACKET ')'
#define LEFT_BRACE '{'
#define RIGHT_BRACE '}'
#define ZERO '0'
#define DOLLAR '$'
#define ADD_SIGN '+'
#define SUBTRACT_SIGN '-'
#define MULTIPLY_SIGN '*'
#define TOP_OF_STACK "(%%rsp)"
#define FIRST_LETTER_OF_RETURN 'r'
#define FIRST_LETTER_OF_LONG 'l'

// for registers
#define NUM_OF_REGISTERS      16
#define NUM_OF_AVAI_REGISTERS 15
#define LEN_OF_REGISTERS_MAX  4
#define ID_OF_STACK_POINTER   15
#define ID_OF_RETURN_VALUE    14

// operations
#define MOV    "\tmovq\t"
#define MOVABS "\tmovabsq\t"
#define LEA    "\tleaq\t"
#define ADD    "\taddq\t"
#define SUB    "\tsubq\t"
#define IMUL   "\timulq\t"
#define PUSH   "\tpushq\t"
#define POP    "\tpopq\t" 
#define RET    "\tretq"

// types of data
#define IMMEDIATE 0
#define REGISTER  1
#define MEMORY    2

const char REGISTERS[NUM_OF_REGISTERS][LEN_OF_REGISTERS_MAX + 1] = {
	"%rdi",
	"%rsi",
	"%rdx",
	"%rcx",
	"%rbx",
	"%rbp",
	"%r8",
	"%r9",
	"%r10",
	"%r11",
	"%r12",
	"%r13",
	"%r14",
	"%r15",
	"%rax",
	"%rsp"
};

// annoucement of functions
void init();
void compile();

void output_function_name();

int calc_num_of_argument();

void allocate(int *, int *, int *, int *);
void deallocate(int);

void get_operand(int *, int *, long *);
char get_operator();

int char_to_value(char c);

void move_data(int, int, long, int, int, int *);
void add_data(int, int, long, int, int, int *);
void multiply_data(int, int, long, int, int, int *);
void lea_add_data(int, int, long, int, int, long, int);

void swap_int(int *, int *);
void swap_long(long *, long *);

void add_rsp(int);
void subtract_rsp(int);

void print_move(int, int, long, int, int, int);
void print_add(int, int, long, int, int, int);
void print_subtract(int, int, long, int, int, int);
void print_multiply(int, int, long, int, int, int);
void print_push(int, int *);
void print_pop(int, int *);
void print_operands(int, int, long, int, int, int);
void print_data(int, int, long, int);

// functions
int main()
{
	init();
	compile();
}

void init()
{
	freopen(INPUT, READ_ONLY, stdin);
	freopen(OUTPUT, WRITE_ONLY, stdout);
}

void compile()
{
	output_function_name();
	int num_of_argument = calc_num_of_argument();
	int num_of_variables = 0;
	int num_of_var_in_stack = 0;
	static int type[NUM_OF_VARIABLES_MAX];
	static int id[NUM_OF_VARIABLES_MAX];
	for (int i = 0; i < num_of_argument; ++i)
	{
		allocate(&num_of_variables, &num_of_var_in_stack, type + i, id + i);
	}
	static char str[LEN_OF_STRING_MAX];
	while (TRUE)
	{
		int c;
		while ((c = getchar()) != FIRST_LETTER_OF_RETURN && c != FIRST_LETTER_OF_LONG)
			;
		if (c == FIRST_LETTER_OF_RETURN)
		{
			if(type[num_of_variables - 1] != REGISTER || id[num_of_variables - 1] != ID_OF_RETURN_VALUE)
			{
				move_data(type[num_of_variables - 1], id[num_of_variables - 1], 0L, REGISTER, ID_OF_RETURN_VALUE, &num_of_var_in_stack);
			}
			break;
		}
		int dest;
		scanf("ong x%d = ", &dest);
		allocate(&num_of_variables, &num_of_var_in_stack, type + dest, id + dest);
		
		int type_lhs, type_rhs;
		int id_lhs, id_rhs;
		long val_lhs, val_rhs;
		char operator;
		get_operand(&type_lhs, &id_lhs, &val_lhs);
		operator = get_operator();
		get_operand(&type_rhs, &id_rhs, &val_rhs);

		if (operator == ADD_SIGN)
		{
			if (type[dest] == REGISTER && type_lhs != MEMORY && type_rhs != MEMORY)
			{
				lea_add_data(type_lhs, id_lhs, val_lhs, type_rhs, id_rhs, val_rhs, id[dest]);
			}
			else
			{
				move_data(type_lhs, id_lhs, val_lhs, type[dest], id[dest], &num_of_var_in_stack);
				add_data(type_rhs, id_rhs, val_rhs, type[dest], id[dest], &num_of_var_in_stack);
			}
		}
		else if(operator == MULTIPLY_SIGN)
		{
			move_data(type_lhs, id_lhs, val_lhs, type[dest], id[dest], &num_of_var_in_stack);
			multiply_data(type_rhs, id_rhs, val_rhs, type[dest], id[dest], &num_of_var_in_stack);
		}
		else
		{
			printf("unknown operator: %c\n", operator);
			assert(FALSE);
		}

	}
	deallocate(num_of_var_in_stack);
	puts(RET);
}

void output_function_name()
{
	int c;
	while ((c = getchar()) != SPACE)
		;
	while ((c = getchar()) != LEFT_BRACKET)
	{
		putchar(c);
	}
	putchar(COLON);
	putchar(NEWLINE);
}

int calc_num_of_argument()
{
	int num = 1;
	int c;
	while ((c = getchar()) != LEFT_BRACE)
	{
		if (c == COMMA) ++num;
	}
	return num;
}

void allocate(int *num_of_variables, int *num_of_var_in_stack, int *type, int *id)
{
	if (*num_of_variables < NUM_OF_AVAI_REGISTERS)
	{
		*type = REGISTER;
		*id = *num_of_variables;
	}
	else
	{
		*type = MEMORY;
		subtract_rsp(NUM_OF_BYTES_OF_REG);
		*id = *num_of_var_in_stack;
		++(*num_of_var_in_stack);
	}
	++(*num_of_variables);
}

void deallocate(int num_of_var_in_stack)
{
	if(num_of_var_in_stack > 0)
	{
		add_rsp(NUM_OF_BYTES_OF_REG * num_of_var_in_stack);
	}
}

void get_operand(int *type, int *id, long *val)
{
	char c;
	while (isspace(c = getchar()))
		;
	int sign = 1;
	if (c == SUBTRACT_SIGN)
	{
		sign = -1;
		c = getchar();
	}
	if (isdigit(c))
	{
		*type = IMMEDIATE;
		*val = char_to_value(c);
		while (isdigit(c = getchar()))
		{
			*val = BASE_OF_DECIMAL * (*val) + char_to_value(c);
		}
		(*val) = sign * (*val);
	}
	else
	{
		int subscript;
		scanf("%d", &subscript);
		if (subscript < NUM_OF_AVAI_REGISTERS)
		{
			*type = REGISTER;
			*id = subscript;
		}
		else
		{
			*type = MEMORY;
			*id = subscript - NUM_OF_AVAI_REGISTERS;
		}
	}
}

char get_operator()
{
	char c;
	while (isspace(c = getchar()))
		;
	return c;
}

int char_to_value(char c)
{
	return c - ZERO;
}

void move_data(int type_src, int id_src, long val_src, int type_dest, int id_dest, int *num_of_var_in_stack)
{
	if (type_src == MEMORY && type_dest == MEMORY)
	{
		print_push(ID_OF_RETURN_VALUE, num_of_var_in_stack);
		print_move(type_src, id_src, val_src, REGISTER, ID_OF_RETURN_VALUE, *num_of_var_in_stack);
		print_move(REGISTER, ID_OF_RETURN_VALUE, 0L, type_dest, id_dest, *num_of_var_in_stack);
		print_pop(ID_OF_RETURN_VALUE, num_of_var_in_stack);
	}
	else
	{
		print_move(type_src, id_src, val_src, type_dest, id_dest, *num_of_var_in_stack);
	}
}

void add_data(int type_src, int id_src, long val_src, int type_dest, int id_dest, int *num_of_var_in_stack)
{
	if (type_src == MEMORY && type_dest == MEMORY)
	{
		print_push(ID_OF_RETURN_VALUE, num_of_var_in_stack);
		print_add(type_src, id_src, val_src, REGISTER, ID_OF_RETURN_VALUE, *num_of_var_in_stack);
		print_add(REGISTER, ID_OF_RETURN_VALUE, 0L, type_dest, id_dest, *num_of_var_in_stack);
		print_pop(ID_OF_RETURN_VALUE, num_of_var_in_stack);
	}
	else
	{
		print_add(type_src, id_src, val_src, type_dest, id_dest, *num_of_var_in_stack);
	}
}

void multiply_data(int type_src, int id_src, long val_src, int type_dest, int id_dest, int *num_of_var_in_stack)
{
	if (type_src == MEMORY && type_dest == MEMORY)
	{
		print_push(ID_OF_RETURN_VALUE, num_of_var_in_stack);
		print_multiply(type_src, id_src, val_src, REGISTER, ID_OF_RETURN_VALUE, *num_of_var_in_stack);
		print_multiply(REGISTER, ID_OF_RETURN_VALUE, 0L, type_dest, id_dest, *num_of_var_in_stack);
		print_pop(ID_OF_RETURN_VALUE, num_of_var_in_stack);
	}
	else
	{
		print_multiply(type_src, id_src, val_src, type_dest, id_dest, *num_of_var_in_stack);
	}	
}

void lea_add_data(int type_lhs, int id_lhs, long val_lhs, int type_rhs, int id_rhs, long val_rhs, int id_dest)
{
	printf(LEA);
	if (type_rhs == IMMEDIATE)
	{
		swap_int(&type_lhs, &type_rhs);
		swap_int(&id_lhs, &id_rhs);
		swap_long(&val_lhs, &val_rhs);
	}
	if (type_lhs == IMMEDIATE)
	{
		printf("%ld", val_lhs);
		putchar(LEFT_BRACKET);
		print_data(type_rhs, id_rhs, val_rhs, 0);
		putchar(RIGHT_BRACKET);
		putchar(COMMA);
		putchar(SPACE);
		print_data(REGISTER, id_dest, 0L, 0);
		putchar(NEWLINE);
	}
	else
	{
		putchar(LEFT_BRACKET);
		print_data(type_lhs, id_lhs, val_lhs, 0);
		putchar(COMMA);
		print_data(type_rhs, id_rhs, val_rhs, 0);
		putchar(RIGHT_BRACKET);
		putchar(COMMA);
		putchar(SPACE);
		print_data(REGISTER, id_dest, 0L, 0);
		putchar(NEWLINE);
	}
}

void swap_int(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

void swap_long(long *a, long *b)
{
	long c = *a;
	*a = *b;
	*b = c;
}

void add_rsp(int val)
{
	print_add(IMMEDIATE, 0, (long)val, REGISTER, ID_OF_STACK_POINTER, 0);
}

void subtract_rsp(int val)
{
	print_subtract(IMMEDIATE, 0, (long)val, REGISTER, ID_OF_STACK_POINTER, 0);
}

void print_move(int type_src, int id_src, long val_src, int type_dest, int id_dest, int num_of_var_in_stack)
{
	if (type_src == IMMEDIATE)
	{
		printf(MOVABS);
	}
	else
	{
		printf(MOV);
	}
	print_operands(type_src, id_src, val_src, type_dest, id_dest, num_of_var_in_stack);
}

void print_add(int type_src, int id_src, long val_src, int type_dest, int id_dest, int num_of_var_in_stack)
{
	printf(ADD);
	print_operands(type_src, id_src, val_src, type_dest, id_dest, num_of_var_in_stack);
}

void print_subtract(int type_src, int id_src, long val_src, int type_dest, int id_dest, int num_of_var_in_stack)
{
	printf(SUB);
	print_operands(type_src, id_src, val_src, type_dest, id_dest, num_of_var_in_stack);
}

void print_multiply(int type_src, int id_src, long val_src, int type_dest, int id_dest, int num_of_var_in_stack)
{
	printf(IMUL);
	print_operands(type_src, id_src, val_src, type_dest, id_dest, num_of_var_in_stack);
}

void print_push(int id, int *num_of_var_in_stack)
{
	printf(PUSH);
	print_data(REGISTER, id, 0L, 0);
	putchar(NEWLINE);
	++(*num_of_var_in_stack);
}

void print_pop(int id, int *num_of_var_in_stack)
{
	printf(POP);
	print_data(REGISTER, id, 0L, 0);
	putchar(NEWLINE);
	--(*num_of_var_in_stack);
}

void print_operands(int type_src, int id_src, long val_src, int type_dest, int id_dest, int num_of_var_in_stack)
{
	print_data(type_src, id_src, val_src, num_of_var_in_stack);
	putchar(COMMA);
	putchar(SPACE);
	print_data(type_dest, id_dest, 0L, num_of_var_in_stack);
	putchar(NEWLINE);
}

void print_data(int type, int id, long val, int num_of_var_in_stack)
{
	switch(type)
	{
	case IMMEDIATE:
		putchar(DOLLAR);
		printf("%ld" ,val);
		break;
	case REGISTER:
		printf("%s", REGISTERS[id]);
		break;
	case MEMORY:
		if (id < num_of_var_in_stack - 1)
		{
			printf("%d", NUM_OF_BYTES_OF_REG * (num_of_var_in_stack - 1 - id));
		}
		printf(TOP_OF_STACK);
		break;
	default:
		printf("unknown data type: %d\n", type);
		assert(FALSE);
		break;
	}
}

