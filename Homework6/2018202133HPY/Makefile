LIST_SRC = src/list.c src/student.c
JOSSEPHUS_SRC = src/jossephus.c
SORT_SRC = src/sort.c

LIST_OBJ = ${patsubst src/%.c, tmp/%.o, $(LIST_SRC)}
JOSSEPHUS_OBJ = ${patsubst src/%.c, tmp/%.o, $(JOSSEPHUS_SRC)}
SORT_OBJ = ${patsubst src/%.c, tmp/%.o, $(SORT_SRC)}

CFLAGS = -O0 -fPIC -Wall -fno-strict-aliasing -g

all : clean jossephus sort

jossephus: $(OBJ_NO_MAIN) bin/jossephus
sort: $(OBJ_NO_MAIN) bin/sort

$(LIST_OBJ) : tmp/%.o : src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(JOSSEPHUS_OBJ) : tmp/%.o : src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(SORT_OBJ) : tmp/%.o : src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

bin/jossephus : $(LIST_OBJ) $(JOSSEPHUS_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

bin/sort : $(LIST_OBJ) $(SORT_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean :
	$(RM) bin/* tmp/*

.PHONY : clean
