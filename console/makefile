CC = gcc
CFLAGS = -Wall -Wextra -I../include
LDFLAGS = -L../mySimpleComputer -L../myTerm -lmySimpleComputer -lmyTerm

TARGET = console
SRCS = main.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS) ../mySimpleComputer/libmySimpleComputer.a ../myTerm/libmyTerm.a
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

../mySimpleComputer/libmySimpleComputer.a:
	$(MAKE) -C ../mySimpleComputer

../myTerm/libmyTerm.a:
	$(MAKE) -C ../myTerm

clean:
	rm -f $(OBJS) $(TARGET)
	$(MAKE) -C ../mySimpleComputer clean
	$(MAKE) -C ../myTerm clean

.PHONY: all clean