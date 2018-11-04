all : clean main generator

main :
	$(CC) src/expr_reader.c src/main.c src/expr_compiler.c -O0 -g -o main -Wall -Wextra

generator : 
	$(CC) src/generator.c -O2 -o generator -Wall -Wextra

clean :
	$(RM) generator main