prefix=/usr/local

phook: phook.c 
	gcc -o phook phook.c 

install: our_program
	install -m 0755 phook $(prefix)/bin