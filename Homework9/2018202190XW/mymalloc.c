#ifdef COMPILETIME

#include <stdio.h>
#include <stdlib.h>

/* malloc wrapper function */
void *my_malloc(size_t size)
{
	void *ptr = malloc(size);
	printf("%p malloc\n", ptr);
	return ptr;
}

/* free wrapper function */
void my_free(void *ptr)
{
	printf("%p free\n", ptr);
	free(ptr);
}

#endif // #ifdef COMPILETIME

#ifdef LINKTIME

#include <stdio.h>

void *__real_malloc(size_t size);
void __real_free(void *ptr);

/* malloc wrapper function */
void *__wrap_malloc(size_t size)
{
	void *ptr = __real_malloc(size); // Call libc malloc
	printf("%p malloc\n", ptr);
	return ptr;
}

/* free wrapper function */
void __wrap_free(void *ptr)
{
	__real_free(ptr); // Call libc free
	printf("%p free\n", ptr);
}

#endif // #ifdef LINKTIME

#ifdef RUNTIME

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

/* malloc wrapper function */
void *malloc(size_t size)
{
	void *(*mallocp)(size_t size);
	char *error;

	mallocp = dlsym(RTLD_NEXT, "malloc"); // Get address of libc malloc
	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(1);
	}
	char *ptr = mallocp(size); // Call libc malloc
	printf("%p malloc\n", ptr);
	return ptr;
}

/* free wrapper function */
void free(void *ptr)
{
	void (*freep)(void *) = NULL;
	char *error;

	if (!ptr) return;

	freep = dlsym(RTLD_NEXT, "free"); // Get address of libc free
	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(1);
	}
	freep(ptr); // Call libc free
	printf("%p free\n", ptr);
}

#endif // #ifdef RUNTIME

