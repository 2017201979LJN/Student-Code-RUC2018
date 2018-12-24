#ifdef LEAK_CHECKER

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void *malloc(size_t size)
{
	static void *(*mallocp)(size_t size) = NULL;
	if(!mallocp)
		mallocp = dlsym(RTLD_NEXT, "malloc");
	char *ptr = mallocp(size);
	printf("malloc(%d) = %p\n", (int)size, ptr);
	return ptr;
}

void free(void *ptr)
{
	static void (*freep)(void *) = NULL;
	if (!ptr)
		return;
	if(!freep)
		freep = dlsym(RTLD_NEXT, "free");
	freep(ptr);
	printf("free(%p)\n", ptr);
}

#endif
