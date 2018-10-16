SRC = ${wildcard src/*.c}
OBJ_MAIN = ${patsubst src/%.c, tmp/%.o, $(SRC)}
OBJ_NO_MAIN = ${patsubst src/%.c, tmp/no_main_%.o, $(SRC)}
OBJ_NO_MAIN_32 = ${patsubst src/%.c, tmp/no_main_32_%.o, $(SRC)}
CFLAGS = -O2 -fPIC -Wall -fno-strict-aliasing
override NUMBER = 10000

all : tmp clean bin shared

tmp :
	mkdir -p tmp

bin : tmp $(OBJ_MAIN) double_t_test

shared: tmp $(OBJ_NO_MAIN) double_t_test.so
shared32: tmp $(OBJ_NO_MAIN_32) double_t_test_32.so

gen : 
	$(RM) gen
	$(CC) $(CFLAGS) gen.c -o gen

c_builtin :
	$(RM) c_builtin_double
	$(CC) $(CFLAGS) c_builtin_double.c -o c_builtin_double

$(OBJ_MAIN) : tmp/%.o : src/%.c
	$(CC) -DUSE_MAIN $(CFLAGS) -c $^ -o $@

$(OBJ_NO_MAIN) : tmp/no_main_%.o : src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_NO_MAIN_32) : tmp/no_main_32_%.o : src/%.c
	$(CC) -m32 -DNO_ASM $(CFLAGS) -c $^ -o $@

double_t_test : $(OBJ_MAIN)
	$(CC) $(CFLAGS) $^ -o $@

double_t_test.so : $(OBJ_NO_MAIN)
	$(CC) -shared $(CFLAGS) $^ -o $@

double_t_test_32.so : $(OBJ_NO_MAIN_32)
	$(CC) -m32 -shared $(CFLAGS) $^ -o $@

clean :
	$(RM) tmp/*
	$(RM) double_t_test
	$(RM) double_t_test.so
	$(RM) double_t_test_32.so
	$(RM) gen c_builtin_double

test : all gen c_builtin
	bash ./test.sh

.PHONY : clean
