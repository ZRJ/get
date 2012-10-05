get: get.o logger.o url.o header.o
	gcc -o $@ $^
	rm ./*.o
.c.o:
	gcc -c -g -Wall $<
