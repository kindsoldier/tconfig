#
#  Copyright 2023 Oleg Borodin  <borodin@unix7.org>
#

all: test

CC = gcc
CFLAGS = -O -Wall -I. -std=c99 -pthread
LDFLAGS = -pthread

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

bstream.c: bstream.h
bstream.o: bstream.c

massert.c: massert.h
massert.o: massert.c

clexer.c: clexer.h
clexer.o: clexer.c

mapper.c: mapper.h
mapper.o: mapper.c

cyacc.c: cyacc.h
cyacc.o: cyacc.c


OBJS += bstream.o massert.o clexer.o mapper.o cyacc.o

bstream_test: bstream_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ bstream_test.o $(OBJS)

clexer_test: clexer_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ clexer_test.o $(OBJS)

mapper_test: mapper_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ mapper_test.o $(OBJS)

cyacc_test: cyacc_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ cyacc_test.o $(OBJS)


test: bstream_test clexer_test mapper_test cyacc_test
#	./bstream_test
#	./mapper_test
	./clexer_test
	./cyacc_test

clean:
	rm -f *_test
	rm -f *.o *~
	rm -f *.gch

#EOF
