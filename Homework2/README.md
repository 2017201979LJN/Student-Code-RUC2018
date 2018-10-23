大作业1：设计一个浮点数表示系统

位数64位（需要根据sizeof(double)在自己机器决定）

要严格按照IEEE浮点数表示标准去模拟和设计

包括overflow, underflow和rounding三种情况的考虑

要能够有各种边界值：INF，NaN，+0和-0

要能够支持一系列的运算：+ - * /

本次作业较为简单，只要求有一个运算符号

随机生成10,000组double的单符号运算

然后用c语言的double计算数值，作为标准答案

分为：printf带小数控制位数的打印和不带小数位数控制的打印

保存结果到文件，然后比较（精确比较或者K位小数比较）

语言：C语言，不得使用任何C++新增的语法

不得使用任何C++ IO，函数重载，读写文件等命令

系统：Linux , gcc编译

提交网站：github

核心数据结构

#define NUM_OF_BYTES 8

struct double_n {

          unsigned char bytes[NUM_OF_BYTES];
          
}
