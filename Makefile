all: main colorsupport
main:
	gcc main.c -std=c99 -Wno-implicit-function-declaration -g -lm -lncurses -o main
colorsupport:
#	gcc test.c -g -lncurses -o colorsupport
clean:
	rm -rf main
remake: clean all
