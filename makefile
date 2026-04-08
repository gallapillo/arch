.PHONY: all clean mySimpleComputer myTerm console

all: mySimpleComputer myTerm console

mySimpleComputer:
	$(MAKE) -C mySimpleComputer

myTerm:
	$(MAKE) -C myTerm

console: mySimpleComputer myTerm
	$(MAKE) -C console

clean:
	$(MAKE) -C mySimpleComputer clean
	$(MAKE) -C myTerm clean
	$(MAKE) -C console clean