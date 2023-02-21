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

tclexer.c: tclexer.h
tclexer.o: tclexer.c

vmapper.c: vmapper.h
vmapper.o: vmapper.c

tccomp.c: tccomp.h
tccomp.o: tccomp.c

tconfig.c: tconfig.h
tconfig.o: tconfig.c


OBJS = bstream.o massert.o tclexer.o vmapper.o tccomp.o
OBJS += tconfig.o

bstream_test: bstream_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ bstream_test.o $(OBJS)

tclexer_test: tclexer_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ tclexer_test.o $(OBJS)

vmapper_test: vmapper_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ vmapper_test.o $(OBJS)

tccomp_test: tccomp_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ tccomp_test.o $(OBJS)

tconfig_test: tconfig_test.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ tconfig_test.o $(OBJS)


TESTS = bstream_test tclexer_test vmapper_test tccomp_test
TESTS += tconfig_test

test: $(TESTS)
#	./bstream_test
#	./vmapper_test
#	./tclexer_test
#	./tccomp_test
	./tconfig_test


clean:
	rm -f *_test
	rm -f *.o *~
	rm -f *.gch

#EOF
