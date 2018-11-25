# linked-list-sort-joseph
The task of Advanced Programming class on 2018/11/13. Implementation of linked list and sort &amp; joseph game on a linked list in C.

# Compile
```bash
clang linked_list.c main.c -o test -O3
```

# Run
```bash
./test
```

The input format is:

```plain
[number_of_students] [sort] [joseph]
[name] [score] // a student
[name] [score]
[name] [score]
```

`[name]` must be one or more than one non-empty (i.e. `isempty(ch)` is `false`) character(s).

`[sort] = 0` means don't sort it. `[sort] = 1` means sort it.

`[joseph] = 0` means don't play joseph game. `[joseph] = k` which k > 0 means play joseph game. (i.e. consider the linked list as a circle, delete the first item first, after each deletion skip the next k items to locate the next item to delete util the linked list is empty.)

Any invalid input would result in a failed assertion.
