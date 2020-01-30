flags = -Wall
compiler = gcc

all: aes

run: aes
	./aes

remake: clean aes

aes: aes.o main.c
	$(compiler) $(flags) $^ -o $@

aes.o: aes.c
	$(compiler) $(flags) $^ -c -o $@

clean:
	rm -f *.o aes
