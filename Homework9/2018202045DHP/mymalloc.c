#ifdef COMPILETIME
#include<stdio.h>
#include<malloc.h>

void *mymalloc(size_t size){
	void *ptr = malloc(size);
	fprintf(stderr,"malloc(%d)=%p\n", (int)size, ptr);
	return ptr;
}

void myfree(void *ptr){
	free(ptr);
	fprintf(stderr,"free(%p)\n", ptr);
}

#endif
