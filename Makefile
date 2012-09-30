get: get.o logger.o
	gcc -o $@ $^
.c.o:
	gcc -c $<
