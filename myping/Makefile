CC=gcc

myping: myping.o 
	$(CC) myping.o -o myping
myping.o: myping.c myping.h
	$(CC) -c myping.c

clean:
	rm -f *.o
