SRC = ${wildcard src/*.c}
OBJ_MAIN = ${patsubst src/%.c, tmp/%.o, $(SRC)}
OBJ_NO_MAIN = ${patsubst src/%.c, tmp/no_main_%.o, $(SRC)}
CFLAGS = -g -fPIC -Wall -Wextra -fno-strict-aliasing
LDFLAGS = -Wl,--wrap,malloc -Wl,--wrap,free

override CC=clang


all : tmp test

tmp : 
	mkdir -p tmp

$(OBJ_MAIN) : tmp/%.o : src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@


test : $(OBJ_MAIN)
	$(CC) $(LDFLAGS) $^ -o $@

clean :
	$(RM) tmp/* test

.PHONY : clean
