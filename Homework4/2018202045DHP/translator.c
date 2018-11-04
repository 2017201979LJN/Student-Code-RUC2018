#include <stdio.h>
#include <string.h>

#define RETURN_POS 7
#define DEFINE_POS 13
#define EQUAL_POS 3
#define VARI_NUM 5
#define ORDER_NUM 25
#define BASE -48
#define BITS 8

void translate_main(){
	int endpos, isnum1, isnum2;
	char temp[200], sym;
	puts("	movq	%rdi, -56(%rbp)");
	puts("	movq	%rsi, -64(%rbp)");
	puts("	movq	%rdx, -72(%rbp)");
	puts("	movq	%rcx, -80(%rbp)");
	puts("	movq	%r8, -88(%rbp)");
	for(int i = 1; i <= VARI_NUM; i++){
		memset(temp, 0, sizeof(temp));
		gets(temp);
		endpos = DEFINE_POS;
		if(temp[endpos] == 'x') {
			printf("	movq	");
			printf("%d(%%rbp), %%rdx\n", BASE - BITS * (temp[endpos + 1] - '0'));
			endpos += 2;
			isnum1 = -1;
		}	
		else if(temp[endpos] == 't') {
			printf("	movq	");
			printf("%d(%%rbp), %%rdx\n", BASE + BITS * (temp[endpos + 1] - '0'));
			endpos += 2;
			isnum1 = -1;
		}
		else {
			isnum1 = 0;
			while(temp[endpos] >= '0' && temp[endpos] <= '9')
			{
				isnum1 *= 10;
				isnum1 += temp[endpos++] - '0';
			}
		}
		sym = temp[endpos++];
		if(temp[endpos] == 'x') {
			printf("	movq	");
			printf("%d(%%rbp), %%rax\n", BASE - BITS * (temp[endpos + 1] - '0'));
			endpos += 2;
			isnum2 = -1;
		}
		else if(temp[endpos] == 't') {
			printf("	movq	");
			printf("%d(%%rbp), %%rax\n", BASE + BITS * (temp[endpos + 1] - '0'));
			endpos += 2;
			isnum2 = -1;
		}
		else {
			isnum2 = 0;			
			while(temp[endpos] >= '0' && temp[endpos] <= '9')
			{
				isnum2 *= 10;
				isnum2 += temp[endpos++] - '0';
			}
		}
		if(sym == '+') printf("	addq	");
		else printf("	imulq	");
		if(isnum1 == -1 && isnum2 == -1){
			printf("%%rdx, %%rax\n");
			printf("	movq	%%rax, %d(%%rbp)\n", BASE + BITS * i);
		}
		else if(isnum1 == -1) {
			if(sym == '+') printf("$%d, %%rdx\n", isnum2);
			else printf("$%d, %%rdx, %%rdx\n", isnum2);
			printf("	movq	%%rdx, %d(%%rbp)\n", BASE + BITS * i);
		}
		else if(isnum2 == -1) {
			if(sym == '+') printf("$%d, %%rax\n", isnum1);
			else printf("$%d, %%rax %% rax\n", isnum1);
			printf("	movq	%%rax, %d(%%rbp)\n", BASE + BITS * i);
		}
	}
//	return 0;
	for(int i = 1; i <= ORDER_NUM; i++) {
		memset(temp, 0, sizeof(temp));
		gets(temp);
		endpos = EQUAL_POS;
		if(temp[endpos] == 'x') {
			printf("	movq	");
			printf("%d(%%rbp), %%rdx\n", BASE - BITS * (temp[endpos + 1] - '0'));
			endpos += 2;
			isnum1 = -1;
		}	
		else if(temp[endpos] == 't') {
			printf("	movq	");
			printf("%d(%%rbp), %%rdx\n", BASE + BITS * (temp[endpos + 1] - '0'));
			endpos += 2;
			isnum1 = -1;
		}
		else {
			isnum1 = 0;			
			while(temp[endpos] >= '0' && temp[endpos] <= '9')
			{
				isnum1 *= 10;
				isnum1 += temp[endpos++] - '0';
			}
		}
		sym = temp[endpos++];
		if(temp[endpos] == 'x') {
			printf("	movq	");
			printf("%d(%%rbp), %%rax\n", BASE - BITS * (temp[endpos + 1] - '0'));
			endpos += 2;
			isnum2 = -1;
		}
		else if(temp[endpos] == 't') {
			printf("	movq	");
			printf("%d(%%rbp), %%rax\n", BASE + BITS * (temp[endpos + 1] - '0'));
			endpos += 2;
			isnum2 = -1;
		}
		else {
			isnum2 = 0;			
			while(temp[endpos] >= '0' && temp[endpos] <= '9')
			{
				isnum2 *= 10;
				isnum2 += temp[endpos++] - '0';
			}
		}
		if(sym == '+') printf("	addq	");
		else printf("	imulq	");
		if(isnum1 == -1 && isnum2 == -1){
			printf("%%rdx, %%rax\n");
			printf("	movq	%%rax, ");
		}
		else if(isnum1 == -1) {
			if(sym == '+') printf("$%d, %%rdx\n", isnum2);
			else printf("$%d, %%rdx, %%rdx\n", isnum2);
			printf("	movq	%%rdx, ");
		}
		else if(isnum2 == -1) {
			if(sym == '+') printf("$%d, %%rax\n", isnum1);
			else printf("$%d, %%rax, %%rax\n", isnum1);
			printf("	movq	%%rax, ");
		}
		if(temp[0] == 'x') {
			printf("%d(%%rbp)\n", BASE - BITS * (temp[1] - '0'));		
		}
		else {
			printf("%d(%%rbp)\n", BASE + BITS * (temp[1] - '0'));
		}
	}
	gets(temp);
	printf("	movq	");
	if(temp[RETURN_POS] == 'x')
		printf("%d", BASE - BITS * (temp[RETURN_POS + 1] - '0'));
	else printf("%d", BASE + BITS * (temp[RETURN_POS + 1] - '0'));
	printf("(%%rbp), %%rax\n");
}

