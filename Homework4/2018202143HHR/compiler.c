#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define MAX_ARG     5
#define MAX_EXP     25
#define MAX_VAR     5
#define BUFFER_SIZE 1024

#ifdef COMPILABLE
const char *USABLE_REGISTERS[] = {
    // RAX is used to calculate.
    "%%rbx", "%%rcx", "%%rdx", "%%rsi", "%%rdi", "%%r8",
    "%%r9", "%%r10", "%%r11", "%%r12", "%%r13", "%%r14", "%%r15"
};
const char *RAX = "%%rax";
#else
const char *USABLE_REGISTERS[] = {
    // RAX is used to calculate.
    "%rbx", "%rcx", "%rdx", "%rbp", "%rsi", "%rdi", "%r8",
    "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"
};
const char *RAX = "%rax";
#endif
const int USABLE_REGISTERS_COUNT = sizeof(USABLE_REGISTERS) / sizeof(USABLE_REGISTERS[0]);

typedef struct {
    enum {
        ARGUMENT,
        VARIABLE,
        IMME
    } type;

    int value_or_index;
} operand_t;

typedef struct {
    int result_variable;
    enum {
        ADD, MUL
    } operator;
    operand_t l, r;
} expression_t;

int argument_count,
    variable_count,
    variable_init_value[MAX_VAR];

const char *argument[MAX_VAR], *variable[MAX_VAR];

int expression_count;
expression_t expressions[MAX_EXP];

int return_vaiable;

void read_arguments() {
    // Count ','
    char ch;
    while ((ch = getchar()) != '\n')
        if (ch == ',') argument_count++;
    argument_count++;
}

void read_variable_declares() {
    while (scanf(" long long x%*d = %d;", &variable_init_value[variable_count]) == 1)
        variable_count++;
}

operand_t parse_operand(char *s) {
    int x;
    if (sscanf(s, "a%d", &x) == 1) return (operand_t){ ARGUMENT, x };
    if (sscanf(s, "x%d", &x) == 1) return (operand_t){ VARIABLE, x };
    if (sscanf(s, "%d", &x) == 1) return (operand_t){ IMME, x };
    assert(false);
}

void read_expressions() {
    int result_variable;
    char s1[BUFFER_SIZE], operator[BUFFER_SIZE], s2[BUFFER_SIZE];

    while (scanf(" x%d = %s %s %s;", &result_variable, s1, operator, s2) == 4) {
        switch (*operator) {
        case '+': expressions[expression_count].operator = ADD; break;
        case '*': expressions[expression_count].operator = MUL; break;
        default: assert(false);
        }

        expressions[expression_count].result_variable = result_variable;
        expressions[expression_count].l = parse_operand(s1);
        expressions[expression_count].r = parse_operand(s2);

        expression_count++;
    }
}

void read_return() {
    scanf(" return x%d;", &return_vaiable);
}

void operand_to_asm(const operand_t operand, char *buffer) {
    if (operand.type == ARGUMENT) return (void)strcpy(buffer, argument[operand.value_or_index]);
    if (operand.type == VARIABLE) return (void)strcpy(buffer, variable[operand.value_or_index]);
    if (operand.type == IMME) return (void)sprintf(buffer, "$%d", operand.value_or_index);
    assert(false);
}

void generate_asm() {

#ifdef COMPILABLE
#define instruction2(ins, op1, op2) printf("        \"%s %s, %s\\n\"\n", ins, op1, op2)
#define instruction1(ins, op1) printf("        \"%s %s\\n\"\n", ins, op1)
#define instruction0(ins) printf("        \"%s\\n\"\n", ins)
#else
#define instruction2(ins, op1, op2) printf("%s %s, %s\n", ins, op1, op2)
#define instruction1(ins, op1) printf("%s %s\n", ins, op1)
#define instruction0(ins) printf("%s\n", ins)
#endif

#ifdef COMPILABLE
    // GCC inline asm.
    printf("long long f(");
    for (int i = 0; i < argument_count; i++)
        printf("long long a%d%s", i, i == argument_count - 1 ? ") {\n" : ", ");
    puts("    long long return_value;\n    asm(");

    // Prepare arguments (for GCC)
    for (int i = 0; i < argument_count; i++){
        char s[BUFFER_SIZE];
        // Because %RAX is %0, argument[i] is %(i + 1).
        sprintf(s, "%%%d", i + 1);
        // e.g. movq %0, RBX.
        instruction2("movq", s, argument[i]);
    }
#endif

    // Assign init value to variables.
    for (int i = 0; i < variable_count; i++) {
        char num[BUFFER_SIZE];
        sprintf(num, "$%d", variable_init_value[i]);
        instruction2("movq", num, variable[i]);
    }

    // Evaluate the expressions.
    for (int i = 0; i < expression_count; i++) {
        char lop[BUFFER_SIZE], rop[BUFFER_SIZE];
        operand_to_asm(expressions[i].l, lop);
        operand_to_asm(expressions[i].r, rop);

        // We use RAX to do the calculate.
        // Assign the left operand to RAX.
        instruction2("movq", lop, RAX);

        // ADD/MUL the right operand to RAX.
        switch (expressions[i].operator) {
        case ADD: instruction2("addq", rop, RAX); break;
        case MUL: instruction2("imulq", rop, RAX); break;
        default: assert(false);
        }

        // Assign the result in RAX to result variable.
        instruction2("movq", RAX, variable[expressions[i].result_variable]);
    }

    instruction2("movq", variable[return_vaiable], RAX);

#ifdef COMPILABLE
    // GCC inline asm.
    instruction2("movq", RAX, "%0");
    printf("%s", "    :\"=m\"(return_value):");
    for (int i = 0; i < argument_count; i++) {
        printf("\"m\"(a%d)%c", i, ",:"[i == argument_count - 1]);
    }

    // Why + 1? Beacuse USABLE_REGISTER[] is like '%%RAX' but we need to print
    // something like '%RAX'.
    for (int i = 0; i < variable_count; i++)
        printf("\"%s\",", variable[i] + 1);
    for (int i = 0; i < argument_count; i++)
        printf("\"%s\",", argument[i] + 1);
    puts("\"%rax\");\n    return return_value;\n}");
#endif

#undef instruction2
#undef instruction1
#undef instruction0

}

int main() {
    read_arguments();
    read_variable_declares();
    read_expressions();
    read_return();

    for (int i = 0; i < variable_count; i++) {
        assert(i < USABLE_REGISTERS_COUNT);
        variable[i] = USABLE_REGISTERS[i];
        fprintf(stderr, "variable x%d is stored in %s\n", i, variable[i]);
    }
    
    for (int i = 0; i < argument_count; i++) {
        assert(i + variable_count < USABLE_REGISTERS_COUNT);
        argument[i] = USABLE_REGISTERS[i + variable_count];
        fprintf(stderr, "argument a%d is stored in %s\n", i, argument[i]);
    }

    generate_asm();

#ifdef COMPILABLE
    // Add main() code for testing.
    puts("\n#include <stdio.h>\n");
    puts("int main() {");
    for (int i = 0; i < argument_count; i++) {
        printf("    long long a%d;\n    scanf(\"%%lld\", &a%d);\n", i, i);
    }
    printf("    long long res = f(");
    for (int i = 0; i < argument_count; i++) {
        printf("a%d%s", i, i == argument_count - 1 ? ");\n" : ", ");
    }
    puts("    printf(\"%lld\\n\", res);\n    return 0;\n}");
#endif
}
