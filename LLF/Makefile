#CROSS_COMPILE = arm-linux-gnueabihf-
CC=$(CROSS_COMPILE)gcc
CFLAGS += -MD -MP
LDFLAGS += -lpthread

SRC = $(wildcard *.c)
OBJS = $(SRC:%.c=%.o)
DEPS = $(SRC:%.c=%.d)
TARGET = llf_sim

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f $(OBJS)
	-rm -f $(DEPS)
	-rm -f $(TARGET)

-include $(DEPS)