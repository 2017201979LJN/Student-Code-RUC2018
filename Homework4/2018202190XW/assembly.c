#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define BUF_LEN 1000
#define MAX_NUM_OF_PARAMETER 5
#define MAX_NUM_OF_EXPRESS 200
#define MAX_NUM_OF_REGISTER 15
#define ID_OF_RVAL 190
#define SIZE_OF_BYTE 8
#define POS_OF_RAX 9

struct rec{
	int id_of_num1, id_of_num2;
	int type_of_op;
	int has_num, is_useful;
	long num;
	int out_degree;
};

int getline_(FILE*, char*, int);
int get_id(char*, int);
int is_num(char*, int);
long get_num(char*, int);
void work(struct rec*, int, int);
void prt_register(int);
void prt_memory(int);
void prt_movq(int, int);
void prt_movabsq(long, int);
void prt_imulq(int, int);
void prt_addq(int, int);
void prt_leaq(int, int, int);
void push_callee_saved();
void pop_callee_saved();

int main() {
	FILE* input = fopen("tmp.c", "r");
	char s[BUF_LEN];
	int len = getline_(input, s, BUF_LEN);
	int num_of_parameter;
	for (int i = 0; i < len; i++) {
		if (isdigit(s[i])) num_of_parameter = s[i] - '0';
	}

	struct rec state[MAX_NUM_OF_EXPRESS];
	int num_of_state = num_of_parameter;
	int last_id_of_var[MAX_NUM_OF_EXPRESS];
	for (int i = 0; i < MAX_NUM_OF_EXPRESS; i++) last_id_of_var[i] = 0;
	for (int i = 1; i <= num_of_parameter; i++) {
		last_id_of_var[i] = i;
		state[i].has_num = 0;
		state[i].is_useful = 0;
		state[i].out_degree = 0;
	}
	for (int i = 0; i < MAX_NUM_OF_EXPRESS; i++) state[i].is_useful = 0;
	while (len = getline_(input, s, BUF_LEN)) {
		int pos_of_equal = -1, pos_of_op = -1;
		for (int i = 0; i < len; i++) {
			if (s[i] == '=') pos_of_equal = i;
			if (s[i] == '+') pos_of_op = i;
			if (s[i] == '*') pos_of_op = i;
		}
		if (pos_of_equal < 0) continue;
		num_of_state++;
		state[num_of_state].has_num = 0;
		state[num_of_state].out_degree = 0;
		if (s[pos_of_op] == '+') state[num_of_state].type_of_op = 0;
		else state[num_of_state].type_of_op = 1;
		int is_num1 = is_num(s + pos_of_equal + 1, pos_of_op - pos_of_equal - 1);
		int is_num2 = is_num(s + pos_of_op + 1, len - pos_of_op - 1);
		int tmp_id;
		
		if (is_num1) {
			state[num_of_state].has_num = 1;
			state[num_of_state].num = get_num(s + pos_of_equal + 1, pos_of_op - pos_of_equal - 1);
			tmp_id = get_id(s + pos_of_op + 1, len - pos_of_op - 1);
		}
		else if (is_num2) {
			state[num_of_state].has_num = 1;
			state[num_of_state].num = get_num(s + pos_of_op + 1, len - pos_of_op - 1);
			tmp_id = get_id(s + pos_of_equal + 1, pos_of_op - pos_of_equal - 1);
		}
		else tmp_id = get_id(s + pos_of_equal + 1, pos_of_op - pos_of_equal - 1);
		state[num_of_state].id_of_num1 = last_id_of_var[tmp_id];
		
		tmp_id = get_id(s + pos_of_op + 1, len - pos_of_op - 1);
		state[num_of_state].id_of_num2 = last_id_of_var[tmp_id];
		tmp_id = get_id(s, pos_of_equal - 1);
		last_id_of_var[tmp_id] = num_of_state;
	}
	state[last_id_of_var[ID_OF_RVAL]].is_useful = 1;
	for (int i = num_of_state; i > num_of_parameter; i--) if (state[i].is_useful) {
		state[state[i].id_of_num1].is_useful = 1;
		if (!state[i].has_num) state[state[i].id_of_num2].is_useful = 1;
		state[state[i].id_of_num1].out_degree++;
		if (!state[i].has_num) state[state[i].id_of_num2].out_degree++;
	}
	push_callee_saved();
	work(state, num_of_state, num_of_parameter);
	pop_callee_saved();
	puts("\tretq");
	return 0;
}

