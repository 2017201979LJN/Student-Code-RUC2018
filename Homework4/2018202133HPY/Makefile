override CFLAGS=-mno-red-zone -Wall -Wextra

all : generator compiler

test : all
	python3 test.py

generator :
	$(CC) $(CFLAGS) src/generator.c -o bin/generator

compiler :
	$(CC) $(CFLAGS) src/compiler.c -o bin/compiler

clean : 
	$(RM) bin/* tmp/*

.PHONY : clean
