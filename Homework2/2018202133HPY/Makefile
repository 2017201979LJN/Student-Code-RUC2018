SRC = ${wildcard src/*.c}
OBJ_MAIN = ${patsubst src/%.c, tmp/%.o, $(SRC)}
OBJ_NO_MAIN = ${patsubst src/%.c, tmp/no_main_%.o, $(SRC)}
CFLAGS = -O2 -fPIC -Wall -fno-strict-aliasing
override NUMBER = 10000

all : tmp clean bin shared

tmp :
	mkdir -p tmp

bin : tmp $(OBJ_MAIN) double_t_test
shared: tmp $(OBJ_NO_MAIN) double_t_test.so

gen : 
	$(RM) gen
	$(CC) $(CFLAGS) gen.c -o gen

c_builtin :
	$(RM) c_builtin_double
	$(CC) $(CFLAGS) c_builtin_double.c -o c_builtin_double

$(OBJ_MAIN) : tmp/%.o : src/%.c
	$(CC) -DUSE_MAIN -DNO_ASM $(CFLAGS) -c $^ -o $@

$(OBJ_NO_MAIN) : tmp/no_main_%.o : src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@


double_t_test : $(OBJ_MAIN)
	$(CC) $(CFLAGS) $^ -o $@

double_t_test.so : $(OBJ_NO_MAIN)
	$(CC) -shared $(CFLAGS) $^ -o $@

clean :
	$(RM) tmp/*
	$(RM) double_t_test
	$(RM) double_t_test.so
	$(RM) gen c_builtin_double

test : all gen c_builtin
	bash ./test.sh

.PHONY : clean
