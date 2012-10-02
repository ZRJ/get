get: get.o logger.o
	gcc -o -g $@ $^
.c.o:
	gcc -c $<
