prefix=/usr/local

phook: phook.c 
	gcc -o phook phook.c 

install: phook
	install -m 0755 phook $(prefix)/bin