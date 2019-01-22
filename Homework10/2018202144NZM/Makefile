_zip: _zip.so main.c
	gcc -o _zip main.c ./_zip.so 
_zip.so: zip.c
	gcc -shared -fPIC -o _zip.so zip.c
clean:
	rm -f _zip.so
