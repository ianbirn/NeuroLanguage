
langRec: train.o computations.o test.o main.o
	gcc -O5 -Wall train.c computations.c test.c main.c -o langRec -lm

train.o:
	gcc -O5 -c train.c 

computations.o:
	gcc -O5 -c computations.c 

test.o: 
	gcc -O5 -c test.c 

main.o:
	gcc -O5 -c main.c 

clean:
	rm *.o 
