CC = gcc
CFLAGS = -Wall -Wextra -I.

TARGET = sbt
OBJS = main.o basic.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean