.PHONY: all clean myTerm mySimpleComputer myBigChars myReadkey console

all: myTerm mySimpleComputer myBigChars myReadkey console

myTerm:
	$(MAKE) -C myTerm

mySimpleComputer: myTerm
	$(MAKE) -C mySimpleComputer

myBigChars: myTerm
	$(MAKE) -C myBigChars

myReadkey: myTerm
	$(MAKE) -C myReadkey

console: mySimpleComputer myBigChars myReadkey
	$(MAKE) -C console

clean:
	$(MAKE) -C myTerm clean
	$(MAKE) -C mySimpleComputer clean
	$(MAKE) -C myBigChars clean
	$(MAKE) -C myReadkey clean
	$(MAKE) -C console clean