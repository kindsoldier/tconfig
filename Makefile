#
#  Copyright 2023 Oleg Borodin  <borodin@unix7.org>
#

all: test

CC = gcc
CFLAGS = -O -I. -std=c99 -pthread
CFLAGS += -Wpedantic -Wall -Wextra
CFLAGS += -Wbool-compare -Wbool-operation
CFLAGS += -Wint-in-bool-context
CFLAGS += -Wuninitialized

CFLAGS += -Wcast-align
CFLAGS += -Wcast-qual
CFLAGS += -Wconversion
#CFLAGS += -Wduplicated-branches
#CFLAGS += -Wduplicated-cond
#CFLAGS += -Wfloat-equal
#CFLAGS += -Wlogical-op
#CFLAGS += -Wredundant-decls
CFLAGS += -Wsign-conversion

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

tconfig.c: tconfig.h
tconfig.o: tconfig.c


OBJS = bstream.o massert.o clexer.o mapper.o cyacc.o
OBJS += tconfig.o

bstream_test: bstream_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ bstream_test.o $(OBJS)

clexer_test: clexer_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ clexer_test.o $(OBJS)

mapper_test: mapper_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ mapper_test.o $(OBJS)

cyacc_test: cyacc_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ cyacc_test.o $(OBJS)

tconfig_test: tconfig_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ tconfig_test.o $(OBJS)


TESTS = bstream_test clexer_test mapper_test cyacc_test
TESTS += tconfig_test

test: $(TESTS)
#	./bstream_test
#	./mapper_test
#	./clexer_test
#	./cyacc_test
	./tconfig_test


clean:
	rm -f *_test
	rm -f *.o *~
	rm -f *.gch

#EOF
