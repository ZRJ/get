get: get.o logger.o
	gcc -o $@ $^
	rm ./*.o
.c.o:
	gcc -c -g $<
