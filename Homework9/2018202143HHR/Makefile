debug: avl-tree.so debug.c
	cc -o debug debug.c ./avl-tree.so -g
memory-leak-checker: memory-leak-checker.c
	cc -o memory-leak-checker.so memory-leak-checker.c -g -ldl -shared -fPIC
loj-107: avl-tree.so loj-107.c
	cc -o loj-107 loj-107.c ./avl-tree.so -g
loj-107-check: loj-107 memory-leak-checker
	LD_PRELOAD=./memory-leak-checker.so ./loj-107
loj-107-single: avl-tree-internel.c avl-tree.c avl-tree-internel.h avl-tree.h loj-107.c
	cp /dev/null loj-107-single.c
	/bin/echo -e "//\n// avl-tree.h\n//\n" >> loj-107-single.c
	cat avl-tree.h >> loj-107-single.c
	/bin/echo -e "\n//\n// avl-tree-internel.h\n//\n" >> loj-107-single.c
	cat avl-tree-internel.h >> loj-107-single.c
	/bin/echo -e "\n//\n// avl-tree.c\n//\n" >> loj-107-single.c
	cat avl-tree.c >> loj-107-single.c
	/bin/echo -e "\n//\n// avl-tree-internel.c\n//\n" >> loj-107-single.c
	cat avl-tree-internel.c >> loj-107-single.c
	/bin/echo -e "\n//\n// loj-107.c\n//\n" >> loj-107-single.c
	cat loj-107.c >> loj-107-single.c
	sed -i 's/#include "/\/\/ #include "/g' loj-107-single.c
	cc -o loj-107-single loj-107-single.c -g
loj-104: avl-tree.so loj-104.c
	cc -o loj-104 loj-104.c ./avl-tree.so -g
loj-104-single: avl-tree-internel.c avl-tree.c avl-tree-internel.h avl-tree.h loj-104.c
	cp /dev/null loj-104-single.c
	/bin/echo -e "//\n// avl-tree.h\n//\n" >> loj-104-single.c
	cat avl-tree.h >> loj-104-single.c
	/bin/echo -e "\n//\n// avl-tree-internel.h\n//\n" >> loj-104-single.c
	cat avl-tree-internel.h >> loj-104-single.c
	/bin/echo -e "\n//\n// avl-tree.c\n//\n" >> loj-104-single.c
	cat avl-tree.c >> loj-104-single.c
	/bin/echo -e "\n//\n// avl-tree-internel.c\n//\n" >> loj-104-single.c
	cat avl-tree-internel.c >> loj-104-single.c
	/bin/echo -e "\n//\n// loj-104.c\n//\n" >> loj-104-single.c
	cat loj-104.c >> loj-104-single.c
	sed -i 's/#include "/\/\/ #include "/g' loj-104-single.c
	cc -o loj-104-single loj-104-single.c -g
avl-tree.so: avl-tree-internel.c avl-tree.c
	cc -o avl-tree.so -shared avl-tree-internel.c avl-tree.c -g -fPIC -Wall -Wextra
clean:
	rm -f loj-107 loj-107-single loj-107-single.c loj-104 loj-104-single loj-104-single.c avl-tree.so debug memory-leak-checker.so
