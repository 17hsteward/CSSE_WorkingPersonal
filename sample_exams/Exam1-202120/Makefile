all: shell.bin order.bin cook.bin deliver.bin

shell.o: shell.c
	gcc -c shell.c -o shell.o

shell.bin: shell.o 
	gcc shell.o -o shell.bin

order.o: order.c library.h
	gcc -c order.c -o order.o

order.bin: order.o 
	gcc order.o -o order.bin

cook.o: cook.c library.h
	gcc -c cook.c -o cook.o

cook.bin: cook.o 
	gcc cook.o -o cook.bin

deliver.o: deliver.c library.h
	gcc -c deliver.c -o deliver.o

deliver.bin: deliver.o 
	gcc deliver.o -o deliver.bin

clean:
	rm *.o *.bin