OBJ = tmp/bitio.o tmp/main.o tmp/tree.o tmp/zip.o

override CFLAGS = -O2
override LDFLAGS = -lm

all: gen main

gen:
	$(CC) $(CFLAGS) $(LDFLAGS) gen.c -o gen

tmp/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

main : $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o main

include depend

depend:
	$(CXX) -MM ${OBJ:tmp/%.o=src/%.c} > depend

clean:
	$(RM) tmp/* main gen

.PHONY: clean
