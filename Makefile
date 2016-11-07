all: main colorsupport
main:
	clang main.c -g -lm -lncurses -o main
colorsupport:
#	gcc test.c -g -lncurses -o colorsupport
clean:
	rm -rf main
remake: clean all
