a_link : main.o sbt.o
	cc -o a main.o sbt.o

a_sl : main.o sbt.a
	cc -static -o a main.o ./sbt.a

a_dl : main.o sbt.so
	cc -o a main.o ./sbt.so

ct_interp : main.o sbt_interp.o mymalloc_ct.o
	cc -o a main.o sbt_interp.o mymalloc_ct.o

lt_interp : main.o sbt.o mymalloc_lt.o
	cc -o a -Wl,--wrap,malloc -Wl,--wrap,free main.o sbt.o mymalloc_lt.o
	
rt_interp : a_link mymalloc.so
	LD_PRELOAD="./mymalloc.so" ./a

main.o : main.c sbt.h
	cc -c main.c

sbt.a : sbt.o
	ar rcs sbt.a sbt.o

sbt.so : sbt_fpic.o
	cc -shared -o sbt.so sbt_fpic.o

sbt.o : sbt.c sbt.h
	cc -c sbt.c

sbt_fpic.o : sbt.c sbt.h
	cc -o sbt_fpic.o -fPIC -c sbt.c

sbt_interp.o : sbt.c sbt.h malloc.h
	cc -o sbt_interp.o -I. -c sbt.c

mymalloc_ct.o : mymalloc.c
	cc -o mymalloc_ct.o -c -DCOMPILETIME mymalloc.c

mymalloc_lt.o : mymalloc.c
	cc -o mymalloc_lt.o -c -DLINKTIME mymalloc.c

mymalloc.so : mymalloc.c
	cc -DRUNTIME -shared -fPIC -o mymalloc.so mymalloc.c -ldl

clean :
	rm -f a main.o sbt.o sbt_fpic.o sbt_interp.o sbt.a sbt.so mymalloc_ct.o mymalloc_lt.o mymalloc.so data.out interp.out

