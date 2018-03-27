INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= 
CFILES= source.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	source
	./source

clean:
	-rm -f *.o *.d
	-rm -f source

source: source.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lm

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
