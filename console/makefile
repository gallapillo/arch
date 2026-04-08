CC = gcc
CFLAGS = -Wall -Wextra -I../include
LDFLAGS = -L../mySimpleComputer -L../myTerm -L../myBigChars -L../myReadkey \
          -lmySimpleComputer -lmyTerm -lmyBigChars -lmyReadkey

TARGET = console
FONT_TARGET = font
SRCS = main.c
FONT_SRCS = font.c
OBJS = $(SRCS:.c=.o)
FONT_OBJS = $(FONT_SRCS:.c=.o)

all: $(TARGET) $(FONT_TARGET)

$(TARGET): $(OBJS) ../mySimpleComputer/libmySimpleComputer.a \
                   ../myTerm/libmyTerm.a \
                   ../myBigChars/libmyBigChars.a \
                   ../myReadkey/libmyReadkey.a
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(FONT_TARGET): $(FONT_OBJS) ../myBigChars/libmyBigChars.a
	$(CC) $(FONT_OBJS) -o $@ -L../myBigChars -lmyBigChars

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

../mySimpleComputer/libmySimpleComputer.a:
	$(MAKE) -C ../mySimpleComputer

../myTerm/libmyTerm.a:
	$(MAKE) -C ../myTerm

../myBigChars/libmyBigChars.a:
	$(MAKE) -C ../myBigChars

../myReadkey/libmyReadkey.a:
	$(MAKE) -C ../myReadkey

clean:
	rm -f $(OBJS) $(FONT_OBJS) $(TARGET) $(FONT_TARGET)
	$(MAKE) -C ../mySimpleComputer clean
	$(MAKE) -C ../myTerm clean
	$(MAKE) -C ../myBigChars clean
	$(MAKE) -C ../myReadkey clean

.PHONY: all clean