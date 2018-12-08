# avl-tree
The task of Advanced Programming class on 2018/12/05. Implementation of AVL tree and heap memory leak check in C.

# Compile
```bash
make avl-tree.so
```

# Code for [LOJ #107](https://loj.ac/problem/107)
```bash
make loj-107-single
```

Check `loj-107-single.c` and submit.

My submission is [here](https://loj.ac/submission/281811).

# Check heap memory leak
```bash
make memory-leak-checker
LD_PRELOAD=./memory-leak-checker.so [your program]
```

Or just test code of [LOJ #107](https://loj.ac/problem/107):

```bash
make loj-107-check
```
