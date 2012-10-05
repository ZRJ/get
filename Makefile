.PHONY:clean

CC=gcc
CFLAGS=-Wall -g
LIBS=-pthread
BIN=get
OBJS=get.o logger.o url.o header.o file.o

$(BIN):$(OBJS) 
	$(CC) $(LIBS) $(CFLAGS) $^ -o $@
.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm ./*.o $(BIN)