int main(){
	char a[200];
	freopen("test.c", "r", stdin);
	freopen("final.s", "w", stdout);	
	gets(a);
	gets(a);
	gets(a);
puts("	.file	\"test.c\"");
puts("	.text");
puts("	.globl	play");
puts("	.type	play, @function");
puts("play:");
puts(".LFB0:");
puts("	.cfi_startproc");
puts("	pushq	%rbp");
puts("	.cfi_def_cfa_offset 16");
puts("	.cfi_offset 6, -16");
puts("	movq	%rsp, %rbp");
puts("	.cfi_def_cfa_register 6");
	translate_main();
/*

puts("	movq	-72(%rbp), %rdx");
puts("	movq	-64(%rbp), %rax");
puts("	addq	%rdx, %rax");
puts("	movq	%rax, -40(%rbp)");
puts("	movq	-88(%rbp), %rdx");
puts("	movq	-72(%rbp), %rax");
puts("	addq	%rdx, %rax");
puts("	movq	%rax, -32(%rbp)");
puts("	movq	-32(%rbp), %rdx");
puts("	movq	-40(%rbp), %rax");
puts("	addq	%rdx, %rax");
puts("	movq	%rax, -24(%rbp)");
puts("	movq	-32(%rbp), %rax");
puts("	addq	$13244, %rax");
puts("	movq	%rax, -16(%rbp)");
puts("	movq	-88(%rbp), %rax");
puts("	addq	$4435, %rax");
puts("	movq	%rax, -8(%rbp)");
puts("	addq	$10930, -32(%rbp)");
puts("	movq	-32(%rbp), %rax");
puts("	addq	$32364, %rax");
puts("	movq	%rax, -72(%rbp)");
puts("	movq	-64(%rbp), %rdx");
puts("	movq	-16(%rbp), %rax");
puts("	addq	%rdx, %rax");
puts("	movq	%rax, -40(%rbp)");
puts("	movq	-72(%rbp), %rdx");
puts("	movq	-80(%rbp), %rax");
puts("	addq	%rdx, %rax");
puts("	movq	%rax, -16(%rbp)");
puts("	movq	-88(%rbp), %rdx");
puts("	movq	%rdx, %rax");
puts("	salq	$3, %rax");
puts("	addq	%rdx, %rax");
puts("	addq	%rax, %rax");
puts("	addq	%rdx, %rax");
puts("	salq	$4, %rax");
puts("	movq	%rax, -72(%rbp)");
puts("	movq	-8(%rbp), %rax");
puts("	imulq	$275, %rax, %rax");
puts("	movq	%rax, -88(%rbp)");
puts("	movq	-56(%rbp), %rax");
puts("	imulq	$270, %rax, %rax");
puts("	movq	%rax, -80(%rbp)");
puts("	movq	-8(%rbp), %rax");
puts("	addq	$28921, %rax");
puts("	movq	%rax, -24(%rbp)");
puts("	movq	-64(%rbp), %rax");
puts("	addq	$19984, %rax");
puts("	movq	%rax, -80(%rbp)");
puts("	movq	-80(%rbp), %rax");
puts("	addq	$20757, %rax");
puts("	movq	%rax, -8(%rbp)");
puts("	movq	-8(%rbp), %rax");
puts("	addq	$11788, %rax");
puts("	movq	%rax, -32(%rbp)");
puts("	addq	$12218, -80(%rbp)");
puts("	movq	-16(%rbp), %rdx");
puts("	movq	-8(%rbp), %rax");
puts("	addq	%rdx, %rax");
puts("	movq	%rax, -72(%rbp)");
puts("	movq	-40(%rbp), %rax");
puts("	imulq	$306, %rax, %rax");
puts("	movq	%rax, -16(%rbp)");
puts("	movq	-80(%rbp), %rax");
puts("	imulq	-16(%rbp), %rax");
puts("	movq	%rax, -64(%rbp)");
puts("	movq	-72(%rbp), %rax");
puts("	addq	$14579, %rax");
puts("	movq	%rax, -40(%rbp)");
puts("	addq	$12640, -24(%rbp)");
puts("	movq	-72(%rbp), %rax");
puts("	addq	$16528, %rax");
puts("	movq	%rax, -56(%rbp)");
puts("	movq	-40(%rbp), %rdx");
puts("	movq	-72(%rbp), %rax");
puts("	addq	%rdx, %rax");
puts("	movq	%rax, -80(%rbp)");
puts("	movq	-16(%rbp), %rax");
puts("	imulq	$271, %rax, %rax");
puts("	movq	%rax, -32(%rbp)");
puts("	movq	-88(%rbp), %rax");
puts("	addq	$31344, %rax");
puts("	movq	%rax, -16(%rbp)");
puts("	movq	-72(%rbp), %rdx");
puts("	movq	%rdx, %rax");
puts("	salq	$5, %rax");
puts("	addq	%rdx, %rax");
puts("	salq	$3, %rax");
puts("	movq	%rax, -72(%rbp)");
puts("	movq	-56(%rbp), %rdx");
puts("	movq	%rdx, %rax");
puts("	addq	%rax, %rax");
puts("	addq	%rdx, %rax");
puts("	leaq	0(,%rax,8), %rdx");
puts("	addq	%rdx, %rax");
puts("	salq	$2, %rax");
puts("	movq	%rax, -56(%rbp)");
puts("	movq	-72(%rbp), %rax");
puts("	addq	$15895, %rax");
puts("	movq	%rax, -88(%rbp)");
puts("	movq	-72(%rbp), %rax");
puts("	addq	$9264, %rax");
puts("	movq	%rax, -8(%rbp)");
puts("	movq	-88(%rbp), %rax");
*/
puts("	popq	%rbp");
puts("	.cfi_def_cfa 7, 8");
puts("	ret");
puts("	.cfi_endproc");
puts(".LFE0:");
puts("	.size	play, .-play");
puts("	.section	.rodata");
puts(".LC0:");
puts("	.string	\"%lld %lld %lld %lld %lld\"");
puts(".LC1:");
puts("	.string	\"%lld\\n\"");
puts("	.text");
puts("	.globl	main");
puts("	.type	main, @function");
puts("main:");
puts(".LFB1:");
puts("	.cfi_startproc");
puts("	pushq	%rbp");
puts("	.cfi_def_cfa_offset 16");
puts("	.cfi_offset 6, -16");
puts("	movq	%rsp, %rbp");
puts("	.cfi_def_cfa_register 6");
puts("	subq	$48, %rsp");
puts("	movq	%fs:40, %rax");
puts("	movq	%rax, -8(%rbp)");
puts("	xorl	%eax, %eax");
puts("	leaq	-16(%rbp), %rdi");
puts("	leaq	-24(%rbp), %rsi");
puts("	leaq	-32(%rbp), %rcx");
puts("	leaq	-40(%rbp), %rdx");
puts("	leaq	-48(%rbp), %rax");
puts("	movq	%rdi, %r9");
puts("	movq	%rsi, %r8");
puts("	movq	%rax, %rsi");
puts("	leaq	.LC0(%rip), %rdi");
puts("	movl	$0, %eax");
puts("	call	__isoc99_scanf@PLT");
puts("	movq	-16(%rbp), %rdi");
puts("	movq	-24(%rbp), %rcx");
puts("	movq	-32(%rbp), %rdx");
puts("	movq	-40(%rbp), %rsi");
puts("	movq	-48(%rbp), %rax");
puts("	movq	%rdi, %r8");
puts("	movq	%rax, %rdi");
puts("	call	play");
puts("	movq	%rax, %rsi");
puts("	leaq	.LC1(%rip), %rdi");
puts("	movl	$0, %eax");
puts("	call	printf@PLT");
puts("	movl	$0, %eax");
puts("	movq	-8(%rbp), %rdx");
puts("	xorq	%fs:40, %rdx");
puts("	je	.L5");
puts("	call	__stack_chk_fail@PLT");
puts(".L5:");
puts("	leave");
puts("	.cfi_def_cfa 7, 8");
puts("	ret");
puts("	.cfi_endproc");
puts(".LFE1:");
puts("	.size	main, .-main");
puts("	.ident	\"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0\"");
puts("	.section	.note.GNU-stack,\"\",@progbits");
return 0;

}