int getline_(FILE* input, char s[],int lim){
	char c;
	int len = 0;
	while (fscanf(input, "%c", &c) != EOF && c != '\n' && len < lim - 1) {
        s[len++] = c;
    }
	s[len] = '\0';
	return len;
}
int get_id(char s[], int len) {
	int pos_of_letter = -1;
	for (int i = 0; i < len; i++) {
		if (s[i] == 'y' || s[i] == 'r' || s[i] == 't') {
			pos_of_letter = i;
			break;
		}
	}
	if (pos_of_letter == -1) return 1;
	if (s[pos_of_letter] == 'r') return ID_OF_RVAL;
	long num = get_num(s + pos_of_letter + 1, len - pos_of_letter - 1);
	if (s[pos_of_letter] == 'y') return num;
	return num + MAX_NUM_OF_PARAMETER;
}
int is_num(char s[], int len) {
	for (int i = 0; i < len; i++) if (islower(s[i])) return 0;
	return 1;
}
long get_num(char s[], int len) {
	long ret = 0, sign = 1;
	for (int i = 0; i < len; i++) {
		if (isdigit(s[i])) ret = ret * 10 + s[i] - '0';
		if (s[i] == '-') sign = -1;
	}
	return ret * sign;
}
void prt_register(int x) {
	switch (x) {
		case 1: printf("%%rdi"); break;
		case 2: printf("%%rsi"); break;
		case 3: printf("%%rdx"); break;
		case 4: printf("%%rcx"); break;
		case 5: printf("%%r8"); break;
		case 6: printf("%%r9"); break;
		case 7: printf("%%r10"); break;
		case 8: printf("%%r11"); break;
		case 9: printf("%%rax"); break;
		case 10: printf("%%r12"); break;
		case 11: printf("%%r13"); break;
		case 12: printf("%%r14"); break;
		case 13: printf("%%r15"); break;
		case 14: printf("%%rbx"); break;
		case 15: printf("%%rbp"); break;
		default: break;
	}
}
void prt_memory(int x) {
	printf("-%02X(%%rsp)", x * SIZE_OF_BYTE);
}
void prt_operand(int x) {
	if (x <= MAX_NUM_OF_REGISTER) prt_register(x);
	else prt_memory(x - MAX_NUM_OF_REGISTER);
}
void prt_movq(int src, int dest) {
	printf("\tmovq\t");
	prt_operand(src);
	printf(", ");
	prt_operand(dest);
	printf("\n");
}
void prt_movabsq(long x, int dest) {
	printf("\tmovabsq\t$%ld, ", x);
	prt_operand(dest);
	printf("\n");
}
void prt_imulq(int src, int dest) {
	printf("\timulq\t");
	prt_operand(src);
	printf(", ");
	prt_operand(dest);
	printf("\n");
}
void prt_addq(int src, int dest) {
	printf("\taddq\t");
	prt_operand(src);
	printf(", ");
	prt_operand(dest);
	printf("\n");
}
void prt_leaq(int src1, int src2, int dest) {
	printf("\tleaq\t(");
	prt_operand(src1);
	printf(",");
	prt_operand(src2);
	printf(") ");
	prt_operand(dest);
	printf("\n");
}
void work(struct rec state[], int num_of_state, int num_of_parameter) {
	int get_pos_by_id[MAX_NUM_OF_EXPRESS];
	int get_id_by_pos[MAX_NUM_OF_EXPRESS];
	int pos_is_used[MAX_NUM_OF_EXPRESS];
	for (int i = 0; i < MAX_NUM_OF_EXPRESS; i++) pos_is_used[i] = 0;
	for (int i = 1; i <= num_of_parameter; i++) if (state[i].is_useful) {
		get_pos_by_id[i] = i;
		get_id_by_pos[i] = i;
		pos_is_used[i] = 1;
	}
	for (int i = num_of_parameter + 1; i <= num_of_state; i++) if (state[i].is_useful) {
		if (state[i].has_num) {
			int rst_pos = -1;
			for (int j = 1; j < MAX_NUM_OF_EXPRESS; j++) if (!pos_is_used[j]) {
				rst_pos = j;
				break;
			}
			if (rst_pos > MAX_NUM_OF_REGISTER) {
				prt_movq(1, rst_pos);
				get_pos_by_id[get_id_by_pos[1]] = rst_pos;
				get_id_by_pos[rst_pos] = get_id_by_pos[1];
				pos_is_used[rst_pos] = 1;
				pos_is_used[1] = 0;
				rst_pos = 1;
			}
			prt_movabsq(state[i].num, rst_pos);
			if (state[i].type_of_op) prt_imulq(get_pos_by_id[state[i].id_of_num1], rst_pos);
			else prt_addq(get_pos_by_id[state[i].id_of_num1], rst_pos);
			get_id_by_pos[rst_pos] = i;
			get_pos_by_id[i] = rst_pos;
			pos_is_used[rst_pos] = 1;
			state[state[i].id_of_num1].out_degree--;
			if (state[state[i].id_of_num1].out_degree == 0) {
				pos_is_used[get_pos_by_id[state[i].id_of_num1]] = 0;
			}
		}
		else {
			int rst_pos = -1;
			for (int j = 1; j < MAX_NUM_OF_EXPRESS; j++) if (!pos_is_used[j]) {
				rst_pos = j;
				break;
			}
			state[state[i].id_of_num1].out_degree--;
			state[state[i].id_of_num2].out_degree--;
			if (state[state[i].id_of_num1].out_degree == 0) {
				pos_is_used[get_pos_by_id[state[i].id_of_num1]] = 0;
			}
			if (state[state[i].id_of_num2].out_degree == 0) {
				pos_is_used[get_pos_by_id[state[i].id_of_num2]] = 0;
			}
			if ((get_pos_by_id[state[i].id_of_num1] <= MAX_NUM_OF_REGISTER && 
				state[state[i].id_of_num1].out_degree == 0) ||
				(get_pos_by_id[state[i].id_of_num2] <= MAX_NUM_OF_REGISTER &&
				state[state[i].id_of_num2].out_degree == 0)) {
				if (state[state[i].id_of_num1].out_degree == 0) {
					if (state[i].type_of_op) prt_imulq(get_pos_by_id[state[i].id_of_num2], get_pos_by_id[state[i].id_of_num1]);
					else prt_addq(get_pos_by_id[state[i].id_of_num2], get_pos_by_id[state[i].id_of_num1]);
					get_pos_by_id[i] = get_pos_by_id[state[i].id_of_num1];
					get_id_by_pos[get_pos_by_id[i]] = i;
					pos_is_used[get_pos_by_id[i]] = 1;
				}
				else if (state[state[i].id_of_num2].out_degree == 0) {
					if (state[i].type_of_op) prt_imulq(get_pos_by_id[state[i].id_of_num1], get_pos_by_id[state[i].id_of_num2]);
					else prt_addq(get_pos_by_id[state[i].id_of_num1], get_pos_by_id[state[i].id_of_num2]);
					get_pos_by_id[i] = get_pos_by_id[state[i].id_of_num2];
					get_id_by_pos[get_pos_by_id[i]] = i;
					pos_is_used[get_pos_by_id[i]] = 1;
				}
			}
			else if (rst_pos <= MAX_NUM_OF_REGISTER) {
				if (get_pos_by_id[state[i].id_of_num1] <= MAX_NUM_OF_REGISTER &&
					get_pos_by_id[state[i].id_of_num2] <= MAX_NUM_OF_REGISTER &&
					state[i].type_of_op == 0) {
					prt_leaq(get_pos_by_id[state[i].id_of_num1], get_pos_by_id[state[i].id_of_num2], rst_pos);
				}
				else {
					prt_movq(get_pos_by_id[state[i].id_of_num1], rst_pos);
					if (state[i].type_of_op) prt_imulq(get_pos_by_id[state[i].id_of_num2], rst_pos);
					else prt_addq(get_pos_by_id[state[i].id_of_num2], rst_pos);
				}
				get_pos_by_id[i] = rst_pos;
				get_id_by_pos[rst_pos] = i;
				pos_is_used[rst_pos] = 1;
			}
			else if (get_pos_by_id[state[i].id_of_num1] <= MAX_NUM_OF_REGISTER) {
				int tmp_pos = get_pos_by_id[state[i].id_of_num1];
				prt_movq(get_pos_by_id[state[i].id_of_num1], rst_pos);
				get_pos_by_id[state[i].id_of_num1] = rst_pos;
				get_id_by_pos[rst_pos] = state[i].id_of_num1;
				if (pos_is_used[tmp_pos]) pos_is_used[rst_pos] = 1;
				if (state[i].type_of_op) prt_imulq(get_pos_by_id[state[i].id_of_num2], tmp_pos);
				else prt_addq(get_pos_by_id[state[i].id_of_num2], tmp_pos);
				get_pos_by_id[i] = tmp_pos;
				get_id_by_pos[get_pos_by_id[i]] = i;
			}
			else if (get_pos_by_id[state[i].id_of_num2] <= MAX_NUM_OF_REGISTER) {
				int tmp_pos = get_pos_by_id[state[i].id_of_num2];
				prt_movq(get_pos_by_id[state[i].id_of_num2], rst_pos);
				get_pos_by_id[state[i].id_of_num2] = rst_pos;
				get_id_by_pos[rst_pos] = state[i].id_of_num2;
				if (pos_is_used[tmp_pos]) pos_is_used[rst_pos] = 1;
				if (state[i].type_of_op) prt_imulq(get_pos_by_id[state[i].id_of_num1], tmp_pos);
				else prt_addq(get_pos_by_id[state[i].id_of_num1], tmp_pos);
				get_pos_by_id[i] = tmp_pos;
				get_id_by_pos[get_pos_by_id[i]] = i;
			}
			else {
				prt_movq(1, rst_pos);
				get_pos_by_id[get_id_by_pos[1]] = rst_pos;
				get_id_by_pos[rst_pos] = get_id_by_pos[1];
				pos_is_used[rst_pos] = 1;
				pos_is_used[1] = 0;
				prt_movq(get_pos_by_id[state[i].id_of_num1], 1);
				if (state[i].type_of_op) prt_imulq(get_pos_by_id[state[i].id_of_num2], 1);
				else prt_addq(get_pos_by_id[state[i].id_of_num2], 1);
				get_pos_by_id[i] = 1;
				get_id_by_pos[1] = i;
				pos_is_used[1] = 1;
			}
		}
	}
	prt_movq(get_pos_by_id[num_of_state], POS_OF_RAX);
}
void push_callee_saved() {
	puts("\tpushq\t%r15");
	puts("\tpushq\t%r14");
	puts("\tpushq\t%r13");
	puts("\tpushq\t%r12");
	puts("\tpushq\t%rbp");
	puts("\tpushq\t%rbx");
}
void pop_callee_saved() {
	puts("\tpopq\t%rbx");
	puts("\tpopq\t%rbp");
	puts("\tpopq\t%r12");
	puts("\tpopq\t%r13");
	puts("\tpopq\t%r14");
	puts("\tpopq\t%r15");
}
