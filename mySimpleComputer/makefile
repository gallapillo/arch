CC = gcc
CFLAGS = -Wall -Wextra -I../include
AR = ar
ARFLAGS = rcs

LIB = libmySimpleComputer.a
OBJS = sc_memoryInit.o sc_memorySet.o sc_memoryGet.o sc_memorySave.o \
       sc_memoryLoad.o sc_regInit.o sc_regSet.o sc_regGet.o \
       sc_accumulatorInit.o sc_accumulatorSet.o sc_accumulatorGet.o \
       sc_icounterInit.o sc_icounterSet.o sc_icounterGet.o \
       sc_commandEncode.o sc_commandDecode.o sc_commandValidate.o \
       sc_variables.o sc_printCell.o sc_printFlags.o sc_printDecodedCommand.o \
       sc_printAccumulator.o sc_printCounters.o sc_printBigCell.o

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c sc.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: all clean