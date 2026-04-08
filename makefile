.PHONY: all clean myTerm mySimpleComputer myBigChars myReadkey console simpleassembler simplebasic

all: myTerm mySimpleComputer myBigChars myReadkey console simpleassembler simplebasic

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

simpleassembler: mySimpleComputer
	$(MAKE) -C simpleassembler

simplebasic: simpleassembler
	$(MAKE) -C simplebasic

clean:
	$(MAKE) -C myTerm clean
	$(MAKE) -C mySimpleComputer clean
	$(MAKE) -C myBigChars clean
	$(MAKE) -C myReadkey clean
	$(MAKE) -C console clean
	$(MAKE) -C simpleassembler clean
	$(MAKE) -C simplebasic clean