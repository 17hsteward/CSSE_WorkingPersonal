all: beezy

beezy: beezy.o say_hello.o
	gcc beezy.o say_hello.o -o beezy 

beezy.o: beezy.c say_hello.h
	gcc -c beezy.c -o beezy.o

say_hello.o: say_hello.c say_hello.h
	gcc -c say_hello.c -o say_hello.o

clean:
	rm -rf *.o repair_demon beezy *.dSYM
