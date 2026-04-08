CC = gcc
CFLAGS = -Wall -Wextra -I../include
AR = ar
ARFLAGS = rcs

LIB = libmyBigChars.a
OBJS = bc_strlen.o bc_printA.o bc_box.o bc_setbigcharpos.o \
       bc_getbigcharpos.o bc_printbigchar.o bc_bigcharwrite.o \
       bc_bigcharread.o

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: all clean