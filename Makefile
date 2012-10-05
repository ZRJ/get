get: get.o logger.o url.o header.o file.o
	gcc -pthread $^ -o $@
	rm ./*.o
.c.o:
	gcc -c -g -Wall $<
