CC = gcc
CFLAGS = -Wall -Wextra -I../include
AR = ar
ARFLAGS = rcs

LIB = libmyTerm.a
OBJS = mt_clrscr.o mt_gotoXY.o mt_getscreensize.o \
       mt_setfgcolor.o mt_setbgcolor.o mt_setdefaultcolor.o \
       mt_setcursorvisible.o mt_delline.o

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: all clean