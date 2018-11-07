#include <time.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
	srand(time(0));
	printf("long calc(");
	int num_of_parameter = rand() % 5 + 1, cnt = 0;
	num_of_parameter = 5;//
	for (int i = 1; i < num_of_parameter; i++) printf("long y%d, ", ++cnt);
	printf("long y%d) {\n", ++cnt);
	int n = num_of_parameter;
	int num_of_tmp_var = 0, num_of_express = rand() % (4 * n + 1) + n;
	num_of_express = 20 * n;//
	for (int i = 0; i < num_of_express; i++) {
		int flag = 1;
		if (i + 1 != num_of_express) {
			if (rand() % 2) {
				flag = 0;
				int rand_num = rand() % (num_of_parameter + num_of_tmp_var) + 1;
				if (rand_num > num_of_parameter) {
					printf("\tt%d = ", rand_num - num_of_parameter);
				}
				else printf("\ty%d = ", rand_num);
			}
			else printf("\tlong t%d = ", num_of_tmp_var + 1);
		}
		else {
			printf("\tlong rval = ");
		}
		//if (0) {//
		if (!(rand() % 3)) {
			long x;
			int var_num;
			char var_type;
			char op;
			int rand_num;

			x = (long) ((((unsigned long)rand() << 56) & 0xFF00000000000000UL) |
			            (((unsigned long)rand() << 28) & 0x00FFFFFFF0000000UL) |
				        (((unsigned long)rand()      ) & 0x000000000FFFFFFFUL));
			if (rand() % 2) x = -1L - x;

			rand_num = rand() % (num_of_parameter + num_of_tmp_var) + 1;
			rand_num = num_of_parameter + num_of_tmp_var;//
			if (rand_num > num_of_parameter) {
				var_type = 't';
				var_num = rand_num - num_of_parameter;
			}
			else {
				var_type = 'y';
				var_num = rand_num;
			}

			rand_num = rand() % 2;
			if (rand_num == 0) op = '+';
			else if (rand_num == 1) op = '*';

			if (rand() % 2) {
				printf("%ldL %c %c%d;\n", x, op, var_type, var_num);
			}
			else {
				printf("%c%d %c %ldL;\n", var_type, var_num, op, x);
			}
		}
		else {
			int var_num1, var_num2;
			char var_type1, var_type2;
			char op;
			int rand_num;

			rand_num = rand() % (num_of_parameter + num_of_tmp_var) + 1;
			rand_num = (num_of_parameter + num_of_tmp_var) / 2 + 1;//
			if (rand_num > num_of_parameter) {
				var_type1 = 't';
				var_num1 = rand_num - num_of_parameter;
			}
			else {
				var_type1 = 'y';
				var_num1 = rand_num;
			}
			rand_num = rand() % (num_of_parameter + num_of_tmp_var) + 1;
			rand_num = num_of_parameter + num_of_tmp_var;//
			if (rand_num > num_of_parameter) {
				var_type2 = 't';
				var_num2 = rand_num - num_of_parameter;
			}
			else {
				var_type2 = 'y';
				var_num2 = rand_num;
			}

			rand_num = rand() % 2;
			if (rand_num == 0) op = '+';
			else if (rand_num == 1) op = '*';

			printf("%c%d %c %c%d;\n", var_type1, var_num1, op, var_type2, var_num2);
		}
		if (flag) num_of_tmp_var++;
	}
	puts("\treturn rval;");
	puts("}");
	return 0;
}
