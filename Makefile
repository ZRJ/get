get: get.o logger.o url.o
	gcc -o $@ $^
	rm ./*.o
.c.o:
	gcc -c -g $<
