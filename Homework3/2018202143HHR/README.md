# double
Implementation of IEEE 754 floating point number. Like `double` in C.

The task of Advanced Programming class on 2018/10/10.

Update 2018/10/21: Add double I/O and expression evaluate, which is the task of Advanced Programming class on 2018/10/17.

# Usage
Only a expression with numbers, operators in `+`, `-`, `*`, `/` and brackets is supported. All space characters in the input within a line will be ignored.

It will print as precise as possible and omit the extra zeros after decimal point.

```bash
$ ./compile_exec.sh
$ ./double <<< '0.1+0.2+(0.3+0.4)*2'
1.70000000000000017763568394002504646778106689453125
```
