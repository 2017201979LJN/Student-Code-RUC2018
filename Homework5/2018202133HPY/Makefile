CFLAGS=-O2 -lm -Wall -Wextra -g
SRC = ${wildcard src/*.c}

all : bin/qsort

bin/qsort : $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean :
	$(RM) bin/* tmp/*

.PHONY : clean
