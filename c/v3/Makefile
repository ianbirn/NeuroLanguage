
langRec: train.o computations.o test.o MAIN.o
	gcc -O5 -Wall train.c computations.c test.c MAIN.c -o langRec -lm

train.o:
	gcc -O5 -c train.c 

computations.o:
	gcc -O5 -c computations.c 

test.o: 
	gcc -O5 -c test.c 

MAIN.o:
	gcc -O5 -c MAIN.c 

clean:
	rm *.o 
