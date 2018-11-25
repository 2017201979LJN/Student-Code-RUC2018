generator.c version 1.0
compiler.c verision 1.0
-------------------------------------------------------------------
Description: 
    generator.c : Generate a function with symbol '*', '+'. 
                  Print to "data.c"
    ---------------------------------------------------------------
    long long cal(long long a, long long b, long long c){
     	a = 41 + a ;
    	b = a + 40 ;
     	c = c * c ;
    	a = 6 + a ;
     	b = c + b ;
    	c = c * b ;
    	a = c * b ;
    	b = a + a ;
    	a = 14 + a ;
    	c = a + c ;
    	b = a + 37 ;
    	a = a + 48 ;
    	c = c + c ;
    	b = c + 41 ;
    	return c ;
     }
    ----------------------------------------------------------------    

    compiler.c : Read the function in "data.c", and compile it to assembly code.
                 Print to "data.s" 
    ----------------------------------------------------------------
    	addq	$41, %rdi
		movq	%rdi, %rsi
		addq	$40, %rsi
		movq	%rdx, %rax
		imulq   %rdx, %rax
		movq	%rax, %rdx
		addq	$6, %rdi
		addq	%rdx, %rsi
		imulq   %rsi, %rdx
		movq	%rdx, %rdi
		imulq   %rsi, %rdi
		movq	%rdi, %rsi
		salq	%rsi
		addq	$14, %rdi
		addq	%rdi, %rdx
		movq	%rdi, %rsi
		addq	$37, %rsi
		addq	$48, %rdi
		salq	%rdx
		movq	%rdx, %rsi
		addq	$41, %rsi
		movq	%rdx, %rax
-----------------------------------------------------------------------

17:20 2018/11/4


