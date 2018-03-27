INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS= 
CFLAGS= -O3 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lpthread -lrt
LIBS_MATH = -lm

PRODUCT=EDF_scheduling

HFILES=
CFILES= EDF_scheduling.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~ *.d
	-rm -f ${PRODUCT} ${GARBAGE}

EDF_scheduling:	EDF_scheduling.o
	$(CC) -o $@ EDF_scheduling.o $(LIBS_MATH)

depend:

.c.o:
	$(CC) -MD $(CFLAGS) -c $<
