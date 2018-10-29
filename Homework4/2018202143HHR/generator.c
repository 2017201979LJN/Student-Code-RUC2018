#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE        1024
#define NUMBER_PROBABILITY 60

int random_range,
    variable_count,
    argument_count,
    expression_count;

char *generate_operand(int current_variable, char *buffer, bool no_imme) {
    if (rand() % 100 < NUMBER_PROBABILITY && !no_imme)
        sprintf(buffer, "%d", rand() % random_range);
    else {
        int id = rand() % (argument_count + variable_count - 1);
        if (id < argument_count) sprintf(buffer, "a%d", id);
        else {
            id -= argument_count;
            if (id >= current_variable) id++;
            sprintf(buffer, "x%d", id);
        }
    }
    return buffer;
}

char *generate_expression(int current_variable, char *buffer) {
    static char buffer1[BUFFER_SIZE],
                buffer2[BUFFER_SIZE];
    sprintf(buffer,
            "    x%d = %s %c %s;",
            current_variable,
            generate_operand(current_variable, buffer1, false),
            (rand() & 1) ? '+' : '*',
            generate_operand(current_variable, buffer2, true));
    return buffer;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    
    if (argc <= 1 || sscanf(argv[1], "%d", &random_range) != 1)
        random_range = 10000;

    if (argc <= 2 || sscanf(argv[2], "%d", &variable_count) != 1)
        variable_count = 5;

    if (argc <= 3 || sscanf(argv[3], "%d", &argument_count) != 1)
        argument_count = 5;

    if (argc <= 4 || sscanf(argv[4], "%d", &expression_count) != 1)
        expression_count = 5 * variable_count;

    printf("long long f(");
    for (int i = 0; i < argument_count; i++)
        printf("long long a%d%s", i, i == argument_count - 1 ? ") {\n" : ", ");

    for (int i = 0; i < variable_count; i++) {
        printf("    long long x%d = %d;\n", i, rand() % random_range);
    }

    for (int i = 0; i < expression_count; i++) {
        int current_variable = rand() % variable_count;
        static char buffer[BUFFER_SIZE];
        puts(generate_expression(current_variable, buffer));
    }

    printf("    return x%d;\n}", rand() % variable_count);

    // Add main() code for testing.
    puts("\n#include <stdio.h>\n");
    puts("\nint main() {");
    for (int i = 0; i < argument_count; i++) {
        printf("    long long a%d;\n    scanf(\"%%lld\", &a%d);\n", i, i);
    }
    printf("    long long res = f(");
    for (int i = 0; i < argument_count; i++) {
        printf("a%d%s", i, i == argument_count - 1 ? ");\n" : ", ");
    }
    puts("    printf(\"%lld\\n\", res);\n    return 0;\n}");
    
    if (argc == 1) {
        fputs("\nIt seems that you didn't pass any argument to me! I worked fine but here is the usage.\nUsage: generator [random_range = 10000] [variable_count = 5] [argument_count = 5] [expression_count = 5 * variable_count]\n", stderr);
    }
}
