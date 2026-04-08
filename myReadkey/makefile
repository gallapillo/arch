CC = gcc
CFLAGS = -Wall -Wextra -I../include
AR = ar
ARFLAGS = rcs

LIB = libmyReadkey.a
OBJS = rk_readkey.o rk_mytermsave.o rk_mytermrestore.o \
       rk_mytermregime.o rk_readvalue.o rk_variables.o

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: all clean