.PHONY: all clean myTerm mySimpleComputer myBigChars console

all: myTerm mySimpleComputer myBigChars console

myTerm:
	$(MAKE) -C myTerm

mySimpleComputer: myTerm
	$(MAKE) -C mySimpleComputer

myBigChars: myTerm
	$(MAKE) -C myBigChars

console: mySimpleComputer myBigChars
	$(MAKE) -C console

clean:
	$(MAKE) -C myTerm clean
	$(MAKE) -C mySimpleComputer clean
	$(MAKE) -C myBigChars clean
	$(MAKE) -C console clean