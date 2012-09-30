get: get.o logger.o
	gcc -o get get.o logger.o
get.o: get.c logger.h
	gcc -c get.c
logger.o: logger.h logger.c
	gcc -c logger.c
