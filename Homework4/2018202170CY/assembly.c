#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXLINE 100
#define MAXLEN 100
#define NUM_OF_VAR 50
#define BEGIN_OF_PARAMETER 40
#define NUM_OF_REGISTER 14
#define EAX "%eax"
#define EBX "%ebx"
#define ECX "%ecx"
#define EDX "%edx"
#define ESI "%esi"
#define EDI "%edi"
#define R8D "%r8d"
#define R9D "%r9d"
#define R10D "%r10d"
#define R11D "%r11d"
#define R12D "%r12d"
#define R13D "%r13D"
#define R14D "%r14D"
#define R15D "%r15D"
#define ADDL "addl"
#define IMULL "imull"
#define LEAL "leal"
#define RET "ret"
#define SALL "sall"
#define true 1
#define false 0
#define NUM_OF_EAX 8
#define BEGIN_OF_LINE 3
#define first_line(a) str[1][a]

char str[MAXLINE][MAXLEN];
int map[NUM_OF_VAR];
int last_appear[NUM_OF_VAR];
int used[NUM_OF_REGISTER + 1];
int NUM_OF_LINE;
void getline_(int a)
{
	int len = 0;
	char c;
	while(true)
	{
		c = getchar();
		if (c == '\n' || c == EOF)
		{
			str[a][len] = 0;
			return ;
		}
		str[a][len++] = c;
	}
}
void _register(int a)
{
	switch (a)
	{
		case 1 : 
			printf("%s",EDI);
			break;
		case 2 : 
			printf("%s",ESI);
			break;
		case 3 : 
			printf("%s",EDX);
			break;
		case 4 : 
			printf("%s",ECX);
			break;
		case 5 : 
			printf("%s",R8D);
			break;
		case 6 : 
			printf("%s",R9D);
			break;
		case 7 : 
			printf("%s",EBX);
			break;
		case 8 : 
			printf("%s",EAX);
			break;
		case 9 : 
			printf("%s",R10D);
			break;
		case 10 : 
			printf("%s",R11D);
			break;
		case 11 : 
			printf("%s",R12D);
			break;
		case 12 : 
			printf("%s",R13D);
			break;
		case 13 : 
			printf("%s",R14D);
			break;
		case 14 : 
			printf("%s",R15D);
			break;
		default :
			break;
	}
}
int is_charactor(char a)
{
	return (a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z');
}
int get_var()
{
	int l,r;
	for (int i = 0; i < strlen(str[1]); i++)
	{
		if (first_line(i) == '(') l = i;
		if (first_line(i) == ')') r = i;
	}
	int pl = l;
	while (first_line(pl - 1) != ' ') pl --;
	for (int i = pl; i < l; i++)
	printf("%c", first_line(i));
	printf(":\n");
	int num_of_var = 0;
	for (int i = l; i <= r; i++)
	{
		if (first_line(i) == ',' || first_line(i) == ')')
		{
			num_of_var++;
			map[num_of_var + BEGIN_OF_PARAMETER] = num_of_var;
			used[num_of_var] = true;
		}
	}
	return num_of_var;
}
int is_digit(char a)
{
	return a >= '0' && a <= '9';
}
int check_t(int line, int l, int r)
{
	int ret = 0;
	int flag = false;
	for (int i = l; i <= r; i++)
	{
		if ( is_digit(str[line][i]) )
		{
			ret = ret * 10 + str[line][i] - '0';
		}
		if (str[line][i] == 't' && is_digit(str[line][i + 1]) )
		flag = true;
	}
	if (!flag) return -1;
	return ret;
}
int check_parameter(int line, int l, int r)
{
	for (int i = l; i <= r; i++)
	{
		if (str[line][i] >= 'a' && str[line][i] <= 'e') return str[line][i] - 'a' + BEGIN_OF_PARAMETER + 1;
	}
	return -1;
}
int get_No(int line, int l, int r)
{
	if (check_t(line, l ,r) != -1) return check_t(line, l, r);
	if (check_parameter(line, l, r) != -1) return check_parameter(line, l, r);
	return -1;
}
void get_last_appear(int num_of_var)
{
	for (int i = BEGIN_OF_LINE; i < NUM_OF_LINE - 1; i++)
	{
		int place_of_equal_sign, place_of_op;
		for (int j = 0; j < strlen(str[i]); j++)
		{
			if (str[i][j] == '=') place_of_equal_sign = j;
			if (str[i][j] == '+' || str[i][j] == '*') place_of_op = j;
		}
		int a, b;
		a = get_No(i, place_of_equal_sign, place_of_op);
		b = get_No(i, place_of_op, strlen(str[i]) - 1);
		if (a != -1)
		{
			last_appear[a] = i;
		}
		if (b != -1)
		{
			last_appear[b] = i;
		}
		last_appear[i - BEGIN_OF_LINE + 1] = i;
	}
}
void assembly_add(int register_of_a, int register_of_b, int unused_register)
{
	if (unused_register == register_of_b)
	{
		int t = register_of_a;
		register_of_a = register_of_b;
		register_of_b = t;
	}
	if (unused_register == register_of_a)
	{
		printf("\t%s\t", ADDL);
		_register(register_of_b);
		printf(", ");
		_register(register_of_a);
		printf("\n");
		return ;
	}
	else
	{
		printf("\t%s\t(", LEAL);
		_register(register_of_a);
		printf(", ");
		_register(register_of_b);
		printf(" ), ");
		_register(unused_register);
		printf("\n");
		return ;
	}
}
void assembly_mul(int register_of_a, int register_of_b, int register_of_c)
{
	if (register_of_c == register_of_b)
	{
		int t = register_of_b;
		register_of_b = register_of_a;
		register_of_a = t;
	}
	if (register_of_c == register_of_a)
	{
		printf("\t%s\t", IMULL);
		_register(register_of_b);
		printf(", ");
		_register(register_of_c);
		printf("\n");
		return ;
	}
	else
	{
		printf("\t%s\t", LEAL);
		_register(register_of_a);
		printf(", ");
		_register(register_of_c);
		printf("\n");
		printf("\t%s\t", IMULL);
		_register(register_of_b);
		printf(", ");
		_register(register_of_a);
		printf("\n");
		return ;
	}
}
int get_number(int line, int l, int r)
{
	int ret = 0;
	for (int i = l; i <= r; i++)
	{
		if (is_digit(str[line][i]) )
			ret = ret * 10 + str[line][i] - '0';
	}
	return ret;
}
int check_rval(int line)
{
	for (int i = 0; i < strlen(str[line]) ; i++)
	{
		if (str[line][i] == 'r' && str[line][i + 1] =='v') return 1;
	}
	return 0;
}
void register_add_number(int number, int register_of_a, int register_of_c)
{
	if (register_of_a == register_of_c)
	{
		printf("\t%s\t", ADDL);
		printf("$%d, ", number);
		_register(register_of_c);
		printf("\n");
	}
	else
	{
		printf("\t%s\t", LEAL);
		printf("%d(", number);
		_register(register_of_a);
		printf("), ");
		_register(register_of_c);
		printf("\n");
	}
}
void register_mul_number(int number, int register_of_a, int register_of_c)
{
	if (register_of_a == register_of_c)
	{
		printf("\t%s\t", IMULL);
		printf("$%d, ", number);
		_register(register_of_c);
		printf("\n");
	}
	else
	{
		printf("\t%s\t", LEAL);
		_register(register_of_a);
		printf(", ");
		_register(register_of_c);
		printf("\n");
		printf("\t%s\t", IMULL);
		printf("$%d, ", number);
		_register(register_of_c);
		printf("\n");
	}
}
void assembly_line(int a)
{
	int place_of_equal_sign, place_of_op;
	int unused_register = 0;
	for (int i = 0; i < strlen(str[a]) - 1; i++)
	{
		if (str[a][i] == '=' ) place_of_equal_sign = i;
		if (str[a][i] == '+' || str[a][i] == '*') place_of_op = i;
	}
	int x = get_No(a, place_of_equal_sign, place_of_op);
	int y = get_No(a, place_of_op, strlen(str[a]) - 1);
	int number = 0;
	if (x == -1)
		number = get_number(a, place_of_equal_sign, place_of_op);
	if (y == -1)
		number = get_number(a, place_of_op, strlen(str[a]) - 1);
	if (x == -1)
	{
		int t = x;
		x = y;
		y = t;
	}
	if (y == -1)
	{
		if (last_appear[x] == a)
		{
			unused_register = map[x];	 
		}
		else
		{
			for (int i = 1; i <= NUM_OF_REGISTER; i++)
			{
				if (!used[i])
				{
					used[i] = 1;
					unused_register = i;
					break;
				}
			}
		}
		if (check_rval(a)) unused_register = NUM_OF_EAX;
		if (str[a][place_of_op] == '+')
		{
			register_add_number(number, map[x], unused_register);
		}
		if (str[a][place_of_op] == '*')
		{
			register_mul_number(number, map[x], unused_register);
		}
		map[a - BEGIN_OF_LINE + 1] = unused_register;
		return ;
	}
	if (last_appear[x] == a)
	{
		used[map[x]] = false;
		unused_register = map[x];
	}
	if (last_appear[y] == a)
	{
		used[map[y]] = false;
		unused_register = map[y];
	}
	if (!unused_register)
	{
		for (int i = 1; i <= NUM_OF_REGISTER; i++)
		{
			if (!used[i])
			{
				unused_register = i;
				break;
			}
		}
	}
	if (check_rval(a) ) unused_register = NUM_OF_EAX;
	map[a - BEGIN_OF_LINE + 1] = unused_register;
	used[unused_register] = true;
	if (str[a][place_of_op] == '+')
	{
		assembly_add(map[x], map[y], unused_register);
	}
	if (str[a][place_of_op] == '*')
	{
		assembly_mul(map[x], map[y], unused_register);
	}
}

int main()
{
	freopen("arith.txt","r",stdin);
//	freopen("my_answer.txt","w",stdout);

	scanf("%d",&NUM_OF_LINE);
	getchar();
	for (int i = 1; i <= NUM_OF_LINE; i++)
	{
		getline_(i);
	}
	int num_of_var = get_var();
	get_last_appear(num_of_var);
	for (int i = BEGIN_OF_LINE; i <= NUM_OF_LINE - 1; i++)
	{
		assembly_line(i);
	}
	printf("\tret\n");
}
