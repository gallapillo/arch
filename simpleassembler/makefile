CC = gcc
CFLAGS = -Wall -Wextra -I../include -I.
LDFLAGS = -L../mySimpleComputer -lmySimpleComputer

TARGET = sat
OBJS = main.o assembler.o

all: $(TARGET)

$(TARGET): $(OBJS) ../mySimpleComputer/libmySimpleComputer.a
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

../mySimpleComputer/libmySimpleComputer.a:
	$(MAKE) -C ../mySimpleComputer

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean